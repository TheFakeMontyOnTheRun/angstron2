#ifndef GAMEELEMENTS_H
#define GAMEELEMENTS_H

enum TRIGGERS
  {
    
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
    TRIG_PING, //19 ( retorna um trigger 0 com o valor )
    TRIG_HALT,  //20
    TRIG_JOIN, //21
    TRIG_LEAVE,//22
    TRIG_CLASS, //23 muda classe de entidade
    TRIG_CREATE_ENTITY, //24
    TRIG_LOADMAP, //25
    TRIG_CLOSEMAP, //26
    TRIG_NOP, //27
    TRIG_NEUTRAL, //28
    TRIG_LOOK_N,
    TRIG_LOOK_S,
    TRIG_LOOK_W,
    TRIG_LOOK_E,
    TRIG_LOOK_UP,
    TRIG_LOOK_DOWN,
  };

#define TRIG_MAX TRIG_NEUTRAL

#include "BZK_Entity.h"
#include "BZK_Sector.h"
#include "BZK_VolumetricSector.h"
#include "BZK_World.h"
#include "BZK_Actor.h"
#include "BZK_ActorManager.h"
#endif
