/**
@brief four-dimensional color. Useful for both RGBA or CMYK (the later is mistery to me)
*/
class BZK_Color4f : public BZK_Color3f {
///////////////////////// functions /////////////////////////////


public:
///@brief class's constructor
	BZK_Color4f() : BZK_Color3f(0, 0, 0), iC3(0) {
	}

///@brief alternative constructor
	BZK_Color4f(BZK_FixP aC0, BZK_FixP aC1, BZK_FixP aC2, BZK_FixP aC3) : BZK_Color3f(aC0, aC1, aC2), iC3(aC3) {
	}

///@brief getter for the fourth channel
	inline BZK_FixP GetC3() {
		return iC3;
	}

///@brief setter for the fourth channel
	inline void SetC3(BZK_FixP aC3) {
		iC3 = aC3;
	}
////////////////////////// fields /////////////////////
private:
	BZK_FixP iC3; // the fourth channel

};
