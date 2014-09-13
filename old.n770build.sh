clear
echo =========================================================

g++ -w -O3 -mtune=arm926ej-s -DNETWORKSUPPORT -fno-exceptions -DWGA -DFIXP -DBZK_FULLSCREEN -DUNIX_DIST -DMAEMO22 -ffast-math -on770_hunter  BZKmain.cpp AppStart.cpp SDL_gfxPrimitives.c  `sdl-config --cflags --libs`  -lSDL_mixer -lXsp -I/usr/X11R6/include -lX11 -L/usr/X11R6/lib
strip n770_hunter




