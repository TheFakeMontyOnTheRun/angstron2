g++ -g -w -O3  -mtune=arm926ej-s -DSUPPORT_SPG  -fno-exceptions -DWGA -DFPU -DBZK_FULLSCREEN -DUNIX_DIST -DMAEMO22 -ffast-math  test.cpp  sprig/*.c  -otest770  `sdl-config --cflags --libs`  -lSDL_mixer -lXsp -I/usr/X11R6/include -lX11 -L/usr/X11R6/lib
strip test770
