clear
echo =========================================================

g++  -w -g  -DNETWORKSUPPORT -DSUPPORT_BITMAP_FONTS -DSUPPORT_SDL_GFX -DWGA -DFPU -DBZK_NO_USE_OPENGL    -fno-exceptions -DDEBUG -ffast-math -oangstron2  BZKmain.cpp AppStart.cpp SDL_gfx/SDL_gfxPrimitives.c sprig/*.c `sdl-config --cflags --libs` -lSDL_mixer 
#strip angstron2




