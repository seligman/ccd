#include "main.h"
#include "xtrack.h"
#include "crc32.h"
#include "HashList.h"

/* An item in the hash list */
class HashItem
{
public:
	HashItem(int index, char * value, HashItem * next)
	{
		/* Copy the value, don't free on the destroy */
		size_t len = strlen(value) + 1;
		Value = new char[len];
		XTRACK(Value);
		strcpy_s(Value, len, value);

		Index = index;
		Next = next;
	}

	char * Value;
	int Index;
	HashItem * Next;
};

HashList::HashList()
{
	/* Initialize basic data structures */
	Count = 0;
	NumBuckets = 100;
	Buckets = new HashItem*[NumBuckets];
	XTRACK(Buckets);
	memset(Buckets, 0, NumBuckets * sizeof(HashItem*));
}

void HashList::Clear()
{
	/* Delete all the entries in each bucket */
	for (int i = 0; i < NumBuckets; i ++)
	{
		HashItem * cur = Buckets[i];
		while (cur)
		{
			HashItem * next = cur->Next;
			XUNTRACK(cur->Value);
			delete[] cur->Value;
			XUNTRACK(cur);
			delete cur;
			cur = next;
		}
	}

	/* Delete remaining data */
	XUNTRACK(Buckets);
	delete[] Buckets;
	Buckets = NULL;
	NumBuckets = 0;
	Count = 0;
}

HashList::~HashList()
{
	/* Clean all memory */
	Clear();
}

void HashList::Add(char * value)
{
	if (Count >= NumBuckets)
	{
		/* We have too many items for the number of buckets, 
		   so increase the number of buckets and move everything */
		int nextNum = NumBuckets * 2;
		HashItem ** next = new HashItem*[nextNum];
		XTRACK(next);
		memset(next, 0, nextNum * sizeof(HashItem*));

		for (int i = 0; i < NumBuckets; i ++)
		{
			HashItem * cur = Buckets[i];
			while (cur)
			{
				/* Pull out each item, calculate the new bucket ID, and store it */
				HashItem * nextCur = cur->Next;

				int bucket = crc32_calc((unsigned char*)cur->Value, strlen(cur->Value)) % nextNum;
				cur->Next = next[bucket];
				next[bucket] = cur;

				cur = nextCur;
			}
		}

		/* Free the old array of buckets and track the new one */
		XUNTRACK(Buckets);
		delete[] Buckets;
		Buckets = next;
		NumBuckets = nextNum;
	}

	/* Calc the bucket ID and store the item to the linked list for that bucket */
	int bucket = crc32_calc((unsigned char*)value, strlen(value)) % NumBuckets;
	Buckets[bucket] = new HashItem(Count, value, Buckets[bucket]);
	XTRACK(Buckets[bucket]);

	Count++;
}

/* Attempt to look up an item, returns ID or -1 if not found */
int HashList::GetValueID(char * value, size_t len)
{
	/* Find the bucket */
	int bucket = crc32_calc((unsigned char*)value, len) % NumBuckets;
	HashItem * cur = Buckets[bucket];
	while (cur)
	{
		/* See if this item matches */
		if ((strlen(cur->Value)) == len)
		{
			if (strncmp(value, cur->Value, len) == 0)
			{
				/* It matches!  return it */
				return cur->Index;
			}
		}

		cur = cur->Next;
	}

	/* No match found */
	return -1;
}

/* Add a new item to the list */
void HashList::Add(char * value, char ** pData, size_t * pDataLen, int * pLastID)
{
	/* Find the best previous match */
	size_t len = strlen(value);
	int id = -1;
	while (len > 0)
	{
		id = GetValueID(value, len);
		if (id >= 0)
		{
			break;
		}
		len--;
	}

	Add(value);

	/* Store the item, using the previous match to make the string shorter */
	*pDataLen = strlen(value) - len + 1;
	*pData = new char[*pDataLen];
	XTRACK(*pData);
	*pLastID = id + 1;
	memcpy(*pData, &(value[len]), *pDataLen);
}
