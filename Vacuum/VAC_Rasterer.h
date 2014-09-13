//parte de desenhos de primitivas. aqui nada � 3D, vale lembrar.

class VAC_Rasterer:public BZK_View
{
 private:

#ifdef SUPPORT_SDL_GFX
    Sint16 *vx;
    Sint16 *vy;
#endif

#ifdef SUPPORT_SPG  
 SPG_Point *points;
 Uint32* colors;
#endif
  typedef BZK_Vec2f* BZK_Vec2f_ptr;
  BZK_Rect iCanvasSize;
  BZK_Queue<VAC_Shape*> iBuffer; 
int currentalphaincrement;
static VAC_Rasterer *iInstance;
  bool rasterBusy;

  BZK_Stack<VAC_Shape*> iShapeRecycler;
  //  BZK_Bitmap *texture;
 public:

int currentalpha;
static VAC_Rasterer *GetInstance()
{
if (iInstance!=NULL)
{
//	iInstance->currentalpha=0;
	return iInstance;
}
iInstance=new VAC_Rasterer();
//iInstance->currentalpha=0;
return iInstance;
}

  ///////////////////////////////////////////////////////////////////////
  virtual  void Render(BZK_Bitmap *aDst)
  {
    Update(aDst); 
    //texture=BZK_LoadBitmap("./storage/paused.bmp");
  }
 
  virtual void Render()
  {
    // texture=BZK_LoadBitmap("./storage/paused.bmp");
  }

  void ReservePolys(long aQtd)
  {
    //iBuffer.Reserve(aQtd);
    //  iShapeRecycler.Reserve(4*aQtd);
  }

  virtual BZK_View* ConsumeKey(long aKey)
  {
    return this;
  }

  int GetFadeProgress()
	{
	return currentalpha;
	}

  VAC_Rasterer()
    {
	rasterBusy=false;
#ifdef SUPPORT_SDL_GFX
    vx=(Sint16*)malloc(MAX_VERTEX_PER_POLY*sizeof(Sint16)); //yes, I know its 16 bits, but I like it to be more portable
    vy=(Sint16*)malloc(MAX_VERTEX_PER_POLY*sizeof(Sint16));    

#endif

#ifdef SUPPORT_SPG
    points=(SPG_Point*)malloc(MAX_VERTEX_PER_POLY*sizeof(SPG_Point));  
    colors=(Uint32*)malloc(MAX_VERTEX_PER_POLY*sizeof(Uint32));  
#endif
//      currentalpha=255;
      iCanvasSize.SetX0(0);
      iCanvasSize.SetY0(0);
      iCanvasSize.SetX1(0);
      iCanvasSize.SetY1(0);
      //cout << "rasterer:contrutor padr�o"<<endl;
    }
  
  VAC_Rasterer (BZK_Rect &aCanvasSize,int aBPP)
    {
      VAC_Rasterer();
      //cout <<"rasterer:construtor especifico"<<endl;
      iCanvasSize.copy(aCanvasSize);
    }
  
  
  VAC_Shape *RequestShape()
  {
    if (iShapeRecycler.Empty())
      	return new VAC_Shape();
    else
	return iShapeRecycler.Pop();
  }

void PreAllocateResources(int size)
	{
	BZK_Stack<VAC_Shape*> tmp;	

	for (int c=0;c<size;c++)
		tmp.Push(RequestShape());

	while (!tmp.Empty()) DisposeShape(tmp.Pop());
	}
  
  ~VAC_Rasterer()
    {
      while (!iBuffer.Empty()) delete iBuffer.Pop();	
      while (!iShapeRecycler.Empty()) delete iShapeRecycler.Pop();       
#ifdef SUPPORT_SDL_GFX
    free(vx);
    free(vy);
#endif 

#ifdef SUPPORT_SPG
    free(points);
    free(colors);
#endif

    }
 
   
 void DisposeShape(VAC_Shape* aShape)
 {
   /*
   int c=0;
   int qtd=aShape->iPoint->TotalItems();
   BZK_Vec2f_ptr *ptr=(BZK_Vec2f_ptr*)malloc(sizeof(BZK_Vec2f_ptr)*qtd);
   for (c=0;c<qtd;c++)
     ptr[c]=aShape->Point(c);
   aShape->ReleaseAll();
   for (c=0;c<qtd;c++)		
     delete ptr[c];
   free(ptr);
   iShapeRecycler.Push(aShape);   
   */

   //int c=0;
   //int qtd=aShape->iPoint->TotalItems();
   // BZK_Vec2f_ptr *ptr=(BZK_Vec2f_ptr*)malloc(sizeof(BZK_Vec2f_ptr)*qtd);
   //for (c=0;c<qtd;c++)
     //ptr[c]=
     // delete aShape->Point(c);
   // aShape->ReleaseAll();
   //   for (c=0;c<qtd;c++)		     delete ptr[c];
   //free(ptr);


    aShape->DeleteAllPoints();
    iShapeRecycler.Push(aShape);   
   //delete aShape;


   /*
     for (int c=0;c<qtd-1;c++)
     ptr[c]=aFace->GetPoint(c);
     
     ptr[qtd-1]=aFace->GetNormal();
     
     
     aFace->ReleaseAll();
     for (int c=0;c<qtd;c++)
     delete (ptr[c]);
     free(ptr);
     iFaceRecycler.Push(aFace);
     //  delete aFace;
     */
 }
  
 
  
  inline void DrawShape(BZK_Bitmap*aDestination,VAC_Shape *aShape)
  {
//	cout << "size before:" << aShape->GetNumberOfPoints() << endl;

   VAC_Shape *tmp;

   tmp=aShape;
   aShape=CullXNoMoreThan(MXRES,aShape);
   DisposeShape(tmp);

   tmp=aShape;
   aShape=CullYNoMoreThan(MYRES,aShape);
   DisposeShape(tmp);

   tmp=aShape;
   aShape=CullXNoLessThan(0,aShape);
   DisposeShape(tmp);

   tmp=aShape;
   aShape=CullYNoLessThan(0,aShape);
   DisposeShape(tmp);
	


   int size=aShape->GetNumberOfPoints();
    if (size>MAX_VERTEX_INDEX)
		size=MAX_VERTEX_INDEX;
	//cout << "size after:" << size << endl;
	
	//points=(SPG_Point*)malloc(size*sizeof(SPG_Point));
    for (int c=size-1;c>=0;c--)
      {
#ifdef SUPPORT_SDL_GFX
	vx[c]=(Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(c)->GetX());
	vy[c]=(Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(c)->GetY());	

#endif

#ifdef SUPPORT_SPG
	points[c].x=(Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(c)->GetX());
	points[c].y=(Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(c)->GetY());
#endif
	//cout << "point "<<vx[c]<<","<<vy[c]<<endl;
      }
    //too many indirections! I must optmize this!

#ifdef SUPPORT_SDL_GFX
    filledPolygonRGBA(aDestination,vx,vy,size,aShape->iFill.r(),aShape->iFill.g(),aShape->iFill.b(),aShape->iFill.a());
#endif

#ifdef SUPPORT_SPG
	    SPG_PolygonFilledBlend(aDestination,size,points,SDL_MapRGBA(aDestination->format,aShape->iFill.r(),aShape->iFill.g(),aShape->iFill.b(),aShape->iFill.a()),aShape->iFill.a());
#endif


  //  if (aShape->iOutLine.a()!=0) polygonRGBA(aDestination,vx,vy,size,aShape->iOutLine.r(),aShape->iOutLine.g(),aShape->iOutLine.b(),aShape->iOutLine.a());


  }
     inline void DrawTriangle(BZK_Bitmap*aDestination,VAC_Shape *aShape,int first=0,int second=1,int third=2)
     {
   VAC_Shape *tmp;

   tmp=aShape;
   aShape=CullXNoMoreThan(MXRES,aShape);
   DisposeShape(tmp);

   tmp=aShape;
   aShape=CullYNoMoreThan(MYRES,aShape);
   DisposeShape(tmp);

   tmp=aShape;
   aShape=CullXNoLessThan(0,aShape);
   DisposeShape(tmp);

   tmp=aShape;
   aShape=CullYNoLessThan(0,aShape);
   DisposeShape(tmp);

#ifdef SUPPORT_SPG

	SPG_TrigonFilledBlend
		(aDestination,
      (Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(first)->GetX()),
      (Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(first)->GetY()),
      (Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(second)->GetX()),
      (Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(second)->GetY()),
      (Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(third)->GetX()),
      (Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(third)->GetY()),
      SDL_MapRGBA(aDestination->format,aShape->iFill.r(),aShape->iFill.g(),aShape->iFill.b(),aShape->iFill.a()),aShape->iFill.a());


	

#endif

#ifdef SUPPORT_SDL_GFX
	{
      filledTrigonRGBA
	(aDestination,
      (Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(first)->GetX()),
      (Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(first)->GetY()),
      (Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(second)->GetX()),
      (Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(second)->GetY()),
      (Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(third)->GetX()),
      (Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(third)->GetY()),
      aShape->iFill.r(),aShape->iFill.g(),aShape->iFill.b(),aShape->iFill.a());
	}
#endif
     }
	  
    inline void DrawQuad(BZK_Bitmap*aDestination,VAC_Shape *aShape)
    {
  	SDL_Rect rect;
	rect.x=0;
	rect.y=0;
	rect.w=0xFFFF;
	rect.h=0xFFFF;
int menor, maior;
cout << "--" <<endl;
for (int c=0;c<4;c++)
	{
	cout << "X:"<<(Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(c)->GetX())<<","<<(Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(c)->GetY())<<endl;

	if ((Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(c)->GetX())> rect.w)
		rect.w=(Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(c)->GetX());
	if ((Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(c)->GetX())> rect.x)
		rect.x=(Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(c)->GetX());	
	if ((Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(c)->GetY())> rect.h)
		rect.h=(Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(c)->GetY());
	if ((Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(c)->GetY())> rect.y)
		rect.y=(Sint16)BZK_FastMath::Fix32toInt32(aShape->Point(c)->GetY());
	}

	if (rect.x<0) rect.x=0;
	if (rect.y<0) rect.y=0;
	if (rect.w>XRES) rect.w=XRES;
	if (rect.h>YRES) rect.h=YRES;

	rect.w-=rect.x;
	rect.h-=rect.y;



	SDL_FillRect(aDestination,&rect,SDL_MapRGBA(aDestination->format,aShape->iFill.r(),aShape->iFill.g(),aShape->iFill.b(),aShape->iFill.a()));

	
/*
	{
	DrawShape(aDestination,aShape);
	}
*/
    }
    
  void Update(BZK_Bitmap *aSurface)
  {

    VAC_Shape *Shape=NULL;

	if (rasterBusy)
	return;
	
	rasterBusy=true;
	//if (fork()) goto end;
	
  //  cout << "update" <<endl;
    if (aSurface==NULL) return;

  	
    while (!iBuffer.Empty())
      {
	//cout << "shape" << endl;
	Shape=(iBuffer.Pop());	
//	DrawShape(aSurface,Shape);
/*
	if (Shape->GetNumberOfPoints()==4 && Shape->iHint!=XY_ALLIGNED && Shape->iHint!=NOPE)
		DrawQuad(aSurface,Shape);
	else

	if (Shape->GetNumberOfPoints()==3)
		DrawTriangle(aSurface,Shape);
	else
*/
		DrawShape(aSurface,Shape);

	DisposeShape(Shape);
      }    
/*
    if (currentalpha>0) 
	{
  		BZK_Bitmap *video=BZK_GetVideoBuffer();
		Sint16 vx[4],vy[4];
		vx[0]=0;
		vy[0]=0;
		vx[1]=(Sint16)BZK_FastMath::Fix32toInt32(XRES);
		vy[1]=0;
		vx[2]=(Sint16)BZK_FastMath::Fix32toInt32(XRES);
		vy[2]=(Sint16)BZK_FastMath::Fix32toInt32(YRES);	
		vx[3]=(Sint16)BZK_FastMath::Fix32toInt32(0);
		vy[3]=(Sint16)BZK_FastMath::Fix32toInt32(YRES);	
		filledPolygonRGBA(video,vx,vy,4,0,0,0,currentalpha);
//		BZK_UpdateRegion(video,0,0,0,0);
		currentalpha-=currentalphaincrement;
		currentalphaincrement+=3;
	}
*/	
///no caso do fork...
	///exit(0);
	end:
	rasterBusy=false;
 }
  
  
  
  inline
    void DrawPolygon(VAC_Shape *aShape)
  {
    iBuffer.Push(aShape);
  }



  inline void
    AddToImage(VAC_Graphic &aGraphic)
  {
	
   // BZK_Queue<VAC_Shape*> tmp;    
    VAC_Shape*handler;    
	  //  while (!aGraphic.Empty())
	int qtd=aGraphic.TotalItems();
	for (int c=0;c<qtd;c++)
	      {
		handler=aGraphic[c];//.Pop();	
		DrawPolygon(handler->Clone(RequestShape()));
//		else
//			cout << "pulei!" <<endl;
		//delete handler->Clone(RequestShape());
	//	tmp.Push(handler);
	      }
	//    while (!tmp.Empty())      aGraphic.Push(tmp.Pop());
	
}
  inline void
    AddToImage(VAC_Image &aGraphic)
  {
	
//    BZK_Queue<VAC_Shape*> tmp;    
    VAC_Shape*handler;
    if (!aGraphic.Empty())
	{
	int qtd=aGraphic.TotalItems();
	for (int c=0;c<qtd;c++)
	      {
		handler=aGraphic[c];//.Pop();	
		DrawPolygon(handler->Clone(RequestShape()));
		//delete handler->Clone(RequestShape());
	//	tmp.Push(handler);
	      }
	  //  while (!tmp.Empty())      aGraphic.Push(tmp.Pop());
	}
	else
		ShowImage(aGraphic.iBitmap);
	
  }
static void ClearScreen()
    {
      BZK_PaintRegion(BZK_GetVideoBuffer(),NULL,BZK_MapColor(BZK_GetVideoBuffer()->format,0,0,0));
    }

static  void ShowImage(BZK_Bitmap *aImage)
    {
      BZK_Bitmap *Screen=BZK_GetVideoBuffer();       
      BZK_LightRect Region;
      Region.x=(XRES-aImage->w)/2;
      Region.y=(YRES-aImage->h)/2;
      Region.h=(aImage->h);
      Region.w=(aImage->w);

      BZK_BitBlockTransfer(aImage,NULL,Screen,&Region);
      BZK_UpdateRegion(Screen,Region.x,Region.y,Region.x+Region.w,Region.y+Region.h);
    }

 void FadeIn()
    {
	cout << "fade in" <<endl;
	PaintItBlack();
//	currentalpha=255;
	currentalphaincrement=15;
    }

static  void FadeToBlack()
    {
    BZK_Bitmap *video=BZK_GetVideoBuffer();
      int c=11;
	Sint16 vx[4],vy[4];
	vx[0]=0;
	vy[0]=0;
	vx[1]=MXRES;
	vy[1]=0;
	vx[2]=MXRES;
	vy[2]=MYRES;	
	vx[3]=(Sint16)BZK_FastMath::Fix32toInt32(0);
	vy[3]=MYRES;	

      while (c--)
	{
#ifdef SUPPORT_SDL_GFX
	  filledPolygonRGBA(video,vx,vy,4,0,0,0,64);
#endif
	  BZK_Wait(30);
	  BZK_UpdateRegion(video,0,0,XRES,YRES);
	}
      PaintItBlack();
    }

static  void PaintItBlack(int alpha=0)
    {    
        BZK_Bitmap *video=BZK_GetVideoBuffer();    
	Sint16 vx[4],vy[4];
	vx[0]=0;
	vy[0]=0;
	vx[1]=(MXRES);
	vy[1]=0;
	vx[2]=(MXRES);
	vy[2]=(MYRES);	
	vx[3]=(Sint16)BZK_FastMath::Fix32toInt32(0);
	vy[3]=(MYRES);	
#ifdef SUPPORT_SDL_GFX
	  filledPolygonRGBA(video,vx,vy,4,0,0,0,alpha);
#endif


	
	BZK_UpdateRegion(video,0,0,XRES,YRES);
    }

 static  void _ShowImage(BZK_Bitmap *aImage,int y,int srcy)
    {
      //  x=(int)(-(x)/1.0);
      //   if (x>0)
	{
	  // //cout << srcy <<endl;
	  BZK_LightRect bmp;
	  BZK_LightRect src;
	  BZK_Bitmap *Screen=BZK_GetVideoBuffer();  
	  
	  src.y=(aImage->h/2)*srcy;
	  src.x=0;
	  src.w=aImage->w;
	  src.h=aImage->h/2;  

	  bmp.x=0;
	  bmp.y=y;
	  bmp.w=aImage->w;
	  bmp.h=aImage->h;         
	  BZK_BitBlockTransfer(aImage,&src,Screen,&bmp);
	  //bmp.x=bmp.w-bmp.x;	  
	  //BZK_BitBlockTransfer(aImage,NULL,Screen,NULL);
	  
	}
    
      //BZK_UpdateRegion(Screen,0,0,0,0);
    }
    
VAC_Shape *   CullXNoMoreThan(BZK_FixP aSpan,VAC_Shape *aSrc)
   {
	VAC_Shape *pa=RequestShape();
	int qtd=aSrc->iPoint.TotalItems();
	if (qtd ==0 ) return pa;
	bool lastinside=(aSrc->iPoint[qtd-1]->GetX()<aSpan);
	BZK_FixP DX;
	int prev;
	BZK_Vec2f *prev_ptr;	
	BZK_Vec2f *curr_ptr;
	BZK_Vec2f *other;
	BZK_FixP a2,b1,b2;
	for (int c=0;c<qtd;c++)
	{
	curr_ptr=aSrc->iPoint[c];
		if (curr_ptr->GetX()<aSpan)
			{

			
			if (!lastinside)
				{

				prev=c-1;
				while (prev<0)	prev+=qtd;
				prev=prev % qtd;
				prev_ptr=aSrc->iPoint[prev];

				a2=curr_ptr->GetX()-prev_ptr->GetX();
				b1=curr_ptr->GetX()-aSpan;
				b2=curr_ptr->GetY()-prev_ptr->GetY();
				DX=curr_ptr->GetY()-BZK_FastMath::Div(BZK_FastMath::Mul(b1,b2),a2);			
				other=(new BZK_Vec2f(aSpan,DX));	
				pa->iPoint.Add(other);
				lastinside=true;
				}

				other=new BZK_Vec2f(*aSrc->iPoint[c]);
				pa->iPoint.Add(other);	
			}
		else
			{	
			if (lastinside)
				{
				prev=c-1;
				
				while (prev<0)	prev+=qtd;
				prev=prev % qtd;
				prev_ptr=aSrc->iPoint[prev];

				a2=curr_ptr->GetX()-prev_ptr->GetX();
				b1=curr_ptr->GetX()-aSpan;
				b2=curr_ptr->GetY()-prev_ptr->GetY();
				DX=curr_ptr->GetY()-BZK_FastMath::Div(BZK_FastMath::Mul(b1,b2),a2);
				other=(new BZK_Vec2f(aSpan,DX));
				pa->iPoint.Add(other);	
				lastinside=false;
				}
			}
	}	
	pa->iFill.Copy(aSrc->iFill);
	pa->iOutLine.Copy(aSrc->iOutLine);
	return pa;

   }


VAC_Shape *   CullXNoLessThan(BZK_FixP aSpan,VAC_Shape *aSrc)
   {
	VAC_Shape *pa=RequestShape();
	int qtd=aSrc->iPoint.TotalItems();
	if (qtd ==0 ) return pa;
	bool lastinside=(aSrc->iPoint[qtd-1]->GetX()>aSpan);
	BZK_FixP DX;
	int prev;
	BZK_Vec2f *prev_ptr;	
	BZK_Vec2f *curr_ptr;
	BZK_Vec2f *other;
	BZK_FixP a2,b1,b2;
	for (int c=0;c<qtd;c++)
	{
	curr_ptr=aSrc->iPoint[c];
		if (curr_ptr->GetX()>aSpan)
			{			
			if (!lastinside)
				{

				prev=c-1;
				while (prev<0)	prev+=qtd;
				prev=prev % qtd;
				prev_ptr=aSrc->iPoint[prev];

				a2=curr_ptr->GetX()-prev_ptr->GetX();
				b1=curr_ptr->GetX()-aSpan;
				b2=curr_ptr->GetY()-prev_ptr->GetY();
				DX=curr_ptr->GetY()-BZK_FastMath::Div(BZK_FastMath::Mul(b1,b2),a2);			
				other=(new BZK_Vec2f(aSpan,DX));	
				pa->iPoint.Add(other);
				lastinside=true;
				}
				
				other=new BZK_Vec2f(*curr_ptr);
				pa->iPoint.Add(other);	
			}
		else
			{	
			if (lastinside)
				{
				prev=c-1;
				
				while (prev<0)	prev+=qtd;
				prev=prev % qtd;
				prev_ptr=aSrc->iPoint[prev];

				a2=curr_ptr->GetX()-prev_ptr->GetX();
				b1=curr_ptr->GetX()-aSpan;
				b2=curr_ptr->GetY()-prev_ptr->GetY();
				DX=curr_ptr->GetY()-BZK_FastMath::Div(BZK_FastMath::Mul(b1,b2),a2);
				other=(new BZK_Vec2f(aSpan,DX));
				pa->iPoint.Add(other);	
				lastinside=false;
				}
			}
	}
	pa->iFill.Copy(aSrc->iFill);
	pa->iOutLine.Copy(aSrc->iOutLine);

	return pa;

   }

VAC_Shape *   CullYNoMoreThan(BZK_FixP aSpan,VAC_Shape *aSrc)
   {
	VAC_Shape *pa=RequestShape();
	int qtd=aSrc->iPoint.TotalItems();
	if (qtd ==0 ) return pa;
	bool lastinside=(aSrc->iPoint[qtd-1]->GetY()<aSpan);
	BZK_FixP DY;
	int prev;
	BZK_Vec2f *prev_ptr;	
	BZK_Vec2f *curr_ptr;
	BZK_Vec2f *other;
	BZK_FixP a2,b1,b2;
	for (int c=0;c<qtd;c++)
	{
	curr_ptr=aSrc->iPoint[c];
		if (curr_ptr->GetY()<aSpan)
			{

			
			if (!lastinside)
				{

				prev=c-1;
				while (prev<0)	prev+=qtd;
				prev=prev % qtd;
				prev_ptr=aSrc->iPoint[prev];

				a2=curr_ptr->GetY()-prev_ptr->GetY();
				b1=curr_ptr->GetY()-aSpan;
				b2=curr_ptr->GetX()-prev_ptr->GetX();
				DY=curr_ptr->GetX()-BZK_FastMath::Div(BZK_FastMath::Mul(b1,b2),a2);			
				other=(new BZK_Vec2f(DY,aSpan));	
				pa->iPoint.Add(other);
				lastinside=true;
				}

				other=new BZK_Vec2f(*curr_ptr);
				pa->iPoint.Add(other);	
			}
		else
			{	
			if (lastinside)
				{
				prev=c-1;
				
				while (prev<0)	prev+=qtd;
				prev=prev % qtd;
				prev_ptr=aSrc->iPoint[prev];

				a2=curr_ptr->GetY()-prev_ptr->GetY();
				b1=curr_ptr->GetY()-aSpan;
				b2=curr_ptr->GetX()-prev_ptr->GetX();
				DY=curr_ptr->GetX()-BZK_FastMath::Div(BZK_FastMath::Mul(b1,b2),a2);
				other=(new BZK_Vec2f(DY,aSpan));
				pa->iPoint.Add(other);	
				lastinside=false;
				}
			}	
	}
	pa->iFill.Copy(aSrc->iFill);
	pa->iOutLine.Copy(aSrc->iOutLine);

	return pa;

   }


VAC_Shape *   CullYNoLessThan(BZK_FixP aSpan,VAC_Shape *aSrc)
   {
	VAC_Shape *pa=RequestShape();
	int qtd=aSrc->iPoint.TotalItems();
	if (qtd ==0 ) return pa;
	bool lastinside=(aSrc->iPoint[qtd-1]->GetY()>aSpan);
	BZK_FixP DY;
	int prev;
	BZK_Vec2f *prev_ptr;	
	BZK_Vec2f *curr_ptr;
	BZK_Vec2f *other;
	BZK_FixP a2,b1,b2;
	for (int c=0;c<qtd;c++)
	{
	curr_ptr=aSrc->iPoint[c];
		if (curr_ptr->GetY()>aSpan)
			{			
			if (!lastinside)
				{

				prev=c-1;
				while (prev<0)	prev+=qtd;
				prev=prev % qtd;
				prev_ptr=aSrc->iPoint[prev];

				a2=curr_ptr->GetY()-prev_ptr->GetY();
				b1=curr_ptr->GetY()-aSpan;
				b2=curr_ptr->GetX()-prev_ptr->GetX();
				DY=curr_ptr->GetX()-BZK_FastMath::Div(BZK_FastMath::Mul(b1,b2),a2);			
				other=(new BZK_Vec2f(DY,aSpan));	
				pa->iPoint.Add(other);
				lastinside=true;
				}
				
				other=new BZK_Vec2f(*curr_ptr);
				pa->iPoint.Add(other);	
			}
		else
			{	
			if (lastinside)
				{
				prev=c-1;
				
				while (prev<0)	prev+=qtd;
				prev=prev % qtd;
				prev_ptr=aSrc->iPoint[prev];

				a2=curr_ptr->GetY()-prev_ptr->GetY();
				b1=curr_ptr->GetY()-aSpan;
				b2=curr_ptr->GetX()-prev_ptr->GetX();
				DY=curr_ptr->GetX()-BZK_FastMath::Div(BZK_FastMath::Mul(b1,b2),a2);
				other=(new BZK_Vec2f(DY,aSpan));
				pa->iPoint.Add(other);	
				lastinside=false;
				}
			}
	}
	pa->iFill.Copy(aSrc->iFill);
	pa->iOutLine.Copy(aSrc->iOutLine);

	return pa;

   }
};
VAC_Rasterer *VAC_Rasterer::iInstance=NULL;
