#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <malloc.h>

#include "dmphandler.h"

int main(int argc, char **argv)
{
    RegisterDmpHandler();
    //memcpy(NULL, "dmphandler", sizeof("dmphandler"));
    char *buffer = (char *)malloc(5);
    strcpy(buffer, "123");
    strncat_s(buffer, 5, "abcdef", 5);
    UnRegisterDmpHandler();
    return 0;
}
