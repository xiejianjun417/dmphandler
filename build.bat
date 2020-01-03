call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat"
dir "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\redist\x64\Microsoft.VC140.CRT\"
cd src\dmphandler
g++ -fPIC -std=c++11 -Wall -c MiniDumpHandler.cpp -o MiniDumpHandler.o
g++ -fPIC -std=c++11 -Wall -I./include -c dmphandler.cpp -o dmphandler.o
g++ -fPIC -shared -lvcruntime140 -lmsvcp140 -L"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\redist\x64\Microsoft.VC140.CRT\" dmphandler.o MiniDumpHandler.o -o dmphandler.dll

