/**
@brief 2D Vector
@author Daniel Monteiro
*/
class BZK_Vec2f
{
public:

///default constructor
  BZK_Vec2f():iX(0),iY(0)
    {
    }

  BZK_Vec2f(BZK_FixP aX,BZK_FixP aY):iX(aX),iY(aY)
    {
    }

/// getter X
inline BZK_FixP GetX()
{
	return iX;
}

///getter Y
inline BZK_FixP GetY()
{
	return iY;
}

///setter X
inline void SetX(BZK_FixP aX)
{
	iX=aX;
}
///setter Y
inline void SetY(BZK_FixP aY)
{
	iY=aY;
}
/// wide setter
inline void Set(BZK_FixP aX,BZK_FixP aY)
{
	iY=aY;
	iX=aX;
}
//////////////////////////////
 private:
BZK_FixP iX;
BZK_FixP iY;
};
