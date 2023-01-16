///@todo: make it more flexible, instread of just treating 4 points.
class VAC_Face {
public:
	int iMaterial;

	void DoNormal() {
		if (iPoints.TotalItems() > 2) {
			iNormal = BZK_VecOps::VecProduct(*iPoints[0], *iPoints[1]);
			BZK_VecOps::VecNormalize(*iNormal);
		}
	}


	//--------------------------------------------------------------------------------------------
	void AddPoint(BZK_Vec3f *aPoint) {
		BZK_Vec3f *NewVec;
		NewVec = new BZK_Vec3f();
		BZK_VecOps::Copy(*NewVec, *aPoint);
		iPoints.Add(NewVec);
	}

	//--------------------------------------------------------------------------------------------
	void AddRefPoint(BZK_Vec3f *aPoint) {
		iPoints.Add(aPoint);
	}

	//--------------------------------------------------------------------------------------------
	VAC_Face *Clone() {
		VAC_Face *newface = new VAC_Face(iPoints, iHint);
		memcpy(&(newface->iFill), &(iFill), sizeof(VAC_ColorRGBA));
		memcpy(&(newface->iOutLine), &(iOutLine), sizeof(VAC_ColorRGBA));
		newface->iMaterial = iMaterial;
		return newface;
	}


	//--------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------
	VAC_Face(BZK_Vector<BZK_Vec3f *> aPoints, int aHint) :
			iHint(aHint),
			iOutLine(BZK_FastMath::Int32toFix32(0), BZK_FastMath::Int32toFix32(0), BZK_FastMath::Int32toFix32(0),
					 BZK_FastMath::Int32toFix32(255)),
			iFill(BZK_FastMath::Int32toFix32(255), BZK_FastMath::Int32toFix32(255), BZK_FastMath::Int32toFix32(255),
				  BZK_FastMath::Int32toFix32(255)) {
		iMaterial = -1;;
		int c;
		for (c = 0; c < aPoints.TotalItems(); c++)
			AddPoint(aPoints[c]);


		iNormal = BZK_VecOps::VecProduct(*(iPoints)[1], *(iPoints)[2]);
		BZK_VecOps::VecNormalize(*iNormal);
/*
    iHint=aHint;    
    iOutLine.SetColor(0,0,0,255);
    iFill.SetColor(255,0,255,255);
*/
	}

	VAC_Face() : iNormal(NULL) {
		iMaterial = -1;;
	}

	VAC_Face(BZK_Vec3f *Point, BZK_Vec3f *Vec1, BZK_Vec3f *Vec2, BZK_Vec3f *Vec3, int aHint) {
		SetFace(Point, Vec1, Vec2, Vec3, aHint);
		iMaterial = -1;;
	}


	void SetFace(BZK_Vec3f *Point, BZK_Vec3f *Vec1, BZK_Vec3f *Vec2, BZK_Vec3f *Vec3, int aHint) {
		iForceRender = false;
		AddPoint(Point);
		AddPoint(Vec1);
		AddPoint(Vec2);
		AddPoint(Vec3);
		iMaterial = -1;;
		iHint = aHint;
		iNormal = BZK_VecOps::VecProduct(*Vec1, *Vec2);
		BZK_VecOps::VecNormalize(*iNormal);
		iOutLine.SetColor(0, 0, 0, 255);
		iFill.SetColor(255, 255, 255, 255);
	}

//--------------------------------------------------------------------------------------------
///@deprecated. I must do a more flexible check now.
	bool
	Check() {
		return (((iPoints)[0]->GetZ() < 0) || ((iPoints)[1]->GetZ() < 0) || ((iPoints)[2]->GetZ() < 0) ||
				((iPoints)[3]->GetZ() < 0));
	}

	void
	Reset() {
		int c;
		for (c = iPoints.TotalItems() - 1; c >= 0; c--) {
			delete GetPoint(c);
			iPoints[c] = NULL;
		}
		if (iNormal != NULL) {
			delete iNormal;
			iNormal = NULL;
		}
	}

	void
	ReleaseAll() {
		iPoints.ReleaseAll();
	}

//--------------------------------------------------------------------------------------------
	~VAC_Face() {
		Reset();

	}

//---------------------------------------------------------------------------------------------
	BZK_Vec3f *
	GetNormal() {
		//  BZK_VecOps::VecNormalize(*iNormal);
		return iNormal;
	}

	void
	SetNormal(BZK_Vec3f &aNormal) {
		BZK_Vec3f *NewVec;
		NewVec = new BZK_Vec3f();
		BZK_VecOps::Copy(*NewVec, aNormal);
		iNormal = (NewVec);
	}

//--------------------------------------------------------------------------------------------
	BZK_Vec3f *
	GetPoint(int aPoint) {
		//   if (aPoint>iPoints.TotalItems()) return NULL;
		return iPoints[aPoint];
	}
//--------------------------------------------------------------------------------------------
/*  
BZK_Vec3f*
    GetCenter()
    {
      BZK_Vec3f *center=new BZK_Vec3f(0,0,0);
      BZK_Vec3f *point;
      int c;
      	for (c=0;c<iPoints.TotalItems();c++)
	  {
	    point=GetPoint(c);
	    center->iX+=point->iX;
	    center->iY+=point->iY;
	    center->iZ+=point->iZ;	
	  }  
	BZK_FastMath::Div(center->iX,BZK_FastMath::Int32toFix32(iPoints.TotalItems()));
	BZK_FastMath::Div(center->iY,BZK_FastMath::Int32toFix32(iPoints.TotalItems()));
        BZK_FastMath::Div(center->iZ,BZK_FastMath::Int32toFix32(iPoints.TotalItems()));
      return center;
    }
//--------------------------------------------------------------------------------------------
  bool
    Behind(VAC_Face &aOther,BZK_Vec3f &acenter)
    {
      BZK_Vec3f center(0,0,0);
      BZK_Vec3f *MyCenter=GetCenter();
      BZK_Vec3f *OtherCenter=aOther.GetCenter();
      if ((BZK_VecOps::DistanceFix(*MyCenter,center))>=(BZK_VecOps::DistanceFix(*OtherCenter,center))) return 2; //ver...
      //      if ((BZK_VecOps::DistanceFix(*MyCenter,center))==(BZK_VecOps::DistanceFix(*OtherCenter,center))) 
      return 1;
    }
*/
//--------------------------------------------------------------------------------------------
	const int Hint() { return iHint; }
//--------------------------------------------------------------------------------------------

	BZK_VolumetricSector *Volume() {
		BZK_VolumetricSector *Volume = new BZK_VolumetricSector;
		BZK_Rect *Box = new BZK_Rect();
		BZK_Vec3f *point;//=GetPoint(0);
		/*
		BZK_Vec3f *menorx=point;
		BZK_Vec3f *menory=point;
		BZK_Vec3f *maiorx=point;
		BZK_Vec3f *maiory=point;
		BZK_Vec3f *maiorz=point;
		BZK_Vec3f *menorz=point;
		*/
		BZK_FixP maiorx = 0;
		BZK_FixP maiory = 0;
		BZK_FixP maiorz = 0;
		BZK_FixP menorx = BZK_FastMath::Int32toFix32(255);
		BZK_FixP menory = BZK_FastMath::Int32toFix32(255);
		BZK_FixP menorz = BZK_FastMath::Int32toFix32(255);
		int c;
		for (c = iPoints.TotalItems() - 1; c >= 0; c--) {
			point = GetPoint(c);
			/*
			if (BZK_FastMath::Fix32toInt32(point->GetX())>BZK_FastMath::Fix32toInt32(maiorx->GetX())) maiorx=point;
			if (BZK_FastMath::Fix32toInt32(point->GetX())<BZK_FastMath::Fix32toInt32(menorx->GetX())) menorx=point;
			if (BZK_FastMath::Fix32toInt32(point->GetY())>BZK_FastMath::Fix32toInt32(maiory->GetY())) maiory=point;
			if (BZK_FastMath::Fix32toInt32(point->GetY())>BZK_FastMath::Fix32toInt32(menory->GetY())) menory=point;
			if (BZK_FastMath::Fix32toInt32(point->GetZ())>BZK_FastMath::Fix32toInt32(maiorz->GetZ())) maiorz=point;
			if (BZK_FastMath::Fix32toInt32(point->GetZ())>BZK_FastMath::Fix32toInt32(menorz->GetZ())) menorz=point;
			*/
			if (point->GetX() > maiorx) maiorx = point->GetX();
			if (point->GetX() < menorx) menorx = point->GetX();
			if (point->GetY() > maiory) maiory = point->GetY();
			if (point->GetY() < menory) menory = point->GetY();
			if (point->GetZ() > maiorz) maiorz = point->GetZ();
			if (point->GetZ() < menorz) menorz = point->GetZ();
		}
		/*
		if (menorx<0) menorx=-menorx;
		if (maiorx<0) maiorx=-maiorx;
		if (menory<0) menory=-menory;
		if (maiory<0) maiory=-maiory;
		if (menorz<0) menorz=-menorz;
		if (maiorz<0) maiorz=-maiorz;
		*/
		Box->SetX0(menorx);
		Box->SetY0(menory);
		Box->SetW(maiorx - menorx);
		Box->SetH(maiory - menory);
		Volume->SetRect(*Box);
		Volume->SetH0(menorz);
		Volume->SetH1(maiorz);
		delete Box;
		return Volume;
	}
	/*
  //---------------------------------------------------------------------------------------

	void Scan()
	  {
	BZK_Vec3f *point=GetPoint(0);
	int c;
	for (c=1;c<iPoints.TotalItems();c++)
	  {
		point=GetPoint(c);
		if (BZK_FastMath::Fix32toInt32(point->iZ)>BZK_FastMath::Fix32toInt32(iFar->iZ)) iFar=point;
		if (BZK_FastMath::Fix32toInt32(point->iZ)>BZK_FastMath::Fix32toInt32(iNear->iZ)) iNear=point;
	  }

	  }
  //---------------------------------------------------------------------------------------
  */
	//---------------------------------------------------------------------------------------
	void CullX(BZK_FixP aSpan, int index) {
		GetPoint(index)->SetX(aSpan);

	}

	//---------------------------------------------------------------------------------------
	void CullY(BZK_FixP aSpan, int index) {
		GetPoint(index)->SetY(aSpan);
	}

	//---------------------------------------------------------------------------------------
	void CullZ(BZK_FixP aSpan, int index) {
		GetPoint(index)->SetZ(aSpan);
	}

	VAC_Face *FullCullZ(BZK_FixP aSpan, int index) {


		VAC_Face *pa, *pb;
		BZK_Vector<int> pointsinside;
		BZK_Vector<int> pointsoutside;
		bool lastinside = true;
		BZK_FixP DX;
		int next, prev;
		pa = new VAC_Face();
		for (int c = 0; c < iPoints.TotalItems(); c++)
			if (iPoints[c]->GetZ() > aSpan) {
				//	pointsinside.Add(c);


				if (!lastinside) {
					next = c + 1;
					next = next % iPoints.TotalItems();
					BZK_FixP a2, b1, b2;
					a2 = iPoints[next]->GetX() - iPoints[c]->GetX();
					b1 = iPoints[next]->GetZ() - aSpan;
					b2 = iPoints[next]->GetZ() - iPoints[c]->GetZ();
					DX = iPoints[next]->GetX() - BZK_FastMath::Mul(a2, BZK_FastMath::Div(b1, b2));
					pa->AddRefPoint(new BZK_Vec3f(DX, iPoints[c]->GetY(), aSpan));
					lastinside = true;
				}
				pa->AddPoint(iPoints[c]);

			} else {


				if (lastinside) {
					prev = c - 1;
					while (prev < 0) prev += iPoints.TotalItems();
					prev = prev % iPoints.TotalItems();
					cout << "prev:" << prev << endl;
					BZK_FixP a2, b1, b2;
					a2 = iPoints[prev]->GetX() - iPoints[c]->GetX();
					b1 = iPoints[prev]->GetZ() - aSpan;
					b2 = iPoints[prev]->GetZ() - iPoints[c]->GetZ();
					DX = iPoints[prev]->GetX() + BZK_FastMath::Mul(a2, BZK_FastMath::Div(b1, b2));
					pa->AddRefPoint(new BZK_Vec3f(DX, iPoints[c]->GetY(), aSpan));
					lastinside = false;
				}


			}
		//VAC_ColorRGBA cor(GetFill());
		VAC_ColorRGBA cor(255, 0, 0, 255);
		pa->SetFill(cor);
		pa->SetHint(Hint());
		return pa;

	}
//----------------------------------------------------------------------------------------

//--------
	void SetSpace(int aSpace) {
		GetPoint(0)->SetSpace(aSpace);
		GetPoint(1)->SetSpace(aSpace);
		GetPoint(2)->SetSpace(aSpace);
		GetPoint(3)->SetSpace(aSpace);
	}

//=============================================================================================
	VAC_ColorRGBA iOutLine;
	VAC_ColorRGBA iFill;
//  BZK_Vec3f *iNear;
//   BZK_Vec3f *iFar;
	BZK_Vector<BZK_Vec3f *> iPoints;
	BZK_Vec3f *iNormal;


	inline int GetTotalPoints() { return iPoints.TotalItems(); }

	BZK_FixP iDist;
	int iHint;
	bool iForceRender;

	void SetOutLine(VAC_ColorRGBA &aNewColor) { iOutLine.Copy(aNewColor); }

	VAC_ColorRGBA GetOutLine() { return iOutLine; }

	void SetFill(VAC_ColorRGBA &aNewColor) { iFill.Copy(aNewColor); }

	VAC_ColorRGBA GetFill() { return iFill; }

	void SetHint(int aHint) {
		iHint = aHint;
	}

	inline int GetHint() {
		return iHint;
	}
};
