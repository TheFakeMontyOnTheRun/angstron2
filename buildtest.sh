g++ -g -w -DWGA -DFPU -DDEBUG -DUBUNTU test.cpp SDL_gfx/SDL_gfxPrimitives.c sprig/*.c  -otest  `sdl-config --cflags --libs`  -lSDL_mixer
