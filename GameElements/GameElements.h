#ifndef GAMEELEMENTS_H
#define GAMEELEMENTS_H

enum TRIGGERS {

	TRIG_MOVE_N, //0
	TRIG_MOVE_E, //1
	TRIG_MOVE_S, //2
	TRIG_MOVE_W, //3
	TRIG_MOVE_DOWN, //4
	TRIG_MOVE_UP, //5
	TRIG_TURN_L, //6
	TRIG_TURN_R, //7
	//gerencia de atores:
	TRIG_ATACK, //8
	TRIG_DEFENSE, //9
	TRIG_TELEPORT, //10
	TRIG_DAMAGE, //11
	TRIG_VANISH, //12
	TRIG_CONTROL, //????
	TRIG_COLLIDE, //14
	TRIG_CONTACT, //15
	//rede:
	TRIG_CONNECT, //16
	TRIG_ACK, //17
	TRIG_NAK, //18
	TRIG_PING, //19
	TRIG_HALT,  //20
	TRIG_JOIN, //21
	TRIG_LEAVE,//22
	TRIG_CLASS, //23
	TRIG_CREATE_ENTITY, //24
	TRIG_LOADMAP, //25
	TRIG_CLOSEMAP, //26
	TRIG_NOP, //27
	TRIG_NEUTRAL, //28
	TRIG_LOOK_N,//29
	TRIG_LOOK_S,//30
	TRIG_LOOK_W,//31
	TRIG_LOOK_E,//32
	TRIG_LOOK_FLOOR,//33
	TRIG_LOOK_CEILING,//34
	TRIG_ACTION1,//35
	TRIG_ACTION2,//36
	TRIG_ACTION3,//37
	TRIG_ACTION4,//38
	TRIG_OPEN_DOOR,//39
	TRIG_CLOSE_DOOR,//40
	TRIG_RANDOM_SMALL_DELAY,//41
};

#define TRIG_MAX TRIG_ACTION4
#define BZK_STATIC_ACTOR  0xFFFF

#include "BZK_Bitmap.h"
#include "BZK_DeviceOutput.h"
#include "BZK_DeviceInput.h"
#include "BZK_TypeSet.h"
#include "BZK_Entity.h"
#include "BZK_Sector.h"
#include "BZK_VolumetricSector.h"
#include "BZK_World.h"
#include "BZK_Actor.h"
#include "BZK_ActorManager.h"
#include "BZK_DevKB.h"
#include "BZK_DevVideo.h"

#ifdef NOSOUND
#include "BZK_DevSound_stub.h"
#else

#include "BZK_DevSound.h"

#endif

#include "BZK_DeviceIO.h"
#include "BZK_Applet.h"

#endif
