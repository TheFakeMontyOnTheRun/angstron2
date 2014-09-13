clear
echo =========================================================

g++ -w  -g -DNETWORKSUPPORT -DSUPPORT_BITMAP_FONTS -DSUPPORT_SDL_GFX -DWGA -DFPU -DBZK_NO_USE_OPENGL    -fno-exceptions -DDEBUG -ffast-math -oconbed  simbed.cpp   `sdl-config --cflags --libs`  -lSDL_mixer
#strip hunter




