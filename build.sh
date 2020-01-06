#call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat"
#@rem dir "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib\"

mingw32-make -C src\dmphandler all
ret=$?
if [ $ret -ne 0 ]; then
	exit $ret
fi
dir src\dmphandler
mingw32-make -C src\dmphandlerapp all

