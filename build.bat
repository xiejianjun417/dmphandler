@rem call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat"
@rem dir "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib\"
dir "C:/ProgramData/chocolatey/lib/mingw/tools/install/mingw64/x86_64-w64-mingw32"
dir "C:/ProgramData/chocolatey/lib/mingw/tools/install/mingw64/lib/gcc/x86_64-w64-mingw32/"
cd src\dmphandler
gcc -fPIC -Wall -g -I./include -c MiniDumpHandler.cpp -o MiniDumpHandler.o
gcc -fPIC -Wall -g -I./include -c dmphandler.cpp -o dmphandler.o
g++ -fPIC -shared MiniDumpHandler.o dmphandler.o -o dmphandler.dll
dir .

@remmingw32-make -C src\dmphandler all
@remdir src\dmphandler
@rem mingw32-make -C src\dmphandlerapp all
@rem dir .

