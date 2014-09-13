g++ -g -w -DWGA -DFPU -DDEBUG -DSUPPORT_BITMAP_FONTS -DUBUNTU slicer.cpp SDL_gfx/SDL_gfxPrimitives.c sprig/*.c  -oslicer  `sdl-config --cflags --libs`  -lSDL_mixer
