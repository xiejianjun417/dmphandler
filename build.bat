call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat"
dir "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib\"

mingw32-make -C src\dmphandler all
if %ERRORLEVEL% NEQ 0 goto :EOF 
dir src\dmphandler
mingw32-make -C src\dmphandlerapp all

