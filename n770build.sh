clear
echo =========================================================

g++ -w -O3 -mtune=arm926ej-s -DSUPPORT_VECTOR_FONTS -DSUPPORT_SDL_GFX -DNETWORKSUPPORT -fno-exceptions -DDBL_WGA -DFPU -DBZK_FULLSCREEN -DUNIX_DIST -DMAEMO22 -ffast-math -on770_hunter  BZKmain.cpp AppStart.cpp SDL_gfx/SDL_gfxPrimitives.c sprig/*.c  `sdl-config --cflags --libs`  -lSDL_mixer -lXsp -I/usr/X11R6/include -lX11 -L/usr/X11R6/lib 
strip n770_hunter




