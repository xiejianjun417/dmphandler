@rem call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat"
@rem dir "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib\"
dir "C:/ProgramData/chocolatey/lib/mingw/tools/install/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/8.1.0/"
dir "C:/ProgramData/chocolatey/lib/mingw/tools/install/mingw64/bin/../lib/gcc/"
dir "C:/ProgramData/chocolatey/lib/mingw/tools/install/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/8.1.0/../../../../x86_64-w64-mingw32/lib/../lib/"
dir "C:/ProgramData/chocolatey/lib/mingw/tools/install/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/8.1.0/../../../../lib/"
dir "C:/ProgramData/chocolatey/lib/mingw/tools/install/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/8.1.0/../../../../x86_64-w64-mingw32/lib/"
dir "C:/ProgramData/chocolatey/lib/mingw/tools/install/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/8.1.0/../../../"
cd src\dmphandler
g++ -fPIC -Wall -g -I./include -c MiniDumpHandler.cpp -o MiniDumpHandler.o
g++ -fPIC -Wall -g -I./include -c dmphandler.cpp -o dmphandler.o
g++ -v -fPIC -shared MiniDumpHandler.o dmphandler.o -o dmphandler.dll
dir .

@rem mingw32-make -C src\dmphandler all
@rem dir src\dmphandler
@rem mingw32-make -C src\dmphandlerapp all
@rem dir .

