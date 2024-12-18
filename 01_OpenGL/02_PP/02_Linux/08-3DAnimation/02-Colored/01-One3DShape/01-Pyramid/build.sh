g++ -c -o OGL.o -I /usr/include-IGLU OGL.cpp 

g++ -o OGL -L /usr/lib/x86_64-linux-gnu  OGL.o -lX11 -lGL -lGLU -lGLEW
