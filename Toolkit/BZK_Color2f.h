/**
@brief Two dimensional color class. might be useful for artistic purposes or some simpler LCDs
@author Daniel Monteiro
*/

class BZK_Color2f//:public BZK_Vec1f
{
/////////////////////////// functions /////////////////////////////////////////////
public:
/**
@brief class's constructor
*/
	BZK_Color2f() : iC0(0), iC1(0) {
	}

/**
@brief alternative constructor.
*/
	BZK_Color2f(BZK_FixP aC0, BZK_FixP aC1) : iC0(aC0), iC1(aC1) {
	}


/**
@brief setter for the first channel
*/
	inline void SetC0(BZK_FixP aC0) {
		iC0 = aC0;
	}


/**
@brief setter for the second channel
*/

	inline void SetC1(BZK_FixP aC1) {
		iC1 = aC1;
	}


/**
@brief getter for the first channel
*/

	inline BZK_FixP GetC0() {
		return iC0;
	}

/**
@brief getter for the second channel.
*/

	inline BZK_FixP GetC1() {
		return iC1;
	}
	///////////////////fields/////////////////////////////////////
private:
/// the first channel
	BZK_FixP iC0;
/// the second channel
	BZK_FixP iC1;

};
