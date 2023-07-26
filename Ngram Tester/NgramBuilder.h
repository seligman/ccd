#pragma once

class NgramBuilder
{
public:
	void AddWordT(TCHAR * word);
	void AddWord(char * word);
	bool Write();
	NgramBuilder();
	~NgramBuilder();

	static void WriteValue(int32_t value);
	static DWORD GetOffset();
	static void Seek(DWORD pos);
	static bool OpenData(TCHAR * file);
	static void CloseData();

private:
	Array<int>*		m_pGrams[69 * 69 * 69];
	Array<char*>	m_words;

	static HANDLE	m_file;
};
