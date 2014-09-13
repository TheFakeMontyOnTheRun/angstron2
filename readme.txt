a quick start on how to understand this source code (proper documentation coming soon):
The game engine is composed of AppStart.cpp , that handles any extraneous initialization for a particular plataform (in the case of N770, it handled libshadowapp , but it seems this is deprecated), BZKmain.cpp , that contains the proper game initialization , loop and de-initialization (in short, its
life cycle). There is also special modified (very little modifications) versions from SDL_gfx. I want to remove those and keep it in a separate package.

You see lots of directories here. It contain classes used n BZKmain - as you can see, I like to keep only one .h per class (no .cpp) and I do this to save
time. If I have to modify a function prototype, I have to change it only once. 

The most basic "package" (humm, maybe its a good idea to split those in diferent "packages" in the SVN structure later on) is the BZK_Toolkit. It contains
general purpose classes and functions to help coding C++ in areas where it is generally handcapped. Now you probably knows a better package to handle some
of those tasks, but it probably would clutter the code too much and make it overly complex to build, as you would probably need to collect all its 
dependencies. Coding this myself not only helped me to learn some of its functioning, but also helped keeping everything to bare minimum. The only external
resources I use outside C++ and Linux standard libraries are STL (its still probably the best eficiency/generality tradeoff out there), SDL , SDL_gfx and
SDL_mixer. No wonder I can build this on so many plataforms.

Now comes AppArch: it implements a simple MVC framework to keep things tight. I was somewhat inspired on my experiences with Symbian 6 (S60 1.0) and MFC.
It is not that important (in fact, it appeared long after the engine was already working, but was a good thing to keep sanity). 

Then comes GameElements, that gathers some of the most used game elements for game engine. Its creation was inspired with my experiences on Symbian 6 , in
that I created a good 2D framework and it was very easy to extend it to 3D. In fact, I've created a 2D with GameElements before, but I've lost the source
code ( dont bother with it, it was just too slow on the N770, becouse all the graphics were vector).

And finally we have Vacuum, that implements the 3D game engine itself. Calling it Vacuum is somewhat wrong, as it is Vacuum 2.11 (Vacuum 1.0 was desktop 
using OpenGL and SDL, Vacuum 2.0 was for Symbian and was highly...unsucessful). Some parts of both game engines survived on current iteration (specially 
from 2.0), like the the fixed point math class (that had a 5 years old bug fixed just another day), that end up at Toolkit.

In general, the engine is not the best code you will ever see and I wont be surprised if some you dont like it. It doesnt deliver Quake-like graphics nor
speed, but its quite enough for me, as it allows a little more flexibility. The code also still have some bugs and are poorly documented and poorly coded in some points (lots of public class data members!) and this is a matter of time. 

To build:
for x86 workstation: run build.sh
for N770 (also works for N8x0): run n770build.sh
for N8x0 (this script was not tested yet): run n8x0build.sh
Have fun!
