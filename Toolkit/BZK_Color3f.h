/**
@brief three dimensional color. Base for RGB, but useful for other color schemes
@author Daniel Monteiiro
*/
class BZK_Color3f : public BZK_Color2f {
//////////////////// functions //////////////////////////////
public:

/// @brief class's constructor

	BZK_Color3f() : BZK_Color2f(0, 0), iC2(0) {
	}

///@brief alternative constructor

	BZK_Color3f(BZK_FixP aC0, BZK_FixP aC1, BZK_FixP aC2) : BZK_Color2f(aC0, aC1), iC2(aC2) {
		// notice how we delegate most of the initialization to the "super" class.
		;
		/// iC2=(aC2); //init third channel.

	}

///@brief getter for the third channel
	inline BZK_FixP GetC2() {
		return iC2;
	}

/// @brief setter for the third channel
	inline void SetC2(BZK_FixP aC2) {
		iC2 = aC2;
	}
/////////////////////////fields////////////////////////////
private:
/// the third channel
	BZK_FixP iC2;

};
