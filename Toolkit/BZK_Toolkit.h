///@brief base include for using the intire package

#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <fstream>

#include "BZK_Constants.h"
#include "BZK_SAL.h"

#ifdef FPU

#include "BZK_FixP_float.h"
#include "BZK_FloatMath.h"

#endif

#ifdef FIXP
#include "BZK_FixP.h"
#include "BZK_FastMath.h"
#endif

#include "BZK_ParsingTools.h"
#include "BZK_Rect.h"
#include "BZK_Vec2f.h"
#include "BZK_Vec3f.h"
#include "BZK_Color2f.h"
#include "BZK_Color3f.h"
#include "BZK_Color4f.h"
#include "BZK_Vector.h"
#include "BZK_Stack.h"
#include "BZK_Queue.h"
#include "BZK_DOMNode.h"
#include "BZK_XMLReader.h"
#include "BZK_XMLWriter.h"
#include "BZK_VecOps.h"
//#include "BZK_AllocManager.h"
