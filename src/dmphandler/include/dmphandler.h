#ifndef _DMP_HANDLER_H_
#define _DMP_HANDLER_H_

#ifdef _WIN32
#define DMPHANDLER_API __stdcall
#else
#define DMPHANDLER_API __cdecl
#endif//_WIN32

#ifndef IN
#define IN
#endif//IN

#ifndef OUT
#define OUT
#endif//OUT

#ifndef INOUT
#define INOUT
#endif//INOUT

#ifndef OPTIONAL
#define OPTIONAL
#endif//OPTIONAL

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

    //************************************
    // Method:    RegisterDmpHandler
    // FullName:  RegisterDmpHandler
    // Access:    public 
    // Returns:   int DMPHANDLER_API �ɹ�����0�����򷵻ش�����
    // Qualifier:
    // Description: ע���쳣��������
    // Parameter:
    //            void: 
    //************************************
    int DMPHANDLER_API RegisterDmpHandler(void);

    //************************************
    // Method:    UnRegisterDmpHandler
    // FullName:  UnRegisterDmpHandler
    // Access:    public 
    // Returns:   void DMPHANDLER_API
    // Qualifier:
    // Description: ��ע���쳣��������
    // Parameter:
    //            void: 
    //************************************
    void DMPHANDLER_API UnRegisterDmpHandler(void);

#ifdef __cplusplus
};
#endif//__cplusplus

#endif//_DMP_HANDLER_H_
