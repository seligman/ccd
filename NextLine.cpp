#include "main.h"

#include "xtrack.h"
#include "NextLine.h"

#define BUFFER_SIZE 524288

void NextLine::ClearData()
{
    m_hFile = NULL;

    m_nLenData = 0;
    m_nMaxLen = 0;
    m_szData = NULL;
    m_szData2 = NULL;
    m_szCurPos = NULL;
    m_szLine = NULL;
    m_cbLine = 0;
}

NextLine::NextLine()
{
    ClearData();
}

NextLine::NextLine(HANDLE hFile)
{
    ClearData();
    Init(hFile);
}

NextLine::~NextLine()
{
	XUNTRACK(m_szData);
    delete m_szData;
	XUNTRACK(m_szData2);
    delete m_szData2;
	XUNTRACK(m_szLine);
    delete m_szLine;
}

void NextLine::Init(HANDLE hFile)
{
    m_hFile = hFile;
    m_cbLine = 0;
    m_szLine = NULL;

    m_nMaxLen = BUFFER_SIZE * 2;
	XUNTRACK(m_szData);
    delete m_szData;
	XUNTRACK(m_szData2);
    delete m_szData2;

    m_szData = new char[BUFFER_SIZE * 2];
	XTRACK(m_szData);
    m_szData2 = new char[BUFFER_SIZE * 2];
	XTRACK(m_szData2);
    m_szCurPos = m_szData;

    DWORD dwRead;
    ReadFile(hFile, m_szData, BUFFER_SIZE, &dwRead, NULL);
    m_nLenData = dwRead;
}

bool NextLine::HasNext()
{
    do
    {
        char * szPos = m_szCurPos;
        int nLeft = m_nLenData;

        // Look for the newline character within our current buffer
        while (nLeft > 0)
        {
            if (*szPos == '\n')
            {
                // Found a newline, process the line

                *szPos = 0;
                if (*(szPos - 1) == '\r')
                {
                    // Get rid of any CR since we get rid of the LF
                    *(szPos - 1) = 0;
                }

                // Allocate more data to the line if needed
                if ((m_nLenData - nLeft + 2) > m_cbLine)
                {
                    m_cbLine = (m_nLenData - nLeft + 2);
					XUNTRACK(m_szLine);
                    delete m_szLine;
                    m_szLine = new TCHAR[m_cbLine];
					XTRACK(m_szLine);
                }

                // Write the output in our string
                #ifdef UNICODE
                    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_szCurPos, (m_nLenData - nLeft + 1),
                        m_szLine, m_cbLine);
                #else
                    _tcsncpy(m_szLine, m_szCurPos, (m_nLenData - nLeft + 1));
                #endif

                nLeft --;
                szPos ++;
                m_szCurPos = szPos;
                m_nLenData = nLeft;
                return true;
            }
            szPos ++;
            nLeft --;
        }

        // If we get here, there's no more LF's in the string

        if (m_nLenData + BUFFER_SIZE > m_nMaxLen)
        {
            // Whoops, we still have enough data in the buffer that we can't
            //  add BUFFER_SIZE to it, so increase it
            DWORD nNewSize = m_nMaxLen;
            while (m_nLenData + BUFFER_SIZE > nNewSize)
            {
                nNewSize += BUFFER_SIZE;
            }

            // Need to keep track of the current position in the buffer
            size_t nOffset = m_szCurPos - m_szData;
			XUNTRACK(m_szData2);
            delete m_szData2;
            m_szData2 = new char[nNewSize];
			XTRACK(m_szData2);
            memcpy(m_szData2,m_szData, m_nMaxLen);
			XUNTRACK(m_szData);
            delete m_szData;
            m_szData = m_szData2;
            m_szData2 = new char[nNewSize];
			XTRACK(m_szData2);

            m_szCurPos = m_szData + nOffset;
        }

        //Copy the remaining data to our backup buffer
        memcpy(m_szData2, m_szCurPos, m_nLenData);

        // Read in some more data
        DWORD dwRead = 0;
        DWORD nRet = ReadFile(m_hFile, m_szData2 + m_nLenData, BUFFER_SIZE, &dwRead, NULL);
        if (!nRet || !dwRead)
        {
            // Either there's no more data, or a broken file
			XUNTRACK(m_szData);
            delete m_szData;
			XUNTRACK(m_szData2);
            delete m_szData2;

            m_szData = NULL;
            m_szData2 = NULL;

            return false;
        }
        m_nLenData += dwRead;

        // Everything of interest is in the backup buffer, so switch buffers
        char * szTemp = m_szData;
        m_szData = m_szData2;
        m_szData2 = szTemp;
        m_szCurPos = m_szData;

        // And just loop around and work with the new data
    }
    while (true);

    return false;
}

LPTSTR NextLine::Line()
{
    return m_szLine;
}
