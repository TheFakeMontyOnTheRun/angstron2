clear
echo =========================================================

g++ -w  -g -cpu=arm1136j-s -mfpu=vfp -mfloat-abi=softfp -O2 -DWGA -DFPU -DBZK_FULLSCREEN -DN770_DEBUG -ffast-math -on8x0_hunter  BZKmain.cpp AppStart.cpp SDL_gfxPrimitives.c  `sdl-config --cflags --libs`  -lSDL_mixer
strip n8x0_hunter




