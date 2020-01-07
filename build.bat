@rem call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat"
@rem dir "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib\"
@rem dir "C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64\x86_64-w64-mingw32\lib"
@rem dir "C:/ProgramData/chocolatey/lib/mingw/tools/install/mingw64/lib/gcc/x86_64-w64-mingw32/"

mingw32-make -C src\dmphandler all
dir src\dmphandler
mingw32-make -C src\dmphandlerapp all
dir src\dmphandlerapp

