clear
echo =========================================================

g++  -w -g  -DNETWORKSUPPORT -DWGA -DFPU -DBZK_NO_USE_OPENGL  -fno-exceptions -DDEBUG -ffast-math -otrig split.cpp SDL_gfxPrimitives.c `sdl-config --cflags --libs`  -lSDL_mixer
#strip trig




