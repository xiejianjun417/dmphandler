#ifndef _MINI_DUMP_HANDLER_H_
#define _MINI_DUMP_HANDLER_H_

#include <Windows.h>

class CMiniDumpHandler
{
public:
    static int CreateInstance(void);
    static void DestroyInstance(void);
    static CMiniDumpHandler *GetInstance(void);

private:
    CMiniDumpHandler(void);
    ~CMiniDumpHandler(void);

    int Initialize(void);

    static LONG WINAPI ExceptionHandlerCallback(struct _EXCEPTION_POINTERS *pExceptionInfo);

    LONG WriteMiniDump(_EXCEPTION_POINTERS *pExceptionInfo );

private:

    static CMiniDumpHandler* m_instance;

    CRITICAL_SECTION m_lock;

};

#endif//_MINI_DUMP_HANDLER_H_
