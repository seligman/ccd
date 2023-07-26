#pragma once

class HashItem;

class HashList
{
public:
	HashList();
	~HashList();
	void Add(char * value);
	int GetValueID(char * value, size_t len);
	void Add(char * value, char ** pData, size_t * pDataLen, int * pLastID);
	void Clear();

	int Count;
	int NumBuckets;
	HashItem ** Buckets;
};
