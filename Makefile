all:
	mkdir -p bin
	g++ src/* ext/glad/glad.c ext/glutils/src/* -o bin/main.exe -Iinclude -Iext -Iext/glutils/include -Iext/sdl -Llib -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lole32 -lwinmm -lgdi32 -limm32 -lversion -lsetupapi -loleaut32 -Wfatal-errors -std=c++20