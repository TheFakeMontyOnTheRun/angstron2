///brief generic rectangle class. I guess it shouldnt be here, but I've used it atleast once for some very basic apps.
///the second measure can be used both relativily or absolutely

class BZK_Rect {

private:
	BZK_FixP iX0, iY0, iX1, iY1;
	int iPlane; ///not used yet. The idea is to describe a Z
	int iSpace; ///not used yet. The idea is to define in what space this is being used, in order to protect operations between diferent spaces
public:
	std::string toString() {
		std::string toReturn = "";
		toReturn += BZK_FastMath::Tostring(iX0);
		toReturn += ",";
		toReturn += BZK_FastMath::Tostring(iY0);
		toReturn += "-";
		toReturn += BZK_FastMath::Tostring(iX1);
		toReturn += ",";
		toReturn += BZK_FastMath::Tostring(iY1);
		return toReturn;
	}

///default constructor initializes everything to zero
	BZK_Rect() : iX0(0), iY0(0), iX1(0), iY1(0) {
	}

///alternative constructor takes parameters for all dimensions.
	BZK_Rect(BZK_FixP aX0, BZK_FixP aY0, BZK_FixP aX1, BZK_FixP aY1) : iX0(aX0), iY0(aY0), iX1(aX1), iY1(aY1) {
	}


///modify the intire rect to a certain state
	void SetRect(BZK_FixP aX0, BZK_FixP aY0, BZK_FixP aX1, BZK_FixP aY1) {
		iX0 = aX0;
		iY0 = aY0;
		iX1 = aX1;
		iY1 = aY1;
	}


/// copy from another rect
///TODO: rename to CopyFrom
	void copy(BZK_Rect &aAnother) {
		SetX0(aAnother.GetX0());
		SetX1(aAnother.GetX1());
		SetY0(aAnother.GetY0());
		SetY1(aAnother.GetY1());
	}


///invidual getters
	BZK_FixP GetX0() { return iX0; }

	BZK_FixP GetY0() { return iY0; }

	BZK_FixP GetX1() { return iX1; }

	BZK_FixP GetY1() { return iY1; }

	BZK_FixP GetW() { return iX1 - iX0; }

	BZK_FixP GetH() { return iY1 - iY0; }

///individual setters
	void SetX0(BZK_FixP aParm) { iX0 = aParm; }

	void SetY0(BZK_FixP aParm) { iY0 = aParm; }

	void SetX1(BZK_FixP aParm) { iX1 = aParm; }

	void SetY1(BZK_FixP aParm) { iY1 = aParm; }

	void SetW(BZK_FixP aParm) { iX1 = iX0 + aParm; }

	void SetH(BZK_FixP aParm) { iY1 = iY0 + aParm; }


};
