#define SIZESECTOR 15
using namespace std;

#include "BZK_POSIX.h"
#include "BZK_BaseIncludes.h"
#include "Vacuum/VAC_BaseIncludes.h"
#include "bzk1/World.h"
//#include "Vacuum/VAC_LogoLang.h"

//dummy game control
void GameStart(VAC_ActorManager&, VAC_World&){}

VAC_World World;
VAC_ActorManager People;
BZK1_PET PET;
VAC_Sector *CurrentSector;//=World.GetSector(0);
VAC_ColorRGBA Palette[256];
int PaletteTop;
BZK_Vector<string> textures;
void  Export_Lama (string aFilename);
void ImportOBJ(string aobj,string aout);
int gridsizex;
int gridsizey;
int gridsizez;
//int gridsize;
float sectsizex;
float sectsizey;
float sectsizez;
//int sectsize;
int floors;

class rect;

int *_fig;
int *_sec;
void  MakeIrregular ();
long Polycount();
void Prompt();
void Export_OBJ(string aObj);
void Optimize();
BZK_Vector<BZK_Vector<int>*> iPartitions;
enum {VERTICAL,HORIZONTAL};
string orientacao[2];



int sec(int z, int y, int x)
{
  return _sec[((gridsizex*gridsizey)*z)+(gridsizex*y)+x];
}

int fig(int z, int y, int x)
{
  return _fig[((gridsizex*gridsizey)*z)+(gridsizex*y)+x];
}


void setsec(int z, int y, int x,int value)
{
  _sec[((gridsizex*gridsizey)*z)+(gridsizex*y)+x]=value;
}

void setfig(int z, int y, int x,int value)
{
  _fig[((gridsizex*gridsizey)*z)+(gridsizex*y)+x]=value;
}



void VisitAndExportSVG(int aSector , ofstream &aSVG)
{  
  VAC_Sector *ptr=World.GetSector(aSector);
  // if (ptr->HasBeenDrawn()) return;
  /// adiciona setor na lista
  // aList.Add(aSector);
  //  ptr->Visiting();
  cout << aSector <<endl;
  int current[4];
  int r,g,b,x,y,h,w;
  char buffer[12];
  current[N]=BZK_FastMath::Fix32toInt32(ptr->Point(N)->GetZ());
  current[S]=BZK_FastMath::Fix32toInt32(ptr->Point(S)->GetZ());
  current[W]=BZK_FastMath::Fix32toInt32(ptr->Point(W)->GetX());
  current[E]=BZK_FastMath::Fix32toInt32(ptr->Point(E)->GetX());    
  
  r=BZK_FastMath::Fix32toInt32(ptr->iColor[FLOOR].GetR());
  g=BZK_FastMath::Fix32toInt32(ptr->iColor[FLOOR].GetG());
  b=BZK_FastMath::Fix32toInt32(ptr->iColor[FLOOR].GetB());
  x=BZK_FastMath::Fix32toInt32(ptr->Rect()->GetX0());
  y=BZK_FastMath::Fix32toInt32(ptr->Rect()->GetY0());
  h=BZK_FastMath::Fix32toInt32(ptr->Rect()->GetH());
  w=BZK_FastMath::Fix32toInt32(ptr->Rect()->GetW());
  sprintf(buffer,"%x%x%x",r,g,b);
  aSVG<<"<rect  style='fill:#"<<buffer<<"'  width='"<<w<<"' height='"<<h<<"' x='"<<x<<"'  y='"<<y<<"' />"<<endl;  
  cout << "SVG:"<<"<rect  style='fill:#"<<buffer<<"'  width='"<<w<<"' height='"<<h<<"' x='"<<x<<"'  y='"<<y<<"' />"<<endl; 
}

void WriteSVGHeader(ofstream &aSVG)
{
  aSVG <<"<?xml version='1.0' encoding='UTF-8' standalone='no'?>"<<endl;
  aSVG <<"<svg xmlns:dc='http://purl.org/dc/elements/1.1/'"<<endl;
  aSVG <<" xmlns:cc='http://web.resource.org/cc/'"<<endl;
  aSVG <<"   xmlns:rdf='http://www.w3.org/1999/02/22-rdf-syntax-ns#'"<<endl;
  aSVG <<"   xmlns:svg='http://www.w3.org/2000/svg'"<<endl;
  aSVG <<"   xmlns='http://www.w3.org/2000/svg'"<<endl;
  aSVG <<"   xmlns:sodipodi='http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd'"<<endl;
  aSVG <<"   xmlns:inkscape='http://www.inkscape.org/namespaces/inkscape'"<<endl;
  aSVG <<"   width='210mm'"<<endl;
  aSVG <<"   height='297mm'"<<endl;
  aSVG <<"   id='svg2'"<<endl;
  aSVG <<"   sodipodi:version='0.32'"<<endl;
  aSVG <<"   inkscape:version='0.45.1'"<<endl;
  aSVG <<  "   sodipodi:docbase='"<<STOREPATH<<"'"<<endl;
  aSVG <<"   sodipodi:docname='desenho.svg'"<<endl;
  aSVG <<"   inkscape:output_extension='org.inkscape.output.svg.inkscape'>"<<endl;
}

void ExportSVGPaper(const char *aFileName)
{
  char buffer[20]="";
  int  r,g,b;
  ofstream aSVG(aFileName);
  WriteSVGHeader(aSVG);

      aSVG << "<g id='x'>"<<endl;
  for (int z=0;z<gridsizez;z++)
	{
	      for (int x=0;x<gridsizex;x++)
		{	  
		  for (int y=0;y<gridsizey;y++)
		    {
		      if (fig(z,y,x)==-1) continue;
		      r=BZK_FastMath::Fix32toInt32(Palette[fig(z,y,x)].GetR());
		      g=BZK_FastMath::Fix32toInt32(Palette[fig(z,y,x)].GetG());
		      b=BZK_FastMath::Fix32toInt32(Palette[fig(z,y,x)].GetB());
		     
		      sprintf(buffer,"%x%x%x",r,g,b);
		      aSVG<<"<rect  style='fill:#"<<buffer<<"'  width='1' height='1' x='"<<z+x+(z*gridsizex)<<"'  y='"<<y<<"' />"<<endl;
		    }
		}
	}
      aSVG<<"</g>"<<endl;
    
      aSVG << "<g id='x'>"<<endl;
  for (int z=0;z<gridsizex;z++)
	{
	      for (int x=0;x<gridsizez;x++)
		{	  
		  for (int y=0;y<gridsizey;y++)
		    {
		      if (fig(x,y,z)==-1) continue;
		      r=BZK_FastMath::Fix32toInt32(Palette[fig(x,y,z)].GetR());
		      g=BZK_FastMath::Fix32toInt32(Palette[fig(x,y,z)].GetG());
		      b=BZK_FastMath::Fix32toInt32(Palette[fig(x,y,z)].GetB());
		     
		      sprintf(buffer,"%x%x%x",r,g,b);
		      aSVG<<"<rect  style='fill:#"<<buffer<<"'  width='1' height='1' x='"<<z+x+(z*gridsizez)<<"'  y='"<<5+y+gridsizey<<"' />"<<endl;
		    }
		}
	}
aSVG<<"</g>"<<endl;

      aSVG << "<g id='x'>"<<endl;
   for (int z=0;z<gridsizey;z++)
	{
	      for (int x=0;x<gridsizez;x++)
		{	  
		
		  for (int y=0;y<gridsizex;y++)
		    {
		      if (fig(x,z,y)==-1) continue;
		      r=BZK_FastMath::Fix32toInt32(Palette[fig(x,z,y)].GetR());
		      g=BZK_FastMath::Fix32toInt32(Palette[fig(x,z,y)].GetG());
		      b=BZK_FastMath::Fix32toInt32(Palette[fig(x,z,y)].GetB());
		     
		      sprintf(buffer,"%x%x%x",r,g,b);
		      aSVG<<"<rect  style='fill:#"<<buffer<<"'  width='1' height='1' x='"<<z+x+(z*gridsizez)<<"'  y='"<<10+y+(2*gridsizey)<<"' />"<<endl;
			
		    }
				
		}

	}
      aSVG<<"</g>"<<endl;

  aSVG<<"</svg>"<<endl;
exit(0);
}

void ExportSVG(const char *aFileName)
{
  char buffer[6]="";
  int  r,g,b;
  ofstream aSVG(aFileName);
  WriteSVGHeader(aSVG);
  for (int z=0;z<gridsizez;z++)
    {
      aSVG << "<g id='"<<z<<"'>"<<endl;
      for (int x=0;x<gridsizex;x++)
	{	  
	  for (int y=0;y<gridsizey;y++)
	    {
	      if (fig(z,y,x)==-1) continue;
	      r=BZK_FastMath::Fix32toInt32(Palette[fig(z,y,x)].GetR());
	      g=BZK_FastMath::Fix32toInt32(Palette[fig(z,y,x)].GetG());
	      b=BZK_FastMath::Fix32toInt32(Palette[fig(z,y,x)].GetB());
	     
	      sprintf(buffer,"%x%x%x",r,g,b);
	      aSVG<<"<rect  style='fill:#"<<buffer<<"'  width='1' height='1' x='"<<x<<"'  y='"<<y<<"' />"<<endl;
	    }
	}
      aSVG<<"</g>"<<endl;
    } 
  aSVG<<"</svg>"<<endl;

}

bool index(vector<int> &vect,int num)
{
int c;
for (c=0;c<vect.size();c++)
	if (vect[c]==num)
	return c;
return -1;
}

vector<vector<int>*> indivisiveis;


bool subdivide(int aX0,int aX1,int aY0,int aY1,int aAllign,ofstream &aSVG)
{
  cout << "*****************"<<endl;
  orientacao[0]="vertical";
  orientacao[1]="horizontal";
  World.NotifyEndOfCycle();
  int maior[4];
  int current[4];
  int sector[4];
  BZK_Vector<int> *list;
  vector<int> lista;
  vector<int> *lista_ptr=new vector<int>();
  int c;
  int last=0;
  VAC_Sector *ptr;
  memset(maior,0,sizeof(int)*4);
  memset(current,0,sizeof(int)*4);
  memset(sector,0,sizeof(int)*4);
  for (c=1;c<World.TotalSectors();c++)
    {
      ptr=World.GetSector(c);

      //// obtem dimensoes do setor //////////////////////////
      current[N]=BZK_FastMath::Fix32toInt32(ptr->Point(N)->GetZ());
      current[S]=BZK_FastMath::Fix32toInt32(ptr->Point(S)->GetZ());
      current[W]=BZK_FastMath::Fix32toInt32(ptr->Point(W)->GetX());
      current[E]=BZK_FastMath::Fix32toInt32(ptr->Point(E)->GetX());      
      
      ///////////////// se o setor esta fora a area de foco atual, passe pro pr�ximo ///////
      if (current[N] > aY1 || current[N]<aY0) 
	continue;

      if (current[S] > aY1 || current[S]<aY0) 
	continue;

      if (current[W] > aX1 || current[W]<aX0) 
	continue;

      if (current[E] > aX1 || current[E]<aX0) 
	continue;

      /////////////////////////////////////////////////////////

      // como setor � interessante, vamos guarda-lo
      lista.push_back(c);

    }
  if (lista.size()<2) return false;

  cout << aX0<<","<<aY0<<"-"<<aX1<<","<<aY1;//<<endl;
  
  cout << " setores:" <<endl;

  for (c=0;c<lista.size();c++)  
	{
	   cout << lista[c] <<";";
	lista_ptr->push_back(c);
	for (int d=0;d<6;d++)
		if (World.GetSector(lista[c])->Get_Connection(d)!=0)
			if (index(lista,World.GetSector(lista[c])->Get_Connection(d))==-1)
				cout << "tem que dividir" <<endl;
			
	}
  cout << endl;

 
    cout << "orientacao:" << orientacao[aAllign%2] << endl;
    if (lista.size()!=0)
      if (aAllign%2==HORIZONTAL)
	{

	  //se ainda � divisivel sem que de no mesmo.
  if (aY1/2 != aY1) 
    if(subdivide(aX0,aX1,aY0,aY1/2,aAllign+1,aSVG))
      aSVG<<"<rect style='fill:#0f0' width='"<<aX1-aX0<<"' height='"<<aY1-aY0<<"' x='"<<aX0<<"'  y='"<<aY0<<"' />"<<endl;
	else	
	{
	indivisiveis.push_back(lista_ptr);
	return false;
	}

  if (aY1/2 != aY0)
    if (subdivide(aX0,aX1,aY1/2,aY1,aAllign+1,aSVG))
       aSVG<<"<rect style='fill:#0f0' width='"<<aX1-aX0<<"' height='"<<aY1-aY0<<"' x='"<<aX0<<"' y='"<<aY0<<"' />"<<endl;
	else	
	{
	indivisiveis.push_back(lista_ptr);
	return false;
	}

	      
	  return true;
	}
      else
	{
	 
  if (aX1/2 != aX1)
   if ( subdivide(aX0,aX1/2,aY0,aY1,aAllign+1,aSVG))
     aSVG<<"<rect style='fill:#0f0' width='"<<aX1-aX0<<"' height='"<<aY1-aY0<<"' x='"<<aX0<<"' y='"<<aY0<<"' />"<<endl;
	else	
	{
	indivisiveis.push_back(lista_ptr);
	return false;
	}



  if (aX1/2 != aX0)
    if (subdivide(aX1/2,aX1,aY0,aY1,aAllign+1,aSVG))
     aSVG<<"<rect style='fill:#0f0' width='"<<aX1-aX0<<"' height='"<<aY1-aY0<<"' x='"<<aX0<<"' y='"<<aY0<<"' />"<<endl;  
	else	
	{
	indivisiveis.push_back(lista_ptr);
	return false;
	}


     
  return true;
	}
    else
      return false;

}


void initmatrices(int agridsizex,int agridsizey,int agridsizez, float asecsizex, float asecsizey,float asecsizez)
{
  floors=agridsizez;
  gridsizex=agridsizex;
  gridsizey=agridsizey;
  gridsizez=agridsizez;
  //gridsize=agridsizex;

  sectsizex=asecsizex;
  sectsizey=asecsizey;
  sectsizez=asecsizez;
  //sectsize=asecsizex;

  _fig=(int*)malloc(((gridsizex+1)*(gridsizey+1)*(gridsizez+1))*sizeof(int));
  _sec=(int*)malloc(((gridsizex+1)*(gridsizey+1)*(gridsizez+1))*sizeof(int));
}

void InitVoxelPet(int aLayers)
{
  int h;
  for (h=0;h<aLayers;h++)
    {
      PET.patterns[h].HeightFloor=1+(h)*sectsizez;
      PET.patterns[h].HeightCeiling=1+(h+1)*sectsizez;
    }
}





class rect
{
public:
  int x0;
  int y0;
  int x1;
  int y1;
  int z0;
  int z1;
  int divx;
  int divy;
  int padrao;
  int outro;
  //int *_mat;
  rect *next_ptr;
  void
  clear()
  {
    int x,y,z;
    for (z=z0;x<z1;z++)
      for (x=x0;x<x1;x++)
	for(y=y0;y<y1;y++)
	  setfig(z,y,x,0);

  }

 void
 show()
  {
    cout <<"padr�o matricial"<<endl;
    int x,y;
    for (y=y0;y<y1;y++)
      {
	//	cout<<y<<"-";
	for(x=x0;x<x1;x++)
	  cout <<fig(0,y,x)<<".";
	cout<<endl;
      }

  }


void readfile(ifstream &inputfile)
{
  int x,y,h,in;
	string head;
  int agridsizex;
  int agridsizey;
  int agridsizez;
//  int agridsize;
  float asectsizex;
  float asectsizey;
  float asectsizez;
//  int asectsize;
  int afloors;

  inputfile >> agridsizex;
  inputfile >> agridsizey;
  inputfile >> agridsizez;
  inputfile >> sectsizex;
  inputfile >> sectsizey;
  inputfile >> sectsizez;
 // sectsize=sectsizex;
  initmatrices(agridsizex,agridsizey,agridsizez,(sectsizex),(sectsizey),(sectsizez));
  x1=agridsizex;
  y1=agridsizey;
  z1=agridsizez;

  for (h=0;h<floors;h++)
    {
      inputfile >> in;//dummy
      for (y=y0;y<y1;y++)
	{
	  cout<<y<<"-";
	  for(x=x0;x<x1;x++)
	    {
	      inputfile >> in;
	      setfig(h,y,x,in);
	      cout <<fig(h,y,x)<<".";
	    }
	  cout<<endl;
	}
    }
	while (inputfile.good())
	{
		inputfile >> head;
		textures.Add(head);
		cout << "lida textura "<<head<<endl;
	}


  inputfile.close();
  cout << "leitura finalizada"<<endl;
}


void sect()
{
 cout<<endl;
  int x,y,h,a;
  for (h=0;h<floors;h++)
    for (y=y0;y<y1;y++)
      {
	cout<<y<<"-";
	for(x=x0;x<x1;x++)
	  if (sec(h,y,x)!=0)
	    cout <<"#";
	  else
	    cout <<".";
	cout<<endl;
      }
}


  rect(int apadrao,int ax0,int ay0,int ax1,int ay1,int z0,int z1)
  {
  x0=ax0;
  y0=ay0;
  x1=ax1;
  y1=ay1;
  divx=x1;
  divy=y1;
  padrao=apadrao;
  outro=0;
  next_ptr=NULL;

  }

  BZK_Rect*
  ToSector()
  {
	//assume os valores como FixP - o que nao eh muito legal...
    BZK_Rect *New=new BZK_Rect;
    New->SetX0(x0);
    New->SetY0(x0);
    New->SetW(x1-x0);
    New->SetH(y1-y0);
    return New;
  }

  void
  print()
  {
    cout <<"rect:["<<x0<<","<<y0<<"]-["<<x1<<","<<y1<<"]:["<<divx<<","<<divy<<"]=esperado "<<padrao<<" encontrado "<<outro<<endl;
    if (next_ptr!=NULL) next_ptr->print();
  }


};


bool
verifica_h(rect &pesq)
{
  int c;
  int lastx=pesq.x0;
  int lasty=pesq.y0;
  int linha;
  int coluna=c;
  if ((pesq.x0==pesq.x1)&&(pesq.y0==pesq.y1)) return true;

  for (linha=pesq.y0;linha<pesq.y1;linha++)
    {
      for (c=pesq.x0;c<pesq.x1;c++)
	{
	  if (fig(0,linha,c)!=pesq.padrao)
	    {
	      pesq.x1=pesq.x1;
	      pesq.y1=lasty;
	      pesq.divx=c;
	      pesq.divy=linha;
	      pesq.outro=fig(0,linha,c);
	      return false;
	    }
	  lastx=c;
	}
      lasty=linha;
    }
  return true;

}


bool
ScanVoxel(rect &pesq)
{
  int c;
  int lastx=pesq.x0;
  int lasty=pesq.y0;
  int linha;
  int coluna=c;
  int h=0;

  BZK_Rect sect;


  if (World.TotalSectors() ==0)
    {
      //master sector
      sect.SetX0(0);
      sect.SetY0(0);
      sect.SetW(BZK_FastMath::Int32toFix32(255));
      sect.SetH(BZK_FastMath::Int32toFix32(255));
      World.CreateSector(sect,0,255);
    }


  if ((pesq.x0==pesq.x1)&&(pesq.y0==pesq.y1)) return true;

 for (h=0;h<floors;h++)
   for (linha=pesq.y0;linha<pesq.y1;linha++)
     for (c=pesq.x0;c<pesq.x1;c++)
       {
	 sect.SetX0(BZK_FastMath::Real32toFix32(c*sectsizex));
	 sect.SetY0(BZK_FastMath::Real32toFix32(linha*sectsizey));
	 sect.SetW(BZK_FastMath::Real32toFix32(sectsizex));
	 sect.SetH(BZK_FastMath::Real32toFix32(sectsizey));
	 setsec(h,linha,c,0);
	 if (fig(h,linha,c)==-1)
	   {
	     setsec(h,linha,c,World.CreateSector(sect,BZK_FastMath::Fix32toInt32(PET.patterns[h].HeightFloor),BZK_FastMath::Fix32toInt32(PET.patterns[h].HeightCeiling)));
//		setsec(h,linha,h*sectsizez,(h-1)*sectsizez);
	   }
       }

}


bool
LinkVoxel(rect &pesq)
{
  int c;
  int lastx=pesq.x0;
  int lasty=pesq.y0;
  int h=0;
  int linha;
  int coluna=c;
  int k=0;
	int one=1;
  CurrentSector=World.GetSector(0);
//  for (k=0;k<World.TotalSectors();k++)    CurrentSector->iPVS.Add(one); //o ideal � zero, embora isso seja arbitr�rio

  for (h=0;h<floors;h++)
    for (linha=pesq.y0;linha<pesq.y1;linha++)
      for (c=pesq.x0;c<pesq.x1;c++)
	{
	  /*
	    funcionou bem usando W e E invertidos...
	    preciso rever os calculos para entender porque esta tudo invertido na
	    quest�o das coordenadas...
	  */
	  if (sec(h,linha,c)!=0)
	    {
	      CurrentSector=World.GetSector(sec(h,linha,c));

//	      for (k=0;k<World.TotalSectors();k++)		CurrentSector->iPVS.Add(one);


	      if (linha>0)
		if (fig(h,linha-1,c)==-1)
		  CurrentSector->SetConnection(N,sec(h,linha-1,c));
	      	else
		if (fig(h,linha-1,c)>PaletteTop)
			{	
			   CurrentSector->iTextures[N]=new VAC_PolyBuffer();
     	    		   CurrentSector->iTextures[N]->iResourceName=textures[fig(h,linha-1,c)-PaletteTop];
			}
		else
		{
	 	CurrentSector->iColor[N].Copy(Palette[fig(h,linha-1,c)]);

		CurrentSector->iColor[N].SetR(BZK_FastMath::Mul(CurrentSector->iColor[N].GetR(),BZK_FastMath::Real32toFix32(1.0)));
		CurrentSector->iColor[N].SetG(BZK_FastMath::Mul(CurrentSector->iColor[N].GetG(),BZK_FastMath::Real32toFix32(1.0)));
		CurrentSector->iColor[N].SetB(BZK_FastMath::Mul(CurrentSector->iColor[N].GetB(),BZK_FastMath::Real32toFix32(1.0)));

		
		  CurrentSector->SetConnection(N,0);
		 }

		  


	      if (linha<gridsizey)
		if (fig(h,linha+1,c)==-1)
		  CurrentSector->SetConnection(S,sec(h,linha+1,c));
	      	else
		if (fig(h,linha+1,c)>PaletteTop)
			{	
			   CurrentSector->iTextures[S]=new VAC_PolyBuffer();
     	    		   CurrentSector->iTextures[S]->iResourceName=textures[fig(h,linha+1,c)-PaletteTop];
			}
		else
		{
		  CurrentSector->iColor[S].Copy(Palette[fig(h,linha+1,c)]);
		  CurrentSector->iColor[S].SetR(BZK_FastMath::Mul(CurrentSector->iColor[S].GetR(),BZK_FastMath::Real32toFix32(1.0)));
		  CurrentSector->iColor[S].SetG(BZK_FastMath::Mul(CurrentSector->iColor[S].GetG(),BZK_FastMath::Real32toFix32(1.0)));
		  CurrentSector->iColor[S].SetB(BZK_FastMath::Mul(CurrentSector->iColor[S].GetB(),BZK_FastMath::Real32toFix32(1.0)));
		  CurrentSector->SetConnection(S,0);
		}



	      if (c<gridsizex)
		if (fig(h,linha,c+1)==-1)
		  CurrentSector->SetConnection(W,sec(h,linha,c+1));
		else
		if (fig(h,linha,c+1)>PaletteTop)
			{	
			   CurrentSector->iTextures[W]=new VAC_PolyBuffer();
     	    		   CurrentSector->iTextures[W]->iResourceName=textures[fig(h,linha,c+1)-PaletteTop];
			}
		else
		  {
		    CurrentSector->SetConnection(W,0);
		    CurrentSector->iColor[W].Copy(Palette[fig(h,linha,c+1)]);
		    CurrentSector->iColor[W].SetR(BZK_FastMath::Mul(CurrentSector->iColor[W].GetR(),BZK_FastMath::Real32toFix32(1.0)));
		    CurrentSector->iColor[W].SetG(BZK_FastMath::Mul(CurrentSector->iColor[W].GetG(),BZK_FastMath::Real32toFix32(1.0)));
		    CurrentSector->iColor[W].SetB(BZK_FastMath::Mul(CurrentSector->iColor[W].GetB(),BZK_FastMath::Real32toFix32(1.0)));
		    
		  }



	      if (c>0)
		if (fig(h,linha,c-1)==-1)
		  CurrentSector->SetConnection(E,sec(h,linha,c-1));
	      	else
		if (fig(h,linha,c-1)>PaletteTop)
			{	
			   CurrentSector->iTextures[E]=new VAC_PolyBuffer();
     	    		   CurrentSector->iTextures[E]->iResourceName=textures[fig(h,linha,c-1)-PaletteTop];
			}
		else
		  {
		    CurrentSector->SetConnection(E,0);
		    CurrentSector->iColor[E].Copy(Palette[fig(h,linha,c-1)]);
		    CurrentSector->iColor[E].SetR(BZK_FastMath::Mul(CurrentSector->iColor[E].GetR(),BZK_FastMath::Real32toFix32(1.0)));
		    CurrentSector->iColor[E].SetG(BZK_FastMath::Mul(CurrentSector->iColor[E].GetG(),BZK_FastMath::Real32toFix32(1.0)));
		    CurrentSector->iColor[E].SetB(BZK_FastMath::Mul(CurrentSector->iColor[E].GetB(),BZK_FastMath::Real32toFix32(1.0)));
		   
		  }


	      if (h>0)
		if (fig(h-1,linha,c)==-1)
		  CurrentSector->SetConnection(FLOOR,sec(h-1,linha,c));
	      	else
		  {
		    CurrentSector->SetConnection(FLOOR,0);
		    CurrentSector->iColor[FLOOR].Copy(Palette[fig(h-1,linha,c)]);
		    CurrentSector->iColor[FLOOR].SetR(BZK_FastMath::Mul(CurrentSector->iColor[FLOOR].GetR(),BZK_FastMath::Real32toFix32(1.0)));
		    CurrentSector->iColor[FLOOR].SetG(BZK_FastMath::Mul(CurrentSector->iColor[FLOOR].GetG(),BZK_FastMath::Real32toFix32(1.0)));
		    CurrentSector->iColor[FLOOR].SetB(BZK_FastMath::Mul(CurrentSector->iColor[FLOOR].GetB(),BZK_FastMath::Real32toFix32(1.0)));
		   
		  }



	      if (h<floors)
		if (fig(h+1,linha,c)==-1)
		  CurrentSector->SetConnection(CEILING,sec(h+1,linha,c));
	      	else
		  {
		    CurrentSector->SetConnection(CEILING,0);
		    CurrentSector->iColor[CEILING].Copy(Palette[fig(h+1,linha,c)]);
		    CurrentSector->iColor[CEILING].SetR(BZK_FastMath::Mul(CurrentSector->iColor[CEILING].GetR(),BZK_FastMath::Real32toFix32(1.0)));
		    CurrentSector->iColor[CEILING].SetG(BZK_FastMath::Mul(CurrentSector->iColor[CEILING].GetG(),BZK_FastMath::Real32toFix32(1.0)));
		    CurrentSector->iColor[CEILING].SetB(BZK_FastMath::Mul(CurrentSector->iColor[CEILING].GetB(),BZK_FastMath::Real32toFix32(1.0)));		   
		  }




	    }
	}

  pesq.sect();
}



bool
SetPVS(rect &pesq)
{
  int c;
  int lastx=pesq.x0;
  int lasty=pesq.y0;
  int linha;
  int coluna=c;
  int outro=0;
  int x,y,z,temp,temp2;
  bool visivel;
  for (z=0;z<floors;z++)
    for (linha=pesq.y0;linha<pesq.y1;linha++)
      for (c=pesq.x0;c<pesq.x1;c++)
	{
	  /*
	    funcionou bem usando W e E invertidos...
	    preciso rever os calculos para entender porque esta tudo invertido na
	    quest�o das coordenadas...
	  */

	  if (sec(z,linha,c)!=0)
	    {

	      // CurrentSector=(*World.iSector)[(sec[z][linha][c])];
	      CurrentSector=World.GetSector(sec(z,linha,c));

	      //principais

	      visivel=true;

	      //W
	      for (x=c;x<pesq.x0;x--)
		{
		  outro=sec(z,linha,x);
		  if (outro==0)
		    visivel=false;
		  // else
		//    CurrentSector->iPVS[outro]=true&&(visivel);

		}

	      //E
	      visivel=true;
	      for (x=c;x<pesq.x1;x++)
		{
		  outro=sec(z,linha,x);
		  if (outro==0)
		    visivel=false;
		  //		  else
		//    CurrentSector->iPVS[outro]=true&&(visivel);
		}


	      //N
	      visivel=true;
	      for (y=linha;y<pesq.y0;y--)
		{
		  outro=sec(z,y,c);
		  if (outro==0)
		    visivel=false;
		  //		  else
		 //   CurrentSector->iPVS[outro]=true&&(visivel);

		}

	      //S
	      visivel=true;
	      for (y=linha;y<pesq.y1;y++)
		{
		  outro=sec(z,y,c);
		  if (outro==0)
		    visivel=false;
		  //		  else
		//    CurrentSector->iPVS[outro]=true&&(visivel);
		}



	      //FLOOR
	      visivel=true;
	      for (y=z;y>0;y--)
		{
		  outro=sec(y,linha,c);
		  if (outro==0)
		    visivel=false;
		  //		  else
		 //  CurrentSector->iPVS[outro]=true&&(visivel);

		}

	      //CEILING
	      visivel=true;
	      for (y=z;y<floors;y++)
		{
		  outro=sec(y,linha,c);
		  if (outro==0)
		    visivel=false;
		  //		  else
		//    CurrentSector->iPVS[outro]=true&&(visivel);
		}


	      //diagonais

	      //apesar de estar funcionando, ao ver de um setor fora desta diagonal, ele afetou
	      //a PVS de outro setor.
	      visivel=true;
	      for (y=linha;y>pesq.y0;y--)
		{
		  outro=sec(z,y,y);
		  if (outro==0)
		    visivel=false;
		  //	  else
		//    CurrentSector->iPVS[outro]=true&&(visivel);
		}



	      //apesar de estar funcionando, ao ver de um setor fora desta diagonal, ele afetou
	      //a PVS de outro setor.


	      visivel=true;
	      for (y=linha;y<pesq.y1;y++)
		{
		  outro=sec(z,y,y);
		  if (outro==0)
		    visivel=false;

		//  CurrentSector->iPVS[outro]=true&&(visivel);
		}



	      //ruim
	      visivel=true;
	      for (y=linha;y>pesq.y0;y--)
		{
		  outro=sec(z,y,linha-y);
		  if (outro==0)
		    visivel=false;
		  // else
		//    CurrentSector->iPVS[outro]=true&&(visivel);
		}



	      //ruim
	      visivel=true;
	      for (y=linha;y<pesq.y1;y++)
		{
		  outro=sec(z,y,linha-y);
		  if (outro==0)
		    visivel=false;
		  //		  else
		//    CurrentSector->iPVS[outro]=true&&(visivel);
		}



	    }
	}

  pesq.sect();
}




bool
verifica_v(rect &pesq)
{
  int c;
  int lastx=pesq.x0;
  int lasty=pesq.y0;
  int linha=c;
  int coluna;

  for (coluna=pesq.x0;coluna<pesq.x1;coluna++)
    {
      for (c=pesq.y0;c<pesq.y1;c++)
	{
	  if (fig(0,c,coluna)!=pesq.padrao)
	    {
	      pesq.x1=lastx-1;
	      pesq.y1=lasty-1;
	      pesq.divx=coluna;
	      pesq.divy=c;
	      pesq.outro=fig(0,c,coluna);
	      return false;
	    }
	  lastx=coluna;
	}
      lasty=c;
    }
  return true;

}

void
scan(rect &pesq)
{
  rect *last=&pesq;
  rect pesquisa(0,0,0,0,0,0,0);
  rect pesquisa2(0,0,0,0,0,0,0);
  rect pesquisa3(0,0,0,0,0,0,0);
  //  pesquisa.mat=fig;
  //  pesquisa2.mat=fig;
  //  pesquisa3.mat=fig;
  memcpy(&pesquisa,&pesq,sizeof(rect));
  while (!verifica_h(pesquisa))
    {

      memcpy(&pesquisa2,&pesquisa,sizeof(rect));
      pesquisa2.padrao=0;
      pesquisa2.x0=pesquisa.divx-1;
      pesquisa2.y0=pesquisa.divy;
      pesquisa2.x1=pesq.x1;
      pesquisa2.y1=pesq.y1;

      verifica_h(pesquisa2);


      memcpy(&pesquisa3,&pesquisa,sizeof(rect));
      pesquisa3.padrao=0;
      pesquisa3.x0=pesquisa.divx;
      pesquisa3.y0=pesquisa.divy-1;
      pesquisa3.x1=pesq.x1;
      pesquisa3.y1=pesq.y1;

      verifica_v(pesquisa3);

      //pesquisa2.print();
      //      pesquisa3.print();
      //apaga area
      pesquisa2.x1=pesquisa2.divx+1;
      pesquisa2.y1=pesquisa3.divy+1;
      //   pesquisa2.print();


      pesquisa2.clear();



      last->next_ptr=new rect(0,0,0,0,0,0,0);
      memcpy(last->next_ptr,&pesquisa2,sizeof(rect));
      last=last->next_ptr;
      last->next_ptr=NULL;

      memcpy(&pesquisa,&pesq,sizeof(rect));
      cout <<"la vou eu pra outro ciclo!"<<endl;
      pesq.show();
      //    return ;
    }

}






BZK_FixP enterdata(string aTitle,string aText,BZK_FixP aDefault)
{
  float entry=-347.0;
  cout << "####"<<aTitle<<"####"<<endl;
  cout << aText<<endl;
  cout << "valor padrao:"<<BZK_FastMath::Print(aDefault)<<endl;
  cin >> entry;
  cout << endl;
  if (entry==-347.0)
    return aDefault;
  else
    return BZK_FastMath::Real32toFix32(entry);
}




BZK_FixP modifydata(string aTitle,string aText,BZK_FixP aOriginal)
{
  float entry=-347.0;
  cout << "####"<<aTitle<<"####"<<endl;
  cout << aText<<endl;
  cout << "valor original:"<<BZK_FastMath::Print(aOriginal)<<endl;
  cin >> entry;
  cout << endl;
  if (entry==-347.0)
    return aOriginal;
  else
    return BZK_FastMath::Real32toFix32(entry);
}




void ModifySector(VAC_Sector &aSector)
{

  int c,value;
  BZK_FixP h1,h2;
  

  aSector.iRect.SetX0((modifydata(string("Setor"),string("ponto x0:"),aSector.iRect.GetX0())));
  aSector.iRect.SetY0((modifydata(string("Setor"),string("ponto y0:"),aSector.iRect.GetY0())));
  aSector.iRect.SetW((modifydata(string("Setor"),string("largura:"),aSector.iRect.GetW())));
  aSector.iRect.SetH((modifydata(string("Setor"),string("profundidade:"),aSector.iRect.GetH())));
  h1=modifydata(string("Setor"),string("piso:"),aSector.GetH0());
  h2=modifydata(string("Setor"),string("teto:"),aSector.GetH1());
  aSector.SetH0(h1);
  aSector.SetH1(h2);


  for (c=0;c<6;c++)
    {
      VAC_ColorRGBA  *color=&(aSector.iColor[c]);
      color->SetR(modifydata(string("cor do setor"),string("componente R:"),aSector.iColor[c].GetR()));
      color->SetG(modifydata(string("cor do setor"),string("componente G:"),aSector.iColor[c].GetG()));
      color->SetB(modifydata(string("cor do setor"),string("componente B:"),aSector.iColor[c].GetB()));
      color->SetA(modifydata(string("transparencia da cor do setor"),string("componente A:"),aSector.iColor[c].GetA()));
    }
  
  char buffer;
 for (c=0;c<6;c++)
    {
      sprintf(&buffer,"%d",c);
	//i was drunk or what?
      value=modifydata(string("Conex�o"),(string("face ")+string(&buffer)),/*BZK_FastMath::Int32toFix32*/(aSector.Get_Connection(c)));
      aSector.SetConnection(c,/*BZK_FastMath::Fix32toInt32*/(value));
    }
}





void
ImportGEO(char* aFilename)
{
/*
  int z0,z1;
  cout << "altura inicial"<<endl;
  cin >> z0;
  cout << "altura inicial"<<endl;
  cin >> z1;
  rect GeoFile(0,0,0,16,16,0,0);
  //  GeoFile.mat=fig;
  ifstream aFile(aFilename);
  GeoFile.readfile(aFile);
  scan(GeoFile);
  rect *carry=&GeoFile;//.next_ptr;
  VAC_Sector *lastSector=NULL;
  while (carry!=NULL)
    {
      lastSector=CurrentSector;
      CreateSector(*carry->ToSector(),z0,z1);
      if (lastSector!=NULL)
	{
	  lastSector->SetConnection(0,World.iSector->TotalItems()-1);
	  
	  lastSector->SetConnection(1,World.iSector->TotalItems()-1);
	  lastSector->SetConnection(2,World.iSector->TotalItems()-1);
	  lastSector->SetConnection(3,World.iSector->TotalItems()-1);
	  lastSector->SetConnection(4,World.iSector->TotalItems()-1);
	  lastSector->SetConnection(5,World.iSector->TotalItems()-1);
	  
	}
      carry=carry->next_ptr;
    }
*/

}



void
ImportVoxel(const char* aFilename)
{
  int resp;
  int c;
  int r;
  int g;
  int b;
  resp=0;
//  cout << "solidos sao 0 ou =>1?"<<endl;
//  cin >> resp;

  int criados=World.TotalSectors();
  rect GeoFile(resp,0,0,gridsizex,gridsizey,0,floors);
  //  GeoFile.mat=fig;
  ifstream _file(aFilename);

  _file >> PaletteTop;
  float temp;
  for (c=0;c<PaletteTop;c++)
    Palette[c].GetColor(_file);

  GeoFile.readfile(_file);
  InitVoxelPet(gridsizez);
  ScanVoxel(GeoFile);
  LinkVoxel(GeoFile);
//  SetPVS(GeoFile);
  cout<< endl<< World.TotalSectors()- criados << " setor(es) criado(s)"<<endl;
}



void
ExportGEO(char* aFilename)
{
  /*
  rect GeoFile(0,0,0,16,16);
  //  GeoFile.readfile(string(aFilename));
  //  scan(GeoFile);
  rect *carry=&GeoFile;//.next_ptr;
  VAC_Sector *lastSector=NULL;
  while (carry!=NULL)
    {
      lastSector=CurrentSector;
      CreateSector(*carry->ToSector());
      CurrentSector->SetHeights(0,10);
      if (lastSector!=NULL)
	{
	  lastSector->SetConnection(0,World.iSector->TotalItems()-1);
	}
      carry=carry->next_ptr;
    }
  */
}

void
ExportBZK2(char* aFilename)
{
  /*
  rect GeoFile(0,0,0,16,16);
  //  GeoFile.readfile(string(aFilename));
  //  scan(GeoFile);
  rect *carry=&GeoFile;//.next_ptr;
  VAC_Sector *lastSector=NULL;
  while (carry!=NULL)
    {
      lastSector=CurrentSector;
      CreateSector(*carry->ToSector());
      CurrentSector->SetHeights(0,10);
      if (lastSector!=NULL)
	{
	  lastSector->SetConnection(0,World.iSector->TotalItems()-1);
	}
      carry=carry->next_ptr;
    }
  */
}


void
ImportBZK1(char* aFilename)
{
  BZK1_Info Info;
  BZK1_File BzkFile(aFilename,PET,Info);
  int criados=World.TotalSectors();
  rect GeoFile(0,0,0,gridsizex,gridsizey,0,1);
  //  GeoFile.mat=fig;
  ifstream _file(aFilename);
  GeoFile.readfile(_file);
  ScanVoxel(GeoFile);
  LinkVoxel(GeoFile);
//  SetPVS(GeoFile);
  cout<< endl<< World.TotalSectors()- criados << " setor(es) criado(s)"<<endl;
}


int GetCommand()
{
  int option;
  char filename[13];
  string _name;
  int Index;


  while(1)
    {
      cout <<"**********"<<endl;
      cout <<"1 - novo setor"<<endl;
      cout <<"2 - modifica setor selecionado"<<endl;
      cout <<"3 - selectionar setor"<<endl;
      cout <<"4 - exibir"<<endl;
      cout <<"5 - nomeia setor"<<endl;
      cout <<"6 - carregar"<<endl;
      cout <<"7 - salvar"<<endl;
      cout <<"8 - Polycount"<<endl;
      cout <<"9 - importar arquivo de voxels"<<endl;
      cout <<"0 - sair"<<endl;
      cout <<"A - modo prompt"<<endl;
      cout <<">";
      cin >> option;

  switch(option)
  {
  case 1:
    
    CurrentSector=new VAC_Sector();
      cout << "ponteiro pro setor"<<(unsigned int)CurrentSector<<endl;
//      memset(CurrentSector,0,sizeof(VAC_Sector));
      World.AddSector(*CurrentSector);
      cout << "\nsetor criado";
      break;
  case 2:
    ModifySector(*CurrentSector);
    break;
  case 3:
    cout <<World.TotalSectors()<<" setores";
    cout<<"\nindex?";
    cin>>Index;
    CurrentSector=NULL;
    CurrentSector=World.GetSector(Index);
    if (CurrentSector==NULL) cout <<"\nSele��o falhou";
    break;
  case 4:
    cout <<World.ShowSector(*CurrentSector)<<endl;
    break;
    // case 5:	
    //cout << "new name?"<<endl;
    //cin>>_name;
//	break;	
  case 6:
    cout << "filename?";
    cin>>filename;
    World.LoadMap((STOREPATH+string("/")+string(filename)),People);
    CurrentSector=World.GetSector(0);
    break;
  case 7:
    cout << "filename?";
    cin>>filename;
    cout << "verificando PVS..."<<endl;
    //    CheckAndFixPVS();
    cout << "gravando arquivo..."<<endl;
    World.SaveMap((char*)(STOREPATH+string("/")+string(filename)).c_str(),People);
    break;
  case 8:
    cout <<"o mapa atual cont�m "<<Polycount()<<" poligonos"<<endl;
    break;
 case 9:
   cout << "filename?";
   cin>>filename;
   ImportVoxel((STOREPATH+string("/")+string(filename)).c_str());
   break;
  //case 5 :
    //Prompt();
    

  case 0:
    return 0;
    break;
  }
    }
  
}








void
HandleInstruction(BZK_Vector<string> &aArg)
{
  bool consumed=false;
  cout << "decodificando parametros"<<endl;
	if  (aArg[0]==string("divide"))
			{
			  consumed=true;
			  World.LoadMap(STOREPATH+string("/")+aArg[1],People);
			  ofstream SVG((STOREPATH+string("/")+aArg[1]+string("_quadtree.svg")).c_str());
			  WriteSVGHeader(SVG);
			  subdivide(0,255,0,255,HORIZONTAL,SVG);
			  for (int c=0;c<indivisiveis.size();c++)
				{
					for (int d=0;d<indivisiveis[c]->size();d++)
						{
						cout << (*indivisiveis[c])[d]<<";";
						}
					cout <<endl;
				}

			  SVG << "</svg>"<<endl;
			}
	if  (aArg[0]==string("create"))
			{
			  consumed=true;
			World.LoadMap(STOREPATH+string("/")+aArg[1],People);
			////VAC_LogoLang::Translate(aArg,World,People);
			//CheckAndFixPVS();
			World.SaveMap((char*)(STOREPATH+string("out.bzk2")).c_str(),People);			
			}
	if (aArg[0]==string("import"))
		{
		  consumed=true;
		ImportVoxel((char*)(STOREPATH+string("/")+aArg[1]).c_str());
		//ExportSVG((STOREPATH+string("/")+aArg[1]+".svg").c_str());
		//CheckAndFixPVS();
		World.SaveMap((char*)(STOREPATH+aArg[1]+string(".bzk2")).c_str(),People);
		}

	if (aArg[0]==string("geo2svg"))
		{
		  consumed=true;
		ImportVoxel((char*)(STOREPATH+string("/")+aArg[1]).c_str());
		ExportSVG((STOREPATH+string("/")+aArg[1]+".svg").c_str());
		//CheckAndFixPVS();
		//		World.SaveMap((char*)(STOREPATH+aArg[1]+string(".bzk2")).c_str(),People);
		}

	if (aArg[0]==string("exportSVG"))
		{
		  consumed=true;
		  World.LoadMap(STOREPATH+string("/")+aArg[1],People);
		  ofstream SVG((char*)(STOREPATH+aArg[1]+string(".svg")).c_str());		 
		  WriteSVGHeader(SVG);
		  World.NotifyEndOfCycle();
		  for (int c=0;c<World.TotalSectors();c++)
		    VisitAndExportSVG(c,SVG);
		  SVG << "</svg>" <<endl;
		}

	if (aArg[0]==string("export"))
		{
		  consumed=true;
		  World.LoadMap(STOREPATH+string("/")+aArg[1],People);
		//  Export_OBJ((char*)(STOREPATH+aArg[1]+string(".obj")).c_str());		 
		}

	if (aArg[0]==string("ImportOBJ"))
		{
		  consumed=true;
	//	  ImportOBJ(aArg[1],aArg[2]);
		}

	if (aArg[0]==string("optimize"))
		{
		  consumed=true;
		  World.LoadMap(aArg[1],People);
		  cout << "starting optimization"<<endl;
		  Optimize();
		  World.SaveMap((char*)(aArg[1]+string(".opt")).c_str(),People);
		}
/*
	if (aArg[0]==string("makeirregular"))
		{
		  consumed=true;
		  World.LoadMap(STOREPATH+string("/")+aArg[1],People);
		  cout << "starting optimization"<<endl;
		  MakeIrregular();
		  cout << "saving "<<((char*)(STOREPATH+string("opt.")+aArg[1]).c_str())<<"..."<<endl;
		  World.SaveMap((char*)(STOREPATH+aArg[1]+string(".irr")).c_str(),People);
		}
*/
	if (aArg[0]==string("lamamud"))
		{
		  consumed=true;
		  World.LoadMap(STOREPATH+string("/")+aArg[1],People);
		  Export_Lama((char*)(STOREPATH+aArg[1]+string(".cpp")).c_str());		 
		}

	if (aArg[0]==string("color"))
		{
		  consumed=true;
		World.LoadMap(STOREPATH+string("/")+aArg[1],People);
		//VAC_LogoLang::Translate(aArg,World,People);
		//CheckAndFixPVS();
		World.SaveMap((char*)(STOREPATH+string("out.bzk2")).c_str(),People);
		}
		
	if (aArg[0]==string("SetTrigger"))
		{
		  cout << "ajustando trigger" <<endl;
		  consumed=true;
		World.LoadMap(STOREPATH+string("/")+aArg[1],People);
		//VAC_LogoLang::Translate(aArg,World,People);
/*
		//ainda vai funcionar...
		CurrentSector=World.GetSector(atoi(argv[4]));
		CurrentSector->SetConnection(atoi(argv[5]),atoi(argv[2]));
*/
//		CheckAndFixPVS();
		World.SaveMap((char*)(STOREPATH+string("out.bzk2")).c_str(),People);		
		}

	if (aArg[0]==string("connection"))
		{
		  consumed=true;
		World.LoadMap(STOREPATH+string("/")+aArg[1],People);
		//VAC_LogoLang::Translate(aArg,World,People);
/*
		//ainda vai funcionar...
		CurrentSector=World.GetSector(atoi(argv[4]));
		CurrentSector->SetConnection(atoi(argv[5]),atoi(argv[2]));
*/
		//CheckAndFixPVS();
		World.SaveMap((char*)(STOREPATH+string("out.bzk2")).c_str(),People);		
		}

	if (aArg[0]==string("Look"))
	  {  
	    consumed=true;
	    World.LoadMap(STOREPATH+string("/")+aArg[1],People);
	   
	//    cout << //VAC_LogoLang::Translate(aArg,World,People) <<endl;	    
	  }

	if (aArg[0]==string("AddActor"))
		{
		  consumed=true;
		  cout << "carregando mapa"<<endl;

		  World.LoadMap(STOREPATH+string("/")+aArg[1],People);
		  //VAC_LogoLang::Translate(aArg,World,People);
		  //CheckAndFixPVS();
		World.SaveMap((char*)(STOREPATH+string("out.bzk2")).c_str(),People);		
		cout << "salvando em "<<((char*)(STOREPATH+string("out.bzk2")).c_str())<<endl;
		}

	if (aArg[0]==string("RemoveActor"))
		{
		  consumed=true;
		  cout << "carregando mapa"<<endl;

		  World.LoadMap(STOREPATH+string("/")+aArg[1],People);
		 
		 
		  //VAC_LogoLang::Translate(aArg,World,People);
		  //CheckAndFixPVS();
		World.SaveMap((char*)(STOREPATH+string("out.bzk2")).c_str(),People);		
		cout << "salvando em "<<((char*)(STOREPATH+string("out.bzk2")).c_str())<<endl;
		}


	if (aArg[0]==string("MoveActor"))
		{
		  consumed=true;
		  cout << "carregando mapa"<<endl;

		  World.LoadMap(STOREPATH+string("/")+aArg[1],People);
		  //VAC_LogoLang::Translate(aArg,World,People);
		  //CheckAndFixPVS();
		World.SaveMap((char*)(STOREPATH+string("out.bzk2")).c_str(),People);		
		cout << "salvando em "<<((char*)(STOREPATH+string("out.bzk2")).c_str())<<endl;
		}


	if (aArg[0]==string("SetPlayer"))
		{
		  consumed=true;
		  cout << "carregando mapa"<<endl;

		  World.LoadMap(STOREPATH+string("/")+aArg[1],People);
		  //VAC_LogoLang::Translate(aArg,World,People);
		  //CheckAndFixPVS();
		World.SaveMap((char*)(STOREPATH+string("out.bzk2")).c_str(),People);		
		cout << "salvando em "<<((char*)(STOREPATH+string("out.bzk2")).c_str())<<endl;
		}


	if (aArg[0]==string("ListActors"))
		{
		  consumed=true;
		  cout << "carregando mapa"<<endl;

		  World.LoadMap(STOREPATH+string("/")+aArg[1],People);
		  int c=0;
		  cout << "=========atores:"<<People.GetActorsCount()<<"=============="<<endl;
		  for (c=0;c<People.GetActorsCount();c++)
		    {
		      //		      cout << "obtendo ator "<<c<<endl; 
		      
		        BZK_Actor *handler=People.GetActorPtr(c);  

			cout << handler->GetID()<<"\t"<<handler->GetName() <<"\t"<<handler->GetCurrentSector();
		        if (People.GetActivePlayerPtr()->GetID()==handler->GetID()) cout << " (player)";
			
			cout << endl;
		    }
		}

	if (aArg[0]==string("Exit"))
		{
		  consumed=true;
		World.LoadMap(STOREPATH+string("/")+aArg[1],People);
		//VAC_LogoLang::Translate(aArg,World,People);
/*
		//ainda vai funcionar...
		CurrentSector=World.GetSector(atoi(argv[4]));
		CurrentSector->SetConnection(atoi(argv[5]),atoi(argv[2]));

		CheckAndFixPVS();*/
		World.SaveMap((char*)(STOREPATH+string("out.bzk2")).c_str(),People);		
		}

	if (aArg[0]==string("version"))
		{
		  consumed=true;
		cout << "0.7a"<<endl;
		}

	if (aArg[0]==string("license"))
		{
		  consumed=true;
		cout << "soon to be implemented - please check the General Public License v3 on the internet"<<endl;
		}


	if (aArg[0]==string("show"))
		{
		  consumed=true;
		World.LoadMap(STOREPATH+string("/")+aArg[1],People);
		CurrentSector=World.GetSector(atoi(aArg[2].c_str()));
		cout << "saida textual do setor" << atoi(aArg[2].c_str())<<endl;
		cout << World.ShowSector(*CurrentSector)<<endl;	
		}



	if (aArg[0]==string("quit"))
		{
		  consumed=true;
		return;
		}

	if (aArg[0]==string("volume"))
		{
		  consumed=true;
		World.LoadMap(STOREPATH+string("/")+aArg[1],People);
		//VAC_LogoLang::Translate(aArg,World,People);
	//	CheckAndFixPVS();
		World.SaveMap((char*)(STOREPATH+string("out.bzk2")).c_str(),People);
		}

	if (aArg[0]==string("extrude_create"))
	  {
	    consumed=true;
	    World.LoadMap(STOREPATH+string("/")+aArg[1],People);
	    //VAC_LogoLang::Translate(aArg,World,People);	    
	//    CheckAndFixPVS();
	    World.SaveMap((char*)(STOREPATH+string("out.bzk2")).c_str(),People);
	    
	  }

	if (aArg[0]==string("GeoExportPaper"))
	{
		  consumed=true;
		ImportVoxel((char*)(STOREPATH+string("/")+aArg[1]).c_str());
		ExportSVGPaper((STOREPATH+string("/")+aArg[1]+".svg").c_str());
	}

	if (aArg[0]==string("help")||!consumed)
		{
		  if (!consumed)
		    cout << "** comando n�o reconhecido! **"<<endl<<endl;

		cout << "CONsole Bzk EDitor - por TIME BZK (autor original: Daniel \"NeoStrider\" Monteiro)" <<endl;
		cout << "Sob GNU GPL"<<endl;
		cout << "sintaxe:"<<endl;
		cout << "conbed - abre o editor e o executa no modo \"escutador\", com um menu e etc"<<endl;
		cout << "conbed help - mostra esta tela"<<endl;
		cout << "conbed version - mostra a versao"<<endl;
		cout << "conbed SetTrigger arquivo setor trigger(numero) - define trigger do setor"<<endl;
		cout << "conbed AddActor arquivo nome setor classe - adiciona ator de nome indicado no setor indicado"<<endl;
		cout << "conbed RemoveActor arquivo ID - remove ator de ID indicado"<<endl;
		cout << "conbed ListActors arquivo - lista atores, seus IDs e setores de come�o"<<endl;
		cout << "conbed MoveActor arquivo nome setor - move ator de nome indicado para o setor indicado"<<endl;

		cout << "conbed exit arquivo setor arquivo- cria uma saida de um setor para outro mapa"<<endl;
		cout << "conbed export arquivo - exporta o mapa contido em <arquivo> para out.obj , no formato Alias Wavefront OBJ"<<endl;
		cout << "conbed export arquivo - cospe codigo fonte no formato do lama"<<endl;
		cout << "conbed exportSVG arquivo - exporta o mapa contido em <arquivo> , no formato SVG"<<endl;
		cout << "conbed GeoExportPaper arquivo - exporta o mapa contido em <arquivo> , no formato SVG, para maquetes em papel"<<endl;
		cout << "conbed geo2svg arquivo - exporta o voxel (.geo) contido em <arquivo> , no formato SVG"<<endl;
		cout << "conbed license - mostra a licenca de uso do Conbed (GPL)"<<endl;
		cout << "conbed optimize mapa- otimiza mapa"<<endl;
		cout << "conbed import arquivo - importa os voxels contidos no arquivo e transforma em mapa BZK2"<<endl;
		cout << "conbed show arquivo x - mostra informacoes sobre o setor x"<<endl;
		cout << "conbed create arquivo - cria setor com dados padrao no arquivo especificado"<<endl;
		cout << "conbed volume arquivo setor x0 y0 h w h1 h2 - modifica volume do setor dado, no arquivo dado"<<endl;
		cout << "conbed color arquivo setor face R G B A - ajusta a cor da face dada,do dado setor, no arquivo especificado"<<endl;
		cout << "conbed connection arquivo setor face setor face  - determina a conexao entre os dois setores dados"<<endl;
		cout << "todas as opcoes que modificam o arquivo original - todas menos help e show - salvam o resultado no arquivo out.bzk2"<<endl;
		}
}

int main(int argc, char **argv)
{

  // system("clear");
  //  freopen( "CON", "w", stdout ); //in order to work under windows, uncomment this line
 
  CurrentSector=NULL;
  argc--;
  argv++;
  BZK_Vector<string> cmd;
   if (!argc)
	  GetCommand();
	else
	{
	argc++;
	while (--argc) 
		{
		string *ptr=new string(argv[0]);
		cmd.Add(*ptr);
		argv++;
		}
	cout << "agora que peguei os parametros, hora de ordenalos"<<endl;
	HandleInstruction(cmd);
//	while (!_cmd.Empty()) 		cmd.Add(_cmd.Pop());

	}
World.CleanWorld();
return 0;
}

 //---------------------------------------------------------------------------------------
long  Polycount ()
   {
    
	return World.Polycount();
   }
 //---------------------------------------------------------------------------------------
/*
void  Export_OBJ (string aFilename)
   {
     int where;
     int counter;
     int c=0;
     int counter2;
     int counter3=1;
     VAC_PolyBuffer temp;
     VAC_Sector *ThisSector=NULL;
     VAC_Render rnd;
	for (c=1;c<World.TotalSectors();c++)
	{
	  //	  cout << "setor "<<c<<endl;
	ThisSector=World.GetSector(c);
	if (ThisSector==NULL) 
		{
		cout << "NULL pointer!"<<endl;		
		}

	//cout << "extraindo faces" <<endl;
	for (counter=0;counter<6;counter++)		
	  if (ThisSector->Get_Connection(counter)==0)
	    {
	      //    cout << "extraindo uma face" <<endl;
	      temp.Push(rnd.JustExtractFace(*ThisSector,counter));
	      
	    }					 
	}
	
	temp.Externalize(aFilename);
   }
*/
 //---------------------------------------------------------------------------------------
void  Export_Lama (string aFilename)
   {

   }


bool ColorEqual (BZK_Color4f c1,BZK_Color4f c2)
{
  if (c1.GetC0()==c2.GetC0() &&
      c1.GetC1()==c2.GetC1() &&
      c1.GetC2()==c2.GetC2() &&
      c1.GetC3()==c2.GetC3())
    return true;
  return false;
}

bool Similar (int aSector,int aFace,int aSector2)
{
  VAC_Sector *sector[2];
  sector[0]=World.GetSector(aSector);
  sector[1]=World.GetSector(aSector2);




  switch (aFace)
    {
    case N:
      if (ColorEqual(sector[0]->iColor[4],sector[1]->iColor[4]) && 
	  ColorEqual(sector[0]->iColor[5],sector[1]->iColor[5]) && 
	  ColorEqual(sector[0]->iColor[1],sector[1]->iColor[1]) && 
	  ColorEqual(sector[0]->iColor[3],sector[1]->iColor[3]) )

	return true;
    }
  
}

BZK_Vector<BZK_Vector<int>*> grupos; 

BZK_Vector<int>* FindGroupByMember(int Sector)
{
  int c=0;
  int d=0;
  BZK_Vector<int> *group;
  // cout << "groups:"<<grupos.TotalItems()<<endl;
  for (c=0;c<grupos.TotalItems();c++)
    {
      group=grupos[c];
      for (d=0;d<group->TotalItems();d++)
	{
	  //  cout << "vasculhando grupo "<<c<<" , pos:"<<d<< " em busca de "<<Sector<<endl;
	  if ((*group)[d]==Sector) return group;
	}
    }

  return NULL;
}

void PrintGroup(BZK_Vector<int> *group)
{
  for (int d=0;d<group->TotalItems();d++)
    {
      cout <<(*group)[d]<<";";
      // if ((*group)[d]==Sector) return group;
    }
  cout <<endl;
}




void _visitandanalyze(int sector,int from)
{
  int x;
  BZK_Vector<int> *ptr;
  if ( World.GetSector(sector)->HasBeenDrawn())
    return;
  cout << "visitando "<<sector<<endl;
  World.GetSector(sector)->Visiting();
  for (int c=0;c<6;c++)
    if (World.GetSector(sector)->Get_Connection(c) !=0 && !World.GetSector(World.GetSector(sector)->Get_Connection(c))->HasBeenDrawn() )
      {
	if ( Similar(sector,World.GetSector(sector)->Get_Connection(c),c))
	  {
	  cout <<endl<<sector<<" e "<<World.GetSector(sector)->Get_Connection(c)<< " sao similares" <<endl;
	  if (FindGroupByMember(sector)==NULL && FindGroupByMember(World.GetSector(sector)->Get_Connection(c))==NULL)
	    {
	      cout << "criando grupo para "<<sector<<" e "<< World.GetSector(sector)->Get_Connection(c) <<endl;
	      ptr=new BZK_Vector<int>();
	      ptr->Add(sector);
	      ptr->Add(World.GetSector(sector)->Get_Connection(c));
	      grupos.Add(ptr);
	    }
	    else
	      {
	
		ptr=FindGroupByMember(World.GetSector(sector)->Get_Connection(c));
		if (ptr==NULL)
		  {
		    ptr=FindGroupByMember(sector);
		    cout << "achei grupo de "<<sector <<endl;
		    ptr->Add(World.GetSector(sector)->Get_Connection(c));
		  }
		else
		  {
		    cout << "achei grupo de "<<World.GetSector(sector)->Get_Connection(c) <<endl;
		    ptr->Add(sector);
		  }
		  
				
	      }
	  }
      _visitandanalyze(World.GetSector(sector)->Get_Connection(c),c);       
      }
      
}







int HowMany(VAC_Sector*aSector,int dir)
{
cout << "how many" <<endl;
int count=1;
int aux=0;
VAC_Sector *ptr[2];
ptr[0]=aSector;
while (ptr[0]!=NULL && ptr[0]->Get_Connection(dir)!=0)
	{
	ptr[0]=World.GetSector(ptr[0]->Get_Connection(dir));
	ptr[1]=ptr[0];
	++count;
	++aux;
	while (ptr[1]!=NULL && ptr[1]->Get_Connection(BZK_FastMath::GetLeft(dir))!=0)
		{
		cout << "<-" << endl;
		ptr[1]=World.GetSector(ptr[1]->Get_Connection(BZK_FastMath::GetLeft(dir)));
		++aux;
		}
	ptr[1]=ptr[0];
	while (ptr[1]!=NULL && ptr[1]->Get_Connection(BZK_FastMath::GetRight(dir))!=0)
		{
		cout << "->" << endl;
		ptr[1]=World.GetSector(ptr[1]->Get_Connection(BZK_FastMath::GetRight(dir)));
		++aux;
		}
	ptr[1]=ptr[0];
	while (ptr[1]!=NULL && ptr[1]->Get_Connection(FLOOR)!=0)
		{
		cout << "\\/" << endl;
		ptr[1]=World.GetSector(ptr[1]->Get_Connection(FLOOR));
		++aux;
		}
	ptr[1]=ptr[0];
	while (ptr[1]!=NULL && ptr[1]->Get_Connection(CEILING)!=0)
		{
		cout << "/\\" << endl;
		ptr[1]=World.GetSector(ptr[1]->Get_Connection(CEILING));
		++aux;
		}
	
	}
if (aux>count) count=aux;
return count;
}

void CalcTrunkDepth()
{
VAC_Sector *ptr[3];
int bigger=0;
int candidate=0;

for (int c=1;c<World.TotalSectors();c++)
	for (int d=0;d<2;d++)
		{
		cout << "c:"<<c<<" d:"<<d<<endl;
		ptr[0]=World.GetSector(c);
		if (ptr[0]->iParent==-1)
			continue;
		ptr[1]=ptr[0];
/*
		while (ptr[1]!=NULL && ptr[1]->Get_Connection(BZK_FastMath::GetOpositeDirection(d))!=0)
			ptr[1]=World.GetSector(ptr[1]->Get_Connection(BZK_FastMath::GetOpositeDirection(d)));
*/
		if (ptr[1]==NULL)
			continue;
		candidate=HowMany(ptr[1],d);
		if (candidate>bigger)
			bigger=candidate;			
		}
World.iSugestedBufferSize=bigger;
}



void CalcLimits()
{
CalcTrunkDepth();
}


void Split()
{
 int e,f,g,index;
  int maxsize=SIZESECTOR;
	
  VAC_Sector *ptr[3];
  bool repetir;
	int repeticoes=0;
  bool modificou=false;

  for (int c=1;c<World.TotalSectors();c++)
    {
      ///debug/////////
     // if (c>30) exit(0);
      //////////////////
      cout <<c<<","<<World.TotalSectors()<<";" ;
      ptr[0]=World.GetSector(c);
	cout << "X:tamanho de ptr[0]"<<ptr[0]->Rect()->GetX0()<<","<<ptr[0]->Rect()->GetX1()<<endl;
		    if (ptr[0]->Rect()->GetX1()-ptr[0]->Rect()->GetX0()>maxsize)
			{
				cout << "diminuindo por X"<<endl;
				ptr[2]=new VAC_Sector();
				  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
				ptr[2]->Rect()->SetX0(ptr[0]->Rect()->GetX0()+maxsize);
				ptr[2]->Rect()->SetX1(ptr[0]->Rect()->GetX1());
				ptr[2]->Rect()->SetY1(ptr[0]->Rect()->GetY1());

				ptr[0]->Rect()->SetX1(ptr[0]->Rect()->GetX0()+maxsize);
				cout << "novo tamanho de ptr[0]"<<ptr[0]->Rect()->GetX0()<<","<<ptr[0]->Rect()->GetX1()<<endl;
				cout << "novo tamanho de ptr[2]"<<ptr[2]->Rect()->GetX0()<<","<<ptr[2]->Rect()->GetX1()<<endl;
				  World.AddSector(*ptr[2]);
				    g=World.TotalSectors()-1;
				
				for (int d=0;d<6;d++)
					ptr[2]->SetConnection(d,ptr[0]->Get_Connection(d));

				ptr[0]->SetConnection(W,g);
                                ptr[2]->SetConnection(E,c);

				modificou=true;
				for (int o=0;o<50;o++)
				  cout << '*' ;
				cout <<endl;
				//Split();
				//	exit(0);
			}
		    cout <<c<<","<<World.TotalSectors()<<";" ;
		cout << "Y:tamanho de ptr[0]"<<ptr[0]->Rect()->GetY0()<<","<<ptr[0]->Rect()->GetY1()<<endl;
		    if (ptr[0]->Rect()->GetY1()-ptr[0]->Rect()->GetY0()>maxsize)
			{
				cout << "diminuindo por Y"<<endl;
				ptr[2]=new VAC_Sector();
				  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
				ptr[2]->Rect()->SetY0(ptr[0]->Rect()->GetY0()+maxsize);
				ptr[2]->Rect()->SetX0(ptr[0]->Rect()->GetX0());
				ptr[2]->Rect()->SetY1(ptr[0]->Rect()->GetY1());

				ptr[0]->Rect()->SetY1(ptr[0]->Rect()->GetY0()+maxsize);
				cout << "novo tamanho de ptr[0]"<<ptr[0]->Rect()->GetY0()<<","<<ptr[0]->Rect()->GetY1()<<endl;
				cout << "novo tamanho de ptr[2]"<<ptr[2]->Rect()->GetY0()<<","<<ptr[2]->Rect()->GetY1()<<endl;
				  World.AddSector(*ptr[2]);
				    g=World.TotalSectors()-1;

				for (int d=0;d<6;d++)
					ptr[2]->SetConnection(d,ptr[0]->Get_Connection(d));
				ptr[0]->SetConnection(S,g);
    			        ptr[2]->SetConnection(N,c);

				modificou=true;
				for (int o=0;o<50;o++)
				  cout << '*';
				cout <<endl;
				//Split();
				//exit(0);
			}
	}
if (modificou) Split();
}
/////////////////
void ReLink()
{
  VAC_Sector *ptr[4];
  for (int c=1;c<World.TotalSectors();c++)
    {

	cout << "c:"<<c<<endl;
	//caso 1
     ptr[0]=World.GetSector(c);
	for (int e=0;e<6;e++)
		ptr[0]->SetConnection(e,0);

       for (int d=1;d<World.TotalSectors();d++)
	{
		if (c==d) continue;
	     ptr[1]=World.GetSector(d);
	     if (ptr[0]->GetX0()==ptr[1]->GetX0()&&ptr[0]->GetX1()==ptr[1]->GetX1()&&ptr[0]->GetY0()==ptr[1]->GetY1()
		 &&ptr[0]->GetH0()==ptr[1]->GetH0()&&ptr[0]->GetH1()==ptr[1]->GetH1())
			{
				ptr[0]->SetConnection(0,d);
				ptr[1]->SetConnection(2,c);
				continue;
			}

		if (ptr[0]->GetY0()==ptr[1]->GetY0()&&ptr[0]->GetY1()==ptr[1]->GetY1()&&ptr[0]->GetX1()==ptr[1]->GetX0()&&ptr[0]->GetH0()==ptr[1]->GetH0()&&ptr[0]->GetH1()==ptr[1]->GetH1())
			{
				ptr[0]->SetConnection(3,d);
				ptr[1]->SetConnection(1,c);
				continue;
			}

		if (ptr[0]->GetX0()==ptr[1]->GetX0()&&ptr[0]->GetX1()==ptr[1]->GetX1()&&ptr[0]->GetY1()==ptr[1]->GetY0()&&ptr[0]->GetH0()==ptr[1]->GetH0()&&ptr[0]->GetH1()==ptr[1]->GetH1())
			{
				ptr[0]->SetConnection(2,d);
				ptr[1]->SetConnection(0,c);
				continue;
			}

		if (ptr[0]->GetY0()==ptr[1]->GetY0()&&ptr[0]->GetY1()==ptr[1]->GetY1()&&ptr[0]->GetX0()==ptr[1]->GetX1()&&ptr[0]->GetH0()==ptr[1]->GetH0()&&ptr[0]->GetH1()==ptr[1]->GetH1())
			{
				ptr[0]->SetConnection(1,d);
				ptr[1]->SetConnection(3,c);
				continue;
			}

	if (ptr[0]->GetY0()==ptr[1]->GetY0()&&ptr[0]->GetY1()==ptr[1]->GetY1()&&ptr[0]->GetX0()==ptr[1]->GetX0()&&ptr[0]->GetX1()==ptr[1]->GetX1()&&ptr[0]->GetH1()==ptr[1]->GetH0())
			{
				ptr[0]->SetConnection(5,d);
				ptr[1]->SetConnection(4,c);
				continue;
			}

	if (ptr[0]->GetY0()==ptr[1]->GetY0()&&ptr[0]->GetY1()==ptr[1]->GetY1()&&ptr[0]->GetX0()==ptr[1]->GetX0()&&ptr[0]->GetX1()==ptr[1]->GetX1()&&ptr[0]->GetH0()==ptr[1]->GetH1())
			{
				ptr[0]->SetConnection(4,d);
				ptr[1]->SetConnection(5,c);
				continue;
			}

		/*
		era pra ser assim...
		if (ptr[0]->GetX0()==ptr[1]->GetX0()&&ptr[0]->GetX1()==ptr[1]->GetX1()&&ptr[0]->GetY0()==ptr[1]->GetY1())
			{
				ptr[0]->SetConnection(0,d);
				ptr[1]->SetConnection(2,c);
				continue;
			}

		if (ptr[0]->GetY0()==ptr[1]->GetY0()&&ptr[0]->GetY1()==ptr[1]->GetY1()&&ptr[0]->GetX1()==ptr[1]->GetX0())
			{
				ptr[0]->SetConnection(1,d);
				ptr[1]->SetConnection(3,c);
				continue;
			}

		if (ptr[0]->GetX0()==ptr[1]->GetX0()&&ptr[0]->GetX1()==ptr[1]->GetX1()&&ptr[0]->GetY1()==ptr[1]->GetY0())
			{
				ptr[0]->SetConnection(2,d);
				ptr[1]->SetConnection(0,c);
				continue;
			}

		if (ptr[0]->GetY0()==ptr[1]->GetY0()&&ptr[0]->GetY1()==ptr[1]->GetY1()&&ptr[0]->GetX0()==ptr[1]->GetX1())
			{
				ptr[0]->SetConnection(3,d);
				ptr[1]->SetConnection(1,c);
				continue;
			}
		*/


	}
    }

}

////////////////////////
void Optimize()
{

  int e,f,g,index;
  int maxsize=SIZESECTOR; //esse valor, na função que estiver, responde pela granularidade dos setores.
	f=0;
  VAC_Sector *ptr[3];
  bool repetir;
	int repeticoes=0;


  BZK_Vector<VAC_Sector*> aux;

  for (int c=1;c<World.TotalSectors();c++)
	{
	    ptr[0]=World.GetSector(c);
            ptr[2]=new VAC_Sector();
//	    memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
	    ptr[0]->iParent=c;
	    aux.Add(ptr[2]);
	}
	

  do
    {
	repeticoes++;
      repetir=false;
  for (int c=1;c<World.TotalSectors();c++)
    {
      ///debug/////////
     // if (c>30) exit(0);
      //////////////////
      ptr[0]=World.GetSector(c);
		//  cout << "setor "<<c<<":"<<endl;
	
	 // if (ptr[0]->Get_Connection(d)==0) continue;
	  for (int d=1;d<World.TotalSectors();d++)
	    {
 		if (c==d) continue;
                 ptr[1]=World.GetSector(d);

		 //cout << "ptr[1]="<<d<<endl;
		 
		 //////  X
		 ////////////////////////////////////////////////////////

			/*
			____		____
			| 1|		| 1 |
			|__|____	|___|______
			|   0	|  ->	| 0 |  2  |
			|_______|	|___|_____|   

			*/

		    if ( ptr[0]->GetX1() >  ptr[1]->GetX1() &&  ptr[0]->GetX0() ==  ptr[1]->GetX0() && ptr[0]->GetY0()-ptr[1]->GetY1()< BZK_FastMath::Int32toFix32(0))
			{	
			  repetir=true;
			  cout << "XY "<<c<<"-"<<d<<" caso 1" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  ptr[2]->SetX0(ptr[1]->GetX1());
			  ptr[2]->SetX1(ptr[0]->GetX1());
			  ptr[0]->SetX0(ptr[1]->GetX0());
			  ptr[0]->SetX1(ptr[2]->GetX0());			    
			  ptr[2]->SetY0(ptr[0]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());	
			  if (ptr[2]->GetX0()!=ptr[2]->GetX1() && ptr[2]->GetY0()!=ptr[2]->GetY1() && ptr[2]->GetH0()!=ptr[2]->GetH1())
			{
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			}
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
		            ____	     ______
			    |  1|	    | 1   |
			____|___|	____|_____|
			|   0	|  ->	| 0 |  2  |
			|_______|	|___|_____|   

			*/

		    if ( ptr[0]->GetX0() <  ptr[1]->GetX0() &&  ptr[0]->GetX1() ==  ptr[1]->GetX1() && ptr[0]->GetY0()-ptr[1]->GetY1()< BZK_FastMath::Int32toFix32(0))
			{
			  repetir=true;		
			  cout << "XY "<<c<<"-"<<d<<" caso 2" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
	  
			  ptr[2]->SetX0(ptr[1]->GetX0());
			  ptr[2]->SetX1(ptr[1]->GetX1());
			  //			  ptr[0]->SetX0(ptr[0]->GetX0());
			  ptr[0]->SetX1(ptr[2]->GetX0());			    
			  ptr[2]->SetY0(ptr[0]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());			   
			  if (ptr[2]->GetX0()!=ptr[2]->GetX1() && ptr[2]->GetY0()!=ptr[2]->GetY1() && ptr[2]->GetH0()!=ptr[2]->GetH1())
			{
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			}	
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
                        _________       ___________
			|   0	|  ->	| 0 |  2  |
			|_______|	|___|_____|
			    |  1|	    | 1   |
			    |___|	    |_____|


			*/

		    if ( ptr[0]->GetX0() <  ptr[1]->GetX0() &&  ptr[0]->GetX1() ==  ptr[1]->GetX1() && ptr[0]->GetY1()-ptr[1]->GetY0()< BZK_FastMath::Int32toFix32(0))
			{
			  repetir=true;		
			  cout << "XY "<<c<<"-"<<d<<" caso 3" <<endl;			  
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetX0(ptr[1]->GetX0());
			  ptr[2]->SetX1(ptr[1]->GetX1());
			  //			  ptr[0]->SetX0(ptr[0]->GetX0());
			  ptr[0]->SetX1(ptr[2]->GetX0());			    
			  ptr[2]->SetY0(ptr[0]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());			   
			  continue;
			}



		 ////////////////////////////////////////////////////////

			/*
                        _________       ___________
			|   0	|  ->	| 0   |2  |
			|_______|	|_____|___|
	                |  1|	        | 1   |
			|___|	        |_____|


			*/

		    if ( ptr[0]->GetX0() ==  ptr[1]->GetX0() &&  ptr[0]->GetX1() >  ptr[1]->GetX1() && ptr[0]->GetY1()-ptr[1]->GetY0()< BZK_FastMath::Int32toFix32(0))
			{		
			  repetir=true;
			  cout << "XY "<<c<<"-"<<d<<" caso 4" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetX0(ptr[1]->GetX1());
			  ptr[2]->SetX1(ptr[0]->GetX1());
			  //			  ptr[0]->SetX0(ptr[0]->GetX0());
			  ptr[0]->SetX1(ptr[2]->GetX0());			    
			  ptr[2]->SetY0(ptr[0]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());			   
			  continue;
			}
		    	 ////////////////////////////////////////////////////////

			/*
		          ____	           ____
		          |  1|	           | 1 |
			__|___|__	___|___|__
			|   0	|  ->	|0 |  2  |
			|_______|	|__|_____|
			+ recursivity.

			*/

		    if ( ptr[0]->GetX0() <  ptr[1]->GetX0() &&  ptr[0]->GetX1() >  ptr[1]->GetX1() && ptr[0]->GetY0()-ptr[1]->GetY1()< BZK_FastMath::Int32toFix32(0))
			{
			  repetir=true;		
			  cout << "XY "<<c<<"-"<<d<<" caso 5" <<endl;			 
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetX0(ptr[1]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());
			  //			  ptr[0]->SetX0(ptr[0]->GetX0());
			  ptr[0]->SetX1(ptr[2]->GetX0());			    
			  ptr[2]->SetY0(ptr[0]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());			   
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
                        _________       ___________
			|   0	|  ->	| 0|  2   |
			|_______|	|__|______|
		          |  1|	           | 1 |
			  |___|	           |___|

			  + recursivity.
			*/

		    if ( ptr[0]->GetX0() <  ptr[1]->GetX0() &&  ptr[0]->GetX1() >  ptr[1]->GetX1() && ptr[0]->GetY1()-ptr[1]->GetY0()< BZK_FastMath::Int32toFix32(0))
			{
			  repetir=true;		
			  cout << "XY "<<c<<"-"<<d<<" caso 6" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetX0(ptr[1]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());
			  //			  ptr[0]->SetX0(ptr[0]->GetX0());
			  ptr[0]->SetX1(ptr[2]->GetX0());			    
			  ptr[2]->SetY0(ptr[0]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());			   
			  continue;
			}

		 //////  Y
		 ////////////////////////////////////////////////////////

			/*
			____		____
			| 1|		| 1 |
			|__|____	|___|______
			|   0	|  ->	| 0 |  2  |
			|_______|	|___|_____|   

			*/

		    if ( ptr[0]->GetY1() >  ptr[1]->GetY1() &&  ptr[0]->GetY0() ==  ptr[1]->GetY0() && ptr[0]->GetX0()-ptr[1]->GetX1()< BZK_FastMath::Int32toFix32(0))
			{
			  repetir=true;	
			  cout << "XY "<<c<<"-"<<d<<" caso 7" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetY0(ptr[1]->GetY1());
			  ptr[2]->SetY1(ptr[0]->GetY1());
			  ptr[0]->SetY0(ptr[1]->GetY0());
			  ptr[0]->SetY1(ptr[2]->GetY0());			    
			  ptr[2]->SetX0(ptr[0]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());			   
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
		            ____	     ______
			    |  1|	    | 1   |
			____|___|	____|_____|
			|   0	|  ->	| 0 |  2  |
			|_______|	|___|_____|   

			*/

		    if ( ptr[0]->GetY0() <  ptr[1]->GetY0() &&  ptr[0]->GetY1() ==  ptr[1]->GetY1() && ptr[0]->GetX0()-ptr[1]->GetX1()< BZK_FastMath::Int32toFix32(0))
			{
			  repetir=true;		
			  cout << "XY "<<c<<"-"<<d<<" caso 8" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetY0(ptr[1]->GetY0());
			  ptr[2]->SetY1(ptr[1]->GetY1());
			  ptr[0]->SetY1(ptr[2]->GetY0());			    
			  ptr[2]->SetX0(ptr[0]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());			   
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
                        _________       ___________
			|   0	|  ->	| 0 |  2  |
			|_______|	|___|_____|
			    |  1|	    | 1   |
			    |___|	    |_____|


			*/

		    if ( ptr[0]->GetY0() <  ptr[1]->GetY0() &&  ptr[0]->GetY1() ==  ptr[1]->GetY1() && ptr[0]->GetX1()-ptr[1]->GetX0()< BZK_FastMath::Int32toFix32(0))
			{		
			  repetir=true;
			  cout << "XY "<<c<<"-"<<d<<" caso 9" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetY0(ptr[1]->GetY0());
			  ptr[2]->SetY1(ptr[1]->GetY1());
			  ptr[0]->SetY1(ptr[2]->GetY0());			    
			  ptr[2]->SetX0(ptr[0]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());			   
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
                        _________       ___________
			|   0	|  ->	| 0   |2  |
			|_______|	|_____|___|
	                |  1|	        | 1   |
			|___|	        |_____|


			*/

		    if ( ptr[0]->GetY0() ==  ptr[1]->GetY0() &&  ptr[0]->GetY1() >  ptr[1]->GetY1() && ptr[0]->GetX1()-ptr[1]->GetX0()< BZK_FastMath::Int32toFix32(0))
			{		
			  repetir=true;
			  cout << "XY "<<c<<"-"<<d<<" caso 10" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetY0(ptr[1]->GetY1());
			  ptr[2]->SetY1(ptr[0]->GetY1());
			  ptr[0]->SetY1(ptr[2]->GetY0());			    
			  ptr[2]->SetX0(ptr[0]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());			   
			  continue;
			}
		    	 ////////////////////////////////////////////////////////

			/*
		          ____	           ____
		          |  1|	           | 1 |
			__|___|__	___|___|__
			|   0	|  ->	|0 |  2  |
			|_______|	|__|_____|
			+ recursivity.

			*/

		    if ( ptr[0]->GetY0() <  ptr[1]->GetY0() &&  ptr[0]->GetY1() >  ptr[1]->GetY1() && ptr[0]->GetX0()-ptr[1]->GetX1()< BZK_FastMath::Int32toFix32(0))
			{		
			  repetir=true;
			  cout << "XY "<<c<<"-"<<d<<" caso 11" <<endl;			  
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetY0(ptr[1]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());
			  ptr[0]->SetY1(ptr[2]->GetY0());			    
			  ptr[2]->SetX0(ptr[0]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());			   
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
                        _________       ___________
			|   0	|  ->	| 0|  2   |
			|_______|	|__|______|
		          |  1|	           | 1 |
			  |___|	           |___|

			  + recursivity.
			*/

		    if ( ptr[0]->GetY0() <  ptr[1]->GetY0() &&  ptr[0]->GetY1() >  ptr[1]->GetY1() && ptr[0]->GetX1()-ptr[1]->GetX0()< BZK_FastMath::Int32toFix32(0))
			{		
			  repetir=true;
			  cout << "XY "<<c<<"-"<<d<<" caso 12" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetY0(ptr[1]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());
			  ptr[0]->SetY1(ptr[2]->GetY0());			    
			  ptr[2]->SetX0(ptr[0]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());			   
			  continue;
			}

		    ///faltou
		    //X:
		    ////////////////////////////////////////////////////////

			/*
                        ____            ______
			|0  |  ->	| 0|2|
			|___|__  	|__|_|__
		          |  1|	           | 1 |
			  |___|	           |___|

			  + recursivity.
			*/
		    //the first condition protects agains infinite loop, as the result could easily fall on this case again.
		    if (ptr[0]->GetX1() >  ptr[1]->GetX0() &&ptr[0]->GetX0() <  ptr[1]->GetX0() && ptr[0]->GetX1() <  ptr[1]->GetX1() && ptr[0]->GetY1()-ptr[1]->GetY0()< BZK_FastMath::Int32toFix32(0))
			{	
			  repetir=true;	
			  cout << "XY "<<c<<"-"<<d<<" caso 13" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetX0(ptr[1]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());
			  ptr[0]->SetX1(ptr[2]->GetX0());			    
			  ptr[2]->SetY0(ptr[0]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());			   
			  continue;
			  }
		    
		    ///Y
		    ////////////////////////////////////////////////////////

			/*
                        ____            ______
			|0  |  ->	| 0|2|
			|___|__  	|__|_|__
		          |  1|	           | 1 |
			  |___|	           |___|

			  + recursivity.
			*/

		    if (ptr[0]->GetY1() >  ptr[1]->GetY0() && ptr[0]->GetY0() <  ptr[1]->GetY0() &&  ptr[0]->GetY1() >  ptr[1]->GetY1() && ptr[0]->GetX1()-ptr[1]->GetX0()< BZK_FastMath::Int32toFix32(0))
			{	
			  repetir=true;	
			  cout << "XY "<<c<<"-"<<d<<" caso 14" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetY0(ptr[1]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());
			  ptr[0]->SetY1(ptr[2]->GetY0());			    
			  ptr[2]->SetX0(ptr[0]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());			   
			  continue;
			}


		    //X:
		    ////////////////////////////////////////////////////////

			/*
                            ____             ____
			    |0  |  ->	     |0|2|
		          __|___|  	   __|_|_|
		          |  1|	           | 1 |
			  |___|	           |___|

			  + recursivity.
			*/

		    if ( ptr[0]->GetX0() >  ptr[1]->GetX0() && ptr[0]->GetX1() > ptr[1]->GetX1()&& ptr[0]->GetX0() < ptr[1]->GetX1()  && ptr[0]->GetY1()-ptr[1]->GetY0()< BZK_FastMath::Int32toFix32(0))
			{	
			  repetir=true;	
			  cout << "XY "<<c<<"-"<<d<<" caso 15" <<endl;			  
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetX0(ptr[1]->GetX1());
			  ptr[2]->SetX1(ptr[0]->GetX1());
			  ptr[0]->SetX1(ptr[2]->GetX0());			    
			  ptr[2]->SetY0(ptr[0]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());			   
			  continue;
			}
		    ///Y
		    ////////////////////////////////////////////////////////
			/*
                            ____             ____
			    |0  |  ->	     |0|2|
		          __|___|  	   __|_|_|
		          |  1|	           | 1 |
			  |___|	           |___|

			  + recursivity.
			*/
		    if ( ptr[0]->GetY0() <  ptr[1]->GetY0() &&  ptr[0]->GetY1() >  ptr[1]->GetY1() && ptr[0]->GetY0() < ptr[1]->GetY1()  && ptr[0]->GetX1()-ptr[1]->GetX0()< BZK_FastMath::Int32toFix32(0))
			{	
			  repetir=true;	
			  cout << "XY "<<c<<"-"<<d<<" caso 16" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetY0(ptr[1]->GetY1());
			  ptr[2]->SetY1(ptr[0]->GetY1());
			  ptr[0]->SetY1(ptr[2]->GetY0());			    
			  ptr[2]->SetX0(ptr[0]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());			   
			  continue;
			}

		    if ( ptr[0]->GetH1() >  ptr[1]->GetH1() &&  ptr[0]->GetH0() ==  ptr[1]->GetH0() && ptr[0]->GetY0()-ptr[1]->GetY1()< BZK_FastMath::Int32toFix32(0))
			{	
			  repetir=true;
			  cout << "YZ "<<c<<"-"<<d<<" caso 1" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetH0(ptr[1]->GetH1());
			  ptr[2]->SetH1(ptr[0]->GetH1());
			  ptr[0]->SetH0(ptr[1]->GetH0());
			  ptr[0]->SetH1(ptr[2]->GetH0());			    
			  ptr[2]->SetY0(ptr[0]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());			   
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
		            ____	     ______
			    |  1|	    | 1   |
			____|___|	____|_____|
			|   0	|  ->	| 0 |  2  |
			|_______|	|___|_____|   

			*/

		    if ( ptr[0]->GetH0() <  ptr[1]->GetH0() &&  
			 ptr[0]->GetH1() ==  ptr[1]->GetH1() && 
			ptr[0]->GetY0()==ptr[1]->GetY1())
			
			{
			  repetir=true;		
			  cout << "YZ "<<c<<"-"<<d<<" caso 2" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetH0(ptr[1]->GetH0());
			  ptr[2]->SetH1(ptr[1]->GetH1());
			  //			  ptr[0]->SetH0(ptr[0]->GetH0());
			  ptr[0]->SetH1(ptr[2]->GetH0());			    
			  ptr[2]->SetY0(ptr[0]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());			   
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
			H
		   ^	-------->
                   |    _________       ___________
		   |	|   0	|  ->	| 0 |  2  |
		   |	|_______|	|___|_____|
		   |	    |  1|	    | 1   |
		   Y        |___|	    |_____|


			mesmo topo, colados em Z, mas com um é mais profundo que o outro (tem uma quilha)

			*/

		    if ( ptr[0]->GetH0() <  ptr[1]->GetH0() &&  ptr[0]->GetH1() ==  ptr[1]->GetH1() && ptr[0]->GetY1()==ptr[1]->GetY0())
			{
			  repetir=true;		
			  cout << "YZ "<<c<<"-"<<d<<" caso 3" <<endl;			  
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetH0(ptr[1]->GetH0());
			  ptr[2]->SetH1(ptr[1]->GetH1());
			  //			  ptr[0]->SetH0(ptr[0]->GetH0());
			  ptr[0]->SetH1(ptr[2]->GetH0());			    
			  ptr[2]->SetY0(ptr[0]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());			   
			  continue;
			}



		 ////////////////////////////////////////////////////////

			/*
                        _________       ___________
			|   0	|  ->	| 0   |2  |
			|_______|	|_____|___|
	                |  1|	        | 1   |
			|___|	        |_____|


			*/

		    if ( ptr[0]->GetH0() ==  ptr[1]->GetH0() &&  ptr[0]->GetH1() >  ptr[1]->GetH1() && ptr[0]->GetY1()-ptr[1]->GetY0()< BZK_FastMath::Int32toFix32(0))
			{		
			  repetir=true;
			  cout << "YZ "<<c<<"-"<<d<<" caso 4" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetH0(ptr[1]->GetH1());
			  ptr[2]->SetH1(ptr[0]->GetH1());
			  //			  ptr[0]->SetH0(ptr[0]->GetH0());
			  ptr[0]->SetH1(ptr[2]->GetH0());			    
			  ptr[2]->SetY0(ptr[0]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());			   
			  continue;
			}
		    	 ////////////////////////////////////////////////////////

			/*
		          ____	           ____
		          |  1|	           | 1 |
			__|___|__	___|___|__
			|   0	|  ->	|0 |  2  |
			|_______|	|__|_____|
			+ recursivity.


			XZ

				       |--------------
			---------------|
                                       |
			---------------|
	                               |-----------------
			*/


		    if ( ptr[0]->GetH0() <  ptr[1]->GetH0() &&  ptr[0]->GetH1() >  ptr[1]->GetH1() && ptr[0]->GetY0()==ptr[1]->GetY1())
			{
			  repetir=true;		
			  cout << "YZ "<<c<<"-"<<d<<" caso 5" <<endl;			 
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetH0(ptr[1]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());
			  //			  ptr[0]->SetH0(ptr[0]->GetH0());
			  ptr[0]->SetH1(ptr[2]->GetH0());			    
			  ptr[2]->SetY0(ptr[0]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());			   
			  continue;
			}

		 ////////////////////////////////////////////////////////

			/*
                        _________       ___________
			|   0	|  ->	| 0|  2   |
			|_______|	|__|______|
		          |  1|	           | 1 |
			  |___|	           |___|

			  + recursivity.
			*/

		    if ( ptr[0]->GetH0() <  ptr[1]->GetH0() &&  ptr[0]->GetH1() >  ptr[1]->GetH1() && ptr[0]->GetY1()-ptr[1]->GetY0()< BZK_FastMath::Int32toFix32(0))
			{
			  repetir=true;		
			  cout << "YZ "<<c<<"-"<<d<<" caso 6" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetH0(ptr[1]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());
			  //			  ptr[0]->SetH0(ptr[0]->GetH0());
			  ptr[0]->SetH1(ptr[2]->GetH0());			    
			  ptr[2]->SetY0(ptr[0]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());			   
			  continue;
			}

		 //////  Y
		 ////////////////////////////////////////////////////////

			/*
			____		____
			| 1|		| 1 |
			|__|____	|___|______
			|   0	|  ->	| 0 |  2  |
			|_______|	|___|_____|   

			*/

		    if ( ptr[0]->GetY1() >  ptr[1]->GetY1() &&  ptr[0]->GetY0() ==  ptr[1]->GetY0() && ptr[0]->GetH0()-ptr[1]->GetH1()< BZK_FastMath::Int32toFix32(0))
			{
			  repetir=true;	
			  cout << "YZ "<<c<<"-"<<d<<" caso 7" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetY0(ptr[1]->GetY1());
			  ptr[2]->SetY1(ptr[0]->GetY1());
			  ptr[0]->SetY0(ptr[1]->GetY0());
			  ptr[0]->SetY1(ptr[2]->GetY0());			    
			  ptr[2]->SetH0(ptr[0]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());			   
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
		            ____	     ______
			    |  1|	    | 1   |
			____|___|	____|_____|
			|   0	|  ->	| 0 |  2  |
			|_______|	|___|_____|   

			*/

		    if ( ptr[0]->GetY0() <  ptr[1]->GetY0() &&  ptr[0]->GetY1() ==  ptr[1]->GetY1() && ptr[0]->GetH0()-ptr[1]->GetH1()< BZK_FastMath::Int32toFix32(0))
			{
			  repetir=true;		
			  cout << "YZ "<<c<<"-"<<d<<" caso 8" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetY0(ptr[1]->GetY0());
			  ptr[2]->SetY1(ptr[1]->GetY1());
			  ptr[0]->SetY1(ptr[2]->GetY0());			    
			  ptr[2]->SetH0(ptr[0]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());			   
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
                        _________       ___________
			|   0	|  ->	| 0 |  2  |
			|_______|	|___|_____|
			    |  1|	    | 1   |
			    |___|	    |_____|


			*/

		    if ( ptr[0]->GetY0() <  ptr[1]->GetY0() &&  ptr[0]->GetY1() ==  ptr[1]->GetY1() && ptr[0]->GetH1()-ptr[1]->GetH0()< BZK_FastMath::Int32toFix32(0))
			{		
			  repetir=true;
			  cout << "YZ "<<c<<"-"<<d<<" caso 9" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetY0(ptr[1]->GetY0());
			  ptr[2]->SetY1(ptr[1]->GetY1());
			  ptr[0]->SetY1(ptr[2]->GetY0());			    
			  ptr[2]->SetH0(ptr[0]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());			   
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
                        _________       ___________
			|   0	|  ->	| 0   |2  |
			|_______|	|_____|___|
	                |  1|	        | 1   |
			|___|	        |_____|


			*/

		    if ( ptr[0]->GetY0() ==  ptr[1]->GetY0() &&  ptr[0]->GetY1() >  ptr[1]->GetY1() && ptr[0]->GetH1()-ptr[1]->GetH0()< BZK_FastMath::Int32toFix32(0))
			{		
			  repetir=true;
			  cout << "YZ "<<c<<"-"<<d<<" caso 10" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetY0(ptr[1]->GetY1());
			  ptr[2]->SetY1(ptr[0]->GetY1());
			  ptr[0]->SetY1(ptr[2]->GetY0());			    
			  ptr[2]->SetH0(ptr[0]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());			   
			  continue;
			}
		    	 ////////////////////////////////////////////////////////

			/*
		          ____	           ____
		          |  1|	           | 1 |
			__|___|__	___|___|__
			|   0	|  ->	|0 |  2  |
			|_______|	|__|_____|
			+ recursivity.

			*/

		    if ( ptr[0]->GetY0() <  ptr[1]->GetY0() &&  ptr[0]->GetY1() >  ptr[1]->GetY1() && ptr[0]->GetH0()-ptr[1]->GetH1()< BZK_FastMath::Int32toFix32(0))
			{		
			  repetir=true;
			  cout << "YZ "<<c<<"-"<<d<<" caso 11" <<endl;			  
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetY0(ptr[1]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());
			  ptr[0]->SetY1(ptr[2]->GetY0());			    
			  ptr[2]->SetH0(ptr[0]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());			   
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
                        _________       ___________
			|   0	|  ->	| 0|  2   |
			|_______|	|__|______|
		          |  1|	           | 1 |
			  |___|	           |___|

			  + recursivity.
			*/

		    if ( ptr[0]->GetY0() <  ptr[1]->GetY0() &&  ptr[0]->GetY1() >  ptr[1]->GetY1() && ptr[0]->GetH1()-ptr[1]->GetH0()< BZK_FastMath::Int32toFix32(0))
			{		
			  repetir=true;
			  cout << "YZ "<<c<<"-"<<d<<" caso 12" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetY0(ptr[1]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());
			  ptr[0]->SetY1(ptr[2]->GetY0());			    
			  ptr[2]->SetH0(ptr[0]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());			   
			  continue;
			}

		    ///faltou
		    //X:
		    ////////////////////////////////////////////////////////

			/*
                        ____            ______
			|0  |  ->	| 0|2|
			|___|__  	|__|_|__
		          |  1|	           | 1 |
			  |___|	           |___|

			  + recursivity.
			*/
		    //the first condition protects agains infinite loop, as the result could easily fall on this case again.
		    if (ptr[0]->GetH1() >  ptr[1]->GetH0() &&ptr[0]->GetH0() <  ptr[1]->GetH0() && ptr[0]->GetH1() <  ptr[1]->GetH1() && ptr[0]->GetY1()-ptr[1]->GetY0()< BZK_FastMath::Int32toFix32(0))
			{	
			  repetir=true;	
			  cout << "YZ "<<c<<"-"<<d<<" caso 13" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetH0(ptr[1]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());
			  ptr[0]->SetH1(ptr[2]->GetH0());			    
			  ptr[2]->SetY0(ptr[0]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());			   
			  continue;
			  }
		    
		    ///Y
		    ////////////////////////////////////////////////////////

			/*
                        ____            ______
			|0  |  ->	| 0|2|
			|___|__  	|__|_|__
		          |  1|	           | 1 |
			  |___|	           |___|

			  + recursivity.
			*/

		    if (ptr[0]->GetY1() >  ptr[1]->GetY0() && ptr[0]->GetY0() <  ptr[1]->GetY0() &&  ptr[0]->GetY1() >  ptr[1]->GetY1() && ptr[0]->GetH1()-ptr[1]->GetH0()< BZK_FastMath::Int32toFix32(0))
			{	
			  repetir=true;	
			  cout << "YZ "<<c<<"-"<<d<<" caso 14" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetY0(ptr[1]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());
			  ptr[0]->SetY1(ptr[2]->GetY0());			    
			  ptr[2]->SetH0(ptr[0]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());			   
			  continue;
			}


		    //X:
		    ////////////////////////////////////////////////////////

			/*
                            ____             ____
			    |0  |  ->	     |0|2|
		          __|___|  	   __|_|_|
		          |  1|	           | 1 |
			  |___|	           |___|

			  + recursivity.
			*/

		    if ( ptr[0]->GetH0() >  ptr[1]->GetH0() && ptr[0]->GetH1() > ptr[1]->GetH1()&& ptr[0]->GetH0() < ptr[1]->GetH1()  && ptr[0]->GetY1()-ptr[1]->GetY0()< BZK_FastMath::Int32toFix32(0))
			{	
			  repetir=true;	
			  cout << "YZ "<<c<<"-"<<d<<" caso 15" << (ptr[0]->GetH1() > ptr[1]->GetH1()) <<endl;			  
			  cout << ptr[0]->GetH1()<<","<< ptr[1]->GetH1()<<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetH0(ptr[1]->GetH1());
			  ptr[2]->SetH1(ptr[0]->GetH1());
			  ptr[0]->SetH1(ptr[2]->GetH0());			    
			  ptr[2]->SetY0(ptr[0]->GetY0());
			  ptr[2]->SetY1(ptr[0]->GetY1());			   
			  continue;
			}
		    ///Y
		    ////////////////////////////////////////////////////////
			/*
                            ____             ____
			    |0  |  ->	     |0|2|
		          __|___|  	   __|_|_|
		          |  1|	           | 1 |
			  |___|	           |___|

			  + recursivity.
			*/
		    if ( ptr[0]->GetY0() <  ptr[1]->GetY0() &&  ptr[0]->GetY1() >  ptr[1]->GetY1() && ptr[0]->GetY0() < ptr[1]->GetY1()  && ptr[0]->GetH1()-ptr[1]->GetH0()< BZK_FastMath::Int32toFix32(0))
			{	
			  repetir=true;	
			  cout << "YZ "<<c<<"-"<<d<<" caso 16" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetY0(ptr[1]->GetY1());
			  ptr[2]->SetY1(ptr[0]->GetY1());
			  ptr[0]->SetY1(ptr[2]->GetY0());			    
			  ptr[2]->SetH0(ptr[0]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());			   
			  continue;
			}

		    if ( ptr[0]->GetH1() >  ptr[1]->GetH1() &&  ptr[0]->GetH0() ==  ptr[1]->GetH0() && ptr[0]->GetX0()-ptr[1]->GetX1()< BZK_FastMath::Int32toFix32(0))
			{	
			  repetir=true;
			  cout << "XZ "<<c<<"-"<<d<<" caso 1" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetH0(ptr[1]->GetH1());
			  ptr[2]->SetH1(ptr[0]->GetH1());
			  ptr[0]->SetH0(ptr[1]->GetH0());
			  ptr[0]->SetH1(ptr[2]->GetH0());			    
			  ptr[2]->SetX0(ptr[0]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());			   
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
		            ____	     ______
			    |  1|	    | 1   |
			____|___|	____|_____|
			|   0	|  ->	| 0 |  2  |
			|_______|	|___|_____|   

			*/

		    if ( ptr[0]->GetH0() <  ptr[1]->GetH0() &&  
			ptr[0]->GetH1() ==  ptr[1]->GetH1() && 
			ptr[0]->GetX0()==ptr[1]->GetX1())
			{
			  repetir=true;		
			  cout << "XZ "<<c<<"-"<<d<<" caso 2" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetH0(ptr[1]->GetH0());
			  ptr[2]->SetH1(ptr[1]->GetH1());
			  //			  ptr[0]->SetH0(ptr[0]->GetH0());
			  ptr[0]->SetH1(ptr[2]->GetH0());			    
			  ptr[2]->SetX0(ptr[0]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());			   
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
                        _________       ___________
			|   0	|  ->	| 0 |  2  |
			|_______|	|___|_____|
			    |  1|	    | 1   |
			    |___|	    |_____|


			*/

		    if ( ptr[0]->GetH0() <  ptr[1]->GetH0() &&  ptr[0]->GetH1() ==  ptr[1]->GetH1() && ptr[0]->GetX1()-ptr[1]->GetX0()== BZK_FastMath::Int32toFix32(0))
			{
			  repetir=true;		
			  cout << "XZ "<<c<<"-"<<d<<" caso 3" <<endl;			  
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetH0(ptr[1]->GetH0());
			  ptr[2]->SetH1(ptr[1]->GetH1());

			  ptr[0]->SetH1(ptr[2]->GetH0());			    
			  ptr[2]->SetX0(ptr[0]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());			   
			  continue;
			}



		 ////////////////////////////////////////////////////////

			/*
                        _________       ___________
			|   0	|  ->	| 0   |2  |
			|_______|	|_____|___|
	                |  1|	        | 1   |
			|___|	        |_____|


			*/

		    if ( ptr[0]->GetH0() ==  ptr[1]->GetH0() &&  ptr[0]->GetH1() >  ptr[1]->GetH1() && ptr[0]->GetX1()-ptr[1]->GetX0()< BZK_FastMath::Int32toFix32(0))
			{		
			  repetir=true;
			  cout << "XZ "<<c<<"-"<<d<<" caso 4" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetH0(ptr[1]->GetH1());
			  ptr[2]->SetH1(ptr[0]->GetH1());
			  //			  ptr[0]->SetH0(ptr[0]->GetH0());
			  ptr[0]->SetH1(ptr[2]->GetH0());			    
			  ptr[2]->SetX0(ptr[0]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());			   
			  continue;
			}
		    	 ////////////////////////////////////////////////////////

			/*
		          ____	           ____
		          |  1|	           | 1 |
			__|___|__	___|___|__
			|   0	|  ->	|0 |  2  |
			|_______|	|__|_____|
			+ recursivity.

			*/

		    if ( ptr[0]->GetH0() <  ptr[1]->GetH0() &&  ptr[0]->GetH1() >  ptr[1]->GetH1() && ptr[0]->GetX0()-ptr[1]->GetX1()< BZK_FastMath::Int32toFix32(0))
			{
			  repetir=true;		
			  cout << "XZ "<<c<<"-"<<d<<" caso 5" <<endl;			 
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetH0(ptr[1]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());
			  //			  ptr[0]->SetH0(ptr[0]->GetH0());
			  ptr[0]->SetH1(ptr[2]->GetH0());			    
			  ptr[2]->SetX0(ptr[0]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());			   
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
                        _________       ___________
			|   0	|  ->	| 0|  2   |
			|_______|	|__|______|
		          |  1|	           | 1 |
			  |___|	           |___|

			  + recursivity.
			*/

		    if ( ptr[0]->GetH0() <  ptr[1]->GetH0() &&  ptr[0]->GetH1() >  ptr[1]->GetH1() && ptr[0]->GetX1()-ptr[1]->GetX0()< BZK_FastMath::Int32toFix32(0))
			{
			  repetir=true;		
			  cout << "XZ "<<c<<"-"<<d<<" caso 6" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetH0(ptr[1]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());
			  //			  ptr[0]->SetH0(ptr[0]->GetH0());
			  ptr[0]->SetH1(ptr[2]->GetH0());			    
			  ptr[2]->SetX0(ptr[0]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());			   
			  continue;
			}

		 //////  Y
		 ////////////////////////////////////////////////////////

			/*
			____		____
			| 1|		| 1 |
			|__|____	|___|______
			|   0	|  ->	| 0 |  2  |
			|_______|	|___|_____|   

			*/

		    if ( ptr[0]->GetX1() >  ptr[1]->GetX1() &&  ptr[0]->GetX0() ==  ptr[1]->GetX0() && ptr[0]->GetH0()-ptr[1]->GetH1()< BZK_FastMath::Int32toFix32(0))
			{
			  repetir=true;	
			  cout << "XZ "<<c<<"-"<<d<<" caso 7" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetX0(ptr[1]->GetX1());
			  ptr[2]->SetX1(ptr[0]->GetX1());
			  ptr[0]->SetX0(ptr[1]->GetX0());
			  ptr[0]->SetX1(ptr[2]->GetX0());			    
			  ptr[2]->SetH0(ptr[0]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());			   
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
		            ____	     ______
			    |  1|	    | 1   |
			____|___|	____|_____|
			|   0	|  ->	| 0 |  2  |
			|_______|	|___|_____|   

			*/

		    if ( ptr[0]->GetX0() <  ptr[1]->GetX0() &&  ptr[0]->GetX1() ==  ptr[1]->GetX1() && ptr[0]->GetH0()-ptr[1]->GetH1()< BZK_FastMath::Int32toFix32(0))
			{
			  repetir=true;		
			  cout << "XZ "<<c<<"-"<<d<<" caso 8" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetX0(ptr[1]->GetX0());
			  ptr[2]->SetX1(ptr[1]->GetX1());
			  ptr[0]->SetX1(ptr[2]->GetX0());			    
			  ptr[2]->SetH0(ptr[0]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());			   
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
                        _________       ___________
			|   0	|  ->	| 0 |  2  |
			|_______|	|___|_____|
			    |  1|	    | 1   |
			    |___|	    |_____|


			*/

		    if ( ptr[0]->GetX0() <  ptr[1]->GetX0() &&  ptr[0]->GetX1() ==  ptr[1]->GetX1() && ptr[0]->GetH1()-ptr[1]->GetH0()< BZK_FastMath::Int32toFix32(0))
			{		
			  repetir=true;
			  cout << "XZ "<<c<<"-"<<d<<" caso 9" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetX0(ptr[1]->GetX0());
			  ptr[2]->SetX1(ptr[1]->GetX1());
			  ptr[0]->SetX1(ptr[2]->GetX0());			    
			  ptr[2]->SetH0(ptr[0]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());			   
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
                        _________       ___________
			|   0	|  ->	| 0   |2  |
			|_______|	|_____|___|
	                |  1|	        | 1   |
			|___|	        |_____|


			*/

		    if ( ptr[0]->GetX0() ==  ptr[1]->GetX0() &&  ptr[0]->GetX1() >  ptr[1]->GetX1() && ptr[0]->GetH1()-ptr[1]->GetH0()< BZK_FastMath::Int32toFix32(0))
			{		
			  repetir=true;
			  cout << "XZ "<<c<<"-"<<d<<" caso 10" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetX0(ptr[1]->GetX1());
			  ptr[2]->SetX1(ptr[0]->GetX1());
			  ptr[0]->SetX1(ptr[2]->GetX0());			    
			  ptr[2]->SetH0(ptr[0]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());			   
			  continue;
			}
		    	 ////////////////////////////////////////////////////////

			/*
		          ____	           ____
		          |  1|	           | 1 |
			__|___|__	___|___|__
			|   0	|  ->	|0 |  2  |
			|_______|	|__|_____|
			+ recursivity.

			*/

		    if ( ptr[0]->GetX0() <  ptr[1]->GetX0() &&  ptr[0]->GetX1() >  ptr[1]->GetX1() && ptr[0]->GetH0()-ptr[1]->GetH1()< BZK_FastMath::Int32toFix32(0))
			{		
			  repetir=true;
			  cout << "XZ "<<c<<"-"<<d<<" caso 11" <<endl;			  
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetX0(ptr[1]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());
			  ptr[0]->SetX1(ptr[2]->GetX0());			    
			  ptr[2]->SetH0(ptr[0]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());			   
			  continue;
			}
		 ////////////////////////////////////////////////////////

			/*
                        _________       ___________
			|   0	|  ->	| 0|  2   |
			|_______|	|__|______|
		          |  1|	           | 1 |
			  |___|	           |___|

			  + recursivity.
			*/

		    if ( ptr[0]->GetX0() <  ptr[1]->GetX0() &&  ptr[0]->GetX1() >  ptr[1]->GetX1() && ptr[0]->GetH1()-ptr[1]->GetH0()< BZK_FastMath::Int32toFix32(0))
			{		
			  repetir=true;
			  cout << "XZ "<<c<<"-"<<d<<" caso 12" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetX0(ptr[1]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());
			  ptr[0]->SetX1(ptr[2]->GetX0());			    
			  ptr[2]->SetH0(ptr[0]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());			   
			  continue;
			}

		    ///faltou
		    //X:
		    ////////////////////////////////////////////////////////

			/*
                        ____            ______
			|0  |  ->	| 0|2|
			|___|__  	|__|_|__
		          |  1|	           | 1 |
			  |___|	           |___|

			  + recursivity.
			*/
		    //the first condition protects agains infinite loop, as the result could easily fall on this case again.
		    if (ptr[0]->GetH1() >  ptr[1]->GetH0() &&ptr[0]->GetH0() <  ptr[1]->GetH0() && ptr[0]->GetH1() <  ptr[1]->GetH1() && ptr[0]->GetX1()-ptr[1]->GetX0()< BZK_FastMath::Int32toFix32(0))
			{	
			  repetir=true;	
			  cout << "XZ "<<c<<"-"<<d<<" caso 13" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetH0(ptr[1]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());
			  ptr[0]->SetH1(ptr[2]->GetH0());			    
			  ptr[2]->SetX0(ptr[0]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());			   
			  continue;
			  }
		    
		    ///Y
		    ////////////////////////////////////////////////////////

			/*
                        ____            ______
			|0  |  ->	| 0|2|
			|___|__  	|__|_|__
		          |  1|	           | 1 |
			  |___|	           |___|

			  + recursivity.
			*/

		    if (ptr[0]->GetX1() >  ptr[1]->GetX0() && ptr[0]->GetX0() <  ptr[1]->GetX0() &&  ptr[0]->GetX1() >  ptr[1]->GetX1() && ptr[0]->GetH1()-ptr[1]->GetH0() < BZK_FastMath::Int32toFix32(0) )
			{	
			  repetir=true;	
			  cout << "XZ "<<c<<"-"<<d<<" caso 14" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetX0(ptr[1]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());
			  ptr[0]->SetX1(ptr[2]->GetX0());			    
			  ptr[2]->SetH0(ptr[0]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());			   
			  continue;
			}


		    //X:
		    ////////////////////////////////////////////////////////

			/*
                            ____             ____
			    |0  |  ->	     |0|2|
		          __|___|  	   __|_|_|
		          |  1|	           | 1 |
			  |___|	           |___|

			  + recursivity.
			*/
		    
		    if ( ptr[0]->GetH0() >  ptr[1]->GetH0() && ptr[0]->GetH1() > ptr[1]->GetH1()&& ptr[0]->GetH0() < ptr[1]->GetH1()  && ptr[0]->GetX1()-ptr[1]->GetX0() < BZK_FastMath::Int32toFix32(0))
			{	
			  repetir=true;	
			  cout << "XZ "<<c<<"-"<<d<<" caso 15" <<endl;			  
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetH0(ptr[1]->GetH1());
			  ptr[2]->SetH1(ptr[0]->GetH1());
			  ptr[0]->SetH1(ptr[2]->GetH0());			    
			  ptr[2]->SetX0(ptr[0]->GetX0());
			  ptr[2]->SetX1(ptr[0]->GetX1());			   
			  continue;
			  }
		    ///Y
		    ////////////////////////////////////////////////////////
			/*
                            ____             ____
			    |0  |  ->	     |0|2|
		          __|___|  	   __|_|_|
		          |  1|	           | 1 |
			  |___|	           |___|

			  + recursivity.
			*/
		    if ( ptr[0]->GetX0() <  ptr[1]->GetX0() &&  ptr[0]->GetX1() >  ptr[1]->GetX1() && ptr[0]->GetX0() < ptr[1]->GetX1()  &&
 ptr[0]->GetH1()-ptr[1]->GetH0() < BZK_FastMath::Int32toFix32(0))
			{	
			  repetir=true;	
			  cout << "XZ "<<c<<"-"<<d<<" caso 16" <<endl;
			  ptr[2]=new VAC_Sector();
			  memcpy(ptr[2],ptr[0],sizeof(VAC_Sector));
			  World.AddSector(*ptr[2]);
			  g=World.TotalSectors()-1;			  
			  ptr[2]->SetX0(ptr[1]->GetX1());
			  ptr[2]->SetX1(ptr[0]->GetX1());
			  ptr[0]->SetX1(ptr[2]->GetX0());			    
			  ptr[2]->SetH0(ptr[0]->GetH0());
			  ptr[2]->SetH1(ptr[0]->GetH1());			   
			  continue;
			}
	    }
    }
  //  if (repeticoes>100) repetir=false;
    }while (repetir);
  cout << "fim de checagem" <<endl;
  World.GetSector(0)->iCandelas=64;

  for (int c=1;c<World.TotalSectors();c++)
	{
	    ptr[0]=World.GetSector(c);
	    ptr[0]->iParent+=World.TotalSectors();
	}
	
  for (int c=0;c<aux.TotalItems();c++)
	{
	    ptr[0]=aux[c];
	    World.AddSector(*ptr[0]);
	}



 //Split();
  ReLink();
  CalcLimits();
  return;
}




int _indexof(vector<int> &vec,int num)
{
for (int c=0;c<vec.size();c++)
  if (vec[c]==num) return c;
return -1;
}

void addclass(vector<vector<int>*> &groups,vector<int>* vert)
{
		cout << "addclass" <<endl;	
		int linha=-1;
		for (int c=0;c<groups.size();c++)
			for (int e=0;e<vert->size();e++)
				if (_indexof(*groups[c],(*vert)[e])!=-1)
					linha=c;


		if (linha!=-1)
			for (int f=0;f<vert->size();f++)
					{	
					cout << "copiando para a linha "<<linha<<" "<<(*vert)[f]<<endl;
					//if (_indexof(*groups[linha],(*vert)[e])!=-1)		
						groups[linha]->push_back((*vert)[f]);
					}
		else
			groups.push_back(vert);

}
/*
void ImportOBJ(string aOBJ,string aOut)
{
  cout << "obj to svg boxes:"<<endl;
  ifstream obj(aOBJ.c_str());
  string svgfilename=aOut;
  svgfilename+=".svg";
  ofstream svg(svgfilename.c_str());
  ofstream fragment;
  vector<vector<string>*> mesh;
  vector<string> points;
  BZK_Vector<VAC_Sector*> sectors;  
  VAC_Face *tmp4;
  int c=0;
  // VAC_Face*tmp;
  BZK_VolumetricSector*tmp2;
  VAC_Sector *tmp3;
  VAC_PolyBuffer *model;
  BZK_FixP menorx=BZK_FastMath::Int32toFix32(255);
  BZK_FixP maiorx=0;
  BZK_FixP menory=BZK_FastMath::Int32toFix32(255);
  BZK_FixP maiory=0;
  BZK_FixP menorz=BZK_FastMath::Int32toFix32(255);
  BZK_FixP maiorz=0;
  tmp3=new VAC_Sector();
  World.AddSector(*tmp3);
  model=VAC_PolyBuffer::InternalizeFromScratch(obj);

  WriteSVGHeader(svg);
  while (!model->Empty())
    {
      c++;
      tmp4=model->Pop();
      tmp2=tmp4->Volume();
      tmp3=new VAC_Sector();
      //memcpy(tmp3,tmp2,sizeof(BZK_VolumetricSector));
      tmp3->SetRect(tmp2->iRect);
      //sectors.Add(tmp3);  
      // if (tmp4->GetPoint(0)->GetY()!=tmp4->GetPoint(1)->GetY())//|| tmp4->GetPoint(1)->GetY() != tmp4->GetPoint(2)->GetY())     
      //	continue;
      
      svg << "<path id='linedef"<<c<<"' style='fill:#0ff;stroke:#000'  d=\" M "<<BZK_FastMath::Fix32toInt32(tmp4->GetPoint(0)->GetX()) <<" "
	  <<BZK_FastMath::Fix32toInt32(tmp4->GetPoint(0)->GetZ());
      for (int d=1;d<tmp4->iPoints.TotalItems();d++)
	svg << " L "<<BZK_FastMath::Fix32toInt32(tmp4->GetPoint(d)->GetX()) <<" "
	  <<BZK_FastMath::Fix32toInt32(tmp4->GetPoint(d)->GetZ());
      svg << " z\" />"<<endl;
      */
      /*

      svg <<"<rect id='sectdef"<<c<<"' style='fill:#ff0;stroke:#000' ";
      if (BZK_FastMath::Fix32toInt32(tmp2->GetX1()-tmp2->GetX0())>0)
      svg<<"width='"<<BZK_FastMath::Fix32toInt32(tmp2->GetX1()-tmp2->GetX0());
      else
	svg<<"width='"<<-BZK_FastMath::Fix32toInt32(tmp2->GetX1()-tmp2->GetX0());
	
      if (BZK_FastMath::Fix32toInt32(tmp2->GetY1()-tmp2->GetY0())>0)
	svg<<"' height='"<<BZK_FastMath::Fix32toInt32(tmp2->GetY1()-tmp2->GetY0());
      else
	svg<<"' height='"<<-BZK_FastMath::Fix32toInt32(tmp2->GetY1()-tmp2->GetY0());
      
      svg<<"' x=\""<<BZK_FastMath::Fix32toInt32(tmp2->GetX0());
      svg<<"\" y=\""<<BZK_FastMath::Fix32toInt32(tmp2->GetY0());
      svg<<"\"/>"<<endl;;
      */
      //	World.AddSector(*tmp3);
/*
    }
  cout << "setores:"<<World.TotalSectors()<<endl;
	VAC_Sector* point; //I know, Im lazy...
	
	 cout << "starting to write svg body..."<<endl;


  cout << "terminei" <<endl;

  obj.close();
  vector<vector<int> *> groups;
  vector<vector<int> *> tmp;
  vector<int> *vert;
	{
	obj.open(aOBJ.c_str());
	string token;

	while (!obj.eof())
		{
		vert=new vector<int>();
		getline(obj,token);
		//cout << token << endl;
		if (token[0]=='f')
			{
			while (BZK_ParsingTools::GetSubToken(token,vert->size()+1)!="")		
			   vert->push_back(BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(BZK_ParsingTools::GetSubToken(token,vert->size()+1))));
			}
		if (vert->size()!=0)
		  tmp.push_back(vert);
		}
		cout << endl;
	}

	cout << "tmp:"<<endl;
	for (int c=0;c<tmp.size();c++)
		{
		for (int d=0;d<tmp[c]->size();d++)
			cout <<(*tmp[c])[d]<<";";
		cout << endl;
		}
	//agrupa copiando indiscriminadamente
	{

	  //para cada elemento de cada lista de tmp, verifica se ele ja existe em outra lista de tmp
	  //se ele ja existir, copia a lista inteira e passa pra próxima lista
	  for (int c=0;c<tmp.size();c++)
	    {
	      cout << "fila "<<c<<" de tmp de "<<tmp.size()<<endl;
	      cout << "status:"<<endl;



	      for (int z=0;z<tmp.size();z++)
		{
		  for (int x=0;x<tmp[z]->size();x++)
		    cout <<(*tmp[z])[x]<<";";
		  cout << endl;
		}
	      vector<int> *hors;
	      // if (c==1) exit(1);

	      vert=tmp[c];
	      
	      for (int d=0;d<vert->size();d++)
		{
		  cout << "coluna "<<d<<" de "<< vert->size()<<endl;
		  for (int e=c+1;e<tmp.size();e++)
		    {
		      hors=tmp[e];
		      cout << "comparando com linha "<<e<<endl;
		      //		      if (tmp[e]!=vert)
			if (_indexof(*tmp[e],(*vert)[d])!=-1)
			  {
			    cout << "indice:"<<_indexof(*tmp[e],(*vert)[d])<<endl;
			    //  cout << "copiando fila "<<vert<<" para "<<tmp[e]<<endl;
			    for (int f=0;f<vert->size();f++)
			      {
				if (_indexof(*hors,(*vert)[f])==-1)
				  {
				    cout << endl;
				    for (int g=0;g<hors->size();g++)
				      cout << (*hors)[g] <<";";
				    cout << endl;
				    
				    cout << "copiando posicao "<<f<<"="<< (*vert)[f] <<endl;
				    hors->push_back((*vert)[f]);

				    cout << endl;
				    for (int g=0;g<hors->size();g++)
				      cout << (*hors)[g] <<";";
				    cout << endl;
				  }
			      }
			    continue;
			  }
		    }
		}
	    }

//elimina duplicatas de listas.
	      cout << "status:"<<endl;
	      for (int z=0;z<tmp.size();z++)
		{
		  for (int x=0;x<tmp[z]->size();x++)
		    cout <<(*tmp[z])[x]<<";";
		  cout << endl;
		}


//	for (int c=0;c<tmp.size();c++)
//		groups.push_back(tmp[c]);
	      {
		bool incluso=false;
*/
		/*
	      vector <bool> marked;
		for (int d=0;d<tmp.size();d++)
			marked.push_back(false);
		*/
/*
//		bool marked[2000];
		bool *marked=(bool*)malloc(sizeof(bool)*tmp.size());
		for (int d=0;d<tmp.size();d++)
			marked[d]=false;

		int maior=-1;
		for (int d=0;d<tmp.size();d++)
		{
		maior=0;
		while (maior<tmp.size() && marked[maior])
			maior++;
			for (int e=0;e<tmp.size();e++)
				if (tmp[e]->size()> tmp[maior]->size() && !marked[e])
					maior=e;
				
		incluso=false;
		
		for (int e=0;e<tmp[maior]->size();e++)
			for (int f=0;f<groups.size();f++)
		//		if (f!=maior)
					if (_indexof(*groups[f],(*tmp[maior])[e])!=-1)
						incluso=true;	
		
		if (!incluso)
			groups.push_back(tmp[maior]);
		marked[maior]=true;
		} 
	
       		}
	}

	
	cout << "exibindo classes" <<endl;
	vector<string> *_vert;
	for (int c=0;c<groups.size();c++)
		{
		  cout << "**adicionando mesh" <<endl;
		  _vert=new vector<string>;
		  mesh.push_back(_vert);
		  for (int d=0;d<groups[c]->size();d++)
		    cout <<(*groups[c])[d]<<";";
		  cout << endl;
		}
	


	{
	  obj.close();
	  obj.open(aOBJ.c_str());
	string token;
	
	while (!obj.eof())
		{
		vert=new vector<int>();
		getline(obj,token);
		//	cout << token << endl;
		if (token[0]=='f')
			{
			 
			while (BZK_ParsingTools::GetSubToken(token,vert->size()+1)!="")		
			   vert->push_back(BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(BZK_ParsingTools::GetSubToken(token,vert->size()+1))));
			
			  for (int c=0;c<groups.size();c++)
			    for (int d=0;d<vert->size();d++)
			      if (_indexof(*groups[c],(*vert)[d])!=-1)
				{
				  cout <<token << " faz parte da classe "<<c<<endl;
				  mesh[c]->push_back(token);
				  d=vert->size();
				  c=groups.size();
				}
			  
			}

		if (token[0]=='v')
		  points.push_back(token);
		  


		//		if (vert->size()!=0)		  mesh.push_back(vert);
		}
		cout << endl;
		//sector#_mesh
		//123456 7891011
		char *filename=(char*)malloc(50);
		obj.close();
		obj.clear();
		cout << "fazendo o dump dos "<<groups.size()<<" setores"<<endl;
		for (int c=0;c<groups.size();c++)
		  {
		    sprintf(filename,"sector%d_mesh.obj",c);
		    fragment.open(filename);
		    for (int d=0;d<points.size();d++)
		      fragment << points[d] <<endl;
			 
		    for (int d=0;d<mesh[c]->size();d++)
		      fragment << (*mesh[c])[d]<<endl;
			 
		    fragment.close();
		    cout << "opening "<<filename <<endl;
		    obj.open(filename);		   
		    model=VAC_PolyBuffer::InternalizeFromScratch(obj);

			 menorx=BZK_FastMath::Int32toFix32(255);
			 maiorx=0;
			 menory=BZK_FastMath::Int32toFix32(255);
			 maiory=0;
			 menorz=BZK_FastMath::Int32toFix32(255);
			 maiorz=0;
			 while (!model->Empty())
			   {			     
			     tmp4=model->Pop();
			     tmp2=tmp4->Volume();
			     if (tmp2->GetX0()<menorx) menorx=tmp2->GetX0();
			     if (tmp2->GetX1()>maiorx) maiorx=tmp2->GetX1();
			     if (tmp2->GetY0()<menory) menory=tmp2->GetY0();
			     if (tmp2->GetY1()>maiory) maiory=tmp2->GetY1();
			     if (tmp2->GetH0()<menorz) menorz=tmp2->GetH0();
			     if (tmp2->GetH1()>maiorz) maiorz=tmp2->GetH1();
			   }
			 obj.close();
			 tmp3=new VAC_Sector();
			 tmp2->SetX0(menorx);
			 tmp2->SetX1(maiorx);				
			 tmp2->SetY0(menorz);
			 tmp2->SetY1(maiorz);				

			 cout << menorx <<endl;
			 cout << maiorx <<endl;
			 cout << menory <<endl;
			 cout << maiory <<endl;
			 cout << menorz <<endl;
			 cout << maiorz <<endl;			
			 tmp3->SetRect(tmp2->iRect);  
			 tmp3->SetH0(menory);
			 tmp3->SetH1(maiory); 
			// tmp3->iMeshName=filename;
			 World.AddSector(*tmp3);			 
			 svg <<"<rect id='sectdef"<<World.TotalSectors()<<"' style='fill:#ff0;stroke:#000' ";		
			 svg<<"width='"<<BZK_FastMath::Fix32toInt32(tmp3->GetX1()-tmp3->GetX0());			
			 svg<<"' height='"<<BZK_FastMath::Fix32toInt32(tmp3->GetH1()-tmp3->GetH0());			 
			 svg<<"' x=\""<<BZK_FastMath::Fix32toInt32(tmp3->GetX0());
			 svg<<"\" y=\""<<BZK_FastMath::Fix32toInt32(tmp3->GetH0());
			 svg<<"\"/>"<<endl;;			 
		  }
		free(filename);
		Optimize();
		//Split();
		for (int k=0;k<World.TotalSectors();k++)
			{
			World.GetSector(k)->iColor[0].SetR(BZK_FastMath::Int32toFix32(8));
			World.GetSector(k)->iColor[0].SetG(BZK_FastMath::Int32toFix32(8));
			World.GetSector(k)->iColor[0].SetB(BZK_FastMath::Int32toFix32(8));
			World.GetSector(k)->iColor[0].SetA(BZK_FastMath::Int32toFix32(255));
			World.GetSector(k)->iColor[1].SetR(BZK_FastMath::Int32toFix32(16));
			World.GetSector(k)->iColor[1].SetG(BZK_FastMath::Int32toFix32(16));
			World.GetSector(k)->iColor[1].SetB(BZK_FastMath::Int32toFix32(16));
			World.GetSector(k)->iColor[1].SetA(BZK_FastMath::Int32toFix32(255));
			World.GetSector(k)->iColor[2].SetR(BZK_FastMath::Int32toFix32(32));
			World.GetSector(k)->iColor[2].SetG(BZK_FastMath::Int32toFix32(32));
			World.GetSector(k)->iColor[2].SetB(BZK_FastMath::Int32toFix32(32));
			World.GetSector(k)->iColor[2].SetA(BZK_FastMath::Int32toFix32(255));
			World.GetSector(k)->iColor[3].SetR(BZK_FastMath::Int32toFix32(64));
			World.GetSector(k)->iColor[3].SetG(BZK_FastMath::Int32toFix32(64));
			World.GetSector(k)->iColor[3].SetB(BZK_FastMath::Int32toFix32(64));
			World.GetSector(k)->iColor[3].SetA(BZK_FastMath::Int32toFix32(255));
			World.GetSector(k)->iColor[4].SetR(BZK_FastMath::Int32toFix32(128));
			World.GetSector(k)->iColor[4].SetG(BZK_FastMath::Int32toFix32(128));
			World.GetSector(k)->iColor[4].SetB(BZK_FastMath::Int32toFix32(128));
			World.GetSector(k)->iColor[4].SetA(BZK_FastMath::Int32toFix32(255));
			World.GetSector(k)->iColor[5].SetR(BZK_FastMath::Int32toFix32(255));
			World.GetSector(k)->iColor[5].SetG(BZK_FastMath::Int32toFix32(255));
			World.GetSector(k)->iColor[5].SetB(BZK_FastMath::Int32toFix32(255));
			World.GetSector(k)->iColor[5].SetA(BZK_FastMath::Int32toFix32(255));
			}
			
		
		World.SaveMap((char*)aOut.c_str(), People);
		svg << "</svg>"<<endl;
		svg.close();
	}
	
       
}
*/
