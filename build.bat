call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat"
@rem dir "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib\"
dir "C:/ProgramData/chocolatey/lib/mingw/tools/install/mingw64/lib"

mingw32-make -C src\dmphandler all
dir src\dmphandler
mingw32-make -C src\dmphandlerapp all
dir .

