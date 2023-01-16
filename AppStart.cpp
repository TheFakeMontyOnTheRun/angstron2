//just a stub,for cases where other plataforms need special treatments when starting.

#include "Toolkit/BZK_Constants.h"
#include "BZKmain.h"

//uncomment this for a Windows CE build
#ifdef POCKETPC
#include "sdl/SDL.h"
#include "windows.h"
#endif
//comment this to build for Windows CE
#ifndef POCKETPC
#include <SDL.h>
#endif
int main(int argc, char *argv[])
{
 

 
//pula o nome do executavel, incluso em algumas plataformas.	
	   argv++;
	   argc--;
           BZKmain(argc,argv);



  return 0;
}
