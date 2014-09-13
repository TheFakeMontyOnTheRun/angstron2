/**
@brief 3D Vector - has some parsing code.
@author Daniel Monteiro
*/

class BZK_Vec3f:public BZK_Vec2f
{
int iSpace; /// vector subspace
BZK_FixP iZ; ///third dimension
public:


//////iSpace:getter and setter///////////
void SetSpace(int aSpace)
{
	iSpace=aSpace;
}

int GetSpace()
{
	return iSpace;
}
////////////////////////////////////////////

/////////////iZ:getter and setter //////////// 
BZK_FixP GetZ()
	{
	return iZ;
	}

 void SetZ(BZK_FixP aZ)
	{
	iZ=aZ;
	}
/////////////////////////////////////////////

///@brief constructor
BZK_Vec3f():BZK_Vec2f(0,0),iSpace(UNDEFINED),iZ(0)
   {
/*
     SetSpace(UNDEFINED);
     SetX(0);
     SetY(0);
     SetZ(0);*/
   }
 
///@brief alternative constructor
BZK_Vec3f(BZK_FixP aX,BZK_FixP aY,BZK_FixP aZ):BZK_Vec2f(aX,aY),iSpace(UNDEFINED),iZ(aZ)
   {
   //  SetVec(aX,aY,aZ);
   }

///@brief compares two BZK_Vec3f
bool operator==(BZK_Vec3f &aAnother)
{
  return (GetX()==aAnother.GetX())&&(GetY()==aAnother.GetY())&&(GetZ()==aAnother.GetZ())&&(iSpace==aAnother.GetSpace());
}

///@brief re-define the vector (almost) completely
 void
   SetVec(BZK_FixP aX,BZK_FixP aY,BZK_FixP aZ)
   {
     Set(aX,aY);
     iZ=(aZ);
   }
 
///@brief parse 3 values in a std::string as a vector (useful for both XML and Wavefront OBJ)
void Internalize(std::string aText)
{
  Set(BZK_ParsingTools::GetNumber(BZK_ParsingTools::GetSubToken(aText,1)),
      BZK_ParsingTools::GetNumber(BZK_ParsingTools::GetSubToken(aText,2)));
  iZ=BZK_ParsingTools::GetNumber(BZK_ParsingTools::GetSubToken(aText,3));
}

///@brief output a std::string in the same format. wv stands for WaveFront.
std::string wvstring()
{ 
  return BZK_FastMath::Print(GetX()) + " "+ BZK_FastMath::Print(GetY()) + " "+ BZK_FastMath::Print(iZ); 
}

};
