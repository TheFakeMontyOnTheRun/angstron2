//setor 3D especializado
#include <stdlib.h>
class VAC_Sector:public BZK_VolumetricSector
{


public:
class VAC_Sector_Door
{
	public:
	int timeout;
	int currenttimeout;
	int link;
	int action;	
	bool openonpresence;
	Mix_Chunk *OpenSound;
	Mix_Chunk *CloseSound;
	VAC_Sector_Door()
	{
	OpenSound=NULL;
	CloseSound=NULL;
	timeout=100;
	currenttimeout=0;
	link=0;
	action=0;
	openonpresence=true;
	}
};


  int iHint;
  int iTrigger;  
  bool iHasInternalColor;  
  string iMeshName;
  bool iForceNoRender[6];
  VAC_Sector_Door *iDoor[6];
  int iNeartLights[6];
  int iRenderMode;
  BZK_FixP iLevelX;
  BZK_FixP iLevelY;
  int index;
  int iParent;
  VAC_ColorRGBA iInternalColor;
  int iCandelas;
  int iExtraCandelas;
  bool iForceRender[6];
  int iVisibleFaces;
  int iVerticalOcludders;
  int iHorizontalOcludders;
  long iOriginalTrigger;  
  long iVertexIndex[6];  
  VAC_ColorRGBA iColor[6];

  BZK_Vector<int> iActors;
  BZK_Vector<int> iLocalTriggers;
  VAC_PolyBuffer *iTextures[6];
  BZK_Vector<int> iGlobalTriggers;
  VAC_PolyBuffer *iMesh;

void SetMeshName(string aName)
{
iMeshName=aName;
}

void SetMesh(VAC_PolyBuffer *aMesh)
{
iMesh=aMesh;
}

VAC_PolyBuffer *GetMesh()
{
return iMesh;
}

int GetOriginalTrigger()
{
return iOriginalTrigger;
}

void SetOriginalTrigger(int aTrigger)
{
iOriginalTrigger=aTrigger;
}

int GetVisibleFaces()
{
return iVisibleFaces;
}

void SetVisibleFaces(int aFaces)
{
	iVisibleFaces=aFaces;	
}

VAC_ColorRGBA *GetColor(int aFace)
{
	return &iColor[aFace];
}
  int GetFaceCount()
	{ 
	return iVisibleFaces;
	}
  BZK_Vector<int> *LocalTriggers()
    {
	return &iLocalTriggers;
	}

  
  void SetConnection(int aSlot,int aCandidate)
  {
    
    BZK_VolumetricSector::SetConnection(aSlot,aCandidate);
    iVerticalOcludders=0;  
    iHorizontalOcludders=0;
    for (int c=0;c<4;c++)
      if (Get_Connection(c)==0 && iColor[c].GetA()!=0) iVerticalOcludders++;

    if (Get_Connection(FLOOR)==0 && iColor[FLOOR].GetA()!=0) iHorizontalOcludders++;
    if (Get_Connection(CEILING)==0 && iColor[CEILING].GetA()!=0) iHorizontalOcludders++;
	
  }
  
  BZK_Vector<int> *GlobalTriggers()
    {return &iGlobalTriggers;}
  
  void
    AddActor(int aActor)
  {   
    iActors.Push(aActor);
  }
  

  ///for conbed
  BZK_Vec3f *Point(int aPoint) //LHC
  {
    switch (aPoint)
      {
      case N:
	return new BZK_Vec3f(iRect.GetX0(),iHeight[0],iRect.GetY0());
	break;
      case E:
	return new BZK_Vec3f(iRect.GetX1(),iHeight[0],iRect.GetY0());
	break;
      case S:
	return new BZK_Vec3f(iRect.GetX1(),iHeight[0],iRect.GetY1());
	break;
      case W:
	return new BZK_Vec3f(iRect.GetX0(),iHeight[0],iRect.GetY1());
	break;
	
      }
  }
  ///for vertex array
  BZK_Vec3f *Vertex(int aPoint) //LHC
  {
    ///TODO: finish
    /*
    switch (aPoint)
      {
      case 0:
	return new BZK_Vec3f(iRect.GetX0(),iHeight[0],iRect.GetY0());
	break;
      case 1:
	return new BZK_Vec3f(iRect.GetX1(),iHeight[0],iRect.GetY0());
	break;
      case 2:
	return new BZK_Vec3f(iRect.GetX1(),iHeight[0],iRect.GetY1());
	break;
      case 3:
	return new BZK_Vec3f(iRect.GetX0(),iHeight[0],iRect.GetY1());
	break;
	
	}*/
  }

  void SetVertexBuffer(int aSlot,int aIndex)
  {
    
    if (aSlot>5 || aSlot <0 ) return; ///ERROR!
    iVertexIndex[aSlot]=aIndex;
  }

  
  long GetVertexBuffer(int aSlot)
  {
    
    if (aSlot>5 || aSlot <0 ) return -1; ///ERROR!
    return iVertexIndex[aSlot];
  }


  bool HasBeenDrawn() {return iBeenDrawn;}
    
  void NotifyEndOfCicle() 
  {
//cout << "notify"<<endl;

//	if (HasBeenDrawn())
	{
	iExtraCandelas = 0;
	memcpy(&iRect,&iOriginalRect,sizeof(BZK_Rect));
	iBeenDrawn--;
	iHint=-1;
	}
  }
  void Visiting(){
 
  
    iBeenDrawn=true;
  }
  
  VAC_Sector(int aX0,int aY0,int aX1,int aY1,int aH0,int aH1)
    {

      VAC_Sector();
      BZK_Rect *rect=new BZK_Rect(aX0,aY0,aX1,aY1);
      memcpy(&iOriginalRect,rect,sizeof(BZK_Rect));
      SetH0(aH0);
      SetH1(aH1);      
      delete rect;


    }
  
  VAC_Sector()
    {      
	for (int c=0;c<6;c++)
		iDoor[c]=NULL;
      

	iExtraCandelas=0;
      iCandelas=0;//64
      iParent=-1;
      iRenderMode=-1;
      iVisibleFaces=0;
      memset(iForceRender,0,sizeof(bool)*6);
      memset(iForceNoRender,0,sizeof(bool)*6);
      memset(iTextures,0,sizeof(VAC_PolyBuffer*)*6);
      iOriginalTrigger=TRIG_NOP;
      memset(&iVertexIndex,0,sizeof(long)*6);
      SetTrigger(-1);
      iHasInternalColor=false;
      iBeenDrawn=0;
      iMesh=NULL;
      // BZK_VolumetricSector();      
    }
  
  ~VAC_Sector()
    {
//	 while (!iActors.Empty())  iActors.Pop();
	iActors.ReleaseAll();
//  	for (int c=0;c<6;c++)		delete iTextures[c];
//	delete iMesh; //not owned here
    }

/*
VAC_Sector *SpanX(BZK_FixP aSpan)
   {
     VAC_Sector *NewSec;
     NewSec=new VAC_Sector(); 
     BZK_Rect Box1,Box2;
     memcpy(NewSec,this,sizeof(VAC_Sector));   
     memcpy(&Box1,&iRect,sizeof(BZK_Rect));  
     Box2.SetX0(aSpan);
     Box1.SetW(aSpan-Box1.GetX0());
     Box2.SetW(iRect.GetX1()-Box2.GetX0());  
     NewSec->SetRect(Box2);    
     SetRect(Box1);
     return NewSec;     
   }

  //---------------------------------------------------------------------------------------
VAC_Sector *SpanY(BZK_FixP aSpan)
   {
     VAC_Sector *NewSec;
     NewSec=new VAC_Sector();
     BZK_Rect Box1,Box2;
     memcpy(NewSec,this,sizeof(VAC_Sector));
     memcpy(&Box1,&iRect,sizeof(BZK_Rect));
     memcpy(&Box2,&iRect,sizeof(BZK_Rect));
     Box2.SetY0(aSpan);
     Box1.SetH(aSpan-Box1.GetY0());
     Box2.SetH(iRect.GetY1()-Box2.GetY0());
     SetRect(Box1);
     NewSec->SetRect(Box2);     
     return NewSec;     
   }
  //---------------------------------------------------------------------------------------
VAC_Sector *SpanZ(BZK_FixP aSpan)
   {
     VAC_Sector *NewSec;
     NewSec=new VAC_Sector();
     memcpy(NewSec,this,sizeof(VAC_Sector));
     SetH0(iHeight[0]);
     SetH1(aSpan);
     NewSec->SetH0(aSpan);
     NewSec->SetH1(iHeight[1]);     
     return NewSec;     
   }*/
};
