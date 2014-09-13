//ja que todos os arquivos includem as mesmas coisas, pq não agrupar?
using namespace std;

#ifndef VAC_BASEINCLUDES_H
#define VAC_BASEINCLUDES_H
#include "VAC_ColorRGBA.h"
#include "VAC_Vec3f.h"
#include "VAC_Vec2f.h"
#include "VAC_Face.h"
#include "VAC_PolyBuffer.h"
#include "VAC_Shape.h"
#include "VAC_Graphic.h"
#include "VAC_VecFonts.h"
#include "VAC_Image.h"
#include "VAC_Sector.h"
#include "VAC_Actor.h"
#include "VAC_Game.h"
#include "VAC_ActorManager.h"
#include "VAC_World.h"
//#include "VAC_WorldFactory.h"

#ifdef BZK_USE_OPENGL
	#include "VAC_RastererGL.h"
#else
	#include "VAC_Rasterer.h"
#endif

#include "VAC_DeviceIO.h"
#include "VAC_Render.h"
#include "VAC_Display.h"
#include "VAC_WMLView.h"
#include "VAC_Engine.h"
#include "VAC_Applet.h"
#endif
