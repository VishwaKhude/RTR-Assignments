cls
cl.exe /I..\include /c /EHsc OGL.cpp
link.exe OGL.obj /LIBPATH:..\lib user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

OGL.exe