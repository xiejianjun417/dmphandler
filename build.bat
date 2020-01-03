call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
dir "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\redist\x64\"
cd src\dmphandler
g++ -fPIC -std=c++11 -Wall -c MiniDumpHandler.cpp -o MiniDumpHandler.o
g++ -fPIC -std=c++11 -Wall -I./include -c dmphandler.cpp -o dmphandler.o
g++ -fPIC -shared -lmsvcr150 -lmsvcp150 dmphandler.o MiniDumpHandler.o -o dmphandler.dll

