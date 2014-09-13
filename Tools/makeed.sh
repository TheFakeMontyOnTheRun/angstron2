clear
echo =========================================================

g++ -w  -g -DDEBUG -DUBUNTU -DFPU -DQVGA   -ffast-math -oconbed  simbed.cpp   `sdl-config --cflags --libs`  -lSDL_mixer
#strip hunter




