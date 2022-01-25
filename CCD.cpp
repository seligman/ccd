#include "main.h"

#ifdef _DEBUG
#define TEST_MODE // A mode to just test out the UI and match algo
#endif

#include "xtrack.h"
#include "NextLine.h"
#include "RemoteThread.h"
#include "LinkedList.h"
#include "UI.h"
#include "ProcessInfo.h"
#include "resource.h"
#include "Hidden.h"
#include "SQLite.h"
#include "ExecEngine.h"
#include "Ngram Tester\Array.h"
#include "Ngram Tester\Ngram.h"
#include "Ngram Tester\NgramBuilder.h"
#include "Ngram Tester\NgramReader.h"
#include "CCD.h"

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LinkedList<Result>  g_llFound;
LinkedList<Result>  g_llIgnore;
LinkedList<Result>  g_llShortcut;
bool        g_bCrustyWarning = false;
DWORD       g_dwLastScan = 0;
int         g_nScanned = 0;
extern bool g_bIsCMD;
TCHAR       g_szIgnoreDirs[MAX_PATH];
TCHAR       g_szAllDirs[MAX_PATH];
TCHAR       g_szShortcutDirs[MAX_PATH];
TCHAR       g_charDrive;
bool		g_bOutputOnly = false;
LPFN_ISWOW64PROCESS	g_fnIsWow64Process = NULL;

void InitSettingsFiles()
{
    // Determine where to store the files
    DWORD dwAttr = 0;
	TCHAR szAppData[MAX_PATH] = {0};
	
    // Note the current drive
	TCHAR szCurrent[MAX_PATH] = {0};
    GetCurrentDirectory(_countof(szCurrent), szCurrent);
	szCurrent[1] = 0;
    g_charDrive = szCurrent[0];

	if (dwAttr == 0)
	{
		// Look for the %APPDATA%\CCD\%DRIVE% version first
		if (S_OK == SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szAppData))
		{
			_tcscat_s(szAppData, HIDEW_SLASHCCDSLASH);
			_tcscat_s(szAppData, szCurrent);

			dwAttr = GetFileAttributes(szAppData);

			if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
			{
				_tcscpy_s(g_szIgnoreDirs, szAppData);
				_tcscat_s(g_szIgnoreDirs, HIDEW_IGNOREDIRS);
				_tcscpy_s(g_szAllDirs, szAppData);
				_tcscat_s(g_szAllDirs, HIDEW_ALLDIRS);
				_tcscpy_s(g_szShortcutDirs, szAppData);
				_tcscat_s(g_szShortcutDirs, HIDEW_SHORTCUTDIRS);
			}
			else
			{
				dwAttr = 0;
			}
		}
	}

	if (dwAttr == 0)
	{
		// Look for \CCD
		dwAttr = GetFileAttributes(HIDEW_SETTINGSPATH);

		if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
		{
			_tcscpy_s(g_szIgnoreDirs, HIDEW_SETTINGSPATH);
			_tcscat_s(g_szIgnoreDirs, HIDEW_IGNOREDIRS);
			_tcscpy_s(g_szAllDirs, HIDEW_SETTINGSPATH);
			_tcscat_s(g_szAllDirs, HIDEW_ALLDIRS);
			_tcscpy_s(g_szShortcutDirs, HIDEW_SETTINGSPATH);
			_tcscat_s(g_szShortcutDirs, HIDEW_SHORTCUTDIRS);
		}
		else
		{
			dwAttr = 0;
		}
	}

	if (dwAttr == 0)
	{
		// Just use the root of the drive
        _tcscpy_s(g_szIgnoreDirs, HIDEW_IGNOREDIRS);
        _tcscpy_s(g_szAllDirs, HIDEW_ALLDIRS);
        _tcscpy_s(g_szShortcutDirs, HIDEW_SHORTCUTDIRS);
    }
}

bool IsParent64()
{
    SYSTEM_INFO si = {0};
    GetNativeSystemInfo(&si);
    if (si.wProcessorArchitecture  == PROCESSOR_ARCHITECTURE_INTEL)
    {
        return false;
    }

    bool bReturn = false;
    BOOL bIsWow64 = FALSE;

    if (NULL == g_fnIsWow64Process)
    {
        g_fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
            GetModuleHandle(HIDEW_KERNEL32),HIDEA_ISWOW64PROCESS);
    }
  
    if (NULL != g_fnIsWow64Process)
    {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 
            FALSE, (DWORD)GetParentProcessID(GetCurrentProcessId()));
		XTRACK(hProcess);

        if (!g_fnIsWow64Process(hProcess, &bIsWow64))
        {
			bIsWow64 = FALSE;
        }
        else
        {
            if (!bIsWow64)
            {
                bReturn = true;
            }
        }
		XUNTRACK(hProcess);
        CloseHandle(hProcess);
    }
    return bReturn;
}

void RescanFolder(NgramBuilder * buildMain, NgramBuilder * buildMini, char * szPath, size_t * pnLastLen)
{
    if (GetTickCount() - g_dwLastScan > (250))
    {
		TCHAR temp[100] = {0};
		for (size_t i = 0; i < *pnLastLen; i ++)
		{
			temp[i] = _T('\b');
		}
		_fputts(temp, stdout);

		memset(temp, 0, 100 * sizeof(TCHAR));

		TCHAR * pos = HIDEW_SCANNING;
		TCHAR * cur = temp;
		size_t left = 78;
		while (*pos)
		{
			*cur = *pos;
			cur++;
			pos++;
			left--;
		}

		*cur = ' ';
		cur++;
		left--;

		char * pos2 = szPath;

		if (strlen(pos2) > left)
		{
			for (int i = 0; i < 3; i ++)
			{
				*cur = '.';
				cur++;
				left--;
			}
			while (strlen(pos2) > left)
			{
				pos2++;
			}
		}

		while (*pos2 && left)
		{
			*cur = *pos2;
			cur++;
			pos2++;
			left--;
		}

		_fputts(temp, stdout);

		size_t nextLen = _tcslen(temp);
		if (nextLen < *pnLastLen)
		{
			size_t diff = *pnLastLen - nextLen;
			memset(temp, 0, 100 * sizeof(TCHAR));
			for (size_t i = 0; i < diff; i ++)
			{
				temp[i] = _T(' ');
			}
			_fputts(temp, stdout);
			for (size_t i = 0; i < diff; i ++)
			{
				temp[i] = _T('\b');
			}
			_fputts(temp, stdout);
		}

		*pnLastLen = nextLen;
        g_dwLastScan = GetTickCount();
    }

    WIN32_FIND_DATAA fd;

    size_t nSize = strlen(szPath) + MAX_PATH + 5;
    char * szNext = new char[nSize];
	XTRACK(szNext);
    WCHAR * szNextW = new WCHAR[nSize];
	XTRACK(szNextW);

    if (*szPath)
    {
        strcpy_s(szNext, nSize, szPath);
        strcat_s(szNext, nSize, HIDEA_SLASHSTAR);
    }
    else
    {
        strcpy_s(szNext, nSize, HIDEA_SLASHSTAR);
    }

    HANDLE hFind = FindFirstFileA(szNext, &fd);

    while (hFind != NULL && hFind != INVALID_HANDLE_VALUE)
    {
        if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
            (!(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)))
        {
            bool bSkipFolder = false;
            if (strcmp(fd.cFileName, HIDEA_DOTDOT) == 0 ||
                strcmp(fd.cFileName, HIDEA_DOT) == 0)
            {
                bSkipFolder = true;
            }

            if (!bSkipFolder)
            {
                strcpy_s(szNext, nSize, szPath);
                strcat_s(szNext, nSize, HIDEA_SLASH);
                strcat_s(szNext, nSize, fd.cFileName);

                swprintf_s(szNextW, nSize,  HIDEW_CAPPERCS, szNext);

                for (LinkedListItem<Result>* pCur = g_llIgnore.GetHead(); pCur; pCur = pCur->pNext)
                {
                    size_t nIgnoreLen = _tcslen(pCur->Data.szFullName);
                    if (nIgnoreLen > 0)
                    {
                        for (TCHAR * szCur = szNextW; *szCur; szCur ++)
                        {
                            if (_tcsnicmp(szCur, pCur->Data.szFullName, nIgnoreLen) == 0)
                            {
                                bSkipFolder = true;
                                break;
                            }
                        }
                        if (bSkipFolder)
                        {
                            break;
                        }
                    }
                }
            }

            if (!bSkipFolder)
            {
				buildMain->AddWord(szNext);
				buildMini->AddWord(fd.cFileName);

                g_nScanned ++;

                RescanFolder(buildMain, buildMini, szNext, pnLastLen);
            }
        }

        if (!FindNextFileA(hFind, &fd))
        {
            FindClose(hFind);
            hFind = NULL;
        }
    }

	XUNTRACK(szNext);
    delete(szNext);
	XUNTRACK(szNextW);
    delete(szNextW);
}

void RescanAll()
{
	size_t nLastLen = 0;

    _fputts(HIDEW_SCANNING, stdout);

	nLastLen = _tcslen(HIDEW_SCANNING);

    HANDLE hIgnoreFile = CreateFile(g_szIgnoreDirs, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);

    if (hIgnoreFile != INVALID_HANDLE_VALUE)
    {
		XTRACK(hIgnoreFile);

        NextLine * pnl = new NextLine(hIgnoreFile);
		XTRACK(pnl);
        while (pnl->HasNext())
        {
            LinkedListItem<Result> * pCur = g_llIgnore.AddTail();
            size_t dwLen = _tcslen(pnl->Line()) + 1;
            pCur->Data.szFullName = new TCHAR[dwLen];
			XTRACK(pCur->Data.szFullName);
            _tcscpy_s(pCur->Data.szFullName, dwLen, pnl->Line());
        }
		XUNTRACK(pnl);
        delete pnl;
		XUNTRACK(hIgnoreFile);
        CloseHandle(hIgnoreFile);
    }

	DeleteFile(g_szAllDirs);

	if (!NgramBuilder::OpenData(g_szAllDirs))
	{
        _fputts(HIDEW_NOALLDIRS, stdout);
        return;
	}

    char * szCur = NULL;
    int nDirLen = 0;
    nDirLen = GetCurrentDirectoryA(0, NULL);
    nDirLen ++;
    szCur = new char[nDirLen];
	XTRACK(szCur);
    GetCurrentDirectoryA(nDirLen, szCur);
    if (szCur[1] == ':')
    {
        szCur[2] = 0;
    }
    else
    {
        szCur[0] = 0;
    }

	NgramBuilder * buildMain = new NgramBuilder();
	XTRACK(buildMain);
	NgramBuilder * buildMini = new NgramBuilder();
	XTRACK(buildMini);

    RescanFolder(buildMain, buildMini, szCur, &nLastLen);

	NgramBuilder::WriteValue(NGRAM_COOKIE);

	/* Space for the pointers to data */
	DWORD ptrMain = 0;
	DWORD ptrMini = 0;
	DWORD pointers = NgramBuilder::GetOffset();
	NgramBuilder::WriteValue(ptrMini);
	NgramBuilder::WriteValue(ptrMain);

	ptrMini = NgramBuilder::GetOffset();
	buildMini->Write();
	ptrMain = NgramBuilder::GetOffset();
	buildMain->Write();

	/* Go back and write the pointers */
	NgramBuilder::Seek(pointers);
	NgramBuilder::WriteValue(ptrMini);
	NgramBuilder::WriteValue(ptrMain);

	XUNTRACK(buildMini);
	delete buildMini;
	XUNTRACK(buildMain);
	delete buildMain;

	NgramBuilder::CloseData();

	XUNTRACK(szCur);
    delete[] szCur;

	TCHAR temp[100] = {0};
	for (size_t i = 0; i < nLastLen; i ++)
	{
		temp[i] = _T('\b');
	}
	_fputts(temp, stdout);
	for (size_t i = 0; i < nLastLen; i ++)
	{
		temp[i] = _T(' ');
	}
	_fputts(temp, stdout);
	for (size_t i = 0; i < nLastLen; i ++)
	{
		temp[i] = _T('\b');
	}
	_fputts(temp, stdout);

    _fputts(HIDEW_DONE, stdout);
}

void CreateHelpers()
{
    TCHAR szWin[MAX_PATH];
    SHGetFolderPath(NULL, CSIDL_WINDOWS, NULL, SHGFP_TYPE_CURRENT, szWin);
    for (int i = 0; i < 20; i ++)
    {
        TCHAR szFile[MAX_PATH + 20 + 7];
        char szCmd[200];

        strcpy_s(szCmd, HIDEA_ATCCDDOTDOT);

        _tcscpy_s(szFile, szWin);
        _tcscat_s(szFile, HIDEW_SLASHCCDDOTDOT);

        for (int j = 0; j < i; j ++)
        {
            _tcscat_s(szFile, HIDEW_DOT);
            strcat_s(szCmd, HIDEA_DOT);
        }

        _tcscat_s(szFile, HIDEW_DOTCMD);
        strcat_s(szCmd, HIDEA_NEWLINE);

        DWORD dwWrote;
        HANDLE hFile = CreateFile(szFile, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL);
		if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
		{
			XTRACK(hFile);
			WriteFile(hFile, szCmd, (int)strlen(szCmd), &dwWrote, NULL);
			XUNTRACK(hFile);
			CloseHandle(hFile);
		}
    }

    _tprintf(HIDEW_HELPERDONE, szWin);
}

bool TryGoUp(TCHAR * szPath)
{
    int nGoUp = -1;
    for (TCHAR*szCur=szPath;*szCur;szCur++)
    {
        if (*szCur == _T('.'))
        {
            nGoUp ++;
        }
        else
        {
            nGoUp = 0;
            break;
        }
    }

    if (nGoUp > 0)
    {
        for (int i = 0; i < nGoUp; i ++)
        {
#ifdef TEST_MODE
			_tprintf_s(_T("CD: %s\n"), _T(".."));
#else
            ChangeParentCurrentDirectory(HIDEW_DOTDOT);
#endif
        }
        return true;
    }

    return false;
}

bool TryStandardCD(TCHAR * szInputPath)
{
    bool bRet = false;

    TCHAR * szPath = new TCHAR[_tcslen(szInputPath) + 5];
	XTRACK(szPath);
    _tcscpy_s(szPath, _tcslen(szInputPath) + 1, szInputPath);
    if (_tcslen(szPath) > 0 && szPath[_tcslen(szPath) - 1] == _T('\\'))
    {
        szPath[_tcslen(szPath) - 1] = 0;
    }

    if (_tcslen(szInputPath) == 1 && szInputPath[0] == _T('\\'))
    {
#ifdef TEST_MODE
		_tprintf_s(_T("CD: %s\n"), _T("\\"));
#else
		ChangeParentCurrentDirectory(HIDEW_SLASH);
#endif
        bRet = true;
    }
    else if (_tcslen(szPath) == 2 && szPath[1] == _T(':'))
    {
        szPath[0] = _totupper(szPath[0]);
        bRet = true;
        szPath[2] = _T('\\');
        szPath[3] = 0;
#ifdef TEST_MODE
		_tprintf_s(_T("CD: %s\n"), szPath);
#else
		ChangeParentCurrentDirectory(szPath);
#endif
    }
    else
    {
        WIN32_FIND_DATA wfd = {0};
        HANDLE hFind = FindFirstFile(szPath, &wfd);
        TCHAR * szPartial = NULL;

        if ((hFind != INVALID_HANDLE_VALUE) && 
            (hFind != NULL))
        {
            if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                size_t nLastBack = -1;
                size_t nMax = _tcslen(szPath);

                for (size_t nCur = 0; nCur < nMax; nCur ++)
                {
                    if (szPath[nCur] == _T('\\'))
                    {
                        nLastBack = nCur;
                    }
                }

                if (nLastBack >= 0)
                {
                    size_t dwBytes = nLastBack + _tcslen(wfd.cFileName) + 2;
                    szPartial = new TCHAR[dwBytes];
					XTRACK(szPartial);
                    _tcsncpy_s(szPartial, dwBytes, szPath, nLastBack + 1);
                    _tcscat_s(szPartial, dwBytes, wfd.cFileName);
                }
                else
                {
                    size_t dwBytes = _tcslen(wfd.cFileName) + 1;
                    szPartial = new TCHAR[dwBytes];
					XTRACK(szPartial);
                    _tcscpy_s(szPartial, dwBytes, wfd.cFileName);
                }

                bRet = true;
            }

            FindClose(hFind);
        }

        if (bRet)
        {
            TCHAR * szProperName;
            TCHAR * szFile;

            DWORD dwLen = GetFullPathName(szPartial, 0, NULL, NULL) + 1;
            szProperName = new TCHAR[dwLen];
			XTRACK(szProperName);
            GetFullPathName(szPartial, dwLen, szProperName, &szFile);

#ifdef TEST_MODE
			_tprintf_s(_T("CD: %s\n"), szProperName);
#else
			ChangeParentCurrentDirectory(szProperName);
#endif

			XUNTRACK(szProperName);
            delete[] szProperName;
        }

		XUNTRACK(szPartial);
        delete[] szPartial;
    }

	XUNTRACK(szPath);
    delete[] szPath;

    return bRet;
}

void ReadShortcuts()
{
    HANDLE hShortcutFile = CreateFile(g_szShortcutDirs, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);

    if (hShortcutFile != INVALID_HANDLE_VALUE)
    {
		XTRACK(hShortcutFile);

        NextLine * pnl = new NextLine(hShortcutFile);
		XTRACK(pnl);
        while (pnl->HasNext())
        {
            TCHAR * szToken = _tcsstr(pnl->Line(), HIDEW_EQUALS);
            if (szToken)
            {
                *szToken = 0;
                szToken ++;
                if (_tcslen(szToken) > 0 && _tcslen(pnl->Line()) > 0)
                {
                    LinkedListItem<Result> * pCur = g_llShortcut.AddTail();
                    size_t dwLen;
                    dwLen = _tcslen(pnl->Line()) + 1;
                    pCur->Data.szShortcut = new TCHAR[dwLen];
					XTRACK(pCur->Data.szShortcut);
                    _tcscpy_s(pCur->Data.szShortcut, dwLen, pnl->Line());
                    dwLen = _tcslen(szToken) + 1;
                    pCur->Data.szFullName = new TCHAR[dwLen];
					XTRACK(pCur->Data.szFullName);
                    _tcscpy_s(pCur->Data.szFullName, dwLen, szToken);
                }
            }
        }
		XUNTRACK(pnl);
        delete pnl;
		XUNTRACK(hShortcutFile);
        CloseHandle(hShortcutFile);
    }
}

bool ReadStoredDirs(TCHAR charDrive, TCHAR ** szPaths, bool bOnlyUnder)
{
	OldWarnings();

	if (NgramReader::OpenData(g_szAllDirs) && NgramReader::ReadValue(0) == NGRAM_COOKIE)
	{
		NgramReader * readMini = new NgramReader(NgramReader::ReadValue(1));
		XTRACK(readMini);
		NgramReader * readMain = new NgramReader(NgramReader::ReadValue(2));
		XTRACK(readMain);

		Array<int> indexes;
		Array<char*> words;

		for (int pass = 0; g_llFound.GetCount() == 0 && pass < 2; pass++)
		{
			switch (pass)
			{
			case 0:
				readMini->FindPhrasesT(szPaths, &words, &indexes);
				break;
			case 1:
				readMain->FindPhrasesT(szPaths, &words, &indexes);
				break;
			}

			for (int i = 0; i < indexes.Count; i ++)
			{
				TCHAR * path;
				switch (pass)
				{
				case 0:
					path = NgramReader::CopyToTchar(readMain->GetFullWord(indexes.List[i]));
					break;
				case 1:
					path = NgramReader::CopyToTchar(words.List[i]);
					break;
				}

				bool use = true;
				if (bOnlyUnder)
				{
					TCHAR * szCurDir = NULL;
					int nLenCurDir = 0;
					nLenCurDir = GetCurrentDirectory(0, NULL);
					nLenCurDir ++;
					szCurDir = new TCHAR[nLenCurDir];
					XTRACK(szCurDir);
					GetCurrentDirectory(nLenCurDir, szCurDir);

					if (_tcsnicmp(path, szCurDir, _tcslen(szCurDir)) != 0)
					{
						use = false;
					}

					XUNTRACK(szCurDir);
					delete[] szCurDir;
				}

				if (use)
				{
					LinkedListItem<Result> * obj = g_llFound.AddTail();
					obj->Data.bOnlyDir = (pass == 0);
					obj->Data.szFullName = path;
					obj->Data.szFullName[0] = charDrive;
					path = NULL;
				}

				XUNTRACK(path);
				delete[] path;
			}
		}

		XUNTRACK(readMain);
		delete readMain;
		XUNTRACK(readMini);
		delete readMini;

		NgramReader::CloseData();

		return true;
	}
	else
	{
		return false;
	}
}

void OldWarnings()
{
    if (!g_bCrustyWarning)
    {
        FILETIME ftWriteTime;
        FILETIME ftNow;
        SYSTEMTIME stNow;

		HANDLE hDirFile = CreateFile(g_szAllDirs, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hDirFile != INVALID_HANDLE_VALUE)
		{
			XTRACK(hDirFile);
			GetFileTime(hDirFile, NULL, NULL, &ftWriteTime);
			XUNTRACK(hDirFile);
			CloseHandle(hDirFile);
			GetLocalTime(&stNow);
			SystemTimeToFileTime(&stNow, &ftNow);

			if (CompareFileTime(&ftNow, &ftWriteTime) == 1)
			{
				ULARGE_INTEGER uliNow;
				ULARGE_INTEGER uliWriteTime;

				uliNow.HighPart = ftNow.dwHighDateTime;
				uliNow.LowPart = ftNow.dwLowDateTime;

				uliWriteTime.HighPart = ftWriteTime.dwHighDateTime;
				uliWriteTime.LowPart = ftWriteTime.dwLowDateTime;

				ULARGE_INTEGER uliDelta;
				uliDelta.QuadPart = uliNow.QuadPart - uliWriteTime.QuadPart;

				if (uliDelta.QuadPart > 53568000000000) 
					//(10000000 * 60 * 60 * 24 * 31 * 2)
					// 2 Months
				{
					_fputts(HIDEW_CRUSTY, stdout);
					g_bCrustyWarning = true;
				}
			}
		}
    }
}

HMODULE Local_Psapi()
{
    static HMODULE ret = NULL;
    if (!ret)
    {
        ret = LoadLibraryW(HIDEW_PSAPI);
    }
    return ret;
}

BOOL Local_EnumProcessModules(HANDLE hProcess, HMODULE *lphModule, DWORD cb, LPDWORD lpcbNeeded)
{
    if (Local_Psapi())
    {
        static FARPROC ret = NULL;

        if (!ret)
        {
            ret = GetProcAddress(Local_Psapi(), HIDEA_ENUMPROCESSMODULES);
        }

        if (ret)
        {
            typedef BOOL (WINAPI * PFN_Func)(HANDLE, HMODULE*, DWORD, LPDWORD);
            return ((PFN_Func) ret)(hProcess, lphModule, cb, lpcbNeeded);
        }
    }

    return FALSE;
}

DWORD Local_GetModuleFileNameExW(HANDLE hProcess, HMODULE hModule, LPTSTR lpFilename, DWORD nSize)
{
    if (Local_Psapi())
    {
        static FARPROC ret = NULL;

        if (!ret)
        {
            ret = GetProcAddress(Local_Psapi(), HIDEA_GETMODULEFILENAMEEXW);
        }

        if (ret)
        {
            typedef DWORD (WINAPI * PFN_Func)(HANDLE, HMODULE, LPTSTR, DWORD);
            return ((PFN_Func) ret)(hProcess, hModule, lpFilename, nSize);
        }
    }

    return 0;
}

bool VerifyIsCMD()
{
    bool bRunningUnderCMD = false;

    DWORD dwParentID = (DWORD)GetParentProcessID(GetCurrentProcessId());
    TCHAR szProcName[MAX_PATH];
    memset(szProcName, 0, MAX_PATH * sizeof TCHAR);

    if (!(dwParentID == 0 || dwParentID == 0xFFFFFFFF))
    {
        HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION |
                PROCESS_VM_READ, FALSE, dwParentID);
		XTRACK(hProc);
        if (hProc != INVALID_HANDLE_VALUE)
        {
            DWORD dwCount;
            HMODULE hModule;

            Local_EnumProcessModules(hProc, &hModule, sizeof(hModule), &dwCount);
            Local_GetModuleFileNameExW(hProc, hModule, szProcName, MAX_PATH);

			XUNTRACK(hProc);
            CloseHandle(hProc);
        }
    }

    if (_tcslen(szProcName) >= 7 &&
        _tcsicmp(szProcName + (_tcslen(szProcName) - 7), HIDEW_CMDEXE) == 0)
    {
        bRunningUnderCMD = true;
    }

    g_bIsCMD = bRunningUnderCMD;
#ifdef _DEBUG
    return true;
#else
    return bRunningUnderCMD;
#endif
}

int Run64Bit(int argc, _TCHAR* argv[])
{
    // Load the embedded resource
    HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_CCD64BIT), HIDEW_DATAFILES);
    DWORD dwMem = (DWORD) LoadResource(NULL, hRsrc);
    LPVOID lpMem = (LPVOID) LockResource((LPVOID) dwMem);
	DWORD dwSize = SizeofResource(NULL, hRsrc);

    // Construct a 64-bit filename
	TCHAR szFile[MAX_PATH];
	GetModuleFileName(NULL, szFile, MAX_PATH);
	_tcscat_s(szFile, HIDEW_DOT64DOTEXE);

	HANDLE hFile = CreateFile(szFile, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL);
	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
	{
		XTRACK(hFile);
	}

    if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
    {
        // If we couldn't write in the running-dir, try using the temp folder
        GetTempPath(MAX_PATH, szFile);
        _tcscat_s(szFile, HIDEW_CCDDOT64DOTEXE);
	    hFile = CreateFile(szFile, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL);

		if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
		{
			XTRACK(hFile);
		}
    }

    if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
    {
        _fputts(HIDEW_CANTWRITE64, stdout);
        return 1;
    }

    // Write the 64-bit version out
	WriteFile(hFile, lpMem, dwSize, &dwSize, NULL);
	XTRACK(hFile);
	CloseHandle(hFile);

    // Construct the command line
	size_t dwLen = 1;
	TCHAR * szArg = NULL;
	for (int i = 1; i < argc; i ++)
	{
		dwLen += 3 + _tcslen(argv[i]);
	}
	dwLen += _tcslen(szFile) + 3;
	szArg = new TCHAR[dwLen];
	XTRACK(szArg);

	_tcscpy_s(szArg, dwLen, HIDEW_QUOTE);
	_tcscat_s(szArg, dwLen, szFile);
	_tcscat_s(szArg, dwLen, HIDEW_QUOTESPACE);

	for (int i = 1; i < argc; i ++)
	{
		if (i > 1)
		{
			_tcscat_s(szArg, dwLen, HIDEW_SPACE);
		}
		bool bQuote = false;
		for (TCHAR * szCur = argv[i]; *szCur; szCur ++)
		{
			if (*szCur == _T(' '))
			{
				bQuote = true;
			}
		}
		if (bQuote)
		{
			_tcscat_s(szArg, dwLen, HIDEW_QUOTE);
		}
		_tcscat_s(szArg, dwLen, argv[i]);
		if (bQuote)
		{
			_tcscat_s(szArg, dwLen, HIDEW_QUOTE);
		}
	}

    // Launch the 64-bit version
	TCHAR szDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szDir);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof si);
	memset(&pi, 0, sizeof pi);
	si.cb = sizeof si;

	CreateProcess(NULL, szArg, NULL, NULL, FALSE, NULL, NULL, szDir, &si, &pi);

	WaitForSingleObject(pi.hProcess, INFINITE);

    // Clean up
	DeleteFile(szFile);
	XUNTRACK(szArg);
	delete szArg;
    return 0;
}

void ShowHelp()
{
    // Determine where to store the files
    DWORD dwAttr = 0;
	TCHAR szAppData[MAX_PATH] = {0};
	
    // Note the current drive
	TCHAR szCurrent[MAX_PATH] = {0};
    GetCurrentDirectory(_countof(szCurrent), szCurrent);
	szCurrent[1] = 0;

	szCurrent[0] = _totupper(szCurrent[0]);
	szAppData[0] = _totupper(szAppData[0]);

	if (S_OK == SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szAppData))
	{
		_tcscat_s(szAppData, HIDEW_SLASHCCDSLASH);
		_tcscat_s(szAppData, szCurrent);
	}

    _tprintf(HIDEW_HELP, szAppData, szCurrent[0], HIDEW_SETTINGSPATH, szCurrent[0]);
}

void CreateMyMutex()
{
    bool bWarningShown = false;
    while (sizeof(ULONG_PTR) == 4)
    {
        HANDLE hMutex = CreateMutex(NULL, TRUE, HIDEW_MUTEXNAME);
		// XTRACK(hMutex); // Let this leak, so it's only closed when the app closes
        if (GetLastError() == ERROR_ALREADY_EXISTS)
        {
            if (!bWarningShown)
            {
                bWarningShown = true;
                _fputts(HIDEW_PREVINST, stdout);
            }
            WaitForSingleObject(hMutex, INFINITE);
			XUNTRACK(hMutex);
            CloseHandle(hMutex);
        }
        else
        {
            return;
        }
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
	XTRACK_START();
	int ret = MainInternal(argc, argv);
	XTRACK_STOP();
	return ret;
}

void Commas(int value, TCHAR* out)
{
	size_t left = 100;
	if (value < 0)
	{
		_stprintf_s(out, left, HIDEW_MINUS);
		left -= _tcslen(out);
		out += _tcslen(out);
		value = -value;
	}

	if (value < 1000)
	{
		_stprintf_s(out, left, HIDEW_PERCD, value);
		left -= _tcslen(out);
		out += _tcslen(out);
	}
	else
	{
		int offset = 1;
		while (offset < value)
		{
			offset *= 1000;
		}
		offset /= 1000;

		bool first = true;
		while (true)
		{
			if (first)
			{
				first = false;
				_stprintf_s(out, left, HIDEW_PERCD, value / offset);
			}
			else
			{
				_stprintf_s(out, left, HIDEW_PERC03D, (value / offset) % 1000);
			}
			left -= _tcslen(out);
			out += _tcslen(out);

			if (offset == 1)
			{
				break;
			}

			offset /= 1000;
		}
	}
}

void ShowStats()
{
	if (NgramReader::OpenData(g_szAllDirs) && NgramReader::ReadValue(0) == NGRAM_COOKIE)
	{
		NgramReader * readMini = new NgramReader(NgramReader::ReadValue(1));
		XTRACK(readMini);

		TCHAR count[100];
		TCHAR size[100];
		Commas(readMini->GetCount(), count);
		Commas(readMini->GetDataSize(), size);

		_tprintf(HIDEW_STATBLOCK, count, size);

		XUNTRACK(readMini);
		delete readMini;

		NgramReader::CloseData();
	}
	else
	{
		_fputts(HIDEW_STATERROR, stdout);
	}
}

int MainInternal(int argc, _TCHAR* argv[])
{
#ifndef TEST_MODE
    // If the parent process is 64-bit (and we're not) then run the 64-bit version
	if (IsParent64() && (sizeof(ULONG_PTR) != 8))
	{
        return Run64Bit(argc, argv);
	}
#endif

	bool work = true;
	int ret = 0;
    TCHAR * szArgs = NULL;
	LPTSTR * szPath = new LPTSTR[2];
	XTRACK(szPath);

	if (work)
	{
		// Make sure this is a singleton
		CreateMyMutex();

		// Find where we should store the settings
		InitSettingsFiles();
	}

#ifndef TEST_MODE
    // No clue what this will do for non-CMD processes, so don't even try
    if (work && !VerifyIsCMD())
    {
        _fputts(HIDEW_NOTCMD, stdout);
		work = false;
		ret = 1;
    }
#endif

	bool bShowHelp = false;
	bool bNoSmartMatching = false;
	bool bOnlyUnder = false;
	Array<TCHAR*> pArgs;

	if (work)
	{
		// Gather up the command line switches
		if (argc < 2)
		{
			bShowHelp = true;
		}

		for (int i = 1; work && i < argc; i ++)
		{
			if (work &&
				_tcsicmp(argv[i], HIDEW_DASHH) == 0 ||
				_tcsicmp(argv[i], HIDEW_SLASHH) == 0 ||
				_tcsicmp(argv[i], HIDEW_DASHQUES) == 0 ||
				_tcsicmp(argv[i], HIDEW_SLASHQUES) == 0)
			{
				bShowHelp = true;
			}
			else if (work && 
				_tcsicmp(argv[i], HIDEW_DASHN) == 0 ||
				_tcsicmp(argv[i], HIDEW_SLASHN) == 0)
			{
				g_bOutputOnly = true;
			}
			else if (work && 
				_tcsicmp(argv[i], HIDEW_DASHR) == 0 ||
				_tcsicmp(argv[i], HIDEW_SLASHR) == 0)
			{
				work = false;
				RescanAll();
			}
			else if (work && 
				_tcsicmp(argv[i], HIDEW_DASHC) == 0 ||
				_tcsicmp(argv[i], HIDEW_SLASHC) == 0)
			{
				work = false;
				CreateHelpers();
			}
			else if (work && 
				_tcsicmp(argv[i], HIDEW_DASHS) == 0 ||
				_tcsicmp(argv[i], HIDEW_SLASHS) == 0)
			{
				work = false;
				ShowStats();
			}
			else if (work &&
				_tcsicmp(argv[i], HIDEW_DASHF) == 0 ||
				_tcsicmp(argv[i], HIDEW_SLASHF) == 0)
			{
				bNoSmartMatching = true;
			}
			else if (work &&
				_tcsicmp(argv[i], HIDEW_DASHU) == 0 ||
				_tcsicmp(argv[i], HIDEW_SLASHU) == 0)
			{
				bOnlyUnder = true;
			}
			else
			{
				pArgs.Add(argv[i]);
			}
		}

		if (work)
		{
			size_t nLen = 0;
			for (int i = 0; i < pArgs.Count; i ++)
			{
				if (nLen > 0)
				{
					nLen ++;
				}
				nLen += _tcslen(pArgs.List[i]);
			}
			nLen += 1;
			szArgs = new TCHAR[nLen];
			XTRACK(szArgs);
			*szArgs = 0;
			for (int i = 0; i < pArgs.Count; i ++)
			{
				if (_tcslen(szArgs) > 0)
				{
					_tcscat_s(szArgs, nLen, HIDEW_SPACE);
				}
				_tcscat_s(szArgs, nLen, pArgs.List[i]);
			}
		}

		pArgs.Clean();
    }

	if (work)
	{
		if (szArgs == NULL || _tcslen(szArgs) == 0)
		{
			bShowHelp = true;
		}

		if (bShowHelp)
		{
			ShowHelp();
			work = false;
		}
	}

	if (work)
	{
		// See if this is a .., ..., ...., etc path
		if (TryGoUp(szArgs))
		{
			work = false;
		}
	}

	if (work)
	{
		ReadShortcuts();

		// Look for shortcut matches
		if (!bNoSmartMatching)
		{
			for (LinkedListItem<Result>* pCur = g_llShortcut.GetHead(); pCur; pCur = pCur->pNext)
			{
				if (_tcsicmp(pCur->Data.szShortcut, szArgs) == 0)
				{
					XUNTRACK(szArgs);
					delete szArgs;
					size_t dwLen = _tcslen(pCur->Data.szFullName) + 1;
					szArgs = new TCHAR[dwLen];
					XTRACK(szArgs);
					_tcscpy_s(szArgs, dwLen, pCur->Data.szFullName);
					break;
				}
			}

			// Now that the shortcuts have been loaded, just try it and see what happens
			if (TryStandardCD(szArgs))
			{
				work = false;
			}
		}
	}

	if (work)
	{
		if (GetFileAttributes(g_szAllDirs) == INVALID_FILE_ATTRIBUTES)
		{
			_fputts(HIDEW_NOALLDIRSINFO, stdout);
			work = false;
		}
    }

	if (work)
	{
		szPath[0] = szArgs;
		szPath[1] = NULL;

		// Try loading from the main database
		if (!ReadStoredDirs(g_charDrive, szPath, bOnlyUnder))
		{
			ret = 1;
			work = false;
		}
	}

	if (work)
	{
		// Try loading, using the list as a list of words to match
		if (!bNoSmartMatching)
		{
			if (g_llFound.GetHead() == NULL)
			{
				int nSpaces = 0;
				for (TCHAR * szCur = szArgs; *szCur; szCur++)
				{
					if (*szCur == _T(' '))
					{
						nSpaces ++;
					}
				}
				nSpaces += 2;
				XUNTRACK(szPath);
				delete szPath;
				szPath = new LPTSTR[nSpaces];
				XTRACK(szPath);
				nSpaces = 0;
				szPath[0] = szArgs;
				for (TCHAR * szCur = szArgs; *szCur; szCur++)
				{
					if (*szCur == _T(' '))
					{
						*szCur = 0;
						nSpaces ++;
						szPath[nSpaces] = szCur + 1;
					}
				}
				nSpaces ++;
				szPath[nSpaces] = NULL;

				if (!ReadStoredDirs(g_charDrive, szPath, bOnlyUnder))
				{
					work = false;
					ret = 1;
				}
			}
		}
	}

	if (work)
	{
		if (g_llFound.GetHead() == NULL)
		{
			_fputts(HIDEW_NOMATCHES, stdout);
		}
		else
		{
			int nMatches = 0;
			int nMatchOnlyDir = 0;
			size_t nMaxLen = 0;

			// Count the matches, and the directory only matches
			for (LinkedListItem<Result> * cur = g_llFound.GetHead(); cur; cur = cur->pNext)
			{
				cur->Data.bHidden = false;
				nMatches ++;
				size_t nLen = _tcslen(cur->Data.szFullName);
				if (nLen > nMaxLen)
				{
					nMaxLen = nLen;
				}

				if (cur->Data.bOnlyDir == true)
				{
					nMatchOnlyDir ++;
				}
			}

			// If there's only one match, use it
			if (nMatches == 1)
			{
				for (LinkedListItem<Result> * cur = g_llFound.GetHead(); cur; cur = cur->pNext)
				{
					if (g_bOutputOnly)
					{
						_putts(cur->Data.szFullName);
					}
					else
					{
#ifdef TEST_MODE
						_tprintf_s(_T("CD: %s\n"), cur->Data.szFullName);
#else
						ChangeParentCurrentDirectory(cur->Data.szFullName);
#endif
					}
					work = false;
				}
			}

			if (work)
			{
				// If there's only one directory match, use it
				if (!bNoSmartMatching)
				{
					if (nMatchOnlyDir == 1)
					{
						for (LinkedListItem<Result> * cur = g_llFound.GetHead(); cur; cur = cur->pNext)
						{
							if (cur->Data.bOnlyDir)
							{
								if (g_bOutputOnly)
								{
									_putts(cur->Data.szFullName);
								}
								else
								{
#ifdef TEST_MODE
									_tprintf_s(_T("CD: %s\n"), cur->Data.szFullName);
#else
									ChangeParentCurrentDirectory(cur->Data.szFullName);
#endif
								}
								work = false;
							}
						}
					}
				}
			}

			if (work)
			{
				// If all the matches descend from that first match, use the first match
				if (!bNoSmartMatching)
				{
					TCHAR * szFirst = NULL;
					size_t cFirst = 0;
					bool bAllDescend = false;
					for (LinkedListItem<Result> * cur = g_llFound.GetHead(); cur; cur = cur->pNext)
					{
						if (szFirst)
						{
							if (!((_tcslen(cur->Data.szFullName) > cFirst) &&
								  (_tcsnicmp(szFirst, cur->Data.szFullName, cFirst) == 0)))
							{
								bAllDescend = false;
								break;
							}
						}
						else
						{
							szFirst = cur->Data.szFullName;
							cFirst = _tcslen(cur->Data.szFullName);
							bAllDescend = true;
						}
					}

					if (bAllDescend)
					{
						if (g_bOutputOnly)
						{
							_putts(szFirst);
						}
						else
						{
#ifdef TEST_MODE
							_tprintf_s(_T("CD: %s\n"), szFirst);
#else
							ChangeParentCurrentDirectory(szFirst);
#endif
						}
						work = false;
					}
				}
			}

			if (work)
			{
				if (g_bOutputOnly)
				{
					for (LinkedListItem<Result> * cur = g_llFound.GetHead(); cur; cur = cur->pNext)
					{
						_putts(cur->Data.szFullName);
					}
				}
				else
				{
					// I don't know what to use, ask the user
					UI * ui = new UI(&g_llFound);
					XTRACK(ui);
					TCHAR * szSelection = ui->Show();
					if (szSelection)
					{
#ifdef TEST_MODE
						_tprintf_s(_T("CD: %s\n"), szSelection);
#else
						ChangeParentCurrentDirectory(szSelection);
#endif
					}
					XUNTRACK(ui);
					delete ui;
				}
			}
		}
	}

	XUNTRACK(szArgs);
	delete[] szArgs;

    // Clean up
	XUNTRACK(szPath);
    delete szPath;
	g_llFound.Clear();
	g_llIgnore.Clear();
	g_llShortcut.Clear();

    return ret;
}
