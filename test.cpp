//using namespace std;
#include "BZK_POSIX.h"
#include "BZK_BaseIncludes.h"
#include "Vacuum/VAC_BaseIncludes.h"

int main(int argc, char *argv[])
{
  bool quit=false;
  BZK_SysInit(); 
  BZK_Bitmap *video=BZK_InitVideo();
  BZK_SystemEvent event;
  VAC_Render *render=new VAC_Render();
  render->LoadCannonical();
  BZK_Vec3f point[20];
  int values[20+4];
  VAC_Face *face=new VAC_Face();
  int times[4];

  SDL_FillRect(video,0,0xFFFFFF);
  for (int f=3;f<20;f++)
	{
  times[0]=time(0);
  for (int d=0;d<1000;d++)
	{
	times[2]=time(0);
	for (int c=0;c<13;c++)
		{
		values[c]=rand();
		if (c<9)
			values[c]=values[c]/0xFFFFFFF;
		else
			values[c]=values[c]%255;
	//	cout << values[c] << endl;
/*
		if (values[c]>-2)
			values[c]=-2;
		if (values[c]<2)
			values[c]=2;
*/
		}
	point[0].SetVec(BZK_FastMath::Int32toFix32(2-values[0]),BZK_FastMath::Int32toFix32(2-values[1]),BZK_FastMath::Int32toFix32(values[2]));
	point[1].SetVec(BZK_FastMath::Int32toFix32(2-values[3]),BZK_FastMath::Int32toFix32(2-values[4]),BZK_FastMath::Int32toFix32(values[5]));
	point[2].SetVec(BZK_FastMath::Int32toFix32(2-values[6]),BZK_FastMath::Int32toFix32(2-values[7]),BZK_FastMath::Int32toFix32(values[8]));	
	face=render->RequestFace();
	face->iFill.SetColor(BZK_FastMath::Int32toFix32(values[9]),BZK_FastMath::Int32toFix32(values[10]),BZK_FastMath::Int32toFix32(values[11]),BZK_FastMath::Int32toFix32(values[12]));
	face->AddPoint(&point[0]);
	face->AddPoint(&point[1]);
	face->AddPoint(&point[2]);  

	render->AddToImageFinal(face);
	render->Update(video);
	BZK_CheckEvents(&event);
	BZK_SwapBuffers(video);
	if (event.key.keysym.sym==BZK_KeyESCAPE)
		quit=true;
	times[3]=time(0);
	}  
  times[1]=time(0);
  cout << f<<"-total:"<<times[1]-times[0] << endl;
	}
  BZK_Quit();
  return 0;
}
