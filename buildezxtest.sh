source ~/Scripts/setenv-a1200.sh
arm-linux-gnu-g++  -L/opt/crosstool/arm-linux-gnu/lib -L/opt/a1200/lib -L/opt/a1200/lib/ezx/lib -L/opt/a1200/ezx/lib -L/opt/a1200/lib/ezx/lib/inputmethods -L/opt/crosstool/lib  -L/opt/a1200 -I/opt/crosstool/arm-linux-gnu/include -I/opt/a1200/ezx/include -I/opt/a1200/qt/include -I/opt/crosstool/include    -O3 test.cpp sprig/*.c -g3 -mcpu=iwmmxt -mtune=iwmmxt -w -DEZX -DFIXP -DBZK_FULLSCREEN -DDEBUG -o testezx `/opt/crosstool/bin/sdl-config --cflags --libs`  -lSDL_mixer
arm-linux-gnu-strip testezx
