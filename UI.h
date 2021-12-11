#pragma once

class UI
{
public:
            UI(LinkedList<Result> * pllFound);
            ~UI();

    TCHAR*  Show();

private:
    void    InitConsole();
    void    DisplayUI();
    void    SetChar(int x, int y, TCHAR c, WORD a);
    bool    HandleKey(WORD nVK, WCHAR wChar);
    void    CalcMatches();

    int         m_nMatches;
    int         m_nMatchOnlyDir;
    size_t      m_nMaxLen;
    LinkedList<Result>* m_pllFound;
    bool        m_bRecalcSquare;
    TCHAR       m_szFilter[MAX_PATH];

    TCHAR *     m_szReturnPath;
    CONSOLE_SCREEN_BUFFER_INFO m_csbi;
    CONSOLE_CURSOR_INFO m_cciOriginal;
    CONSOLE_CURSOR_INFO m_cciHidden;
    HANDLE      m_hConsoleOutput;
    HANDLE      m_hConsoleInput;
    SMALL_RECT  m_srSaved;
    COORD       m_cSize;
    COORD       m_cZero;
    LinkedListItem<Result> *m_pCurSel;
    CHAR_INFO * m_ciOriginal;
    CHAR_INFO * m_ciTemp;
    CHAR_INFO * m_ciLastDisplay;
    CHAR_INFO * m_ciCopy;
    int         m_nOffsetX;
    int         m_nOffsetY;
    size_t      m_nMaxLineLen;
    int         m_nMaxLines;
    int         m_nCurSel;
    int         m_nStartShow;

};
