

#ifdef BZK_USE_OPENGL
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifndef POCKETPC
#include <SDL.h>
#include <SDL_mixer.h>
#include "SDL_gfx/SDL_gfxPrimitives.h"
#endif

#ifdef MAEMO22
#include <X11/Xlib.h>
#include <X11/extensions/Xsp.h>
#include <SDL/SDL_syswm.h>
#endif

#include <stdlib.h>

#ifdef BZK_SUPPORT_SPG
#include "sprig/sprig.h"
#endif

#define  BZK_QUIT SDL_QUIT
#define  BZK_KeyDownEvent SDL_KEYDOWN
#define  BZK_KeyUpEvent SDL_KEYUP

//key constants
#define BZK_KeyMinorJ SDLK_i
#define BZK_KeyMinorI SDLK_j
#define BZK_KeyPAUSE SDLK_PAUSE
#define BZK_KeyDOWN SDLK_DOWN
#define BZK_KeyUP SDLK_UP
#define BZK_KeyLEFT SDLK_LEFT
#define BZK_KeyRIGHT SDLK_RIGHT
#define BZK_KeyENTER SDLK_RETURN
#define BZK_KeyMinorA SDLK_a
#define BZK_KeyMinorZ SDLK_z
#define BZK_KeyMinorQ SDLK_q
#define BZK_KeyMinorW SDLK_w
#define BZK_KeyCOMMA SDLK_COMMA
#define BZK_KeyPERIOD SDLK_PERIOD
#define BZK_KeyTAB SDLK_TAB
#define BZK_KeySPACE SDLK_SPACE
#define BZK_KeyF4 SDLK_F4
#define BZK_KeyF7 SDLK_F7
#define BZK_KeyF8 SDLK_F8
#define BZK_KeyESCAPE SDLK_ESCAPE




typedef SDL_Event BZK_SystemEvent;
typedef SDL_PixelFormat BZK_PixelFormat;
typedef SDL_Surface BZK_Bitmap;
typedef SDL_Rect BZK_LightRect;
typedef SDL_Color BZK_RawColor;
typedef SDL_KeyboardEvent BZK_KBEvent;


long BZK_GetSysTicks()
{
return SDL_GetTicks();
}

BZK_Bitmap *BZK_LoadBitmap(const char *aFilename)
{
return SDL_LoadBMP(aFilename);
}

void BZK_UnloadBitmap(BZK_Bitmap *aBitmap)
{
	SDL_FreeSurface(aBitmap);
}

void BZK_Wait(long aTime)
{
#ifndef __EMSCRIPTEN__
	SDL_Delay(aTime);
#endif
}

BZK_Bitmap *BZK_GetVideoBuffer()
{
	return SDL_GetVideoSurface();
}

BZK_Uint16 BZK_MapColor( BZK_PixelFormat *aFmt,char aR,char aG,char aB)
{
	return SDL_MapRGB(aFmt,aR,aG,aB);
}

bool BZK_SysInit()
{
	std::cout << "system layer ok" << std::endl;
	bool result=SDL_Init(SDL_INIT_EVERYTHING| SDL_INIT_JOYSTICK )==0;
        if (result) SDL_EnableUNICODE(1);
	return result;
}

void BZK_Quit()
{
	std::cout << "~BZK" << std::endl;
	#ifdef MAEMO22

	  SDL_SysWMinfo wminfo;
	  SDL_VERSION(&wminfo.version);
	  SDL_GetWMInfo(&wminfo);
	  XSPSetPixelDoubling(wminfo.info.x11.display, 0, false);


	#endif
	SDL_Quit();
}

void BZK_BitBlockTransfer(BZK_Bitmap *aDst,BZK_LightRect*aArea,BZK_Bitmap *aSource,BZK_LightRect *aRegion)
{
	SDL_BlitSurface(aDst,aArea,aSource,aRegion);
}

void BZK_UpdateRegion(BZK_Bitmap *aLayer, BZK_Sint32 iX, BZK_Sint32 iY, BZK_Sint32 iW, BZK_Sint32 iH)
{
	SDL_UpdateRect(aLayer,iX,iY,iW,iH);
}


void BZK_PaintRegion(BZK_Bitmap *aDst, BZK_LightRect *aRegion, BZK_Uint32 aColor)
{
	SDL_FillRect(aDst,aRegion,aColor);
}

void BZK_CheckEvents(BZK_SystemEvent *aEvent)
{
	SDL_PollEvent(aEvent);
}

void BZK_SwapBuffers(BZK_Bitmap *aBuffer)
{
	#ifdef BZK_USE_OPENGL
    SDL_GL_SwapBuffers( );

    /* Clear The Screen And The Depth Buffer */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    /* Move Left 1.5 Units And Into The Screen 6.0 */
    glLoadIdentity();
  //  glTranslatef( 0.0f, 0.0f, -6.0f );
	#else
	#ifdef MAEMO22
	SDL_UpdateRect (aBuffer, 0, 0, XRES, YRES);
	#endif

	#ifndef MAEMO22
	SDL_Flip(aBuffer);
	#endif

	#endif
}

BZK_Bitmap *BZK_InitVideo()
{
   	SDL_ShowCursor(false); 
	int flags=SDL_HWSURFACE|SDL_DOUBLEBUF;
	#ifdef BZK_USE_OPENGL
	flags=flags|SDL_OPENGL;	
	#endif
	#ifdef BZK_FULLSCREEN
	flags=flags|SDL_FULLSCREEN;	
	#endif
	
	
	#ifdef MAEMO22
	BZK_Bitmap *ToReturn= SDL_SetVideoMode(2*XRES,2*YRES,0,flags);
	SDL_ShowCursor(false); 

	  SDL_SysWMinfo wminfo;
	  SDL_VERSION(&wminfo.version);
	  SDL_GetWMInfo(&wminfo);
	  XSPSetPixelDoubling(wminfo.info.x11.display, 0, true);
	#endif
	
	#ifndef MAEMO22
	BZK_Bitmap *ToReturn= SDL_SetVideoMode(XRES,YRES,0,flags);
	SDL_ShowCursor(false); 
	#endif
	return ToReturn;
}

void BZK_SetTransparentColor(BZK_Bitmap *aBuffer,BZK_Uint32 aColorKey)
{
 SDL_SetColorKey(aBuffer, SDL_RLEACCEL | SDL_SRCCOLORKEY, aColorKey);
}

