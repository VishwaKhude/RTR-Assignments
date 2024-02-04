cls

del *.obj

del *.exe

del *.txt

cl.exe /c /EHsc /I..\..\..\..\include\ OGL.c

rc.exe /I..\..\..\..\include OGL.rc

link.exe OGL.obj OGL.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS /OUT:..\..\..\..\bin\%1.exe

del *.obj

..\..\..\..\bin\%1.exe 
