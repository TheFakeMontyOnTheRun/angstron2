
//setor 3D

class BZK_VolumetricSector:public BZK_Sector
{
 public:
  
  BZK_FixP iHeight[2];  
  int iConnections[6];
  BZK_Rect iOriginalRect; 
  int iBeenDrawn;
  BZK_Color4f iColor[6];
  BZK_Rect iRect;
  
  
 

  virtual void NotifyEndOfCicle() {}
  virtual bool HasBeenDrawn() {}
  
  BZK_VolumetricSector *Clone()
  {
    BZK_VolumetricSector *tmp=new BZK_VolumetricSector();
    memcpy (tmp,this,sizeof(BZK_VolumetricSector));
  }
  void SetConnection(int aSlot,int aCandidate)
    {
	////cout << "ajustando conexao "<< aSlot<< " como "<<aCandidate<<endl;
      iConnections[aSlot]=aCandidate;
    }

  void KillConnection(int aVictim)
    {
      iConnections[aVictim]=0;
    }

  BZK_Rect *Rect() {return &iRect;}
  
  BZK_Vec3f center() //BSC
    {    
	BZK_FixP HALF=BZK_FastMath::Real32toFix32(2);
        BZK_Vec3f ToReturn(
			iRect.GetX0()+BZK_FastMath::Div(iRect.GetW(),HALF),
			iRect.GetY0()+BZK_FastMath::Div(iRect.GetH(),HALF),
			GetH0()+BZK_FastMath::Div(GetH1()-GetH0(),HALF)
			);     
	ToReturn.SetSpace(BSC);
	return ToReturn;
    }

  int Get_Connection(int aIndex)
    {
	if (aIndex>5)
		{
			//cout <<"indice invalido"<<endl;
			exit(-1);
		}
      return iConnections[aIndex];
    }


  BZK_VolumetricSector()
    {
      SetConnection(0,0);
      SetConnection(1,0);
      SetConnection(2,0);
      SetConnection(3,0);
      SetConnection(4,0);
      SetConnection(5,0);		
      memset(&iRect,0,sizeof(BZK_Rect));
/*
	iColor[0].SetColor(0,0,0,0);
	iColor[1].SetColor(0,0,0,0);
	iColor[2].SetColor(0,0,0,0);
	iColor[3].SetColor(0,0,0,0);
	iColor[4].SetColor(0,0,0,0);
	iColor[5].SetColor(0,0,0,0);
*/
	SetH0(0);
	SetH1(0);
    }

  ~BZK_VolumetricSector()
    {

    }


  void SetRect(BZK_Rect &aRect) 
    {
  memcpy(&iRect,&aRect,sizeof(BZK_Rect));
  memcpy(&iOriginalRect,&iRect,sizeof(BZK_Rect));
    }
/*
  void SetHeights(Sint32 aHeight1,Sint32 aHeight2)
    {
      iHeight1=aHeight1;
      iHeight2=aHeight2;
    }*/
//  Sint32 GetHeight1() {return iHeight1;}
//  Sint32 GetHeight2(){return iHeight2;}


 // int _GetHeight1() {return iHeight1;}
 // int _GetHeight2(){return iHeight2;}

 //---------------------------------------------------------------------------------------

 bool Inside (BZK_VolumetricSector &aSector)
    {       
      
      BZK_Rect *rect2=&aSector.iRect;    
//	std::cout << rect2->toString()<<"-"<<BZK_FastMath::Tostring(GetH0()) <<","<<BZK_FastMath::Tostring(GetH1()) << std::endl;
//	std::cout << "esta dentro de "<<iRect.toString()<<"-"<<BZK_FastMath::Tostring(GetH0()) <<","<<BZK_FastMath::Tostring(GetH1()) << std::endl;


     if (aSector.GetH0() > GetH1())  return false;
     if (aSector.GetH1() < GetH0()) return false;

        
     if (rect2->GetY0() > iRect.GetY1())  return false;
     if (rect2->GetY1() < iRect.GetY0()) return false;

     if (rect2->GetX0() > iRect.GetX1())  return false;
     if (rect2->GetX1() < iRect.GetX0()) return false;
   

       return true;

    } 

 


 //---------------------------------------------------------------------------------------
 std::string 
   ToString()
   {
        std::string  toreturn="\n=========\nSetor:\n";
	toreturn+= BZK_FastMath::Print(iRect.GetX0())+"\n";
	toreturn+= BZK_FastMath::Print(iRect.GetY0())+"\n";
	toreturn+= BZK_FastMath::Print(iRect.GetW())+"\n";
  	toreturn+= BZK_FastMath::Print(iRect.GetH())+"\n";
	toreturn+= BZK_FastMath::Print(GetH0())+"\n";
	toreturn+= BZK_FastMath::Print(GetH1());
     return toreturn;
   }

 //---------------------------------------------------------------------------------------

 int WhereLHC (BZK_Vec3f aVec)
    {
	//LHC
	BZK_FixP x=aVec.GetX();
	BZK_FixP y=aVec.GetY();
	BZK_FixP z=aVec.GetZ();
	BZK_FixP h1=GetH0();
	BZK_FixP h2=GetH1();

	if (h1 > y) return FLOOR;
	if (h2 < y) return CEILING;
	if (iRect.GetY0() < z) return N;
	if (iRect.GetX1() > x) return W;
	if (iRect.GetX0() < x) return E;
	if (iRect.GetY1() > z) return S;

	return 100;
    } 
 //---------------------------------------------------------------------------------------

 int WhereBSC (BZK_Vec3f aVec)
    {
	//BSC

	BZK_FixP y=aVec.GetY();
	if (iRect.GetY1() < y) return S;
	if (iRect.GetY0() > y) return N;

	BZK_FixP x=aVec.GetX();
	if (iRect.GetX1() < x) return W; // porque apesar de o eixo ser invertido para Y, N eh na verdade S
	if (iRect.GetX0() > x) return E;


	BZK_FixP z=aVec.GetZ();
	BZK_FixP h1=GetH0();
	if (h1 > z) return FLOOR;

	BZK_FixP h2=GetH1();
	if (h2 < z) return CEILING;
	return 100;
    } 
  //---------------------------------------------------------------------------------------
/*
 BZK_VolumetricSector **SpanX(BZK_FixP aSpan)
   {
     BZK_VolumetricSector *NewSec[2];
     NewSec[0]=new BZK_VolumetricSector;
     NewSec[1]=new BZK_VolumetricSector;

     BZK_Rect Box1,Box2;
     memcpy(NewSec[0],this,sizeof(BZK_VolumetricSector));
     memcpy(NewSec[1],this,sizeof(BZK_VolumetricSector));
     memcpy(&Box1,&iRect,sizeof(BZK_Rect));
     memcpy(&Box2,&iRect,sizeof(BZK_Rect));
     Box2.x=aSpan;
     Box1.w=aSpan-Box1.x;
     Box2.w=(iRect.x+iRect.w)-Box2.x;
     NewSec[0]->SetRect(Box1);
     NewSec[1]->SetRect(Box2);    
     
     return NewSec;

     
   }

  //---------------------------------------------------------------------------------------
 BZK_VolumetricSector **SpanY(BZK_FixP aSpan)
   {
     BZK_VolumetricSector *NewSec[2];
     NewSec[0]=new BZK_VolumetricSector;
     NewSec[1]=new BZK_VolumetricSector;

     BZK_Rect Box1,Box2;
     memcpy(NewSec[0],this,sizeof(BZK_VolumetricSector));
     memcpy(NewSec[1],this,sizeof(BZK_VolumetricSector));
     memcpy(&Box1,&iRect,sizeof(BZK_Rect));
     memcpy(&Box2,&iRect,sizeof(BZK_Rect));
     Box2.y=aSpan;
     Box1.h=aSpan-Box1.y;
     Box2.h=(iRect.y+iRect.h)-Box2.y;
     NewSec[0]->SetRect(Box1);
     NewSec[1]->SetRect(Box2);    
     
     return NewSec;     
   }
  //---------------------------------------------------------------------------------------
 BZK_VolumetricSector **SpanZ(BZK_FixP aSpan)
   {
     BZK_VolumetricSector *NewSec[2];
     NewSec[0]=new BZK_VolumetricSector;
     NewSec[1]=new BZK_VolumetricSector;

     memcpy(NewSec[0],this,sizeof(BZK_VolumetricSector));
     memcpy(NewSec[1],this,sizeof(BZK_VolumetricSector));

     NewSec[0]->SetHeights(iHeight1,aSpan);
     NewSec[1]->SetHeights(aSpan,iHeight2);    
     
     return NewSec;     
   }
  //---------------------------------------------------------------------------------------
 int BehindZ(BZK_VolumetricSector &aSector)
   {
     
     BZK_Rect *rect1=&iRect;
     BZK_Rect *rect2=&aSector.iRect;


     
     //se this estiver na frente (for menor), retorna 1
     //cout <<"quem é maior em Z?"<<(rect1->y) <<" ou "<<rect2->y<<endl;
     //cout <<"quem é maior?"<<(rect1->y+rect1->h) <<" ou "<<rect2->y+rect2->h<<endl;

    if ((rect1->y)<rect2->y) 
      {
      if ((rect1->y+rect1->h)<rect2->y)  return 1;

      return 0; //definir caso de subdivisão.
      }


     if ((rect2->y)<rect1->y) 
       {
	 if ((rect2->y+rect2->h)<rect1->y) return 2; 	 
	 return 0; //caso de subdivisao
       }

   if ((rect1->y)==rect2->y) 
      {
	//dividir e comparar novamente.
        if ((rect2->y+rect2->h)>(rect1->y+rect1->h))  return 1; 
	if ((rect2->y+rect2->h)<(rect1->y+rect1->h))  return 2;  

      return 0; //definir caso de subdivisão.
      }


  

       //  return BehindX(aSector); //por enquanto ainda não sabe direito oque fazer...
       return 0;
   }
 //-----------------------------------------------------
 int BehindX(BZK_VolumetricSector &aSector)
   {
     BZK_Rect *rect1=&iRect;
     BZK_Rect *rect2=&aSector.iRect;
     
     //se this estiver na frente (for menor), retorna 1
     //cout <<"quem é maior em X?"<<(rect1->x) <<" ou "<<rect2->x<<endl;
     if ((rect1->x+rect1->w)<rect2->x) return 1; //ok

     if ((rect2->x+rect2->w)<rect1->x) return 2; 
     //cout <<"quem é maior?"<<(rect1->x+rect1->w) <<" ou "<<rect2->x+rect2->w<<endl;
 
    if (rect2->x==rect1->x)     
       {
        if ((rect2->x+rect2->w)>(rect1->x+rect1->w)) return 1; 
	if ((rect2->x+rect2->w)<(rect1->x+rect1->w)) return 2; 
       }

 
     return 0; //por enquanto ainda não sabe direito oque fazer...
   }
*/
 //-----------------------------------------------------
 /*
 int BehindY(BZK_VolumetricSector &aSector)
   {
     BZK_Rect *rect1=&iRect;
     BZK_Rect *rect2=&aSector.iRect;
     
     //se this estiver na frente (for menor), retorna 1

     if ((rect1->x+rect1->w)<rect2->x) return 1; //ok

     if ((rect2->x+rect2->w)<rect1->x) return 2; 

     if (rect2->x==rect1->x)     
        if ((rect2->x+rect2->w)>(rect1->x+rect1->w))  
	  {
	     return 1; 
	  }
	else
	  {
	     return 2;
	  }
 
     return 0; //por enquanto ainda não sabe direito oque fazer...
   }
 */
 public:
  BZK_FixP GetH0() {return iHeight[0];}
  BZK_FixP GetH1() {return iHeight[1];}
  void SetH0(BZK_FixP aH0) {iHeight[0]=aH0;}
  void SetH1(BZK_FixP aH1) {iHeight[1]=aH1;}
  void SetX0(BZK_FixP aX0) {iRect.SetX0(aX0);}
  void SetY0(BZK_FixP aY0) {iRect.SetY0(aY0);}
  void SetX1(BZK_FixP aX1) {iRect.SetX1(aX1);}
  void SetY1(BZK_FixP aY1) {iRect.SetY1(aY1);}

  BZK_FixP GetX0() {return iRect.GetX0();}
  BZK_FixP GetX1() {return iRect.GetX1();}
  BZK_FixP GetY1() {return iRect.GetY1();}
  BZK_FixP GetY0() {return iRect.GetY0();}
};
