call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat"
@rem dir "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib\"

mingw32-make -C src\dmphandler all
@set compRet=%ERRORLEVEL%
@if %compRet% equ 0 (
    exit /b %compRet%
)
dir src\dmphandler
mingw32-make -C src\dmphandlerapp all

