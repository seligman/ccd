#pragma once

class NgramReader
{
public:
	static bool OpenData(TCHAR * file);
	static void CloseData();
	static int ReadValue(int index);
	static TCHAR* CopyToTchar(char * value);
	static char* CopyFromTchar(TCHAR * value);

	NgramReader(int offset);
	~NgramReader();

	char * GetFullWord(int index);
	void FindPhrase(char * phrase, Array<char*> * words, Array<int> * indexes);
	void FindPhrases(char ** phrase, Array<char*> * words, Array<int> * indexes);
	void FindPhrasesT(TCHAR ** phrase, Array<char*> * words, Array<int> * indexes);
	int GetCount();
	int GetDataSize();

private:
	void Search(char * phrase, int ** pList1, int ** pList2, int * pCount);
	void Intersect(unsigned char * list, int ** pList1, int ** pList2, int * pCount);
	void Intersect(Array<int> * pList, int ** pList1, int ** pList2, int * pCount);
	void Union(unsigned char * list, Array<int> * target);

	int m_offset;
	int m_count;

	int* m_dataMain;
	int* m_dataWordJumps;
	int* m_dataNgramJumps;
	unsigned char* m_dataBytes;

	static HANDLE m_file;
	static HANDLE m_map;
	static void* m_data;
};
