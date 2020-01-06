#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <time.h>
#include <Windows.h>
#include <ImageHlp.h>
#include "MiniDumpHandler.h"

#define DEF_DUMP_NAME_PREFIX    "C:\\Windows\\Minidump\\"

typedef BOOL (WINAPI *FnMINIDUMPWRITEDUMP)(HANDLE hProcess,
    DWORD dwPid,
    HANDLE hFile,
    MINIDUMP_TYPE DumpType,
    CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
    CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
    CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

CMiniDumpHandler *CMiniDumpHandler::m_instance = NULL;

static void DisableSetUnhandledExceptionFilter(void)
{
    void *addr = (void*)GetProcAddress(LoadLibraryA(("kernel32.dll")),
        "SetUnhandledExceptionFilter");
    if (addr) 
    {
        unsigned char code[16];
        int size = 0;
        code[size++] = 0x33;
        code[size++] = 0xC0;
        code[size++] = 0xC2;
        code[size++] = 0x04;
        code[size++] = 0x00;

        DWORD dwOldFlag, dwTempFlag;
        VirtualProtect(addr, size, PAGE_READWRITE, &dwOldFlag);
        WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);
        VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);
    }
}

static int GetImpersonationToken(IN OUT HANDLE * phToken)
{
    int ret = 0;

    assert(NULL != phToken);

    *phToken = NULL;
    if (!OpenThreadToken( GetCurrentThread(),
        TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, TRUE, phToken))
    {
        ret = GetLastError();
        if (ERROR_NO_TOKEN == ret)
        {
            if(!OpenProcessToken(GetCurrentProcess(),
                TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, phToken))
            {
                ret = GetLastError();
            }
            else
            {
                ret = 0;
            }
        }
    }
    return ret;
}

static BOOL SetPrivilege(
    HANDLE hToken,          // access token handle
    LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
    BOOL bEnablePrivilege,   // to enable or disable privilege
    TOKEN_PRIVILEGES* ptpOld = NULL) 
{
    TOKEN_PRIVILEGES tp;
    LUID luid;
    DWORD dwReturn = 0;

    if ( !LookupPrivilegeValue( 
        NULL,            // lookup privilege on local system
        lpszPrivilege,   // privilege to lookup 
        &luid ) )        // receives LUID of privilege
    {
        printf("LookupPrivilegeValue error: %u\n", (unsigned int)GetLastError() ); 
        return FALSE; 
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    // Enable the privilege or disable all privileges.

    if ( !AdjustTokenPrivileges(
        hToken, 
        FALSE, 
        &tp, 
        sizeof(TOKEN_PRIVILEGES),
        ptpOld, 
        ((NULL != ptpOld) ? &dwReturn : NULL) ))
    { 
        printf("AdjustTokenPrivileges error: %u\n", (unsigned int)GetLastError() ); 
        return FALSE; 
    } 

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

    {
        printf("The token does not have the specified privilege. \n");
        return FALSE;
    } 

    return TRUE;
}

static BOOL RestorePrivilege( HANDLE hToken, TOKEN_PRIVILEGES* ptpOld )
{
    BOOL bRet = AdjustTokenPrivileges(hToken, FALSE, ptpOld, 0, NULL, NULL);
    return ( bRet && (ERROR_NOT_ALL_ASSIGNED != GetLastError()) );
}

static void InnerMakeupDumpFilenameA(char *buffer, unsigned int bufLen)
{
    time_t curTime;
    struct tm localTime;

    time(&curTime);
    if(0 == localtime_s(&localTime, &curTime))
    {
        localTime.tm_year += 1900;
        localTime.tm_mon += 1;
        sprintf_s(buffer, bufLen, "_%04d%02d%02d%02d%02d%02d.dmp", localTime.tm_year,
            localTime.tm_mon, localTime.tm_mday, localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
    }
    else
    {
        srand((unsigned int)curTime);
        sprintf_s(buffer, bufLen, "_%d.dmp", rand());
    }
}
#if 0
static void InnerMakeupDumpFilenameW(wchar_t *buffer, unsigned int bufLen)
{
    time_t curTime;
    struct tm localTime;

    time(&curTime);
    if(0 == localtime_s(&localTime, &curTime))
    {
        localTime.tm_year += 1900;
        localTime.tm_mon += 1;
        swprintf_s(buffer, bufLen, L"_%04d%02d%02d%02d%02d%02d.dmp", localTime.tm_year,
            localTime.tm_mon, localTime.tm_yday, localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
    }
    else
    {
        srand((unsigned int)curTime);
        swprintf_s(buffer, bufLen, L"_%d.dmp", rand());
    }
}
#endif//
static void __cdecl InnerInvalidParameterHandler(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t)
{
    printf("InnerInvalidParameterHandler\n");
}

static void __cdecl InnerPurecallHandler(void)
{
    printf("InnerPurecallHandler\n");
}


int CMiniDumpHandler::CreateInstance(void)
{
    int ret = 0;
    CMiniDumpHandler *instance;

    assert(NULL == m_instance);
    
   instance = new CMiniDumpHandler();
   if (NULL != instance)
   {
       ret = instance->Initialize();
       if (0 != ret)
       {
           delete instance;
       }
       else
       {
           m_instance = instance;
       }
   }
   else
   {
       ret = ENOMEM;
   }
   return ret;
}

void CMiniDumpHandler::DestroyInstance(void)
{
    if (NULL != m_instance)
    {
        delete m_instance;
        m_instance = NULL;
    }
}

CMiniDumpHandler * CMiniDumpHandler::GetInstance(void)
{
    assert(NULL != m_instance);
    return m_instance;
}

CMiniDumpHandler::CMiniDumpHandler(void)
{
    InitializeCriticalSection(&m_lock);
}

CMiniDumpHandler::~CMiniDumpHandler(void)
{
    DeleteCriticalSection(&m_lock);
}

int CMiniDumpHandler::Initialize(void)
{
    SetUnhandledExceptionFilter(ExceptionHandlerCallback);
    _set_abort_behavior(0, _WRITE_ABORT_MSG);
    _set_abort_behavior(0, _CALL_REPORTFAULT);
    _set_invalid_parameter_handler(InnerInvalidParameterHandler);
    _set_purecall_handler(InnerPurecallHandler);
    DisableSetUnhandledExceptionFilter();
    return 0;
}

LONG WINAPI CMiniDumpHandler::ExceptionHandlerCallback(struct _EXCEPTION_POINTERS *pExceptionInfo)
{
    LONG ret;

    assert(NULL != m_instance);

    EnterCriticalSection(&m_instance->m_lock);
    ret = m_instance->WriteMiniDump(pExceptionInfo);
    LeaveCriticalSection(&m_instance->m_lock);
    return ret;
}

LONG CMiniDumpHandler::WriteMiniDump(_EXCEPTION_POINTERS *pExceptionInfo)
{
    LONG ret = EXCEPTION_CONTINUE_SEARCH;
    HANDLE hImpersonationToken = NULL;
    char appPath[MAX_PATH] = {0};
    char *pSep;
    HMODULE hModule = NULL;
    FnMINIDUMPWRITEDUMP fnMiniDumpWriteDump;
    HANDLE hFile = INVALID_HANDLE_VALUE;
    MINIDUMP_EXCEPTION_INFORMATION ExInfo;
    TOKEN_PRIVILEGES tokenPriv;

    hModule = LoadLibraryA("dbghelp.dll");
    if (NULL == hModule)
    {
        goto _OUT;
    }
    fnMiniDumpWriteDump = (FnMINIDUMPWRITEDUMP)GetProcAddress( hModule, "MiniDumpWriteDump");
    if (NULL == fnMiniDumpWriteDump)
    {
        goto _OUT;
    }
    if (0 != GetImpersonationToken(&hImpersonationToken))
    {
        goto _OUT;
    }
    if (0 == GetModuleFileNameA(NULL, appPath, sizeof(appPath))
        || (NULL == (pSep = strrchr(appPath, '.'))))
    {
        strcpy_s(appPath, sizeof(appPath), DEF_DUMP_NAME_PREFIX);
        pSep = appPath + sizeof(DEF_DUMP_NAME_PREFIX) - 1;
    }
    do 
    {
        InnerMakeupDumpFilenameA(pSep, sizeof(appPath) - (unsigned int)(pSep - appPath + 1));
        hFile = CreateFileA(appPath, GENERIC_WRITE, FILE_SHARE_READ, NULL,
            CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        if (INVALID_HANDLE_VALUE != hFile)//success
        {
            break;
        }
        else if (ERROR_FILE_EXISTS == GetLastError())//file exists
        {
            continue;
        }
        else// other error
        {
            goto _OUT;
        }
    } while (TRUE);
    ExInfo.ThreadId          = GetCurrentThreadId();
    ExInfo.ExceptionPointers = pExceptionInfo;
    ExInfo.ClientPointers    = FALSE;
    if (!SetPrivilege(hImpersonationToken, SE_DEBUG_NAME, TRUE, &tokenPriv))
    {
        goto _OUT;
    }
    if(fnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile,
        MiniDumpNormal, &ExInfo, NULL, NULL))
    {
        ret = EXCEPTION_EXECUTE_HANDLER;
    }
    RestorePrivilege(hImpersonationToken, &tokenPriv);
_OUT:
    if (INVALID_HANDLE_VALUE != hFile)
    {
        CloseHandle(hFile);
    }
    if (NULL != hModule)
    {
        FreeLibrary(hModule);
    }
    if (NULL != hImpersonationToken)
    {
        CloseHandle(hImpersonationToken);
    }
    //TerminateProcess( GetCurrentProcess(), 0 );
    return ret;
}
