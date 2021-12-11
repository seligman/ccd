#include "main.h"

#include <windows.h>
#include <tchar.h>
#include "ProcessInfo.h"

typedef LONG NTSTATUS;

typedef struct _tagPROCESS_BASIC_INFORMATION
{
    ULONG_PTR ExitStatus;
    ULONG_PTR PebBaseAddress;
    ULONG_PTR AffinityMask;
    ULONG_PTR BasePriority;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR InheritedFromUniqueProcessId;
}   PROCESS_BASIC_INFORMATION;

typedef enum _tagPROCESSINFOCLASS 
{
    ProcessBasicInformation,
    ProcessQuotaLimits,
    ProcessIoCounters,
    ProcessVmCounters,
    ProcessTimes,
    ProcessBasePriority,
    ProcessRaisePriority,
    ProcessDebugPort,
    ProcessExceptionPort,
    ProcessAccessToken,
    ProcessLdtInformation,
    ProcessLdtSize,
    ProcessDefaultHardErrorMode,
    ProcessIoPortHandlers,
    ProcessPooledUsageAndLimits,
    ProcessWorkingSetWatch,
    ProcessUserModeIOPL,
    ProcessEnableAlignmentFaultFixup,
    ProcessPriorityClass,
    MaxProcessInfoClass
}   PROCESSINFOCLASS;


#define DYNLOADED_FPTR( ptrname, procname, dllname)\
FPTR_##procname ptrname = \
( FPTR_##procname ) GetProcAddress ( GetModuleHandle (  _TEXT( #dllname)), #procname);

#define CREATE_DYNFUNC_5( ptrname, procname, dllname, rettype, callconv, a1, a2, a3, a4, a5)\
typedef  rettype (callconv *FPTR_##procname) ( a1, a2, a3, a4, a5);\
DYNLOADED_FPTR( ptrname, procname, dllname);

BOOL IsNT()
{
    OSVERSIONINFO osviVerInfo;

    osviVerInfo.dwOSVersionInfoSize = sizeof  (OSVERSIONINFO);
    
    GetVersionEx (&osviVerInfo);

    return (VER_PLATFORM_WIN32_NT == osviVerInfo.dwPlatformId);
}

ULONG_PTR xGetParentProcessID (ULONG_PTR dwPID)
{
    NTSTATUS ntStatus;
    ULONG_PTR dwParentPID = 0xffffffff;

    HANDLE hProcess;
    PROCESS_BASIC_INFORMATION pbi;
    ULONG ulRetLen;

    CREATE_DYNFUNC_5(NtQueryInformationProcess,
                     NtQueryInformationProcess,
                     ntdll,
                     NTSTATUS,
                     __stdcall,
                     HANDLE,
                     PROCESSINFOCLASS,
                     PVOID,
                     ULONG,
                     PULONG);

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,
                           FALSE,
                           (DWORD)dwPID);

    if (!hProcess)
    {
        return (0xffffffff);
    }

    ntStatus = NtQueryInformationProcess(hProcess,
                            ProcessBasicInformation,
                            ( void*) &pbi,
                            sizeof  (PROCESS_BASIC_INFORMATION),
                            &ulRetLen);

    if (!ntStatus)
    {
        dwParentPID = pbi.InheritedFromUniqueProcessId;
    }

    CloseHandle(hProcess);

    return (dwParentPID);
}

ULONG_PTR GetParentProcessID (ULONG_PTR dwPID)
{
	if (sizeof(ULONG_PTR) == 8)
	{
		return xGetParentProcessID(xGetParentProcessID(dwPID));
	}
	else
	{
		return xGetParentProcessID(dwPID);
	}
}
