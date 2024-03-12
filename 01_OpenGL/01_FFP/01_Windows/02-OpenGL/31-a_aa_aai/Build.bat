cls

cl.exe /c /EHsc a_aa_aai.c

link.exe  a_aa_aai.obj user32.lib gdi32.lib /SUBSYSTEM:WINDOWS 

a_aa_aai.exe
