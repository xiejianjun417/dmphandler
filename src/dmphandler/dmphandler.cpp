#include "dmphandler.h"
#include "MiniDumpHandler.h"

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

    int DMPHANDLER_API RegisterDmpHandler(void)
    {
        return CMiniDumpHandler::CreateInstance();
    }

    void DMPHANDLER_API UnRegisterDmpHandler(void)
    {
        CMiniDumpHandler::DestroyInstance();
    }

#ifdef __cplusplus
};
#endif//__cplusplus
