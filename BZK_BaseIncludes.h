#include "Toolkit/BZK_Toolkit.h"
#include "AppArch/BZK_AppArch.h"

#ifdef POCKETPC
#include "BZK_WCE.h" // SDL on Windows CE using CeGCC (kudos for the great job, fellows!)
#endif

#ifndef POCKETPC
#include "BZK_SDL.h" //SDL on a full POSIX
#endif

#include "GameElements/GameElements.h"

