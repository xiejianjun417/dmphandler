call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat"
@rem dir "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib\"

@rem g++ -fPIC -std=c++11 -Wall -c MiniDumpHandler.cpp -o MiniDumpHandler.o
@rem g++ -fPIC -std=c++11 -Wall -I./include -c dmphandler.cpp -o dmphandler.o
@rem g++ -fPIC -shared -lmsvcrt -L"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib\" dmphandler.o MiniDumpHandler.o -o dmphandler.dll
make -C src\dmphandler all
make -C src\dmphandlerapp all

