enum {
	POLYGON_TOTALLY_OUT, POLYGON_PARTIALLY_IN, POLYGON_TOTALLY_IN
};
enum {
	LOD_FAR, LOD_NEAR, LOD_NEARER
};

class VAC_Render : public VAC_Rasterer {
	typedef BZK_Vec3f *BZK_Vec3f_ptr;
	//==============================================================================
	//data

	BZK_Vec3f iCurrentCoordenates;
	BZK_Vec3f iCurrentOrigin;
	BZK_Vec3f iCannonicalOrigin;
	BZK_Vec3f iCannonicalCoordenates;
	BZK_Vec3f iSavedCoordenates;
	BZK_Vec3f iSavedOrigin;
	BZK_FixP iSavedAngle;
	BZK_FixP iSavedTail;
	BZK_FixP iSavedHead;
	BZK_FixP HALFX, HALFY, RESX, RESY, ONE;
	BZK_FixP FIX_AngleLimit;


	BZK_FixP Cos;
	BZK_FixP Sin;
	BZK_FixP FIX_0xFF;
	BZK_FixP SinT[360];
	BZK_FixP CosT[360];
	//   BZK_Queue<VAC_Face*> iDepthBuffer;
	BZK_Queue<VAC_Face *> iFinalBuffer;
	BZK_Stack<BZK_Vec3f *> iVecRecycler;
	BZK_Stack<VAC_Face *> iFaceRecycler;
	BZK_Queue<BZK_Vec3f *> iCoordenateStack;
	//buffers
	BZK_Vec3f_ptr *ptr;
	BZK_Vec3f *vex;
	bool renderBusy;
	//======================================================================================
public:
	VAC_Sector iFrustum, iCannonicalFrustum;
	BZK_FixP iNearClippingPlane, iFarClippingPlane, iAlpha, iAngle;

	//======================================================================================
	//--------------------------------------------------------------------------------------
	//methods

	VAC_Sector *GetFrustum() {
		return &iFrustum;
	}

	VAC_Face *RequestFace() {
		if (iFaceRecycler.Empty())
			return new VAC_Face();
		else
			return iFaceRecycler.Pop();
	}
	//---------------------------------------------------------------------------------------

	void
	SaveContext() {
		BZK_VecOps::Copy(iSavedCoordenates, iCurrentCoordenates);
		BZK_VecOps::Copy(iSavedOrigin, iCurrentOrigin);
		iSavedAngle = iAngle;
	} //---------------------------------------------------------------------------------------

	void
	RestoreContext() {
		BZK_VecOps::Copy(iCurrentCoordenates, iSavedCoordenates);
		BZK_VecOps::Copy(iCurrentOrigin, iSavedOrigin);
		iAngle = iSavedAngle;
		//     iDepthBuffer.iHead = iSavedHead;
		//   iDepthBuffer.iTail = iSavedTail;
	}

	~VAC_Render() {

		//iDepthBuffer.EraseAll();// while (!iDepthBuffer.Empty()) delete iDepthBuffer.Pop();
		while (!iFinalBuffer.Empty()) delete iFinalBuffer.Pop();

		while (!iVecRecycler.Empty()) delete iVecRecycler.Pop();
		while (!iFaceRecycler.Empty()) delete iFaceRecycler.Pop();
		while (!iCoordenateStack.Empty()) delete iCoordenateStack.Pop();

		free(vex);
		free(ptr);
	}

	BZK_Vec3f *RequestVec3f() {
		if (iVecRecycler.Empty())
			return new BZK_Vec3f();
		else
			return iVecRecycler.Pop();
	}

	void DisposeVec(BZK_Vec3f *aVec) {
		iVecRecycler.Push(aVec);
	}

	void PreAllocateResources(int size) {

		BZK_Stack<VAC_Face *> tmp;

		for (int c = 0; c < size; c++)
			tmp.Push(RequestFace());

		while (!tmp.Empty()) DisposeFace(tmp.Pop());

		VAC_Rasterer::PreAllocateResources(size);
	}

	void DisposeFace(VAC_Face *aFace) {

		int qtd = aFace->GetTotalPoints(); //+1;
		if (qtd > MAX_VERTEX_INDEX)
			qtd = MAX_VERTEX_INDEX;
		for (int c = qtd - 1; c >= 0; c--)
			ptr[c] = aFace->GetPoint(c);
		delete aFace->GetNormal();
		aFace->iNormal = NULL;
		aFace->ReleaseAll();
		for (int c = qtd - 1; c >= 0; c--) {
			delete (ptr[c]);
			ptr[c] = NULL;
		}

		//delete aFace;
		iFaceRecycler.Push(aFace);
	}

	VAC_Render() {
		renderBusy = false;
		FIX_AngleLimit = BZK_FastMath::Int32toFix32(5 * 90);
		RESX = BZK_FastMath::Int32toFix32(XRES);
		RESY = BZK_FastMath::Int32toFix32(YRES);
		HALFX = BZK_FastMath::Int32toFix32(XRES / 2);
		HALFY = BZK_FastMath::Int32toFix32(YRES / 2);
		ONE = BZK_FastMath::Int32toFix32(1);
		FIX_0xFF = BZK_FastMath::Real32toFix32(255.0);
		iCannonicalCoordenates.SetX(BZK_FastMath::Int32toFix32(1));
		iCannonicalCoordenates.SetY(BZK_FastMath::Int32toFix32(1));
		iCannonicalCoordenates.SetZ(BZK_FastMath::Int32toFix32(1));
		iCannonicalOrigin.SetX(BZK_FastMath::Int32toFix32(0));
		iCannonicalOrigin.SetY(BZK_FastMath::Int32toFix32(0));
		iCannonicalOrigin.SetZ(BZK_FastMath::Int32toFix32(0));
		LoadCannonical();
		SetFrustum(BZK_FastMath::Real32toFix32(254.0f), BZK_FastMath::Real32toFix32(1.0f));
		iAlpha = BZK_FastMath::Real32toFix32(45 * (3.14159257 / 180));
		for (float c = 0.0; c < 360.0; c += 1.0) {
			SinT[(int) c] = BZK_FastMath::Real32toFix32((float) sin(c * (3.141519257 / 180)));
			CosT[(int) c] = BZK_FastMath::Real32toFix32((float) cos(c * (3.141519257 / 180)));
		}
		vex = (BZK_Vec3f *) malloc(sizeof(BZK_Vec3f) * MAX_VERTEX_PER_POLY); //handler->GetTotalPoints());
		ptr = (BZK_Vec3f_ptr *) malloc(sizeof(BZK_Vec3f_ptr) * MAX_VERTEX_PER_POLY);
	}

	void SetFrustum(BZK_FixP aFarClippingPlane, BZK_FixP aNearClippingPlane) {
		BZK_FixP two = BZK_FastMath::Int32toFix32(2);
		iFarClippingPlane = aFarClippingPlane;
		iNearClippingPlane = aNearClippingPlane;

		BZK_Rect *Rect = new BZK_Rect;
		Rect->SetY0(iFarClippingPlane + iNearClippingPlane);
		Rect->SetH(-iFarClippingPlane);

		//   Rect->SetX0(BZK_FastMath::Div(iFarClippingPlane,BZK_FastMath::Int32toFix32(2)) ); //25 ok
		// Rect->SetW(-iFarClippingPlane); //50 ok

		Rect->SetX0(BZK_FastMath::Int32toFix32(255));//60 ok //25 ok
		Rect->SetW(-BZK_FastMath::Int32toFix32(512));//120 ok //50 ok


		iCannonicalFrustum.SetRect(*Rect);
		iCannonicalFrustum.SetH0(-iFarClippingPlane + iNearClippingPlane);
		iCannonicalFrustum.SetH1(iFarClippingPlane - iNearClippingPlane);

		delete Rect;
	}


	//---------------------------------------------------------------------------------------

	inline void
	SetCamera(VAC_Actor &aActor) {
		SetAngle(aActor.GetAngle());
		Translate(*aActor.GetPosition());
	}
	//---------------------------------------------------------------------------------------

	inline void
	SetAngle(BZK_FixP aAngle) {
		iAngle = aAngle;
		int intangle = BZK_FastMath::Fix32toInt32(iAngle);
		while (intangle < 0) intangle = 360 + intangle;
		while (intangle >= 360) intangle = intangle - 360;
		Sin = SinT[intangle];
		Cos = CosT[intangle];
	}

	//---------------------------------------------------------------------------------------

	void
	RotateVec(BZK_FixP aAngle, BZK_Vec3f &aVec) {
		BZK_FixP X = aVec.GetX();
		BZK_FixP Z = aVec.GetZ();
		aVec.SetX(BZK_FastMath::Mul(X, Cos) - BZK_FastMath::Mul(Z, Sin));
		aVec.SetZ(BZK_FastMath::Mul(Z, Cos) + BZK_FastMath::Mul(X, Sin));
	}
	//---------------------------------------------------------------------------------------

	void
	LoadCannonical() {
		iAngle = 0;
		memcpy(&iFrustum, &iCannonicalFrustum, sizeof(BZK_VolumetricSector));
		BZK_VecOps::Copy(iCurrentCoordenates, iCannonicalCoordenates);
		BZK_VecOps::Copy(iCurrentOrigin, iCannonicalOrigin);
//        iDepthBuffer.iHead = 0;
		//      iDepthBuffer.iTail = 0;

	}
	//---------------------------------------------------------------------------------------

	void
	Translate(BZK_Vec3f aTranslation) {
		BZK_VecOps::BSCtoLHC(aTranslation);
		iCurrentOrigin.SetX(iCurrentOrigin.GetX() - aTranslation.GetX());
		iCurrentOrigin.SetY(iCurrentOrigin.GetY() - aTranslation.GetY());
		iCurrentOrigin.SetZ(iCurrentOrigin.GetZ() - aTranslation.GetZ());
	}

	void
	Transform(BZK_Vec3f &aVector) {
		BZK_VecOps::BSCtoLHC(aVector);
		aVector.SetX(BZK_FastMath::Mul(iCurrentCoordenates.GetX(), aVector.GetX()) + iCurrentOrigin.GetX());
		aVector.SetY(BZK_FastMath::Mul(iCurrentCoordenates.GetY(), aVector.GetY()) + iCurrentOrigin.GetY());
		aVector.SetZ(BZK_FastMath::Mul(iCurrentCoordenates.GetZ(), aVector.GetZ()) + iCurrentOrigin.GetZ());
		RotateVec(iAngle, aVector);
	}
	//---------------------------------------------------------------------------------------

	BZK_Vec3f *
	_Transform(BZK_Vec3f &aVector) {
		BZK_Vec3f *_vector = RequestVec3f();
		_vector->SetSpace(LHC);
		_vector->SetX(BZK_FastMath::Mul(iCurrentCoordenates.GetX(), aVector.GetX()) + iCurrentOrigin.GetX());
		_vector->SetY(BZK_FastMath::Mul(iCurrentCoordenates.GetY(), aVector.GetY()) + iCurrentOrigin.GetY());
		_vector->SetZ(BZK_FastMath::Mul(iCurrentCoordenates.GetZ(), aVector.GetZ()) + iCurrentOrigin.GetZ());
		RotateVec(iAngle, *_vector);
		return _vector;
	}

	//---------------------------------------------------------------------------------------

	void
	Transform(VAC_Face &aFace) {
		for (int c = 0; c < aFace.GetTotalPoints(); c++)
			Transform(*aFace.GetPoint(c));
	}

	//---------------------------------------------------------------------------------------

	VAC_Face *
	_Transform(VAC_Face &aFace) {
		VAC_Face *handler = RequestFace();
		BZK_Vec3f *hnd;
		for (int c = aFace.GetTotalPoints() - 1; c >= 0; c--) {
			aFace.GetPoint(c)->SetSpace(BSC);
			hnd = _Transform(*aFace.GetPoint(c));
			handler->AddPoint(hnd);
			DisposeVec(hnd); // solve a memory leak =-D
			aFace.GetPoint(c)->SetSpace(LHC);
		}
		return handler;
	}

	//---------------------------------------------------------------------------------------

	void
	Transform(VAC_PolyBuffer &aMesh) {
		VAC_Face *handler;
		VAC_PolyBuffer temp;
		// while (!aMesh.Empty())
		int qtd = aMesh.TotalItems();
		for (int c = 0; c < qtd; c++) {
			handler = aMesh[c];//.Pop();
			Transform(*handler);
			//   temp.Push(handler);
		}
		// aMesh.Push(temp);
	}
	//---------------------------------------------------------------------------------------

	inline VAC_Face *
	ExtractFace(VAC_Sector &aSector, int aWhere) {
		BZK_Vec3f *Vec1, *Vec2, *Vec3, *Vec4;
		VAC_Face *Face;
		int hint;
		Face = RequestFace();
		Vec1 = RequestVec3f();
		Vec2 = RequestVec3f();
		Vec3 = RequestVec3f();
		Vec4 = RequestVec3f();
		BZK_FixP X0;
		BZK_FixP X1;
		BZK_FixP Y0;
		BZK_FixP Y1;
		BZK_FixP H0;
		BZK_FixP H1;

		switch (aWhere) {
			case W:
				hint = YZ_ALLIGNED;
				X1 = aSector.GetX1();
				Y0 = aSector.GetY0();
				Y1 = aSector.GetY1();
				H0 = aSector.GetH0();
				H1 = aSector.GetH1();
				Vec1->SetVec(X1, Y0, H0);
				Vec2->SetVec(X1, Y0, H1);
				Vec3->SetVec(X1, Y1, H1);
				Vec4->SetVec(X1, Y1, H0);
				break;
			case N:
				hint = XZ_ALLIGNED;
				X1 = aSector.GetX1();
				X0 = aSector.GetX0();
				Y0 = aSector.GetY0();
				H0 = aSector.GetH0();
				H1 = aSector.GetH1();
				Vec1->SetVec(X0, Y0, H0);
				Vec2->SetVec(X0, Y0, H1);
				Vec3->SetVec(X1, Y0, H1);
				Vec4->SetVec(X1, Y0, H0);
				break;
			case E:
				hint = YZ_ALLIGNED;
				X0 = aSector.GetX0();
				Y0 = aSector.GetY0();
				Y1 = aSector.GetY1();
				H0 = aSector.GetH0();
				H1 = aSector.GetH1();
				Vec1->SetVec(X0, Y0, H0);
				Vec2->SetVec(X0, Y0, H1);
				Vec3->SetVec(X0, Y1, H1);
				Vec4->SetVec(X0, Y1, H0);
				break;
			case S:
				hint = XZ_ALLIGNED;
				X1 = aSector.GetX1();
				X0 = aSector.GetX0();
				Y1 = aSector.GetY1();
				H0 = aSector.GetH0();
				H1 = aSector.GetH1();
				Vec1->SetVec(X0, Y1, H0);
				Vec2->SetVec(X0, Y1, H1);
				Vec3->SetVec(X1, Y1, H1);
				Vec4->SetVec(X1, Y1, H0);
				break;
			case FLOOR:
				hint = XY_ALLIGNED;
				X1 = aSector.GetX1();
				X0 = aSector.GetX0();
				Y1 = aSector.GetY1();
				H0 = aSector.GetH0();
				Y0 = aSector.GetY0();
				Vec1->SetVec(X0, Y0, H0);
				Vec2->SetVec(X1, Y0, H0);
				Vec3->SetVec(X1, Y1, H0);
				Vec4->SetVec(X0, Y1, H0);
				break;
			case CEILING:
				hint = XY_ALLIGNED;
				X1 = aSector.GetX1();
				X0 = aSector.GetX0();
				Y1 = aSector.GetY1();
				H1 = aSector.GetH1();
				Y0 = aSector.GetY0();

				Vec1->SetVec(X0, Y0, H1);
				Vec2->SetVec(X1, Y0, H1);
				Vec3->SetVec(X1, Y1, H1);
				Vec4->SetVec(X0, Y1, H1);
				break;
		}
		//ajustador como BSC porque vieram dos setores
		Vec1->SetSpace(BSC);
		Vec2->SetSpace(BSC);
		Vec3->SetSpace(BSC);
		Vec4->SetSpace(BSC);
		//transformados pro espaco da camera
		Transform(*Vec1);
		Transform(*Vec3);
		Transform(*Vec2);
		Transform(*Vec4);

		Face->SetFace(Vec1, Vec2, Vec3, Vec4, hint);
		DisposeVec(Vec1);
		DisposeVec(Vec2);
		DisposeVec(Vec3);
		DisposeVec(Vec4);
		Face->iFill.Copy(aSector.iColor[aWhere]);

		return Face;
	}
	//---------------------------------------------------------------------------------------

	inline VAC_Face *
	JustExtractFace(VAC_Sector &aSector, int aWhere) {
		BZK_Vec3f *Vec1, *Vec2, *Vec3, *Vec4;
		VAC_Face *Face;
		int hint;
		Face = RequestFace();
		Vec1 = RequestVec3f();
		Vec2 = RequestVec3f();
		Vec3 = RequestVec3f();
		Vec4 = RequestVec3f();

		BZK_FixP X0;
		BZK_FixP X1;
		BZK_FixP Y0;
		BZK_FixP Y1;
		BZK_FixP H0;
		BZK_FixP H1;

		switch (aWhere) {
			case W:
				hint = YZ_ALLIGNED;
				X1 = aSector.GetX1();
				Y0 = aSector.GetY0();
				Y1 = aSector.GetY1();
				H0 = aSector.GetH0();
				H1 = aSector.GetH1();

				Vec1->SetVec(X1, Y0, H0);
				Vec2->SetVec(X1, Y0, H1);
				Vec3->SetVec(X1, Y1, H1);
				Vec4->SetVec(X1, Y1, H0);
				break;
			case N:
				hint = XZ_ALLIGNED;
				X1 = aSector.GetX1();
				X0 = aSector.GetX0();
				Y0 = aSector.GetY0();
				H0 = aSector.GetH0();
				H1 = aSector.GetH1();
				Vec1->SetVec(X0, Y0, H0);
				Vec2->SetVec(X0, Y0, H1);
				Vec3->SetVec(X1, Y0, H1);
				Vec4->SetVec(X1, Y0, H0);
				break;
			case E:
				hint = YZ_ALLIGNED;
				X0 = aSector.GetX0();
				Y0 = aSector.GetY0();
				Y1 = aSector.GetY1();
				H0 = aSector.GetH0();
				H1 = aSector.GetH1();
				Vec1->SetVec(X0, Y0, H0);
				Vec2->SetVec(X0, Y0, H1);
				Vec3->SetVec(X0, Y1, H1);
				Vec4->SetVec(X0, Y1, H0);
				break;
			case S:
				hint = XZ_ALLIGNED;
				X1 = aSector.GetX1();
				X0 = aSector.GetX0();
				Y1 = aSector.GetY1();
				H0 = aSector.GetH0();
				H1 = aSector.GetH1();
				Vec1->SetVec(X0, Y1, H0);
				Vec2->SetVec(X0, Y1, H1);
				Vec3->SetVec(X1, Y1, H1);
				Vec4->SetVec(X1, Y1, H0);

				break;
			case FLOOR:
				hint = XY_ALLIGNED;
				X1 = aSector.GetX1();
				X0 = aSector.GetX0();
				Y1 = aSector.GetY1();
				H0 = aSector.GetH0();
				Y0 = aSector.GetY0();
				Vec1->SetVec(X0, Y0, H0);
				Vec2->SetVec(X1, Y0, H0);
				Vec3->SetVec(X1, Y1, H0);
				Vec4->SetVec(X0, Y1, H0);
				break;
			case CEILING:
				hint = XY_ALLIGNED;
				X1 = aSector.GetX1();
				X0 = aSector.GetX0();
				Y1 = aSector.GetY1();
				H1 = aSector.GetH1();
				Y0 = aSector.GetY0();

				Vec1->SetVec(X0, Y0, H1);
				Vec2->SetVec(X1, Y0, H1);
				Vec3->SetVec(X1, Y1, H1);
				Vec4->SetVec(X0, Y1, H1);
				break;
		}

		//ajustador como BSC porque vieram dos setores
		Vec1->SetSpace(BSC);
		Vec2->SetSpace(BSC);
		Vec3->SetSpace(BSC);
		Vec4->SetSpace(BSC);
		//transformados pro espaco da camera

		//ateh porque faces sempre estao em LHC

		Face->SetFace(Vec1, Vec2, Vec3, Vec4, hint);
		DisposeVec(Vec1);
		DisposeVec(Vec2);
		DisposeVec(Vec3);
		DisposeVec(Vec4);
		Face->iFill.Copy(aSector.iColor[aWhere]);
		Face->iOutLine.SetColor(0, 0, 0, 0);
		return Face;
	}
	//---------------------------------------------------------------------------------------

	inline BZK_Vec3f
	GetTexturePosition(VAC_Sector &aSector, int aWhere) {
		BZK_Vec3f Vec1;
		switch (aWhere) {
			case W:
				Vec1.SetVec(aSector.GetX1(), aSector.GetY1(), aSector.GetH1());
				break;
			case N:
				Vec1.SetVec(aSector.GetX0(), aSector.GetY0(), aSector.GetH1());
				break;
			case E:
				Vec1.SetVec(aSector.GetX0(), aSector.GetY0(), aSector.GetH1());
				break;
			case S:
				Vec1.SetVec(aSector.GetX1(), aSector.GetY1(), aSector.GetH1());
				break;
			case FLOOR:
				Vec1.SetVec(aSector.GetX0(), aSector.GetY0(), aSector.GetH0());
				break;
			case CEILING:
				Vec1.SetVec(aSector.GetX0(), aSector.GetY0(), aSector.GetH1());
				break;
		}
		//ajustador como BSC porque vieram dos setores
		Vec1.SetSpace(BSC);
		return Vec1;
	}
	//---------------------------------------------------------------------------------------
	//

	inline VAC_Shape *
	Planify(VAC_Face &aFace) {
		/*
		int counter = 0;
		VAC_Shape *NewShape = RequestShape();
		NewShape->SetShape(aFace.iPoints.TotalItems(), aFace.Hint());
		BZK_Vec3f *point;
		BZK_FixP tmp;
		for (counter = 0; counter < aFace.iPoints.TotalItems(); counter++) {
			point = aFace.GetPoint(counter);

			if (point->GetX() > 0 && point->GetZ()>point->GetX())
				{
				DisposeFace(point);
				continue;
				}
			if (point->GetX() < 0 && point->GetZ()<point->GetX())
				{
				DisposeFace(point);
				continue;
				}

		#ifdef BZK_USE_OPENGL
			//NewShape->Z=-BZK_FastMath::Fix32toReal32(point->GetZ());
			//NewShape->Point(counter)->SetX(point->GetX());
			//NewShape->Point(counter)->SetY(point->GetY());

			NewShape->Point(counter)->SetX(BZK_FastMath::Div(point->GetX(), point->GetZ()));
			NewShape->Point(counter)->SetY(BZK_FastMath::Div(point->GetY(), point->GetZ()));
		#else
			NewShape->Point(counter)->SetX(BZK_FastMath::Div(point->GetX(), point->GetZ()));
			NewShape->Point(counter)->SetY(BZK_FastMath::Div(point->GetY(), point->GetZ()));
		#endif
		//	cout <<"Z:"<< NewShape->Z <<endl;
			NewShape->Point(counter)->SetX(HALFX + BZK_FastMath::Mul(RESX, NewShape->Point(counter)->GetX()));
			NewShape->Point(counter)->SetY(HALFY - BZK_FastMath::Mul(RESY, NewShape->Point(counter)->GetY()));
		}

		NewShape->iFill.Copy(aFace.iFill);
		NewShape->iOutLine.Copy(aFace.iOutLine);
		return NewShape;

		 */
		//////////////////////////////////////////////////////////////////////////////////
#ifdef BZK_USE_OPENGL

		VAC_Shape *NewShape = RequestShape();
		NewShape->SetShape(aFace.iPoints.TotalItems(), aFace.Hint());
		int counter = 0;
		BZK_Vec3f *point;
		BZK_FixP tmp;
		float r, g, b, a;
		r = BZK_FastMath::Fix32toReal32(aFace.iFill.GetR()) / 255.0;
		g = BZK_FastMath::Fix32toReal32(aFace.iFill.GetG()) / 255.0;
		b = BZK_FastMath::Fix32toReal32(aFace.iFill.GetB()) / 255.0;
		a = BZK_FastMath::Fix32toReal32(aFace.iFill.GetA()) / 255.0;
		glColor4f(r, g, b, a);
		glBegin(GL_POLYGON);
		for (counter = 0; counter < aFace.iPoints.TotalItems(); counter++) {
			point = aFace.GetPoint(counter);
			glVertex3f(BZK_FastMath::Fix32toReal32(point->GetX()), BZK_FastMath::Fix32toReal32(point->GetY()), -BZK_FastMath::Fix32toReal32(point->GetZ()));


		}
		glEnd();
		NewShape->iFill.Copy(aFace.iFill);
		NewShape->iOutLine.Copy(aFace.iOutLine);
	NewShape->iHint=aFace.iHint;
		return NewShape;
#else
		/////////////////////////////////////////////////////////////////////////////////////////////////

		int counter = aFace.iPoints.TotalItems();
		BZK_Vec3f *point;
		BZK_Vec2f *point2d;
		BZK_FixP OneOverZ;
		BZK_FixP X;
		BZK_FixP Y;
		BZK_FixP Z;
		VAC_Shape *NewShape = RequestShape();

		NewShape->SetShape(counter, aFace.Hint());
		counter--;
		for (; counter >= 0; counter--) {
			point = aFace.GetPoint(counter);
			point2d = NewShape->Point(counter);
			Z = point->GetZ();
			OneOverZ = BZK_FastMath::Div(ONE, Z);
			X = BZK_FastMath::Mul(point->GetX(), RESX);
			Y = BZK_FastMath::Mul(point->GetY(), RESY);
			X = BZK_FastMath::Mul(X, OneOverZ);
			Y = BZK_FastMath::Mul(Y, OneOverZ);
			point2d->SetX(HALFX + X);
			point2d->SetY(HALFY - Y);
		}

		NewShape->iFill.Copy(aFace.iFill);

		NewShape->iOutLine.Copy(aFace.iOutLine);
//	cout << "Z:"<<BZK_FastMath::Fix32toReal32(Z) <<endl;
//	NewShape->iOutLine.SetA(BZK_FastMath::Mul(NewShape->iOutLine.GetA(),Z));
		return NewShape;
#endif
	}

	//---------------------------------------------------------------------------------------

	inline bool
	AddToImageFinal(VAC_Face *aFace, int aCandelas) {
		int result = InsideFrustum(*aFace);
		if (result != POLYGON_TOTALLY_OUT) //ja faz o culling;
		{
			if (aCandelas >= 255) aCandelas = 254;
			BZK_FixP light = BZK_FastMath::Div(BZK_FastMath::Int32toFix32(aCandelas), FIX_0xFF);
			aFace->iFill.SetColor(BZK_FastMath::Mul(aFace->iFill.GetR(), light),
								  BZK_FastMath::Mul(aFace->iFill.GetG(), light),
								  BZK_FastMath::Mul(aFace->iFill.GetB(), light),
								  aFace->iFill.GetA()
			);
			aFace->iOutLine.SetColor(light, light, light, BZK_FastMath::Int32toFix32((255 - aCandelas) / 8));
			//	     aFace->iOutLine.SetColor(0,0,0,255);
			if (result == POLYGON_TOTALLY_IN)
				iFinalBuffer.Push(aFace);
			else
				iFinalBuffer.Push(Cull(aFace));
			return true;
		} else
			DisposeFace(aFace);
		return false;
	}

	//---------------------------------------------------------------------------------------
/*
    inline void
    AddToImage(VAC_Face *aFace) {
	int result=InsideFrustum(*aFace);
        if (result!=POLYGON_TOTALLY_OUT) //ja faz o culling;
        {

	    if (result==POLYGON_TOTALLY_IN)
	            iDepthBuffer.Push(aFace);
		else
	            iDepthBuffer.Push(Cull(aFace));
	}
        else
            DisposeFace(aFace);

    }
*/
	//---------------------------------------------------------------------------------------

	inline void
	AddToImageFinal(VAC_Face *aFace) {
		int result = InsideFrustum(*aFace);
		if (result != POLYGON_TOTALLY_OUT) //ja faz o culling;
		{
			if (result == POLYGON_TOTALLY_IN)
				iFinalBuffer.Push(aFace);
			else
				iFinalBuffer.Push(Cull(aFace));
		} else
			DisposeFace(aFace);
	}
	//---------------------------------------------------------------------------------------

	void
	AddToImage(VAC_PolyBuffer &aFaces, BZK_Vec3f aPosition, BZK_FixP aAngle) {
		if (aAngle == FIX_AngleLimit)
			aAngle = aAngle - FIX_AngleLimit;

		BZK_VecOps::Negate(aPosition);
		BZK_VecOps::LHCtoBSC(aPosition);
		VAC_Face *handler;
		VAC_Face copythat;
		int qtd = aFaces.TotalItems();
		int face;
		int result;
		//   VAC_PolyBuffer tmp;
		BZK_Vec3f handler2;
		FlushToSurface();
		//while (!aFaces.Empty())
		for (int fc = 0; fc < qtd; fc++) {
			SaveContext();
			LoadCannonical();
			SetAngle(-aAngle);
			handler = aFaces[fc];//.Pop();
			for (int face = handler->GetTotalPoints() - 1; face >= 0; face--) {
				BZK_VecOps::Copy(handler2, *handler->GetPoint(face));
				copythat.AddPoint(&handler2);
			}

			copythat.iFill.SetColor(handler->iFill.GetR(), handler->iFill.GetG(), handler->iFill.GetB(),
									handler->iFill.GetA());
			copythat.iOutLine.SetColor(handler->iOutLine.GetR(), handler->iOutLine.GetG(), handler->iOutLine.GetB(),
									   handler->iOutLine.GetA());
			copythat.iHint = NOPE;
			//     copythat.DoNormal();
			Transform(copythat);
			RestoreContext();
			SetAngle(iAngle);
			SaveContext();
			Translate(aPosition);
			Transform(copythat);
			result = InsideFrustum(copythat);

			if (result == POLYGON_PARTIALLY_IN) DrawPolygon(Planify(*CullNoDeleteOriginal(&copythat)));
			if (result == POLYGON_TOTALLY_IN) DrawPolygon(Planify(copythat));

			copythat.Reset();
			copythat.iPoints.ReleaseAll();
			//    tmp.Push(handler);
			RestoreContext();
		}
		//  while (!tmp.Empty()) aFaces.Push(tmp.Pop());
	}
	//---------------------------------------------------------------------------------------
	///TODO: make the ordering optional (sort of a poor's man LOD)

	void
	AddToImage(VAC_PolyBuffer &aFaces, BZK_Vec3f aPosition, BZK_FixP aAngle, int aCandelas, int aLod = LOD_FAR) {
		//	cout << "adicionando "<<aFaces.iResourceName <<endl;
		if (aCandelas >= 255) aCandelas = 254;
		BZK_VecOps::Negate(aPosition);
		BZK_VecOps::LHCtoBSC(aPosition);
		VAC_Face *handler;
		VAC_Face copythat;
		int qtd = aFaces.TotalItems();
		int *vect = (int *) malloc(qtd * sizeof(int));
		BZK_FixP *dist = (BZK_FixP *) malloc(qtd * sizeof(BZK_FixP));
		VAC_Shape **faces = (VAC_Shape **) malloc(qtd * sizeof(VAC_Shape *));
		int max;
		int result;
		int head = 0;
		BZK_FixP light = BZK_FastMath::Div(aCandelas, FIX_0xFF);
		//    VAC_PolyBuffer tmp;
		BZK_Vector<VAC_Face *> _tmp;
		FlushToSurface();
		for (int fc = 0; fc < qtd; fc++)
			//    while (!aFaces.Empty())
		{
			//// needed for rendering - set the context//////////////
			SaveContext();
			LoadCannonical();
			SetAngle(-aAngle);
			/////////////////////////////////////////////////////////

			handler = aFaces[fc];//.Pop();

			//////////////////copy the verteces//////////////////////////
			max = handler->GetTotalPoints();
			if (max > MAX_VERTEX_INDEX)
				max = MAX_VERTEX_INDEX;
			for (int face = max - 1; face >= 0; face--) {
				BZK_VecOps::Copy(vex[face], *handler->GetPoint(face));
				copythat.AddRefPoint(&vex[face]);
			}
			///////////////////////////////////////////////////////////////




			////////////////// transform face's copy//////////////////////////
			copythat.iHint = NOPE;
			Transform(copythat);
			RestoreContext();
			SetAngle(iAngle);
			SaveContext();
			Translate(aPosition);
			Transform(copythat);
			//////////////////////////////////////////////////////////////////////
			result = InsideFrustum(copythat);

			if (result != POLYGON_TOTALLY_OUT) {
				vect[head] = head;
				dist[head] = copythat.GetPoint(0)->GetZ();
				if (aLod == LOD_NEARER)
					for (int l = 1; l < max; l++)
						dist[head] += copythat.GetPoint(l)->GetZ();
				//////////////copy fill colour//////////////////////////////////
				copythat.iFill.SetColor(BZK_FastMath::Mul(handler->iFill.GetR(), light),
										BZK_FastMath::Mul(handler->iFill.GetG(), light),
										BZK_FastMath::Mul(handler->iFill.GetB(), light),
										handler->iFill.GetA()
				);
				copythat.iOutLine.SetColor(handler->iOutLine.GetR(), handler->iOutLine.GetG(), handler->iOutLine.GetB(),
										   handler->iOutLine.GetA());
				/////////////////////////////////////////////////////////////////
				/////////// calculate the normal vector /////////////////////////
				//copythat.DoNormal();
				if (result == POLYGON_PARTIALLY_IN)
					faces[head] = Planify(*CullNoDeleteOriginal(&copythat));
				else
					faces[head] = Planify(copythat);
				head++;
			}

			copythat.iPoints.ReleaseAll();
			// tmp.Push(handler);

			RestoreContext();
		}

		if (aLod != LOD_FAR)
			BZK_FastMath::Order(&dist, &vect, head);

		if (head > 0)
			for (int c = head - 1; c >= 0; c--)
				DrawPolygon(faces[vect[c]]);

		free(faces);
		free(dist);
		free(vect);

		//   while (!tmp.Empty()) aFaces.Push(tmp.Pop());
	}


	//---------------------------------------------------------------------------------------

	virtual void Update(BZK_Bitmap *aSurface) {
		if (renderBusy)
			return;

		renderBusy = true;
		FlushToSurface();
		VAC_Rasterer::Update(aSurface);
		renderBusy = false;
	}
	//---------------------------------------------------------------------------------------

	void
	FlushToSurface() {
		VAC_Shape *NewShape;
		VAC_Face *aFace;
		while (!iFinalBuffer.Empty()) {
			aFace = (iFinalBuffer.Pop());
			NewShape = Planify(*aFace);
			DisposeFace(aFace);
			DrawPolygon(NewShape);
		}
	}

	//---------------------------------------------------------------------------------------

	VAC_Face *Cull(VAC_Face *aFace) {

		VAC_Face *other[2];
		other[0] = aFace;
		other[1] = CullNoDeleteOriginal(aFace);
		delete other[0];
		return other[1];
	}

	VAC_Face *CullNoDeleteOriginal(VAC_Face *aFace) {
		/*
			VAC_Face * other[7];

			other[0] = aFace;
			other[1] = CullXNoMoreThan(iFrustum.GetX0(), other[0]);
		if (other[1]->GetTotalPoints()<3)
			return other[1];
			other[2] = CullXNoLessThan(iFrustum.GetX1(), other[1]);
			delete other[1];
		if (other[2]->GetTotalPoints()<3)
			return other[2];
			other[3] = CullYNoMoreThan(iFrustum.GetH1(), other[2]);
			delete other[2];
		if (other[3]->GetTotalPoints()<3)
			return other[3];
			other[4] = CullYNoLessThan(iFrustum.GetH0(), other[3]);
			delete other[3];
		if (other[4]->GetTotalPoints()<3)
			return other[4];
			other[5] = CullZNoMoreThan(iFrustum.GetY0(), other[4]);
			delete other[4];
		if (other[5]->GetTotalPoints()<3)
			return other[5];

		other[5]=aFace;
			other[6] = CullZNoLessThan(iFrustum.GetY1(), other[5]);
			delete other[5];

			return other[6];
		*/
		if (aFace->GetTotalPoints() < 3)
			return aFace;
		return CullZNoLessThan(iFrustum.GetY1(), aFace);
	}

	int
	InsideFrustum(VAC_Face &aFace) {
		int c;
		int value;
		int qtd = aFace.iPoints.TotalItems();
		int in = 0;
		int out = 0;

		for (c = qtd - 1; c >= 0; c--) {
			value = iFrustum.WhereLHC(*aFace.iPoints[c]);
			if (value == 100)
				in++;
			else
				out++;
		}

		if (out == 0)
			return POLYGON_TOTALLY_IN;

		if (in == 0)
			return POLYGON_TOTALLY_OUT;

		return POLYGON_PARTIALLY_IN;

	}
	//---------------------------------------------------------------------------------------
	//que tal passar isso pra dentro de volumétric sector?

	bool Inside(BZK_Vec3f &aVec) {
		return (aVec.GetZ() > iNearClippingPlane) && (aVec.GetZ() < iFarClippingPlane);
	}
	//---------------------------------------------------------------------------------------

	bool
	Inside(VAC_Face &aFace) {
		for (int c = aFace.iPoints.TotalItems() - 1; c >= 0; c--)
			if ((aFace.iPoints[c]->GetZ() > iNearClippingPlane) &&
				(aFace.iPoints[c]->GetZ() < iFarClippingPlane))
				return true;
		return false;
	}

	VAC_Face *CullXNoMoreThan(BZK_FixP aSpan, VAC_Face *aSrc) {
		VAC_Face *pa = RequestFace();
		int qtd = aSrc->iPoints.TotalItems();
		if (qtd == 0) return pa;
		bool lastinside = (aSrc->iPoints[qtd - 1]->GetX() < aSpan);
		BZK_FixP DX, DZ;
		int prev;
		BZK_Vec3f *prev_ptr;
		BZK_Vec3f *curr_ptr;
		BZK_FixP a2, b1, b2;
		BZK_Vec3f *other;

		for (int c = 0; c < qtd; c++) {
			curr_ptr = aSrc->iPoints[c];
			if (curr_ptr->GetX() < aSpan) {


				if (!lastinside) {

					prev = c - 1;
					while (prev < 0) prev += qtd;
					prev = prev % qtd;
					prev_ptr = aSrc->iPoints[prev];

					a2 = curr_ptr->GetX() - prev_ptr->GetX();
					b1 = curr_ptr->GetX() - aSpan;
					b2 = curr_ptr->GetZ() - prev_ptr->GetZ();
					DZ = curr_ptr->GetZ() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);


					a2 = curr_ptr->GetX() - prev_ptr->GetX();
					b1 = curr_ptr->GetX() - aSpan;
					b2 = curr_ptr->GetY() - prev_ptr->GetY();
					DX = curr_ptr->GetY() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);
					other = (new BZK_Vec3f(aSpan, DX, DZ));
					pa->iPoints.Add(other);
					lastinside = true;
				}

				other = new BZK_Vec3f(*aSrc->iPoints[c]);
				pa->iPoints.Add(other);
			} else {
				if (lastinside) {
					prev = c - 1;

					while (prev < 0) prev += qtd;
					prev = prev % qtd;
					prev_ptr = aSrc->iPoints[prev];

					a2 = curr_ptr->GetX() - prev_ptr->GetX();
					b1 = curr_ptr->GetX() - aSpan;
					b2 = curr_ptr->GetZ() - prev_ptr->GetZ();
					DZ = curr_ptr->GetZ() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);

					a2 = curr_ptr->GetX() - prev_ptr->GetX();
					b1 = curr_ptr->GetX() - aSpan;
					b2 = curr_ptr->GetY() - prev_ptr->GetY();
					DX = curr_ptr->GetY() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);
					other = (new BZK_Vec3f(aSpan, DX, DZ));
					pa->iPoints.Add(other);
					lastinside = false;
				}
			}
		}
		pa->iFill.Copy(aSrc->iFill);
		pa->iOutLine.Copy(aSrc->iOutLine);
		return pa;

	}

	VAC_Face *CullXNoLessThan(BZK_FixP aSpan, VAC_Face *aSrc) {
		VAC_Face *pa = RequestFace();
		int qtd = aSrc->iPoints.TotalItems();
		if (qtd == 0) return pa;
		bool lastinside = (aSrc->iPoints[qtd - 1]->GetX() > aSpan);
		BZK_FixP DX, DZ;
		int prev;
		BZK_Vec3f *prev_ptr;
		BZK_Vec3f *curr_ptr;
		BZK_Vec3f *other;
		BZK_FixP a2, b1, b2;

		for (int c = 0; c < qtd; c++) {
			curr_ptr = aSrc->iPoints[c];
			if (curr_ptr->GetX() > aSpan) {


				if (!lastinside) {

					prev = c - 1;
					while (prev < 0) prev += qtd;
					prev = prev % qtd;
					prev_ptr = aSrc->iPoints[prev];

					a2 = curr_ptr->GetX() - prev_ptr->GetX();
					b1 = curr_ptr->GetX() - aSpan;
					b2 = curr_ptr->GetZ() - prev_ptr->GetZ();
					DZ = curr_ptr->GetZ() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);


					a2 = curr_ptr->GetX() - prev_ptr->GetX();
					b1 = curr_ptr->GetX() - aSpan;
					b2 = curr_ptr->GetY() - prev_ptr->GetY();
					DX = curr_ptr->GetY() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);
					other = (new BZK_Vec3f(aSpan, DX, DZ));
					pa->iPoints.Add(other);
					lastinside = true;
				}

				other = new BZK_Vec3f(*aSrc->iPoints[c]);
				pa->iPoints.Add(other);
			} else {
				if (lastinside) {
					prev = c - 1;

					while (prev < 0) prev += qtd;
					prev = prev % qtd;
					prev_ptr = aSrc->iPoints[prev];

					a2 = curr_ptr->GetX() - prev_ptr->GetX();
					b1 = curr_ptr->GetX() - aSpan;
					b2 = curr_ptr->GetZ() - prev_ptr->GetZ();
					DZ = curr_ptr->GetZ() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);

					a2 = curr_ptr->GetX() - prev_ptr->GetX();
					b1 = curr_ptr->GetX() - aSpan;
					b2 = curr_ptr->GetY() - prev_ptr->GetY();
					DX = curr_ptr->GetY() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);
					other = (new BZK_Vec3f(aSpan, DX, DZ));
					pa->iPoints.Add(other);
					lastinside = false;
				}
			}
		}
		pa->iFill.Copy(aSrc->iFill);
		pa->iOutLine.Copy(aSrc->iOutLine);
		return pa;

	}

	VAC_Face *CullYNoMoreThan(BZK_FixP aSpan, VAC_Face *aSrc) {
		VAC_Face *pa = RequestFace();
		int qtd = aSrc->iPoints.TotalItems();
		if (qtd == 0) return pa;
		bool lastinside = (aSrc->iPoints[qtd - 1]->GetY() < aSpan);
		BZK_FixP DY, DZ;
		int prev;
		BZK_Vec3f *prev_ptr;
		BZK_Vec3f *curr_ptr;
		BZK_FixP a2, b1, b2;
		BZK_Vec3f *other;

		for (int c = 0; c < qtd; c++) {
			curr_ptr = aSrc->iPoints[c];
			if (curr_ptr->GetY() < aSpan) {


				if (!lastinside) {

					prev = c - 1;
					while (prev < 0) prev += qtd;
					prev = prev % qtd;
					prev_ptr = aSrc->iPoints[prev];

					a2 = curr_ptr->GetY() - prev_ptr->GetY();
					b1 = curr_ptr->GetY() - aSpan;
					b2 = curr_ptr->GetZ() - prev_ptr->GetZ();
					DZ = curr_ptr->GetZ() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);

					a2 = curr_ptr->GetY() - prev_ptr->GetY();
					b1 = curr_ptr->GetY() - aSpan;
					b2 = curr_ptr->GetX() - prev_ptr->GetX();
					DY = curr_ptr->GetX() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);

					other = (new BZK_Vec3f(DY, aSpan, DZ));
					pa->iPoints.Add(other);
					lastinside = true;
				}

				other = new BZK_Vec3f(*aSrc->iPoints[c]);
				pa->iPoints.Add(other);
			} else {
				if (lastinside) {
					prev = c - 1;

					while (prev < 0) prev += qtd;
					prev = prev % qtd;
					prev_ptr = aSrc->iPoints[prev];

					a2 = curr_ptr->GetY() - prev_ptr->GetY();
					b1 = curr_ptr->GetY() - aSpan;
					b2 = curr_ptr->GetZ() - prev_ptr->GetZ();
					DZ = curr_ptr->GetZ() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);

					a2 = curr_ptr->GetY() - prev_ptr->GetY();
					b1 = curr_ptr->GetY() - aSpan;
					b2 = curr_ptr->GetX() - prev_ptr->GetX();
					DY = curr_ptr->GetX() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);
					other = (new BZK_Vec3f(DY, aSpan, DZ));
					pa->iPoints.Add(other);
					lastinside = false;
				}
			}
		}
		pa->iFill.Copy(aSrc->iFill);
		pa->iOutLine.Copy(aSrc->iOutLine);
		return pa;

	}

	VAC_Face *CullYNoLessThan(BZK_FixP aSpan, VAC_Face *aSrc) {
		VAC_Face *pa = RequestFace();
		int qtd = aSrc->iPoints.TotalItems();
		if (qtd == 0) return pa;
		bool lastinside = (aSrc->iPoints[qtd - 1]->GetY() > aSpan);
		BZK_FixP DY, DZ;
		int prev;
		BZK_FixP a2, b1, b2;
		BZK_Vec3f *prev_ptr;
		BZK_Vec3f *curr_ptr;
		BZK_Vec3f *other;

		for (int c = 0; c < qtd; c++) {
			curr_ptr = aSrc->iPoints[c];
			if (curr_ptr->GetY() > aSpan) {


				if (!lastinside) {

					prev = c - 1;
					while (prev < 0) prev += qtd;
					prev = prev % qtd;
					prev_ptr = aSrc->iPoints[prev];

					a2 = curr_ptr->GetY() - prev_ptr->GetY();
					b1 = curr_ptr->GetY() - aSpan;
					b2 = curr_ptr->GetZ() - prev_ptr->GetZ();
					DZ = curr_ptr->GetZ() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);


					a2 = curr_ptr->GetY() - prev_ptr->GetY();
					b1 = curr_ptr->GetY() - aSpan;
					b2 = curr_ptr->GetX() - prev_ptr->GetX();
					DY = curr_ptr->GetX() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);
					other = (new BZK_Vec3f(DY, aSpan, DZ));
					pa->iPoints.Add(other);
					lastinside = true;
				}

				other = new BZK_Vec3f(*aSrc->iPoints[c]);
				pa->iPoints.Add(other);
			} else {
				if (lastinside) {
					prev = c - 1;

					while (prev < 0) prev += qtd;
					prev = prev % qtd;
					prev_ptr = aSrc->iPoints[prev];

					a2 = curr_ptr->GetY() - prev_ptr->GetY();
					b1 = curr_ptr->GetY() - aSpan;
					b2 = curr_ptr->GetZ() - prev_ptr->GetZ();
					DZ = curr_ptr->GetZ() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);

					a2 = curr_ptr->GetY() - prev_ptr->GetY();
					b1 = curr_ptr->GetY() - aSpan;
					b2 = curr_ptr->GetX() - prev_ptr->GetX();
					DY = curr_ptr->GetX() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);
					other = (new BZK_Vec3f(DY, aSpan, DZ));
					pa->iPoints.Add(other);
					lastinside = false;
				}
			}
		}
		pa->iFill.Copy(aSrc->iFill);
		pa->iOutLine.Copy(aSrc->iOutLine);
		return pa;

	}

	VAC_Face *CullZNoMoreThan(BZK_FixP aSpan, VAC_Face *aSrc) {
		VAC_Face *pa = RequestFace();
		int qtd = aSrc->iPoints.TotalItems();
		if (qtd == 0) return pa;
		bool lastinside = (aSrc->iPoints[qtd - 1]->GetZ() < aSpan);
		BZK_FixP DZ, DY;
		int prev;
		BZK_Vec3f *prev_ptr;
		BZK_Vec3f *curr_ptr;
		BZK_FixP a2, b1, b2;
		BZK_Vec3f *other;

		for (int c = 0; c < qtd; c++) {
			curr_ptr = aSrc->iPoints[c];
			if (curr_ptr->GetZ() < aSpan) {


				if (!lastinside) {

					prev = c - 1;
					while (prev < 0) prev += qtd;
					prev = prev % qtd;
					prev_ptr = aSrc->iPoints[prev];

					a2 = curr_ptr->GetZ() - prev_ptr->GetZ();
					b1 = curr_ptr->GetZ() - aSpan;
					b2 = curr_ptr->GetY() - prev_ptr->GetY();
					DY = curr_ptr->GetY() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);


					a2 = curr_ptr->GetZ() - prev_ptr->GetZ();
					b1 = curr_ptr->GetZ() - aSpan;
					b2 = curr_ptr->GetX() - prev_ptr->GetX();
					DZ = curr_ptr->GetX() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);
					other = (new BZK_Vec3f(DZ, DY, aSpan));
					pa->iPoints.Add(other);
					lastinside = true;
				}

				other = new BZK_Vec3f(*aSrc->iPoints[c]);
				pa->iPoints.Add(other);
			} else {
				if (lastinside) {
					prev = c - 1;

					while (prev < 0) prev += qtd;
					prev = prev % qtd;
					prev_ptr = aSrc->iPoints[prev];

					a2 = curr_ptr->GetZ() - prev_ptr->GetZ();
					b1 = curr_ptr->GetZ() - aSpan;
					b2 = curr_ptr->GetY() - prev_ptr->GetY();
					DY = curr_ptr->GetY() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);


					a2 = curr_ptr->GetZ() - prev_ptr->GetZ();
					b1 = curr_ptr->GetZ() - aSpan;
					b2 = curr_ptr->GetX() - prev_ptr->GetX();
					DZ = curr_ptr->GetX() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);
					other = (new BZK_Vec3f(DZ, DY, aSpan));
					pa->iPoints.Add(other);
					lastinside = false;
				}
			}
		}
		pa->iFill.Copy(aSrc->iFill);
		pa->iOutLine.Copy(aSrc->iOutLine);
		return pa;

	}

	VAC_Face *CullZNoLessThan(BZK_FixP aSpan, VAC_Face *aSrc) {
		VAC_Face *pa = RequestFace();
		int qtd = aSrc->iPoints.TotalItems();
		if (qtd == 0) return pa;
		bool lastinside = (aSrc->iPoints[qtd - 1]->GetZ() > aSpan);
		BZK_FixP DZ, DY;
		int prev;
		BZK_FixP a2, b1, b2;
		BZK_Vec3f *prev_ptr;
		BZK_Vec3f *curr_ptr;
		BZK_Vec3f *other;

		for (int c = 0; c < qtd; c++) {
			curr_ptr = aSrc->iPoints[c];
			if (curr_ptr->GetZ() > aSpan) {


				if (!lastinside) {

					prev = c - 1;
					while (prev < 0) prev += qtd;
					prev = prev % qtd;
					prev_ptr = aSrc->iPoints[prev];
					a2 = curr_ptr->GetZ() - prev_ptr->GetZ();
					b1 = curr_ptr->GetZ() - aSpan;
					b2 = curr_ptr->GetY() - prev_ptr->GetY();
					DY = curr_ptr->GetY() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);
					a2 = curr_ptr->GetZ() - prev_ptr->GetZ();
					b1 = curr_ptr->GetZ() - aSpan;
					b2 = curr_ptr->GetX() - prev_ptr->GetX();
					DZ = curr_ptr->GetX() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);
					other = (new BZK_Vec3f(DZ, DY, aSpan));
					pa->iPoints.Add(other);
					lastinside = true;
				}

				other = new BZK_Vec3f(*aSrc->iPoints[c]);
				pa->iPoints.Add(other);
			} else {
				if (lastinside) {
					prev = c - 1;

					while (prev < 0) prev += qtd;
					prev = prev % qtd;
					prev_ptr = aSrc->iPoints[prev];
					{
						BZK_FixP a2, b1, b2;
						a2 = curr_ptr->GetZ() - prev_ptr->GetZ();
						b1 = curr_ptr->GetZ() - aSpan;
						b2 = curr_ptr->GetY() - prev_ptr->GetY();
						DY = curr_ptr->GetY() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);
					}
					BZK_FixP a2, b1, b2;
					a2 = curr_ptr->GetZ() - prev_ptr->GetZ();
					b1 = curr_ptr->GetZ() - aSpan;
					b2 = curr_ptr->GetX() - prev_ptr->GetX();
					DZ = curr_ptr->GetX() - BZK_FastMath::Div(BZK_FastMath::Mul(b1, b2), a2);
					other = (new BZK_Vec3f(DZ, DY, aSpan));
					pa->iPoints.Add(other);
					lastinside = false;
				}
			}
		}
		pa->iFill.Copy(aSrc->iFill);
		pa->iOutLine.Copy(aSrc->iOutLine);
		return pa;

	}
};
