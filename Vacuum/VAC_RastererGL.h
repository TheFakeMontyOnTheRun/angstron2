//parte de desenhos de primitivas. aqui nada � 3D, vale lembrar.
#include <GL/gl.h>
#include <GL/glu.h>

class VAC_Rasterer : public BZK_View {
private:
	typedef BZK_Vec2f *BZK_Vec2f_ptr;
public:
	BZK_Rect iCanvasSize;
	BZK_Queue<VAC_Shape *> iBuffer;
	BZK_Stack<VAC_Shape *> iShapeRecycler;
	int iBPP;
	static VAC_Rasterer *iInstance;
	//  BZK_Bitmap *texture;
public:

	int currentalpha;
	int currentalphaincrement;

	static VAC_Rasterer *GetInstance() {
		if (iInstance != NULL) {
//	iInstance->currentalpha=0;
			return iInstance;
		}
		iInstance = new VAC_Rasterer();
//iInstance->currentalpha=0;
		return iInstance;
	}

	///////////////////////////////////////////////////////////////////////
	virtual void Render(BZK_Bitmap *aDst) {
		Update(aDst);
		//texture=BZK_LoadBitmap("./storage/paused.bmp");
	}

	virtual void Render() {
		// texture=BZK_LoadBitmap("./storage/paused.bmp");
	}

	void ReservePolys(long aQtd) {
		//iBuffer.Reserve(aQtd);
		//  iShapeRecycler.Reserve(4*aQtd);
	}

	virtual BZK_View *ConsumeKey(long aKey) {
		return this;
	}

	VAC_Rasterer() {
		iCanvasSize.SetX0(0);
		iCanvasSize.SetY0(0);
		iCanvasSize.SetX1(0);
		iCanvasSize.SetY1(0);
		//cout << "rasterer:contrutor padr�o"<<endl;
		iBPP = 32;
		/* Enable smooth shading */
		glShadeModel(GL_SMOOTH);

		/* Set the background black */
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		/* Depth buffer setup */
//	    glClearDepth( 1.0f );

		/* Enables Depth Testing */
		//    glEnable( GL_DEPTH_TEST );

		/* The Type Of Depth Test To Do */
		//  glDepthFunc( GL_LEQUAL );

		/* Really Nice Perspective Calculations */
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		GLfloat ratio;

		ratio = (GLfloat) XRES / (GLfloat) YRES;

		/* Setup our viewport. */
		glViewport(0, 0, (GLsizei) XRES, (GLsizei) YRES);

		/* change to the projection matrix and set our viewing volume. */
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		/* Set our perspective */
		gluPerspective(60.0f, ratio, 0.01f, 255.0f);

		/* Make sure we're chaning the model view and not the projection */
		glMatrixMode(GL_MODELVIEW);

		/* Reset The View */
		glLoadIdentity();
	}

	VAC_Rasterer(BZK_Rect &aCanvasSize, int aBPP) {
		VAC_Rasterer();
		//cout <<"rasterer:construtor especifico"<<endl;
		iCanvasSize.copy(aCanvasSize);
		iBPP = aBPP;
	}


	VAC_Shape *RequestShape() {
		if (iShapeRecycler.Empty())
			return new VAC_Shape();
		else
			return iShapeRecycler.Pop();
	}

	~VAC_Rasterer() {
		while (!iBuffer.Empty()) delete iBuffer.Pop();
		while (!iShapeRecycler.Empty()) delete iShapeRecycler.Pop();
	}


	void DisposeShape(VAC_Shape *aShape) {
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

	inline void
	DrawTriangle(BZK_Bitmap *aDestination, VAC_Shape *aShape, VAC_ColorRGBA &aColor, VAC_ColorRGBA &aOutLine) {

	}

	inline void DrawQuad(BZK_Bitmap *aDestination, VAC_Shape *aShape) {

	}


	inline void DrawShape(BZK_Bitmap *aDestination, VAC_Shape *aShape) {
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);                    // Set Line Antialiasing
//	glEnable(GL_BLEND);							// Enable Blending
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Type Of Blending To Us
		glEnable(GL_LINE_SMOOTH);

		int r = BZK_FastMath::Fix32toInt32(aShape->iFill.GetR()) % 255;
		int g = BZK_FastMath::Fix32toInt32(aShape->iFill.GetG()) % 255;
		int b = BZK_FastMath::Fix32toInt32(aShape->iFill.GetB()) % 255;
		int a = BZK_FastMath::Fix32toInt32(aShape->iFill.GetA()) % 255;
//	cout << r << "," << g << "," << b << endl;
		BZK_Vec2f *point;
		glColor4f(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
		glBegin(GL_POLYGON);
//	glVertex3f(-1.6+BZK_FastMath::Fix32toReal32(aShape->Point(aShape->iPoint.TotalItems()-1)->GetX())/240.0,1- BZK_FastMath::Fix32toReal32(aShape->Point(aShape->iPoint.TotalItems()-1)->GetY())/240.0,-1.6f);
		for (int counter = 0; counter < aShape->iPoint.TotalItems(); counter++) {
			point = aShape->Point(counter % aShape->iPoint.TotalItems());
			glVertex3f(-1.6 + BZK_FastMath::Fix32toReal32(point->GetX()) / 240.0,
					   1 - BZK_FastMath::Fix32toReal32(point->GetY()) / 240.0, -1.6f);
		}
//	glVertex3f(-1.6+BZK_FastMath::Fix32toReal32(aShape->Point(0)->GetX())/240.0,1- BZK_FastMath::Fix32toReal32(aShape->Point(0)->GetY())/240.0,-1.6f);

		glEnd();
	}


	void Update(BZK_Bitmap *aSurface) {
		if (aSurface == NULL) return;
		VAC_Shape *Shape = NULL;

		while (!iBuffer.Empty()) {
			Shape = (iBuffer.Pop());
			DrawShape(aSurface, Shape);
			DisposeShape(Shape);
		}

	}


	inline
	void DrawPolygon(VAC_Shape *aShape) {
		iBuffer.Push(aShape);
	}


	void PreAllocateResources(int x) {}

	void FadeIn() {
	}

	inline void
	AddToImage(VAC_Graphic &aGraphic) {

		// BZK_Queue<VAC_Shape*> tmp;
		VAC_Shape *handler;
		//  while (!aGraphic.Empty())
		int qtd = aGraphic.TotalItems();
		for (int c = 0; c < qtd; c++) {
			handler = aGraphic[c];//.Pop();
			DrawPolygon(handler->Clone(RequestShape()));
//		else
//			cout << "pulei!" <<endl;
			//delete handler->Clone(RequestShape());
			//	tmp.Push(handler);
		}
		//    while (!tmp.Empty())      aGraphic.Push(tmp.Pop());

	}

	inline void
	AddToImage(VAC_Image &aGraphic) {

//    BZK_Queue<VAC_Shape*> tmp;    
		VAC_Shape *handler;
		if (!aGraphic.Empty()) {
			int qtd = aGraphic.TotalItems();
			for (int c = 0; c < qtd; c++) {
				handler = aGraphic[c];//.Pop();
				DrawPolygon(handler->Clone(RequestShape()));
				//delete handler->Clone(RequestShape());
				//	tmp.Push(handler);
			}
			//  while (!tmp.Empty())      aGraphic.Push(tmp.Pop());
		} else
			ShowImage(aGraphic.iBitmap);

	}

	static void ClearScreen() {
		//  BZK_PaintRegion(BZK_GetVideoBuffer(),NULL,BZK_MapColor(BZK_GetVideoBuffer()->format,255,255,255));
	}

	static void ShowImage(BZK_Bitmap *aImage) {
		/*
		  BZK_Bitmap *Screen=BZK_GetVideoBuffer();
		  BZK_LightRect Region;
		  Region.x=(XRES-aImage->w)/2;
		  Region.y=(YRES-aImage->h)/2;
		  Region.h=(aImage->h);
		  Region.w=(aImage->w);

		  BZK_BitBlockTransfer(aImage,NULL,Screen,&Region);
		  BZK_UpdateRegion(Screen,0,0,0,0);
		*/
	}

	static void FadeToBlack() {
		/*
		  int c=8;
		  while (c--)
		{
		  filledTrigonRGBA(BZK_GetVideoBuffer(),0,0,0,YRES,XRES,0,0,0,0,64);
		  filledTrigonRGBA(BZK_GetVideoBuffer(),XRES,YRES,0,YRES,XRES,0,0,0,0,64);
		  BZK_Wait(30);
		  BZK_UpdateRegion(BZK_GetVideoBuffer(),0,0,0,0);
		}
		  PaintItBlack();
		*/
	}

	static void PaintItBlack() {
		/*
		  filledTrigonRGBA(BZK_GetVideoBuffer(),0,0,0,YRES,XRES,0,0,0,0,255);
		  filledTrigonRGBA(BZK_GetVideoBuffer(),XRES,YRES,0,YRES,XRES,0,0,0,0,255);
		  BZK_UpdateRegion(BZK_GetVideoBuffer(),0,0,0,0);
		*/
	}

	static void _ShowImage(BZK_Bitmap *aImage, int y, int srcy) {
		//  x=(int)(-(x)/1.0);
		//   if (x>0)
		{
			// //cout << srcy <<endl;
			BZK_LightRect bmp;
			BZK_LightRect src;
			BZK_Bitmap *Screen = BZK_GetVideoBuffer();

			src.y = (aImage->h / 2) * srcy;
			src.x = 0;
			src.w = aImage->w;
			src.h = aImage->h / 2;

			bmp.x = 0;
			bmp.y = y;
			bmp.w = aImage->w;
			bmp.h = aImage->h;
			BZK_BitBlockTransfer(aImage, &src, Screen, &bmp);
			//bmp.x=bmp.w-bmp.x;
			//BZK_BitBlockTransfer(aImage,NULL,Screen,NULL);

		}

		//BZK_UpdateRegion(Screen,0,0,0,0);
	}

};

VAC_Rasterer *VAC_Rasterer::iInstance = NULL;
