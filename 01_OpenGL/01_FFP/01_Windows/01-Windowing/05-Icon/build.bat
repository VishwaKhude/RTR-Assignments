cls

del *.obj
del *.exe
del *.txt

cl.exe /c /EHsc /I ..\Include\ *.c

rc.exe /i ..\Include\ ..\Resource\window.rc 

link *.obj ..\Resource\window.res user32.lib gdi32.lib kernel32.lib /OUT:..\bin\%1.exe

del *.obj

..\bin\%1.exe