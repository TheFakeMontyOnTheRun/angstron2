clear
echo =========================================================

g++ -w  -g -DWGA -DFPU  -DDEBUG -ffast-math -opa restorer.cpp `sdl-config --cflags --libs` -lSDL_mixer




