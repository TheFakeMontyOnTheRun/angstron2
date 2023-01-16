///@brief 4D vector. Never really used, but someone might like to use it.
///@author Daniel Monteiro

class BZK_Vec4f : public BZK_Vec3f {
public:
	BZK_FixP iW;

///@brief constructor
	BZK_Vec4f() : BZK_Vec3f(), iW(0) {
//     BZK_Vec3f();
		//   SetW(0);
	}

///@brief alternative constructor
	BZK_Vec4f(BZK_FixP aX, BZK_FixP aY, BZK_FixP aZ, BZK_FixP aW) : BZK_Vec3f(aX, aY, aZ), iW(aW) {
		//   BZK_Vec3f::BZK_Vec3f(aX,aY,aZ);
		//   iW=(aW);
	}

////////iW: getter and setter ////////////
	void SetW(BZK_Fix aW) {
		iW = aW;
	}

	BZK_FixP GetW() {
		return iW;
	}
////////////////////////////////////
};
