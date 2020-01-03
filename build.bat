cd src\dmphandler
gcc -fPIC -std=c++11 -Wall -c MiniDumpHandler.cpp -o MiniDumpHandler.o
gcc -fPIC -std=c++11 -Wall -c dmphandler.cpp -o dmphandler.o
gcc -fPIC -shared dmphandler.o MiniDumpHandler.o -o dmphandler.dll

