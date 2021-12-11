#include "main.h"

#include <windows.h>
#include <stdio.h>

#include "RemoteThread.h"
#include "ProcessInfo.h"
#include "Hidden.h"

const DWORD MAXINJECTSIZE = 4096;

typedef HMODULE (__stdcall *PLoadLibraryW)(wchar_t*);
typedef HMODULE (__stdcall *PGetModuleHandleW)(wchar_t*);
typedef BOOL    (__stdcall *PFreeLibrary)(HMODULE);
typedef FARPROC (__stdcall *PGetProcAddress)(HMODULE, char*);
typedef BOOL    (__stdcall *PSetCurrentDirectory)(wchar_t*);
typedef DWORD   (__stdcall *PGetCurrentDirectory)(DWORD, wchar_t*);
typedef BOOL    (__stdcall *PSetEnvironmentVariable)(wchar_t*,wchar_t*);

struct RemoteThreadBlock
{
    PSetCurrentDirectory    fnSetCurrentDirectory;
    PGetCurrentDirectory    fnGetCurrentDirectory;
    PGetProcAddress         fnGetProcAddress;
    PSetEnvironmentVariable fnSetEnvironmentVariable;
    wchar_t                 szDirectoryPath[512];
    wchar_t                 szFullPath[512];
    wchar_t                 szOldPath[512];
    wchar_t                 szDriveName[5];
    DWORD                   dwReturn;
};

#define ERROR_UNABLE_TO_GET_CURRENT_DIR     1
#define ERROR_UNABLE_TO_SET_CURRENT_DIR     2
#define ERROR_NETWORK_PATH_SPECIFIED        3
#define ERROR_UNABLE_TO_SET_ENVIORNMENT     4

void    EnableDebugPriv(void);
bool    ExecuteRemoteThread(HANDLE, wchar_t*);
DWORD   LoadDllForRemoteThread(DWORD, wchar_t*);
BOOL    IsWindowsNT();
DWORD __stdcall RemoteThread(RemoteThreadBlock*);
extern bool g_bIsCMD = false;


/* -------------------------------------------------------------------- */
/* ----------- Local APIs --------------------------------------------- */
/* -------------------------------------------------------------------- */

HMODULE Local_Kernel32()
{
    static HMODULE ret = NULL;
    if (!ret)
    {
        ret = LoadLibraryW(HIDEW_KERNEL32);
    }
    return ret;
}

HMODULE Local_Advapi32()
{
    static HMODULE ret = NULL;
    if (!ret)
    {
        ret = LoadLibraryW(HIDEW_ADVAPI32);
    }
    return ret;
}

HANDLE Local_OpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId)
{
	if (Local_Kernel32())
	{
        static FARPROC ret = NULL;

		if (!ret)
		{
			ret = GetProcAddress(Local_Kernel32(), HIDEA_OPENPROCESS);
		}

		if (ret)
		{
		    typedef HANDLE (WINAPI * PFN_Func)(DWORD, BOOL, DWORD);
			return ((PFN_Func) ret)(dwDesiredAccess, bInheritHandle, dwProcessId);
		}
	}

	return NULL;
}

HANDLE Local_GetCurrentProcess()
{
	if (Local_Kernel32())
	{
        static FARPROC ret = NULL;

		if (!ret)
		{
			ret = GetProcAddress(Local_Kernel32(), HIDEA_GETCURRENTPROCESS);
		}

		if (ret)
		{
		    typedef HANDLE (WINAPI * PFN_Func)();
			return ((PFN_Func) ret)();
		}
	}

	return NULL;
}

BOOL Local_CloseHandle(HANDLE hObject)
{
	if (Local_Kernel32())
	{
        static FARPROC ret = NULL;

		if (!ret)
		{
			ret = GetProcAddress(Local_Kernel32(), HIDEA_CLOSEHANDLE);
		}

		if (ret)
		{
		    typedef BOOL (WINAPI * PFN_Func)(HANDLE);
			return ((PFN_Func) ret)(hObject);
		}
	}

	return FALSE;
}

BOOL Local_OpenProcessToken(HANDLE ProcessHandle, DWORD DesiredAccess, PHANDLE TokenHandle)
{
	if (Local_Advapi32())
	{
        static FARPROC ret = NULL;

		if (!ret)
		{
			ret = GetProcAddress(Local_Advapi32(), HIDEA_OPENPROCESSTOKEN);
		}

		if (ret)
		{
		    typedef BOOL (WINAPI * PFN_Func)(HANDLE, DWORD, PHANDLE);
			return ((PFN_Func) ret)(ProcessHandle, DesiredAccess, TokenHandle);
		}
	}

	return FALSE;
}

BOOL Local_LookupPrivilegeValueW(LPCWSTR lpSystemName, LPCWSTR lpName, PLUID lpLuid)
{
    if (Local_Advapi32())
    {
        static FARPROC ret = NULL;

        if (!ret)
        {
            ret = GetProcAddress(Local_Advapi32(), HIDEA_LOOKUPPRIVILEGEVALUEW);
        }

        if (ret)
        {
            typedef BOOL (WINAPI * PFN_Func)(LPCWSTR, LPCWSTR, PLUID);
            return ((PFN_Func) ret)(lpSystemName, lpName, lpLuid);
        }
    }

    return FALSE;
}

BOOL Local_AdjustTokenPrivileges(HANDLE TokenHandle, BOOL DisableAllPrivileges, PTOKEN_PRIVILEGES NewState, DWORD BufferLength, PTOKEN_PRIVILEGES PreviousState, PDWORD ReturnLength)
{
    if (Local_Advapi32())
    {
        static FARPROC ret = NULL;

        if (!ret)
        {
            ret = GetProcAddress(Local_Advapi32(), HIDEA_ADJUSTTOKENPRIVILEGES);
        }

        if (ret)
        {
            typedef BOOL (WINAPI * PFN_Func)(HANDLE, BOOL, PTOKEN_PRIVILEGES, DWORD, PTOKEN_PRIVILEGES, PDWORD);
            return ((PFN_Func) ret)(TokenHandle, DisableAllPrivileges, NewState, BufferLength, PreviousState, ReturnLength);
        }
    }

    return FALSE;
}

LPVOID Local_VirtualAllocEx(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect)
{
    if (Local_Kernel32())
    {
        static FARPROC ret = NULL;

        if (!ret)
        {
            ret = GetProcAddress(Local_Kernel32(), HIDEA_VIRTUALALLOCEX);
        }

        if (ret)
        {
            typedef LPVOID (WINAPI * PFN_Func)(HANDLE, LPVOID, SIZE_T, DWORD, DWORD);
            return ((PFN_Func) ret)(hProcess, lpAddress, dwSize, flAllocationType, flProtect);
        }
    }

    return NULL;
}

BOOL Local_WriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T *lpNumberOfBytesWritten)
{
    if (Local_Kernel32())
    {
        static FARPROC ret = NULL;

        if (!ret)
        {
            ret = GetProcAddress(Local_Kernel32(), HIDEA_WRITEPROCESSMEMORY);
        }

        if (ret)
        {
            typedef BOOL (WINAPI * PFN_Func)(HANDLE, LPVOID, LPCVOID, SIZE_T, SIZE_T*);
            return ((PFN_Func) ret)(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
        }
    }

    return FALSE;
}

HANDLE Local_CreateRemoteThread(HANDLE hProcess, LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId)
{
    if (Local_Kernel32())
    {
        static FARPROC ret = NULL;

        if (!ret)
        {
            ret = GetProcAddress(Local_Kernel32(), HIDEA_CREATEREMOTETHREAD);
        }

        if (ret)
        {
            typedef HANDLE (WINAPI * PFN_Func)(HANDLE, LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD);
            return ((PFN_Func) ret)(hProcess, lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
        }
    }

    return NULL;
}

DWORD Local_WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds)
{
    if (Local_Kernel32())
    {
        static FARPROC ret = NULL;

        if (!ret)
        {
            ret = GetProcAddress(Local_Kernel32(), HIDEA_WAITFORSINGLEOBJECT);
        }

        if (ret)
        {
            typedef DWORD (WINAPI * PFN_Func)(HANDLE, DWORD);
            return ((PFN_Func) ret)(hHandle, dwMilliseconds);
        }
    }

    return 0;
}

BOOL Local_ReadProcessMemory(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize, SIZE_T *lpNumberOfBytesRead)
{
    if (Local_Kernel32())
    {
        static FARPROC ret = NULL;

        if (!ret)
        {
            ret = GetProcAddress(Local_Kernel32(), HIDEA_READPROCESSMEMORY);
        }

        if (ret)
        {
            typedef BOOL (WINAPI * PFN_Func)(HANDLE, LPCVOID, LPVOID, SIZE_T, SIZE_T*);
            return ((PFN_Func) ret)(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
        }
    }

    return FALSE;
}

BOOL Local_VirtualFreeEx(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType)
{
    if (Local_Kernel32())
    {
        static FARPROC ret = NULL;

        if (!ret)
        {
            ret = GetProcAddress(Local_Kernel32(), HIDEA_VIRTUALFREEEX);
        }

        if (ret)
        {
            typedef BOOL (WINAPI * PFN_Func)(HANDLE, LPVOID, SIZE_T, DWORD);
            return ((PFN_Func) ret)(hProcess, lpAddress, dwSize, dwFreeType);
        }
    }

    return FALSE;
}

/* -------------------------------------------------------------------- */
/* ----------- End of Local APIs -------------------------------------- */
/* -------------------------------------------------------------------- */

void ChangeParentCurrentDirectory(wchar_t* szNewDirectory)
{
#ifndef _DEBUG
    if (g_bIsCMD)
    {
        EnableDebugPriv();

        DWORD dwID = GetParentProcessID(GetCurrentProcessId());

        LoadDllForRemoteThread(dwID, szNewDirectory);
    }
    else
    {
#endif
#ifdef _DEBUG
        OutputDebugString(HIDEW_CHANGETO);
        OutputDebugStringW(szNewDirectory);
        OutputDebugString(HIDEW_LINEFEED);
        // DebugBreak();
#endif
        _tprintf(HIDEW_CHANGETOPARAM, szNewDirectory);
#ifndef _DEBUG
    }
#endif
}

BOOL IsWindowsNT()
{
   OSVERSIONINFOEX osvi;
   BOOL bOsVersionInfoEx;

   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

#pragma warning (push)
#pragma warning (disable : 4996)
   bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi);
#pragma warning (pop)

   if(bOsVersionInfoEx == 0)
   {
      osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
#pragma warning (push)
#pragma warning (disable : 4996)
	  if (!GetVersionEx((OSVERSIONINFO *)&osvi))
#pragma warning (pop)
	  {
         return FALSE;
      }
   }

   return osvi.dwPlatformId == VER_PLATFORM_WIN32_NT;
}


DWORD LoadDllForRemoteThread(DWORD processID, wchar_t* lpPathName)
{
    HANDLE hProcess = Local_OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION |
            PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, 
            FALSE, processID);

    if (hProcess != NULL)
    {
        ExecuteRemoteThread(hProcess, lpPathName);

        Local_CloseHandle(hProcess);
    }

    return 0;
}

void EnableDebugPriv(void)
{
    HANDLE hToken;
    LUID sedebugnameValue;
    TOKEN_PRIVILEGES tkp;

    Local_OpenProcessToken(Local_GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

    Local_LookupPrivilegeValueW(NULL, SE_DEBUG_NAME, &sedebugnameValue);

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = sedebugnameValue;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    Local_AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL);

    Local_CloseHandle(hToken);
}

bool ExecuteRemoteThread(HANDLE hProcess, wchar_t* lpPathName)
{
    HANDLE ht = 0;
    void *p = 0;
    RemoteThreadBlock *c = 0;
    bool result = false;
    DWORD rc;
    HMODULE hKernel32 = 0;
    RemoteThreadBlock localCopy;

    ZeroMemory(&localCopy, sizeof(localCopy));

    p = Local_VirtualAllocEx(hProcess, 0, MAXINJECTSIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (p == 0)
    {
        goto cleanup;
    }

    c = (RemoteThreadBlock*) Local_VirtualAllocEx(hProcess, 0, sizeof(RemoteThreadBlock), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (c == 0)
    {
        goto cleanup;
    }

    if (!Local_WriteProcessMemory(hProcess, p, &RemoteThread, MAXINJECTSIZE, 0))
    {
        goto cleanup;
    }

    hKernel32 = LoadLibrary(HIDEW_KERNEL32);

    if (hKernel32 == NULL)
    {
        goto cleanup;
    }

    localCopy.fnGetProcAddress = (PGetProcAddress)GetProcAddress(hKernel32, HIDEA_GETPROCADDRESS);
    localCopy.fnSetCurrentDirectory = (PSetCurrentDirectory)GetProcAddress(hKernel32, HIDEA_SETCURRENTDIRECTORYW);
    localCopy.fnGetCurrentDirectory = (PGetCurrentDirectory)GetProcAddress(hKernel32, HIDEA_GETCURRENTDIRECTORYW);
    localCopy.fnSetEnvironmentVariable = (PSetEnvironmentVariable)GetProcAddress(hKernel32, HIDEA_SETENVIRONMENTVARIABLEW);

    if (localCopy.fnSetCurrentDirectory == NULL ||
        localCopy.fnGetCurrentDirectory == NULL ||
        localCopy.fnGetProcAddress == NULL ||
        localCopy.fnSetEnvironmentVariable == NULL)
    {
        goto cleanup;
    }

    wchar_t szDriveName[512];
    wchar_t lpFullName[512];
    ZeroMemory(szDriveName, sizeof szDriveName);
    ZeroMemory(lpFullName, sizeof lpFullName);
    GetFullPathName(lpPathName, _countof(lpFullName), lpFullName, NULL);

    szDriveName[1] = lpFullName[0];
    szDriveName[0] = L'=';
    szDriveName[2] = L':';
    szDriveName[3] = 0;

    wcscpy_s(localCopy.szDirectoryPath, lpPathName);
    wcscpy_s(localCopy.szDriveName, szDriveName);
    wcscpy_s(localCopy.szFullPath, lpFullName);

    if (!Local_WriteProcessMemory(hProcess, c, &localCopy, sizeof localCopy, 0))
    {
        goto cleanup;
    }

    ht = Local_CreateRemoteThread(hProcess, 0, 0, (DWORD (__stdcall *)(void *)) p, c, 0, &rc);
    if (ht == NULL)
    {
        goto cleanup;
    }

    rc = Local_WaitForSingleObject(ht, INFINITE);
    switch (rc)
    {
    case WAIT_TIMEOUT:
        goto cleanup;
    case WAIT_FAILED:
        goto cleanup;
    case WAIT_OBJECT_0:
        if (!Local_ReadProcessMemory(hProcess, c, &localCopy, sizeof localCopy, 0))
        {
            goto cleanup;
        }
        else
        {
            switch (localCopy.dwReturn)
            {
            case ERROR_UNABLE_TO_GET_CURRENT_DIR:
                _tprintf(HIDEW_UNABLETOGET);
                break;
            case ERROR_UNABLE_TO_SET_CURRENT_DIR:
                _tprintf(HIDEW_UNABLETOSET);
                break;
            case ERROR_NETWORK_PATH_SPECIFIED:
                _tprintf(HIDEW_NETWORKPATH);
                break;
            case ERROR_UNABLE_TO_SET_ENVIORNMENT:
                _tprintf(HIDEW_UNABLETOSETENV);
                break;
            }
        }
        break;
    default:
        break;
    }

cleanup:
    Local_CloseHandle(ht);

    if (p != 0)
    {
        Local_VirtualFreeEx(hProcess, p, 0, MEM_RELEASE);
    }
    if (c != 0)
    {
        Local_VirtualFreeEx(hProcess, c, 0, MEM_RELEASE);
    }
    if (hKernel32 != NULL)
    {
        FreeLibrary(hKernel32);
    }

    return result;
}


DWORD __stdcall RemoteThread(RemoteThreadBlock* execBlock)
{
    typedef DWORD (*PRemoteDllFunction)();

    if ((*execBlock->fnGetCurrentDirectory)(_countof(execBlock->szOldPath), execBlock->szOldPath) == 0)
    {
        execBlock->dwReturn = ERROR_UNABLE_TO_GET_CURRENT_DIR;
        return execBlock->dwReturn;
    }

    if (!(*execBlock->fnSetCurrentDirectory)(execBlock->szDirectoryPath))
    {
        execBlock->dwReturn = ERROR_UNABLE_TO_SET_CURRENT_DIR;
        return execBlock->dwReturn;
    }

    if ((*execBlock->fnGetCurrentDirectory)(_countof(execBlock->szFullPath), execBlock->szFullPath) == 0)
    {
        execBlock->dwReturn = ERROR_UNABLE_TO_GET_CURRENT_DIR;
        return execBlock->dwReturn;
    }

    if (execBlock->szFullPath[0] == _T('\\'))
    {
        if (!(*execBlock->fnSetCurrentDirectory)(execBlock->szOldPath))
        {
            execBlock->dwReturn = ERROR_UNABLE_TO_SET_CURRENT_DIR;
            return execBlock->dwReturn;
        }
        execBlock->dwReturn = ERROR_NETWORK_PATH_SPECIFIED;
        return execBlock->dwReturn;
    }

    if (!(*execBlock->fnSetEnvironmentVariable)(execBlock->szDriveName, execBlock->szFullPath))
    {
        execBlock->dwReturn = ERROR_UNABLE_TO_SET_ENVIORNMENT;
        return execBlock->dwReturn;
    }

    execBlock->dwReturn = 0;
    return 0;
}
