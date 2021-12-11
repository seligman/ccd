#pragma once

class NextLine
{
public:
            NextLine();
            NextLine(HANDLE hFile);
            ~NextLine();

    void    Init(HANDLE hFile);
    bool    HasNext();
    LPTSTR  Line();

private:
    void    ClearData();

    HANDLE  m_hFile;

    DWORD   m_nLenData;
    DWORD   m_nMaxLen;
    char*   m_szData;
    char*   m_szData2;
    char*   m_szCurPos;
    LPTSTR  m_szLine;
    DWORD   m_cbLine;
};

