#include "main.h"
#include "xtrack.h"
#include "LinkedList.h"
#include "UI.h"
#include "Hidden.h"

#define NORMAL_COLOR (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_GREEN)
#define SEL_COLOR (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)

UI::UI(LinkedList<Result> * pllFound)
{
    m_bRecalcSquare = true;

    _tcscpy_s(m_szFilter, _T(""));
    m_pllFound = pllFound;
}

void UI::CalcMatches()
{
    m_nMatches = 0;
    m_nMatchOnlyDir = 0;
    m_nMaxLen = 0;

    TCHAR szFilters[MAX_PATH+1];
    size_t nLen = 0;
    memset(szFilters, 0, (MAX_PATH+1) * sizeof TCHAR);
    _tcscpy_s(szFilters, m_szFilter);
	_tcscat_s(szFilters, HIDEW_SPACESPACE);
    nLen = _tcslen(szFilters);
    for (size_t i = 0; i < nLen; i ++)
    {
        if (szFilters[i] == _T(' '))
        {
            szFilters[i] = 0;
        }
    }

    for (LinkedListItem<Result> * cur = m_pllFound->GetHead(); cur; cur = cur->pNext)
    {
        cur->Data.bHidden = false;
    }

    for (TCHAR * szFilter = szFilters; *szFilter; szFilter += _tcslen(szFilter) + 1)
    {
        bool bNegate = false;
        if (*szFilter == _T('-'))
        {
            szFilter ++;
            bNegate = true;
        }

        for (LinkedListItem<Result> * cur = m_pllFound->GetHead(); cur; cur = cur->pNext)
        {
            bool bNewState = true;
            size_t nLen = _tcslen(szFilter);
            if (nLen > 0)
            {
                for (TCHAR * szCur = cur->Data.szFullName; *szCur; szCur++)
                {
                    if (_tcsnicmp(szCur, szFilter, nLen) == 0)
                    {
                        bNewState = false;
                        break;
                    }
                }

                if (bNegate)
                {
                    bNewState = !bNewState;
                }
            }
            else
            {
                bNewState = false;
            }

            if (!cur->Data.bHidden && bNewState)
            {
                cur->Data.bHidden = bNewState;
            }
        }
    }

    for (LinkedListItem<Result> * cur = m_pllFound->GetHead(); cur; cur = cur->pNext)
    {
        if (cur->Data.bHidden == false)
        {
            m_nMatches ++;
            size_t nLen = _tcslen(cur->Data.szFullName);
            if (nLen > m_nMaxLen)
            {
                m_nMaxLen = nLen;
            }

            if (cur->Data.bOnlyDir == true)
            {
                m_nMatchOnlyDir ++;
            }
        }
    }
}

void UI::InitConsole()
{
    m_szReturnPath = NULL;

    m_hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    m_hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
    m_pCurSel = NULL;

    GetConsoleScreenBufferInfo(m_hConsoleOutput, &m_csbi);

    m_srSaved = m_csbi.srWindow;
    m_cSize.X = (m_srSaved.Right - m_srSaved.Left + 1);
    m_cSize.Y = (m_srSaved.Bottom - m_srSaved.Top + 1);
    m_cZero.X = 0;
    m_cZero.Y = 0;

    m_ciOriginal = new CHAR_INFO[m_cSize.X * m_cSize.Y];
	XTRACK(m_ciOriginal);
    m_ciTemp = new CHAR_INFO[m_cSize.X * m_cSize.Y];
	XTRACK(m_ciTemp);
    m_ciLastDisplay = new CHAR_INFO[m_cSize.X * m_cSize.Y];
	XTRACK(m_ciLastDisplay);
    m_ciCopy = new CHAR_INFO[m_cSize.X * m_cSize.Y];
	XTRACK(m_ciCopy);

    ReadConsoleOutput(m_hConsoleOutput, m_ciOriginal, m_cSize, m_cZero, &m_srSaved);
    ReadConsoleOutput(m_hConsoleOutput, m_ciCopy, m_cSize, m_cZero, &m_srSaved);
    ReadConsoleOutput(m_hConsoleOutput, m_ciLastDisplay, m_cSize, m_cZero, &m_srSaved);
    GetConsoleCursorInfo(m_hConsoleOutput, &m_cciOriginal);
    m_cciHidden.bVisible = FALSE;
    m_cciHidden.dwSize = 100;
    SetConsoleCursorInfo(m_hConsoleOutput, &m_cciHidden);

    m_nCurSel = 0;
    m_nStartShow = 0;
}

UI::~UI()
{
	XUNTRACK(m_ciOriginal);
    delete(m_ciOriginal);
	XUNTRACK(m_ciTemp);
    delete(m_ciTemp);
	XUNTRACK(m_ciLastDisplay);
    delete(m_ciLastDisplay);
	XUNTRACK(m_ciCopy);
    delete(m_ciCopy);
}

void UI::DisplayUI()
{
    if (m_bRecalcSquare)
    {
        CalcMatches();

        m_bRecalcSquare = false;
        m_nOffsetX = 0;
        m_nOffsetY = 0;

        m_nCurSel = 0;
        m_nStartShow = 0;

        m_nMaxLineLen = m_nMaxLen;
        m_nMaxLines = m_nMatches;
        if (m_nMaxLineLen > (size_t)(m_cSize.X - 4))
        {
            m_nMaxLineLen = m_cSize.X - 4;
        }
        if (m_nMaxLines > m_cSize.Y - 2)
        {
            m_nMaxLines = m_cSize.Y - 2;
        }

        if (m_nMaxLines == 0)
        {
            m_nMaxLines = 1;
        }
        if (m_nMaxLineLen == 0)
        {
            m_nMaxLineLen = 40;
        }

        m_nOffsetX = (m_cSize.X - (((int)m_nMaxLineLen) + 4)) / 2;
        m_nOffsetY = (m_cSize.Y - (((int)m_nMaxLines) + 2)) / 2;
        memcpy(m_ciCopy, m_ciOriginal, m_cSize.X * m_cSize.Y * sizeof(CHAR_INFO));
    }

	TCHAR szInfo[MAX_PATH + 100];
	
	if (*m_szFilter)
	{
		_stprintf_s(szInfo, HIDEW_FILTEREDMATCHES, m_nMatches, m_szFilter);
	}
	else
	{
		_stprintf_s(szInfo, HIDEW_FILTERED, m_nMatches);
	}

	SetConsoleTitle(szInfo);

    SetChar(0, 0, _T('\x250C'), NORMAL_COLOR);
    SetChar(((int)m_nMaxLineLen) + 3, 0, _T('\x2510'), NORMAL_COLOR);
    SetChar(((int)m_nMaxLineLen) + 3, m_nMaxLines + 1, _T('\x2518'), NORMAL_COLOR);
    SetChar(0, m_nMaxLines + 1, _T('\x2514'), NORMAL_COLOR);
    for (size_t i = 0; i < m_nMaxLineLen + 2; i ++)
    {
        SetChar(((int)i) + 1, 0, _T('\x2500'), NORMAL_COLOR);
        SetChar(((int)i) + 1, m_nMaxLines + 1, _T('\x2500'), NORMAL_COLOR);
    }
    for (int i = 0; i < m_nMaxLines; i ++)
    {
        SetChar(0, i + 1, _T('\x2502'), NORMAL_COLOR);
        SetChar(1, i + 1, _T(' '), NORMAL_COLOR);
        SetChar(((int)m_nMaxLineLen) + 2, i + 1, _T(' '), NORMAL_COLOR);
        SetChar(((int)m_nMaxLineLen) + 3, i + 1, _T('\x2502'), NORMAL_COLOR);
    }

    if (m_nMaxLines < m_nMatches)
    {
        int nOff = ((int)((double)(m_nMaxLines-1) * (double)m_nStartShow/(double)(m_nMatches - m_nMaxLines)));
        SetChar(((int)m_nMaxLineLen) + 3, 1 + nOff, _T('\x2550'), NORMAL_COLOR);
    }

    if (m_nMatches == 0)
    {
        TCHAR * szLine = HIDEW_NOMATCHESINPARENS;
        WORD wAttr = NORMAL_COLOR;
        size_t nCurCol = 0;
        for (TCHAR * szCur = szLine; *szCur; szCur ++)
        {
            SetChar(2, 1, *szCur, wAttr);
            nCurCol ++;
        }
        for (nCurCol = nCurCol; nCurCol < m_nMaxLineLen; nCurCol++)
        {
            SetChar(2 + ((int)nCurCol), 1, _T(' '), wAttr);
        }
    }

    int nCurLine = -m_nStartShow;
    for (LinkedListItem<Result> * cur = m_pllFound->GetHead(); cur; cur = cur->pNext)
    {
        if (cur->Data.bHidden == false)
        {
            if (nCurLine >= 0)
            {
                WORD wAttr = NORMAL_COLOR;
                if (nCurLine == m_nCurSel - m_nStartShow)
                {
                    wAttr = SEL_COLOR;
                    m_pCurSel = cur;
                }

                size_t nCurCol = 0;
                TCHAR * szLine = cur->Data.szFullName;
                size_t nLen = _tcslen(szLine);

                if (nLen > m_nMaxLineLen)
                {
                    SetChar(2 + ((int)nCurCol), 1 + nCurLine, _T('.'), wAttr);
                    nCurCol ++;
                    SetChar(2 + ((int)nCurCol), 1 + nCurLine, _T('.'), wAttr);
                    nCurCol ++;
                    SetChar(2 + ((int)nCurCol), 1 + nCurLine, _T('.'), wAttr);
                    nCurCol ++;
                    while (nLen > m_nMaxLineLen - 3)
                    {
                        nLen --;
                        szLine ++;
                    }
                }

                for (TCHAR * szCur = szLine; *szCur; szCur ++)
                {
                    SetChar(2 + ((int)nCurCol), 1 + nCurLine, *szCur, wAttr);
                    nCurCol ++;
                    if (nCurCol >= m_nMaxLineLen)
                    {
                        break;
                    }
                }
                for (nCurCol = nCurCol; nCurCol < m_nMaxLineLen; nCurCol++)
                {
                    SetChar(2 + ((int)nCurCol), 1 + nCurLine, _T(' '), wAttr);
                }

                if (nCurLine == m_nMaxLines - 1)
                {
                    break;
                }
            }
            nCurLine ++;
        }
    }

    COORD cMin;
    COORD cMax;
    bool bFound = false;

    for (int x = 0; x < m_cSize.X; x ++)
    {
        for (int y = 0; y < m_cSize.Y; y ++)
        {
            if (memcmp(&m_ciLastDisplay[x + y * m_cSize.X], &m_ciCopy[x + y * m_cSize.X], sizeof CHAR_INFO) != 0)
            {
                if (!bFound)
                {
                    bFound = true;
                    cMin.X = x;
                    cMin.Y = y;
                    cMax.X = x;
                    cMax.Y = y;
                }
                else
                {
                    if (x < cMin.X) { cMin.X = x; }
                    if (y < cMin.Y) { cMin.Y = y; }
                    if (x > cMax.X) { cMax.X = x; }
                    if (y > cMax.Y) { cMax.Y = y; }
                }
            }
        }
    }

    if (bFound)
    {
        COORD cTempSize;
        cTempSize.X = cMax.X - cMin.X + 1;
        cTempSize.Y = cMax.Y - cMin.Y + 1;
        for (int x = 0; x < cTempSize.X; x ++)
        {
            for (int y = 0; y < cTempSize.Y; y ++)
            {
                m_ciTemp[x + y * cTempSize.X] = m_ciCopy[(cMin.X + x) + (cMin.Y + y) * m_cSize.X];
            }
        }

        SMALL_RECT srTemp;
        srTemp.Left = m_srSaved.Left + cMin.X;
        srTemp.Top = m_srSaved.Top + cMin.Y;
        srTemp.Right = srTemp.Left + cTempSize.X;
        srTemp.Bottom = srTemp.Top + cTempSize.Y;
        WriteConsoleOutput(m_hConsoleOutput, m_ciTemp, cTempSize, m_cZero, &srTemp);
    }

    memcpy(m_ciLastDisplay, m_ciCopy, m_cSize.X * m_cSize.Y * sizeof(CHAR_INFO));
}

void UI::SetChar(int x, int y, TCHAR c, WORD a)
{
    m_ciCopy[(m_nOffsetX + x) + (m_nOffsetY + y) * m_cSize.X].Char.UnicodeChar = c;
    m_ciCopy[(m_nOffsetX + x) + (m_nOffsetY + y) * m_cSize.X].Attributes = a;
}

bool UI::HandleKey(WORD nVK, WCHAR wChar)
{
    bool bRet = true;

    switch (nVK)
    {
    case VK_BACK:
        if (_tcslen(m_szFilter) > 0)
        {
            m_szFilter[_tcslen(m_szFilter)-1] = 0;
        }
        m_bRecalcSquare = true;
        break;
    case VK_HOME:
        m_nCurSel = 0;
        break;
    case VK_END:
        m_nCurSel = m_nMatches;
        break;
    case VK_UP:
        m_nCurSel --;
        break;
    case VK_DOWN:
        m_nCurSel ++;
        break;
    case VK_NEXT:
        m_nCurSel += m_nMaxLines;
        break;
    case VK_PRIOR:
        m_nCurSel -= m_nMaxLines;
        break;
    case VK_RETURN:
        bRet = false;
        if (m_pCurSel && m_nMatches > 0)
        {
            m_szReturnPath = m_pCurSel->Data.szFullName;
        }
        break;
    case VK_ESCAPE:
        bRet = false;
        break;
    default:
        if (wChar != 0)
        {
            size_t nPos = _tcslen(m_szFilter);
			if (nPos < MAX_PATH - 2)
			{
				m_szFilter[nPos] = wChar;
				m_szFilter[nPos+1] = 0;
				m_bRecalcSquare = true;
			}
        }
        break;
    }

    return bRet;
}

TCHAR* UI::Show()
{
    InitConsole();

	DWORD dwOldMode;
	GetConsoleMode(m_hConsoleInput, &dwOldMode);
	DWORD dwNewMode = dwOldMode | ENABLE_MOUSE_INPUT;
	dwNewMode &= ~ENABLE_QUICK_EDIT_MODE;
	dwNewMode |= ENABLE_EXTENDED_FLAGS;

	SetConsoleMode(m_hConsoleInput, dwNewMode);

    bool bContinue = true;
	bool bButtonDown = false;
	bool bDownInText = false;
	bool bDownOnScroll = false;

    while (bContinue)
    {
        DisplayUI();

        INPUT_RECORD ir;
        DWORD dwRead;
        ReadConsoleInput(m_hConsoleInput, &ir, 1, &dwRead);

		if (ir.EventType == MOUSE_EVENT)
		{
			if (ir.Event.MouseEvent.dwEventFlags == MOUSE_WHEELED)
			{
				if (((int)ir.Event.MouseEvent.dwButtonState) < 0)
				{
					ir.EventType = KEY_EVENT;
					ir.Event.KeyEvent.bKeyDown = TRUE;
					ir.Event.KeyEvent.wVirtualKeyCode = VK_DOWN;
					ir.Event.KeyEvent.uChar.UnicodeChar = 0;
				}
				else if (((int)ir.Event.MouseEvent.dwButtonState) > 0)
				{
					ir.EventType = KEY_EVENT;
					ir.Event.KeyEvent.bKeyDown = TRUE;
					ir.Event.KeyEvent.wVirtualKeyCode = VK_UP;
					ir.Event.KeyEvent.uChar.UnicodeChar = 0;
				}
			}
			else if (ir.Event.MouseEvent.dwEventFlags == MOUSE_MOVED)
			{
				CONSOLE_SCREEN_BUFFER_INFO csbi;
				GetConsoleScreenBufferInfo(m_hConsoleOutput, &csbi);

				if (bDownInText)
				{
					if (ir.Event.MouseEvent.dwMousePosition.Y - csbi.srWindow.Top >= m_nOffsetY + 1 &&
						ir.Event.MouseEvent.dwMousePosition.Y - csbi.srWindow.Top <= m_nOffsetY + m_nMaxLines &&
						ir.Event.MouseEvent.dwMousePosition.X - csbi.srWindow.Left >= m_nOffsetX + 2 &&
						ir.Event.MouseEvent.dwMousePosition.X - csbi.srWindow.Left <= m_nOffsetX + 1 + ((int)m_nMaxLineLen))
					{
						m_nCurSel = m_nStartShow + ((ir.Event.MouseEvent.dwMousePosition.Y - csbi.srWindow.Top) - (m_nOffsetY + 1));
					}
				}
				else if (bDownOnScroll)
				{
					int top = m_nOffsetY + 1;
					int bottom = m_nOffsetY + m_nMaxLines;
					int pos = ir.Event.MouseEvent.dwMousePosition.Y - csbi.srWindow.Top;
					if (pos < top)
					{
						pos = top;
					}
					if (pos > bottom)
					{
						pos = bottom;
					}
					m_nStartShow  = (int)(((double)(pos - top)) / ((double)bottom - top) * ((double)(m_nMatches - m_nMaxLines)));
				}
			}
			else if (ir.Event.MouseEvent.dwEventFlags == 0)
			{
				CONSOLE_SCREEN_BUFFER_INFO csbi;
				GetConsoleScreenBufferInfo(m_hConsoleOutput, &csbi);

				if (ir.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
				{
					bButtonDown = true;

					if (ir.Event.MouseEvent.dwMousePosition.Y - csbi.srWindow.Top >= m_nOffsetY + 1 &&
						ir.Event.MouseEvent.dwMousePosition.Y - csbi.srWindow.Top <= m_nOffsetY + m_nMaxLines &&
						ir.Event.MouseEvent.dwMousePosition.X - csbi.srWindow.Left >= m_nOffsetX + 2 &&
						ir.Event.MouseEvent.dwMousePosition.X - csbi.srWindow.Left <= m_nOffsetX + 1 + ((int)m_nMaxLineLen))
					{
						bDownInText = true;
						m_nCurSel = m_nStartShow + ((ir.Event.MouseEvent.dwMousePosition.Y - csbi.srWindow.Top) - (m_nOffsetY + 1));
					}
					else if (m_nMaxLines < m_nMatches)
					{
						if (ir.Event.MouseEvent.dwMousePosition.Y - csbi.srWindow.Top >= m_nOffsetY + 1 &&
							ir.Event.MouseEvent.dwMousePosition.Y - csbi.srWindow.Top <= m_nOffsetY + m_nMaxLines &&
							ir.Event.MouseEvent.dwMousePosition.X - csbi.srWindow.Left >= m_nOffsetX + 3 + ((int)m_nMaxLineLen) &&
							ir.Event.MouseEvent.dwMousePosition.X - csbi.srWindow.Left <= m_nOffsetX + 3 + ((int)m_nMaxLineLen))
						{
							bDownOnScroll = true;

							int top = m_nOffsetY + 1;
							int bottom = m_nOffsetY + m_nMaxLines;
							int pos = ir.Event.MouseEvent.dwMousePosition.Y - csbi.srWindow.Top;
							if (pos < top)
							{
								pos = top;
							}
							if (pos > bottom)
							{
								pos = bottom;
							}
							m_nStartShow  = (int)(((double)(pos - top)) / ((double)bottom - top) * ((double)(m_nMatches - m_nMaxLines)));
						}
					}
				}
				else
				{
					if (bButtonDown)
					{
						if (bDownInText)
						{
							if (ir.Event.MouseEvent.dwMousePosition.Y - csbi.srWindow.Top >= m_nOffsetY + 1 &&
								ir.Event.MouseEvent.dwMousePosition.Y - csbi.srWindow.Top <= m_nOffsetY + m_nMaxLines &&
								ir.Event.MouseEvent.dwMousePosition.X - csbi.srWindow.Left >= m_nOffsetX + 2 &&
								ir.Event.MouseEvent.dwMousePosition.X - csbi.srWindow.Left <= m_nOffsetX + 1 + ((int)m_nMaxLineLen))
							{
								m_nCurSel = m_nStartShow + ((ir.Event.MouseEvent.dwMousePosition.Y - csbi.srWindow.Top) - (m_nOffsetY + 1));
								ir.EventType = KEY_EVENT;
								ir.Event.KeyEvent.bKeyDown = TRUE;
								ir.Event.KeyEvent.wVirtualKeyCode = VK_RETURN;
								ir.Event.KeyEvent.uChar.UnicodeChar = 0;
							}
						}
						else if (bDownOnScroll)
						{
							int top = m_nOffsetY + 1;
							int bottom = m_nOffsetY + m_nMaxLines;
							int pos = ir.Event.MouseEvent.dwMousePosition.Y - csbi.srWindow.Top;
							if (pos < top)
							{
								pos = top;
							}
							if (pos > bottom)
							{
								pos = bottom;
							}
							m_nStartShow  = (int)(((double)(pos - top)) / ((double)bottom - top) * ((double)(m_nMatches - m_nMaxLines)));
						}

						bDownInText = false;
						bButtonDown = false;
						bDownOnScroll = false;
					}
				}
			}
		}

        if (ir.EventType == KEY_EVENT && ir.Event.KeyEvent.bKeyDown)
        {
			bButtonDown = false;
			bDownInText = false;
			bDownOnScroll = false;

            bContinue = HandleKey(ir.Event.KeyEvent.wVirtualKeyCode, ir.Event.KeyEvent.uChar.UnicodeChar);

            if (m_nCurSel < 0)
            {
                m_nCurSel = 0;
            }
            if (m_nCurSel >= m_nMatches)
            {
                m_nCurSel = m_nMatches - 1;
            }

            while (m_nCurSel < m_nStartShow)
            {
                m_nStartShow --;
            }

            while (m_nCurSel > m_nStartShow + m_nMaxLines - 1)
            {
                m_nStartShow ++;
            }
        }
    }

	SetConsoleMode(m_hConsoleInput, dwOldMode);

    WriteConsoleOutput(m_hConsoleOutput, m_ciOriginal, m_cSize, m_cZero, &m_srSaved);
    SetConsoleCursorInfo(m_hConsoleOutput, &m_cciOriginal);

    return m_szReturnPath;
}

