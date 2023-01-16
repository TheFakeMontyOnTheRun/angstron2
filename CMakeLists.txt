cmake_minimum_required(VERSION 3.24)
project(angstron2)

find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)
include_directories(${SDL_INCLUDE_DIRS})
include_directories(${SDL_MIXER_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -g -DNETWORKSUPPORT -DSUPPORT_BITMAP_FONTS -DSUPPORT_SDL_GFX -DWGA -DFPU -DBZK_NO_USE_OPENGL    -fno-exceptions -DDEBUG -ffast-math" )

add_executable(angstron2 BZKmain.cpp AppStart.cpp SDL_gfx/SDL_gfxPrimitives.c)
target_link_libraries(angstron2 ${SDL_LIBRARIES} ${SDL_MIXER_LIBRARIES})