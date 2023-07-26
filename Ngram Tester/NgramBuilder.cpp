#include "main.h"
#include "xtrack.h"
#include "Array.h"
#include "HashList.h"
#include "Ngram.h"
#include "NgramBuilder.h"

HANDLE NgramBuilder::m_file = NULL;

NgramBuilder::NgramBuilder()
{
	memset(m_pGrams, 0, (69 * 69 * 69) * sizeof(Array<int>*));
}

void NgramBuilder::AddWordT(TCHAR * word)
{
#ifdef UNICODE
	size_t len = wcslen(word);
	int needed = WideCharToMultiByte(CP_UTF8, 0, word, (int)len, NULL, 0, NULL, NULL);
	needed += 2;
	char * temp = new char[needed];
	XTRACK(temp);
	memset(temp, 0, needed);
	WideCharToMultiByte(CP_UTF8, 0, word, (int)len, temp, needed, NULL, NULL);

	AddWord(temp);

	XUNTRACK(temp);
	delete[] temp;
#else
	AddWord(word);
#endif
}

void NgramBuilder::AddWord(char * word)
{
	int a = -1;
	int b = -1;
	int c = -1;

	/* Go through each threee leters and add this word to the trigram index */

	size_t len = strlen(word);

	if (len == 2)
	{
		c = 0;
	}
	else if (len == 1)
	{
		c = 0;
		b = 0;
	}

	for (char* cur = word; *cur; cur++)
	{
		a = b;
		b = c;
		if (*cur <= 128 && *cur >= 0)
		{
			c = ngram_lookup[*cur];
		}
		else
		{
			c = -1;
		}

		if (a != -1 && b != -1 && c != -1)
		{
			bool add = true;

			if (m_pGrams[a + b * 69 + c * 4761])
			{
				if (m_pGrams[a + b * 69 + c * 4761]->List[m_pGrams[a + b * 69 + c * 4761]->Count - 1] == m_words.Count)
				{
					add = false;
				}
			}

			if (add)
			{
				if (!m_pGrams[a + b * 69 + c * 4761])
				{
					m_pGrams[a + b * 69 + c * 4761] = new Array<int>();
					XTRACK(m_pGrams[a + b * 69 + c * 4761]);
				}

				m_pGrams[a + b * 69 + c * 4761]->Add(m_words.Count);
			}
		}
	}

	/* Add this word to the list of words */ 
	len++;
	char * value = new char[len];
	XTRACK(value);
	strcpy_s(value, len, word);
	m_words.Add(value);
}

#define BUFFER_SIZE 5000

#define ADD_BYTE(b) \
{ \
	loc++; \
	if (loc == BUFFER_SIZE) \
	{ \
		WriteFile(m_file, values, BUFFER_SIZE, &wrote, NULL); \
		loc = 0; \
	} \
	values[loc] = (b); \
	total++; \
}

#define ADD_PACKED(p) \
{ \
	int packed = p; \
	while (packed >= 128) \
	{ \
		ADD_BYTE(0x80 | (unsigned char)((packed) & 0x7f)); \
		packed >>= 7; \
	} \
	ADD_BYTE((unsigned char)packed); \
}

#define ADD_DWORD(d) \
{ \
	int add_val = (d); \
	ADD_BYTE(((unsigned char*)(&(add_val)))[0]); \
	ADD_BYTE(((unsigned char*)(&(add_val)))[1]); \
	ADD_BYTE(((unsigned char*)(&(add_val)))[2]); \
	ADD_BYTE(((unsigned char*)(&(add_val)))[3]); \
}

#define FLUSH_BYTE() \
{ \
	loc++; \
	WriteFile(m_file, values, loc, &wrote, NULL); \
	loc = -1; \
}

void NgramBuilder::Seek(DWORD val)
{
	SetFilePointer(m_file, val, NULL, FILE_BEGIN);
}

DWORD NgramBuilder::GetOffset()
{
	LARGE_INTEGER cur = {0};
	LARGE_INTEGER zero = {0};
	SetFilePointerEx(m_file, zero, &cur, FILE_CURRENT);
	return cur.LowPart;
}

#define OFFSET() (total)

bool NgramBuilder::Write()
{
	unsigned char * values = new unsigned char[BUFFER_SIZE];
	XTRACK(values);
	DWORD wrote = 0;
	DWORD total = 0;
	int loc = -1;

	DWORD pointers = GetOffset();

	DWORD count = 0;
	DWORD ptrMain = 0;
	DWORD ptrWordJumps = 0;
	DWORD ptrNgramJumps = 0;

	/* Space for pointers */
	ADD_DWORD(count);
	ADD_DWORD(ptrMain);			/* Main matrix */
	ADD_DWORD(ptrWordJumps);	/* Word jumps */
	ADD_DWORD(ptrNgramJumps);	/* Ngram jumps  */
	
	/* Write the main matrix */
	ptrMain = OFFSET();

	int zeros = 0;
	for (int i = 0; i < 69 * 69 * 69; i ++)
	{
		if (m_pGrams[i] > 0)
		{
			ADD_PACKED(zeros);
			zeros = 0;
		}
		else
		{
			zeros ++;
		}
	}

	if (zeros)
	{
		ADD_PACKED(zeros);
	}

	/* Pad to DWORD boundary */
	while ((OFFSET() % 4) != 0)
	{
		ADD_BYTE(0);
	}

	/* Write the ngrams, and note locations */
	Array<int> ngramJumps;
	/* Since 0 is a magic number, add a place for it here */
	ngramJumps.Add(0);
	for (int i = 0; i < 69 * 69 * 69; i ++)
	{
		if (m_pGrams[i] > 0)
		{
			ngramJumps.Add(OFFSET());

			ADD_DWORD(m_pGrams[i]->Count);
			int last = 0;
			for (int j = 0; j < m_pGrams[i]->Count; j ++)
			{
				ADD_PACKED(m_pGrams[i]->List[j] - last);
				last = m_pGrams[i]->List[j];
			}
		}
	}

	/* Write the words, and note locations */
	Array<int> wordJumps;
	HashList hash;
	for (int i = 0; i < m_words.Count; i++)
	{
		wordJumps.Add(OFFSET());

		int lastID = 0;
		char * data = NULL;
		size_t len = 0;

		hash.Add(m_words.List[i], &data, &len, &lastID);

		ADD_PACKED(lastID);

		for (size_t j = 0; j < len; j ++)
		{
			ADD_BYTE(data[j]);
		}

		XUNTRACK(data);
		delete[] data;
	}
	hash.Clear();

	/* Pad to DWORD boundary */
	while ((OFFSET() % 4) != 0)
	{
		ADD_BYTE(0);
	}

	/* Write the word jumps */
	ptrWordJumps = OFFSET();
	for (int i = 0; i < wordJumps.Count; i ++)
	{
		ADD_DWORD(wordJumps.List[i]);
	}

	/* Write the ngram jumps */
	ptrNgramJumps = OFFSET();
	for (int i = 0; i < ngramJumps.Count; i ++)
	{
		ADD_DWORD(ngramJumps.List[i]);
	}

	/* Done, go back and write the pointers */
	FLUSH_BYTE();

	DWORD offset = GetOffset();
	Seek(pointers);
	ADD_DWORD(m_words.Count);
	ADD_DWORD(ptrMain); /* Main matrix */
	ADD_DWORD(ptrWordJumps); /* Word jumps */
	ADD_DWORD(ptrNgramJumps); /* Ngram jumps  */
	FLUSH_BYTE();
	Seek(offset);

	XUNTRACK(values);
	delete[] values;

	return true;
}

bool NgramBuilder::OpenData(TCHAR * file)
{
	/* Open a file for writing */
    m_file = CreateFile(file, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL);
	XTRACK(m_file);

	if (m_file == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else
	{
		return true;
	}
}

NgramBuilder::~NgramBuilder()
{
	/* Clean up */
	for (int i = 0; i < m_words.Count; i ++)
	{
		XUNTRACK(m_words.List[i]);
		delete[] m_words.List[i];
	}

	for (int i = 0; i < 69 * 69 * 69; i ++)
	{
		if (m_pGrams[i])
		{
			XUNTRACK(m_pGrams[i]);
			delete m_pGrams[i];
		}
	}
}

void NgramBuilder::CloseData()
{
	/* Close the open file */
	XUNTRACK(m_file);
	CloseHandle(m_file);
	m_file = NULL;
}

void NgramBuilder::WriteValue(int32_t value)
{
	/* Write a single int to the file */
	DWORD wrote = 0;
	WriteFile(m_file, &value, 4, &wrote, NULL);
}
