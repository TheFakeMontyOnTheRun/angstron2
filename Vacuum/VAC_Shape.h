//primitiva 2D geometrica


class VAC_Shape {
public:
	BZK_Vector<BZK_Vec2f *> iPoint;
	enum {
		POLYGON, CURVE, ELLIPSE
	};
	int iType;
	//  BZK_RawColor iOutLine;
	//  BZK_RawColor iFill;
	VAC_ColorRGBA iOutLine;
	VAC_ColorRGBA iFill;
	BZK_Rect iClipArea;
	int iHint;

	inline int GetNumberOfPoints() { return iPoint.TotalItems(); }

#ifdef BZK_USE_OPENGL
	float Z;
#endif

	BZK_Vec2f *Point(int aIndex) {

		return iPoint[aIndex];
	}

	VAC_Shape *Clone() {
		return Clone(new VAC_Shape());
	}


	VAC_Shape *Clone(VAC_Shape *toReturn) {
		memcpy(&(toReturn->iOutLine), &iOutLine, sizeof(VAC_ColorRGBA));
		memcpy(&(toReturn->iFill), &iFill, sizeof(VAC_ColorRGBA));
		memcpy(&(toReturn->iClipArea), &iClipArea, sizeof(BZK_Rect));
		toReturn->iType = iType;
		toReturn->iHint = iHint;

		BZK_Vec2f *handler;
		for (int c = iPoint.TotalItems() - 1; c >= 0; c--) {
			handler = new BZK_Vec2f();
			BZK_VecOps::Copy(*handler, *Point(c));
			toReturn->iPoint.Add(handler);
		}
		return toReturn;

	}

	VAC_Face *toFace() {
		VAC_Face *toReturn = NULL;
		toReturn = new VAC_Face();

		if (!toReturn) {
			cout << "shape panic" << endl;
			exit(0);
		}

		BZK_Vec3f *handler2;

		for (int c = iPoint.TotalItems() - 1; c >= 0; c--) {
			handler2 = new BZK_Vec3f();
			if (!handler2) {
				cout << "other panic" << endl;
				exit(0);
			}
			///TODO: see what is this constant number all about
			handler2->SetX(Point(c)->GetX() / 400);
			handler2->SetY(-Point(c)->GetY() / 400);
			handler2->SetZ(0);
#ifdef BZK_USE_OPENGL
			handler2->SetZ(Z);
#endif
			toReturn->AddPoint(handler2);
			delete handler2;
		}
		//whats the sense of a billboard having normals?
		//  toReturn->DoNormal();
		memcpy(&toReturn->iFill, &iFill, sizeof(VAC_ColorRGBA));
		memcpy(&toReturn->iOutLine, &iOutLine, sizeof(VAC_ColorRGBA));
		return toReturn;

	}

	VAC_Shape() {
#ifdef BZK_USE_OPENGL
		Z=2;
#endif
		iFill.SetA(BZK_FastMath::Int32toFix32(255));
		iHint = 0;

	}

	VAC_Shape(int aNumberOfPoints, int aHint) {
		VAC_Shape();
		SetShape(aNumberOfPoints, aHint);

	}

	void SetShape(int aNumberOfPoints, int aHint) {
		int c;
		//      std:://cout << "VAC_Shape::VAC_Shape()";
		BZK_Vec2f *ptr;

		for (c = aNumberOfPoints; c > 0; c--) {
			ptr = new BZK_Vec2f();
			iPoint.Add(ptr);
		}

		iHint = aHint;
	}

	void ReleaseAll() {
		iPoint.ReleaseAll();
	}

/*
    BZK_LightRect *Box()
      {
	BZK_LightRect *Box=new BZK_LightRect();
	BZK_Vec2f *point=Point(0);
	BZK_Vec2f *menorx=point;
	BZK_Vec2f *menory=point;
	BZK_Vec2f *maiorx=point;
	BZK_Vec2f *maiory=point;
	int c;
	for (c=1;c<iPoint.TotalItems();c++)
	  {
	    point=Point(c);
	    if (point->iX>maiorx->iX) maiorx=point;
	    if (point->iX<menorx->iX) menorx=point;
	    if (point->iY>maiorx->iY) maiory=point;
	    if (point->iY>menorx->iY) menory=point;
	  }
	Box->x=BZK_FastMath::Fix32toInt32(menorx->iX);
	Box->y=BZK_FastMath::Fix32toInt32(menory->iY);
	Box->w=BZK_FastMath::Fix32toInt32(maiorx->iX)-Box->x;
	Box->h=BZK_FastMath::Fix32toInt32(maiory->iY)-Box->y;
	memcpy(&iClipArea,Box,sizeof(BZK_LightRect));
	return Box;	
      }
*/

	void Translate(BZK_Vec2f aTranslate) {

		int c;
		BZK_Vec2f *point;
		for (c = iPoint.TotalItems() - 1; c >= 0; c--) {
			point = iPoint[c];
			point->SetX(point->GetX() + aTranslate.GetX());
			point->SetY(point->GetY() + aTranslate.GetY());
		}

	}

	~VAC_Shape() {
		DeleteAllPoints();
	}

	void DeleteAllPoints() {
		for (int c = iPoint.TotalItems() - 1; c >= 0; c--) {
			delete Point(c);
			iPoint[c] = NULL;
		}
		ReleaseAll();
	}


};

