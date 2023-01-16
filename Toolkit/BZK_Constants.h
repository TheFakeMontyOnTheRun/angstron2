//******general purpose constant values*****************************
//#define  ZERO BZK_FastMath::Int32toFix32(0);
//#define  ONE BZK_FastMath::Int32toFix32(1);
//*********vector subspaces *********************************************
/**
undefined - please, programmer , IMPROVE YOUR CODE!
BSC - BZK system coordinates ( sectors and actors always use this )
LHC - Left Hand Coordinates ( lookup on foley van dam for help on this )
CAMERA - render's own internal coordenate system.Rendering purposes only.
*/
enum {
	UNDEFINED, BSC, LHC, CAMERA
};
//*********error constants***************************************************
/// deprecated - this will be reviewed soon
///@deprecated
#define ERR_CAST_NON_NUMERIC 0 //cast de tipos incompativeis
///@deprecated
#define ERR_INVALID_FILENAME 1 //arquivo em formato incorreto
///@deprecated
#define ERR_OUT_OF_MEMORY 2 //sem mem�ria
///@deprecated
#define ERR_END_OF_FILE 3 //fim de arquivo. n�o � bem um erro
///@deprecated
#define ERR_UNEXPECTED_END_OF_FILE 4 //arquivo acabou antes que algo essencial fosse definido
///@deprecated
#define ERR_NONE 5 //sem erros
///@deprecated
#define ERR_PANIC 6 //panico de sistema
///@deprecated
#define ERR_SAL_ERROR 7 //erro da SAL

// game signals
#define BZK_GS_PLAY -1 ///remain on current view
#define BZK_GS_RESTART -3 ///restart current view
#define BZK_GS_BACK -4 ///back to previous view
#define BZK_GS_NOVIEW -5 ///???
#define BZK_GS_RESUME -6 ///resume current game 
#define BZK_GS_JOINNETWORK -7 ///look for a open network game 
#define BZK_GS_STARTNETWORK -8 ///create a network game and wait for client
#define BZK_GS_QUIT -2 ///quit to OS
#define MAX_VIEW_TRIGGER -10 /// our limit, for now
#define MAX_VERTEX_PER_POLY 128 /// our vertex limit. It also dictates the size of the polygon buffer
#define MAX_VERTEX_INDEX MAX_VERTEX_PER_POLY -1 /// biggest vertex index


///sound channels
#define SND_MOOD_CHANNEL 0  /// mood setting channel (can be used for some auxiliary music)
#define SND_AMBIENT_CHANNEL 1 /// sound backdrop channel
#define SND_AMBIENT_SFX_CHANNEL 2 /// effects in the current area (other actors)
#define SND_AMBIENT_ACTORS_CHANNEL 3 /// sounds from the player


///plataform constants
//#define EZX
//#define MAEMO2
//#define POCKETPC
//#define EEEPC
//#define UBUNTU
//#define DEBUG
//#define QVGA
//#define FPU
//#define BZK_FULLSCREEN
//#define UNIX_DIST

//#define NOSOUND   
/// biggest frames per second desired
#define BZK_DESIRED_FPS 10

//********* system default resolution ********************************************************
/// rules for plataform constants. Not that used anymore.

#ifndef DEBUG
//#define BZK_FULLSCREEN
#endif

#ifdef MAEMO22
#define BZK_FULLSCREEN
#define WGA
#endif

#ifdef EEEPC
//#define WGA
//#define BZK_FULLSCREEN
#define FPU
#endif

#ifdef LINUXBOX
#define WGA
#define FPU
#endif


#ifdef EZX
#define QVGA
//#define FPU
#endif

#ifdef POCKETPC
//#define QVGA
//#define FPU
//#define FIXP
#endif
////////////////
/// alignment of a given polyface
enum {
	NOPE, XY_ALLIGNED, XZ_ALLIGNED, YZ_ALLIGNED
};
/// directions for game units and portals
enum {
	N, E, S, W, FLOOR, CEILING
};
/// available base actions for the system
enum {
	ACT_MOVE_N,
	ACT_MOVE_E,
	ACT_MOVE_S,
	ACT_MOVE_W,
	ACT_MOVE_DOWN,
	ACT_MOVE_UP,
	ACT_TURN_L,
	ACT_TURN_R,
	ACT_ACTION1,
	ACT_ACTION2,
	ACT_ACTION3,
	ACT_ACTION4
};
//     0          1          2           3         4             5            6          7           8          9


enum {
	PLAYER_SOUND, PEER_SOUND, FAR_SOUND, EFFECTS_SOUND
};

///hints for implementers
//base trigger for move actions
#define MOVE_BASE ACT_MOVE_N
//base trigger for action triggers
#define ACTION_BASE ACT_ACTION1

#ifdef SBOX_DEBUG
///placement of BZK filesystem
#define STOREPATH "/home/daniel/angstron/storage/" //inside scratchbox
#endif

#ifdef N770_DEBUG
#define STOREPATH "/media/mmc1/storage/" //maemo device
#endif

#ifdef DEBUG
#define STOREPATH "./storage/" //debugging on a linux station
#endif


#ifdef POCKETPC
#define STOREPATH "\\angstron2\\storage\\" //debugging on a Windows Mobile/Windows CE handheld
#endif

#ifdef UNIX_DIST
#define STOREPATH "/usr/share/angstron2/" //debian packages
#endif


///widescreen VGA
#ifdef WGA
#define XRES 800
#define YRES 480
#endif

#ifdef DBL_WGA
#define XRES 400
#define YRES 240
#endif


///quarter VGA
#ifdef QVGA
#define XRES 240
#define YRES 320
#endif
/// VGA (planning to use Mer on my dell axim x50v)
#ifdef VGA
#define XRES 480
#define YRES 640
#endif


///never used actually
#ifdef S60ED1
#define XRES 176
#define YRES 208
#endif

#define MXRES BZK_FastMath::Int32toFix32(XRES)
#define MYRES BZK_FastMath::Int32toFix32(YRES)
