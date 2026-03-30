cls
cl.exe /I..\include /c /EHsc D3D.cpp
rc.exe D3D.rc
link.exe D3D.obj D3D.res /LIBPATH:..\lib user32.lib gdi32.lib /SUBSYSTEM:WINDOWS
D3D.exe