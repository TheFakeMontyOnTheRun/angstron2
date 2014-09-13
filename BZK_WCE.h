#include "sdl/SDL.h"
#include "windows.h"
#include "SDL_gfx/SDL_gfxPrimitives_wce.h"
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
	SDL_Delay(aTime);
}

BZK_Bitmap *BZK_GetVideoBuffer()
{
	return SDL_GetVideoSurface();
}

BZK_Uint16 BZK_MapColor(BZK_PixelFormat *aFmt,char aR,char aG,char aB)
{
	return SDL_MapRGB(aFmt,aR,aG,aB);
}

bool BZK_SysInit()
{
	bool result=
	SDL_Init(SDL_INIT_EVERYTHING);
        if (result) SDL_EnableUNICODE(1);
	return result;
}

void BZK_Quit()
{
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
	SDL_Flip(aBuffer);
}

BZK_Bitmap *BZK_InitVideo()
{
	return SDL_SetVideoMode(XRES,YRES,0,SDL_HWSURFACE|SDL_DOUBLEBUF/*|SDL_FULLSCREEN*/);
}

void BZK_SetTransparentColor(BZK_Bitmap *aBuffer,BZK_Uint32 aColorKey)
{
 SDL_SetColorKey(aBuffer, SDL_RLEACCEL | SDL_SRCCOLORKEY, aColorKey);
}

