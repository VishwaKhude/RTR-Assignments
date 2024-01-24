cls

del *.obj

del *.exe

del *.txt

cl.exe /c /EHsc /I..\..\..\include\ Smiley.c

rc.exe /I..\..\..\include\ ..\..\..\resource\Smiley.rc

link.exe Smiley.obj ..\..\..\resource\Smiley.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS /OUT:..\..\..\bin\%1.exe

del *.obj

..\..\..\bin\%1.exe