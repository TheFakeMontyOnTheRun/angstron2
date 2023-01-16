///@brief Vector operations. Maybe not the most optimized code, but works (kind of)
class BZK_VecOps {
public:

///TODO: improve this to handle non-normalized vectors - or maybe rename this.
	inline static BZK_FixP Angle(BZK_Vec3f &aVec1, BZK_Vec3f &aVec2) {
		return DotProductFix(aVec1, aVec2);
	}

///TODO: understand and implement this properly
	inline static BZK_FixP AngleToNormal(BZK_Vec3f &aVec) {
		BZK_Vec3f North(0, 0, 1);
		return Angle(aVec, North);
	}

///copy to a 2D vector from another
	inline static void Copy(BZK_Vec2f &aVecDest, BZK_Vec2f &aVecSrc) {
		memcpy(&aVecDest, &aVecSrc, sizeof(BZK_Vec2f));
	}


///copy to a 3D vector from another
	inline static void Copy(BZK_Vec3f &aVecDest, BZK_Vec3f &aVecSrc) {
		memcpy(&aVecDest, &aVecSrc, sizeof(BZK_Vec3f));
	}

///return dimensions of a vector
///TODO: useful? clutter?
	inline
	static
	int
	Components(BZK_Vec2f *Vec) {
		return (sizeof(*Vec) / sizeof(BZK_FixP));
	}


	inline static BZK_FixP DotProductFix(BZK_Vec3f a, BZK_Vec3f b) {
		return BZK_FastMath::Mul(a.GetX(), b.GetX()) + BZK_FastMath::Mul(a.GetY(), b.GetY()) +
			   BZK_FastMath::Mul(a.GetZ(), b.GetZ());
	}

	inline static BZK_FixP DotProductFix(BZK_Vec2f a, BZK_Vec2f b) {
		return BZK_FastMath::Mul(a.GetX(), b.GetX()) + BZK_FastMath::Mul(a.GetY(), b.GetY());
	}

	inline static BZK_Vec3f *MakeVec(BZK_Vec3f a, BZK_Vec3f b) {
		BZK_Vec3f *NewVec = new BZK_Vec3f;
		NewVec->SetX(a.GetX() - b.GetX());
		NewVec->SetY(a.GetY() - b.GetY());
		NewVec->SetZ(a.GetZ() - b.GetZ());
		return NewVec;
	}


	inline static BZK_Vec3f *VecProduct(BZK_Vec3f a, BZK_Vec3f b) {
		BZK_FixP aX = a.GetX();
		BZK_FixP aY = a.GetY();
		BZK_FixP aZ = a.GetZ();

		BZK_FixP bX = b.GetX();
		BZK_FixP bY = b.GetY();
		BZK_FixP bZ = b.GetZ();


		BZK_Vec3f *NewVec = new BZK_Vec3f;
		NewVec->SetX(BZK_FastMath::Mul(aY, bZ) - BZK_FastMath::Mul(aZ, bY));
		NewVec->SetY(BZK_FastMath::Mul(aZ, bX) - BZK_FastMath::Mul(aX, bZ));
		NewVec->SetZ(BZK_FastMath::Mul(aX, bY) - BZK_FastMath::Mul(aY, bX));
		return NewVec;
	}


	inline static BZK_FixP DistanceFix(BZK_Vec3f &a, BZK_Vec3f &b) {
		BZK_FixP ToBeReturned;
		BZK_Vec3f *DistVec = MakeVec(a, b);
		ToBeReturned = LengthFix(*DistVec);
		delete DistVec;
		return ToBeReturned;
	}


	inline static BZK_FixP LengthFix(BZK_Vec3f a) {
		float ToBeReturned;
		ToBeReturned =/*sqrt*/BZK_FastMath::FastSqrt((int) BZK_FastMath::Fix32toInt32(DotProductFix(a, a)));
		return BZK_FastMath::Int32toFix32(ToBeReturned);
	}

	inline static BZK_FixP LengthFix(BZK_Vec2f a) {
		float ToBeReturned;
		ToBeReturned =/*sqrt*/BZK_FastMath::FastSqrt((int) BZK_FastMath::Fix32toInt32(DotProductFix(a, a)));
		return BZK_FastMath::Int32toFix32(ToBeReturned);
	}

	//---------------------------------------------------------------------------------------
	inline static void
	BSCtoLHC(BZK_Vec3f &aVec) {
		if (aVec.GetSpace() != BSC) return;
		BZK_Vec3f *temp = new BZK_Vec3f;
		BZK_VecOps::Copy(*temp, aVec);
		aVec.SetX(
				temp->GetX()); /// as origins swap from front to back, we have to invert X (what ever this means. I cant remember why I wrote this)
		aVec.SetY(temp->GetZ());
		aVec.SetZ(-temp->GetY());
		aVec.SetSpace(LHC);
		delete temp;
	}

	//---------------------------------------------------------------------------------------
	inline static void
	LHCtoBSC(BZK_Vec3f &aVec) {
		if (aVec.GetSpace() != LHC) return;
		BZK_Vec3f *temp = new BZK_Vec3f();
		BZK_VecOps::Copy(*temp, aVec);
		aVec.SetX(temp->GetX());  // idem BSCtoLHC
		aVec.SetY(-temp->GetZ());
		aVec.SetZ(temp->GetY());
		aVec.SetSpace(BSC);
		delete temp;
	}

	inline static void VecNormalize(BZK_Vec3f &Vec) {
		BZK_FixP Modulus = LengthFix(Vec);//+BZK_FastMath::Int32toFix32(1);
		Vec.SetX(BZK_FastMath::Div(Vec.GetX(), Modulus));
		Vec.SetY(BZK_FastMath::Div(Vec.GetY(), Modulus));
		Vec.SetZ(BZK_FastMath::Div(Vec.GetZ(), Modulus));
	}

	inline static void Negate(BZK_Vec3f &Vec) {
		/*
			BZK_FixP MinusOne=BZK_FastMath::Real32toFix32(-1.0);
		Vec.SetX(BZK_FastMath::Mul(Vec.GetX(),MinusOne));
		Vec.SetY(BZK_FastMath::Mul(Vec.GetY(),MinusOne));
		Vec.SetZ(BZK_FastMath::Mul(Vec.GetZ(),MinusOne));
		*/
		Vec.SetX(-Vec.GetX());
		Vec.SetY(-Vec.GetY());
		Vec.SetZ(-Vec.GetZ());

	}


	inline static void VecNormalize(BZK_Vec2f &Vec) {
		BZK_FixP Modulus = LengthFix(Vec) + BZK_FastMath::Int32toFix32(1);
		Vec.SetX(BZK_FastMath::Div(Vec.GetX(), Modulus));
		Vec.SetY(BZK_FastMath::Div(Vec.GetY(), Modulus));
	}

///@TODO: test
	inline static BZK_Vec3f *SumVec(BZK_Vec3f VecA, BZK_Vec3f VecB) {
		BZK_Vec3f *Sum = new BZK_Vec3f();
		if (VecA.GetSpace() == VecB.GetSpace()) {
			Sum->SetX(VecA.GetX() + VecB.GetX());
			Sum->SetY(VecA.GetY() + VecB.GetY());
			Sum->SetZ(VecA.GetZ() + VecB.GetZ());
		}
		return Sum;
	}


};
