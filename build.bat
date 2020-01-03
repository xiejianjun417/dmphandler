call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat"
dir "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib\"
cd src\dmphandler
g++ -fPIC -std=c++11 -Wall -c MiniDumpHandler.cpp -o MiniDumpHandler.o
g++ -fPIC -std=c++11 -Wall -I./include -c dmphandler.cpp -o dmphandler.o
g++ -fPIC -shared -lmsvcrt -L"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib\" dmphandler.o MiniDumpHandler.o -o dmphandler.dll

