#include "main.h"
#include "xtrack.h"
#include "Array.h"
#include "Ngram.h"
#include "NgramReader.h"

HANDLE NgramReader::m_file = NULL;
HANDLE NgramReader::m_map = NULL;
void* NgramReader::m_data = NULL;

bool NgramReader::OpenData(TCHAR * file)
{
	/* Open a file */
    m_file = CreateFile(file, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	XTRACK(m_file);
	if (m_file == NULL || m_file == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	/* Map a view of the file */
    m_map = CreateFileMapping(m_file, NULL, PAGE_READONLY, 0, GetFileSize(m_file, NULL), NULL);
	XTRACK(m_map);
	if (m_map == NULL || m_map == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	/* Get a pointer to the mapped view */
	m_data = MapViewOfFile(m_map, FILE_MAP_READ, 0, 0, GetFileSize(m_file, NULL));
	XTRACK(m_data);
	if (m_data == NULL)
	{
		return false;
	}

	return true;
}

int NgramReader::GetDataSize()
{
	return (int)GetFileSize(m_file, NULL);
}

void NgramReader::CloseData()
{
	/* Close open resources */
	XUNTRACK(m_data);
    UnmapViewOfFile(m_data);
	XUNTRACK(m_map);
    CloseHandle(m_map);
	XUNTRACK(m_file);
    CloseHandle(m_file);

	m_data = NULL;
	m_map = NULL;
	m_file = NULL;
}

int NgramReader::ReadValue(int index)
{
	/* Read an int from the front of the file */
	return ((int*)m_data)[index];
}

static inline int GET_NEXT_FLAT(unsigned char * list)
{
	static int off = 0;

	if (list)
	{
		int mult = 1;
		int value = 0;
		while (true)
		{
			int val = list[off];
			off++;
			if (val < 0x80)
			{
				value += val * mult;
				break;
			}
			else
			{
				value += (val & 0x7f) * mult;
				mult *= 0x80;
			}
		}
		return value;
	}
	else
	{
		off = 0;
		return 0;
	}
}

NgramReader::NgramReader(int offset)
{
	/* Remember the offset for later */
	m_offset = offset;

	/* The offset to the bytes */
	m_dataBytes = &(((unsigned char*)m_data)[m_offset]);

	m_count = ((int*)(m_dataBytes))[0];

	/* The offset to the main matrix data */
	unsigned char* temp = (unsigned char*)(&m_dataBytes[((int*)(m_dataBytes))[1]]);

	/* Expand the main matrix */
	GET_NEXT_FLAT(NULL);
	int left = 69 * 69 * 69;
	int off = 0;
	int index = 1;

	m_dataMain = new int[69 * 69 * 69];
	XTRACK(m_dataMain);

	while (left > 0)
	{
		int zeros = GET_NEXT_FLAT(temp);

		for (int i = 0; i < zeros; i++)
		{
			m_dataMain[off] = 0;
			off++;
			left--;
		}

		if (left > 0)
		{
			m_dataMain[off] = index;
			left--;
			off++;
			index++;
		}
	}

	/* And grab the other offsets */
	m_dataWordJumps = (int*)(&m_dataBytes[((int*)(m_dataBytes))[2]]);
	m_dataNgramJumps = (int*)(&m_dataBytes[((int*)(m_dataBytes))[3]]);
}

int NgramReader::GetCount()
{
	return m_count;
}

NgramReader::~NgramReader()
{
	XUNTRACK(m_dataMain);
	delete[] m_dataMain;
}

#define WORDJUMP(i) ((char*)&m_dataBytes[m_dataWordJumps[i]])

static inline int GET_PACKED_VALUE(unsigned char ** list)
{
	int mult = 1;
	int value = 0;

	while (true)
	{
		int val = (*list)[0];
		*list = &((*list)[1]);
		if (val < 0x80)
		{
			value += val * mult;
			break;
		}
		else
		{
			value += (val & 0x7f) * mult;
			mult *= 0x80;
		}
	}

	return value;
}

char * NgramReader::GetFullWord(int index)
{
	size_t len = 0;
	int temp = index;
	while (temp != -1)
	{
		unsigned char * word = (unsigned char*) WORDJUMP(temp);
		temp = GET_PACKED_VALUE(&word) - 1;
		len += strlen((char*)word);
	}

	char * ret = new char[len + 1];
	ret[len] = 0;
	char * cur = ret + len;

	temp = index;
	while (temp != -1)
	{
		unsigned char * word = (unsigned char*) WORDJUMP(temp);
		temp = GET_PACKED_VALUE(&word) - 1;
		len = strlen((char*)word);
		cur -= len;
		memcpy(cur, word, len);
	}

	return ret;
}

TCHAR* NgramReader::CopyToTchar(char * value)
{
	/* Helper to copy a string to a TCHAR */
#ifdef UNICODE
	size_t len = strlen(value) + 2;
	TCHAR * ret = new TCHAR[len];
	XTRACK(ret);
	memset(ret, 0, sizeof(TCHAR) * len);
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, value, (int)strlen(value), ret, (int)len);
	return ret;
#else
	size_t len = strlen(value) + 1;
	char * ret = new char[len];
	XTRACK(ret);
	strcpy(ret, value);
	return ret;
#endif
}

char* NgramReader::CopyFromTchar(TCHAR * value)
{
	/* Helper to copy a TCHAR to a char */
#ifdef UNICODE
	size_t len = wcslen(value);
	int needed = WideCharToMultiByte(CP_UTF8, 0, value, (int)len, NULL, 0, NULL, NULL);
	needed += 2;
	char * temp = new char[needed];
	XTRACK(temp);
	memset(temp, 0, needed);
	WideCharToMultiByte(CP_UTF8, 0, value, (int)len, temp, needed, NULL, NULL);

	return temp;
#else
	size_t len = strlen(value) + 1;
	char * ret = new char[len];
	XTRACK(ret);
	strcpy(ret, value);
	return ret;
#endif
}

void NgramReader::FindPhrasesT(TCHAR ** phrases, Array<char*> * words, Array<int> * indexes)
{
	/* Find all matches of all phrases */
#ifdef UNICODE
	int count = 0;
	while (phrases[count])
	{
		count++;
	}
	char ** temp = new char*[count + 1];
	XTRACK(temp);

	count = 0;
	while (phrases[count])
	{
		temp[count] = CopyFromTchar(phrases[count]);
		count++;
		temp[count] = NULL;
	}

	FindPhrases(temp, words, indexes);

	count = 0;
	while (phrases[count])
	{
		XUNTRACK(temp[count]);
		delete[] temp[count];
		count++;
	}

	XUNTRACK(temp);
	delete[] temp;

#else
	FindPhrases(phrases, words, indexes);
#endif
}

void NgramReader::Search(char * phrase, int ** pList1, int ** pList2, int * pCount)
{
	int a = -1;
	int b = -1;
	int c = -1;

	if (strlen(phrase) == 1)
	{
		/* For one character, use all possible values for the other two characters */
		int c = ngram_lookup[phrase[0]];

		Array<int> target;

		if (c != -1)
		{
			for (int a = 0; a < 69; a++)
			{
				for (int b = 0; b < 69; b ++)
				{
					/* Look for "..x" */
					int offset = m_dataMain[a + b * 69 + c * 4761];
					if (offset != 0)
					{
						Union(&m_dataBytes[m_dataNgramJumps[offset]], &target);
					}

					/* Look for ".x." */
					offset = m_dataMain[a + c * 69 + b * 4761];
					if (offset != 0)
					{
						Union(&m_dataBytes[m_dataNgramJumps[offset]], &target);
					}

					/* Look for "x.." */
					offset = m_dataMain[c + a * 69 + b * 4761];
					if (offset != 0)
					{
						Union(&m_dataBytes[m_dataNgramJumps[offset]], &target);
					}
				}
			}
		}

		Intersect(&target, pList1, pList2, pCount);
	}
	else if (strlen(phrase) == 2)
	{
		/* The phrase is just two characters, make up the missing third */
		int b = ngram_lookup[phrase[0]];
		int c = ngram_lookup[phrase[1]];

		Array<int> target;

		if (b != -1 && c != -1)
		{
			for (int a = 0; a < 69; a++)
			{
				/* Look for "xx." */
				int offset = m_dataMain[a + b * 69 + c * 4761];
				if (offset != 0)
				{
					Union(&m_dataBytes[m_dataNgramJumps[offset]], &target);
				}

				/* Look for ".xx" */
				offset = m_dataMain[b + c * 69 + a * 4761];
				if (offset != 0)
				{
					Union(&m_dataBytes[m_dataNgramJumps[offset]], &target);
				}
			}
		}

		Intersect(&target, pList1, pList2, pCount);
	}
	else
	{
		/* For each three leters in the source phrase */
		for (char* cur = phrase; *cur; cur++)
		{
			a = b;
			b = c;
			if (*cur < 255)
			{
				c = ngram_lookup[*cur];
			}
			else
			{
				c = -1;
			}

			/* If it's a valid trigram, look it up */
			if (a != -1 && b != -1 && c != -1)
			{
				int offset = m_dataMain[a + b * 69 + c * 4761];
				/* Find the list of matches for this trigram */
				if (offset == 0)
				{
					/* There's nothing in the list, just empty out our final list and give up */
					*pCount = 0;
					break;
				}

				unsigned char * list = &m_dataBytes[m_dataNgramJumps[offset]];
				Intersect(list, pList1, pList2, pCount);
			}
		}
	}
}

static inline int GET_NEXT(unsigned char * list)
{
	static int last = 0;
	static int off = 4;
	if (list)
	{
		int mult = 1;
		int value = 0;
		while (true)
		{
			int val = list[off];
			off++;
			if (val < 0x80)
			{
				value += val * mult;
				break;
			}
			else
			{
				value += (val & 0x7f) * mult;
				mult *= 0x80;
			}
		}
		last += value;
		return last;
	}
	else
	{
		last = 0;
		off = 4;
		return 0;
	}
}

static inline int GET_COUNT(unsigned char * list)
{
	int ret = 0;
	memcpy(&ret, list, 4);
	GET_NEXT(NULL);
	return ret;
}

void NgramReader::Union(unsigned char * list, Array<int> * target)
{
	/* Compute the union of two lists */
	Array<int> temp;

	int count = GET_COUNT(list);
	int i = 0;
	int j = 0;
	int val = 0;
	
	if (i < count)
	{
		GET_NEXT(list);
	}

	while (i < count && j < target->Count)
	{
		if (val < target->List[j])
		{
			temp.Add(val);
			i++;
			if (i < count)
			{
				val = GET_NEXT(list);
			}
		}
		else if (val > target->List[j])
		{
			temp.Add(target->List[j]);
			j++;
		}
		else
		{
			temp.Add(target->List[j]);
			i++;
			j++;
			if (i < count)
			{
				val = GET_NEXT(list);
			}
		}
	}

	while (i < count)
	{
		temp.Add(val);
		i++;
		if (i < count)
		{
			val = GET_NEXT(list);
		}
	}

	while (j < target->Count)
	{
		temp.Add(target->List[j]);
		j++;
	}

	target->Count = 0;
	for (i = 0; i < temp.Count; i ++)
	{
		target->Add(temp.List[i]);
	}
}

void NgramReader::Intersect(Array<int> * pList, int ** pList1, int ** pList2, int * pCount)
{
	/* Intersect two lists list and pList1, using pList2 as a buffer */
	if (*pList1)
	{
		/* We already have a list, calculate the intersection of the two lists */
		int i = 0;
		int dest = 0;
		int j = 0;
		int count = pList->Count;
		int val = 0;

		if (j < count)
		{
			val = pList->List[j];
		}

		while (i < *pCount && j < count)
		{
			if ((*pList1)[i] < val)
			{
				i++;
			}
			else if ((*pList1)[i] > val)
			{
				j++;
				if (j < count)
				{
					val = pList->List[j];
				}
			}
			else
			{
				(*pList2)[dest] = val;
				dest++;
				i++;
				j++;
				if (j < count)
				{
					val = pList->List[j];
				}
			}
		}

		int* temp = *pList1;
		*pList1 = *pList2;
		*pList2 = temp;
		*pCount = dest;
	}
	else
	{
		int count = pList->Count;
		*pList1 = new int[count];
		XTRACK(*pList1);
		*pList2 = new int[count];
		XTRACK(*pList2);
		*pCount = count;

		for (int i = 0; i < count; i ++)
		{
			(*pList1)[i] = pList->List[i];
		}
	}
}

void NgramReader::Intersect(unsigned char * list, int ** pList1, int ** pList2, int * pCount)
{
	/* Intersect two lists list and pList1, using pList2 as a buffer */
	if (*pList1)
	{
		/* We already have a list, calculate the intersection of the two lists */
		int i = 0;
		int dest = 0;
		int j = 0;
		int count = GET_COUNT(list);
		int val = 0;

		if (j < count)
		{
			val = GET_NEXT(list);
		}

		while (i < *pCount && j < count)
		{
			if ((*pList1)[i] < val)
			{
				i++;
			}
			else if ((*pList1)[i] > val)
			{
				j++;
				if (j < count)
				{
					val = GET_NEXT(list);
				}
			}
			else
			{
				(*pList2)[dest] = val;
				dest++;
				i++;
				j++;
				if (j < count)
				{
					val = GET_NEXT(list);
				}
			}
		}

		int* temp = *pList1;
		*pList1 = *pList2;
		*pList2 = temp;
		*pCount = dest;
	}
	else
	{
		int count = GET_COUNT(list);
		*pList1 = new int[count];
		XTRACK(*pList1);
		*pList2 = new int[count];
		XTRACK(*pList2);
		*pCount = count;

		for (int i = 0; i < count; i ++)
		{
			(*pList1)[i] = GET_NEXT(list);
		}
	}
}

void NgramReader::FindPhrases(char ** phrases, Array<char*> * words, Array<int> * indexes)
{
	/* Find a phrase in the final output file */
	if (words)
	{
		words->Count = 0;
	}

	if (indexes)
	{
		indexes->Count = 0;
	}

	int * list1 = NULL;
	int * list2 = NULL;
	int count = 0;

	for (int phraseIndex = 0; phrases[phraseIndex]; phraseIndex++)
	{
		/* We found something, but it's empty, so give up */
		if (list1 && !count)
		{
			break;
		}

		char* phrase = phrases[phraseIndex];

		Search(phrase, &list1, &list2, &count);

		int next = 0;
		for (int i = 0; i < count; i ++)
		{
			/* Check each word one last time for a match, to eliminate false positives */
			int index = list1[i];
			char * word = GetFullWord(index);

			bool found = false;
			size_t len = strlen(phrase);
			for (char* cur = word; *cur; cur++)
			{
				if (_strnicmp(cur, phrase, len) == 0)
				{
					found = true;
					break;
				}
			}

			delete[] word;

			if (found)
			{
				list2[next] = list1[i];
				next++;
			}
		}

		int* temp = list1;
		list1 = list2;
		list2 = temp;
		count = next;

		if (count == 0)
		{
			break;
		}
	}

	/* And return the final list we ended up with */
	for (int i = 0; i < count; i ++)
	{
		/* Check each word one last time for a match, to elimate false positives */
		int index = list1[i];
		char * word = GetFullWord(index);

		if (indexes)
		{
			indexes->Add(index);
		}

		if (words)
		{
			words->Add(word);
		}
		else
		{
			delete[] word;
		}
	}

	XUNTRACK(list1);
	delete[] list1;
	XUNTRACK(list2);
	delete[] list2;
}


void NgramReader::FindPhrase(char * phrase, Array<char*> * words, Array<int> * indexes)
{
	/* Find a phrase in the final output file */
	if (words)
	{
		words->Count = 0;
	}

	if (indexes)
	{
		indexes->Count = 0;
	}

	int * list1 = NULL;
	int * list2 = NULL;
	int count = 0;

	Search(phrase, &list1, &list2, &count);

	/* And return the final list we ended up with */
	for (int i = 0; i < count; i ++)
	{
		/* Check each word one last time for a match, to elimate false positives */
		int index = list1[i];
		char * word = GetFullWord(index);

		bool found = false;
		size_t len = strlen(phrase);
		for (char* cur = word; *cur; cur++)
		{
			if (_strnicmp(cur, phrase, len) == 0)
			{
				found = true;
				break;
			}
		}

		if (found)
		{
			if (indexes)
			{
				indexes->Add(index);
			}

			if (words)
			{
				words->Add(word);
			}
			else
			{
				delete[] word;
			}
		}
	}

	XUNTRACK(list1);
	delete[] list1;
	XUNTRACK(list2);
	delete[] list2;
}
