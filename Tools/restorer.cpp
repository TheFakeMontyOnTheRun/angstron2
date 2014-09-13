///TODO: make two distintive functionalities - dissasemble maps (revert compilation) and optimize and build hierarchy.
/// if revert, add the master sectors to world[3], dont make references for it in its sons and just save map[3], else, stay as it is.
#define SIZESECTOR 5
using namespace std;
#include "BZK_POSIX.h"

/*
Steps in the algorithm (its not the most eficient, but (kind of) works:
1 - figure out wich sectors make up the border of the map and make a group for each of those sectors
2 - add to each of those groups all the sectors connected to the oposite direction of the border
(eg: if the border is at the south, takes recursivily all sectors connected to the north)
3 - create and expand a master sector capable of holding each group

*/


#include "Toolkit/BZK_Toolkit.h"
#include "BZK_SDL.h"
#include "AppArch/BZK_AppArch.h"
#include "GameElements/GameElements.h"
#include "Vacuum/VAC_BaseIncludes.h"
#include "bzk1/World.h"
//#include "Vacuum/VAC_LogoLang.h"

//groups[0] and groups[1] hold the groups of groups of sectors
//groups[2] holds temporary groups
BZK_Vector<BZK_Vector<int>*> groups[3];
//dummy game control
void GameStart(VAC_ActorManager&, VAC_World&)
{
}

int a;
VAC_World World[3];
VAC_ActorManager People;
BZK1_PET PET;
VAC_Sector *CurrentSector;//=World.GetSector(0);

bool ingroup(BZK_Vector<int> *group,int value)
{
for (int c=0;c<group->TotalItems();c++)
	if (((*group)[c])==value) return true;
return false;
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

//makes a connected graph containing all sectors connected to a certain sector over a certain direction
void CreateGroup(int c)
{
cout << "criando grupo p/ setor "<<c<<endl;
int current;
BZK_Vector<int> *group=new BZK_Vector<int>();

current=c;
while (World[a].GetSector(current)->Get_Connection(a)!=0)
{
group->Add(current);
current=World[a].GetSector(current)->Get_Connection(a);
}
groups[a].Add(group);
}


int main(int argc, char *argv[])
{

if (argc<2)
cout << "usage: pa [optimize|revert] [path]/map1.bzk2" <<endl;
else
//for (a=0;a<2;a++)
{
a=0;
World[a].LoadMap(argv[1],People);

// for each sector, if this sector has no connection in direction contrary to that certain direction, create a group for him
for (int c=1;c<World[a].TotalSectors();c++)
	{
	
		//if (EqSec(c)) World[1].GetSector(c)->iParent=World[a].GetSector(c)->iParent;
		if (World[a].GetSector(c)->Get_Connection(BZK_FastMath::GetOpositeDirection(a))==0)
			{
			CreateGroup(c);			
			}
	}
BZK_Vector <int> *maior;
BZK_Vector <int> *menor;
bool ongroup;

/*
///still trying to figure out what happens here...
///for each direction in the XZ plane
for (int g=0;g<4;g++)
/// for earch group in the current direction of analisys
for (int d=0;d<groups[a].TotalItems();d++)
/// process against other groups of the current direction of analisys
	for (int e=0;e<groups[a].TotalItems();e++)
		{
		//if they're the same, skip
		if (e==d) continue;
		///if they're empty, skip
		if (!groups[a][e]->TotalItems()) continue;
		if (!groups[a][d]->TotalItems()) continue;

		cout << d<<","<<e<<","<<groups[a].TotalItems()<<endl;

		ongroup=true;
		///get the biggest and the smallest group
		if (groups[a][e]->TotalItems() < groups[a][d]->TotalItems())
			{
			maior=groups[a][d];
			menor=groups[a][e];
			}
		else
			{	
			maior=groups[a][e];
			menor=groups[a][d];
			}
		
		{
		///assumes all connected sectors are "on group", that is, the graphs are disjoint
		ongroup=true;
			///for each sector in the bigger group...
			for (int f=0;f<maior->TotalItems();f++)			
			{
			///if its connection in the current direction is not null...
			 if (World[a].GetSector((*maior)[f])->Get_Connection(g)!=0)
				///if the connected sector is inside the smaller group		
				if(!ingroup(menor,World[a].GetSector((*maior)[f])->Get_Connection(g)))
				  ///sets the state var of "not inside"
				  ongroup=false;
			}
		}
		/// if the graphs are disjoint
		if (ongroup)
			{
			cout << "copiando "<<menor->TotalItems()<<" items" <<endl;
			///for each of the smaller group sectors
			for (int f=0;f<menor->TotalItems();f++)
				{
				cout <<"verificando se ja tem"<< (*menor)[f]<<"...";
				/// if the sector in the smaller group is not also in the bigger group
				if (!ingroup(maior,(*menor)[f]))
					{
					///copies it to the bigger group
					maior->Add((*menor)[f]);
					cout <<"nao tem.adicionado"<<endl;
					}
					else cout <<"ja tem"<<endl;
				}
			///erase the smaller group
			menor->ReleaseAll();
			}
		}
*/

cout <<  groups[0].TotalItems() <<" contra "<<groups[1].TotalItems()<<endl;


//if (groups[0].TotalItems() >groups[1].TotalItems())
	a=0;
//else
//	a=1;

BZK_Rect *tmp2;
VAC_Sector *tmp3;
int _x[2],_y[2],_z[2];

/// nodos marks the stages of processing.
///nodos[0] holds parent sectors as they're being shaped
///nodos[1] holds parent sectors 

BZK_Vector<VAC_Sector*> nodos[3];
BZK_Vector<int> *tmp;
int original=World[0].TotalSectors();


//consolidate the groups into sectors

///for each group
for (int z=0;z<groups[a].TotalItems();z++)
	{	
	tmp3=NULL;
	///if the group is not empty
	if (groups[a][z]->TotalItems()>0)
		{
		///get the dimensions of the first sector in the group
	       _x[0]=BZK_FastMath::Fix32toInt32(World[a].GetSector((*groups[a][z])[0])->GetX0());
	       _y[0]=BZK_FastMath::Fix32toInt32(World[a].GetSector((*groups[a][z])[0])->GetY0());
	       _x[1]=BZK_FastMath::Fix32toInt32(World[a].GetSector((*groups[a][z])[0])->GetX1());
	       _y[1]=BZK_FastMath::Fix32toInt32(World[a].GetSector((*groups[a][z])[0])->GetY1());
	       _z[0]=BZK_FastMath::Fix32toInt32(World[a].GetSector((*groups[a][z])[0])->GetH0());
	       _z[1]=BZK_FastMath::Fix32toInt32(World[a].GetSector((*groups[a][z])[0])->GetH1());
		//the sector to be made father
		tmp3=new VAC_Sector();
		//and a group to hold its children
		tmp=new BZK_Vector<int>();
		///adds the parent sector to the world		
		World[a].AddSector(*tmp3);		
		///and to the group of parents sectors to be processed
		nodos[0].Add(tmp3);
		}
		///save the groups of sons
		groups[2].Add(tmp);

	///for each sector in the group
	for (int y=0;y<groups[a][z]->TotalItems();y++)
		{
		
			//cout << (*groups[a][z])[y]<<";";
			///register the sector in the group of sons
			tmp->Add((*groups[a][z])[y]);
			//cout.flush();
			////////////////////expands the father sector's dimensions if needed to acomodate its sons////////
			if (BZK_FastMath::Fix32toInt32(World[a].GetSector((*groups[a][z])[y])->GetX0())
				<_x[0])
				_x[0]=BZK_FastMath::Fix32toInt32(World[a].GetSector((*groups[a][z])[y])->GetX0());

			if (BZK_FastMath::Fix32toInt32(World[a].GetSector((*groups[a][z])[y])->GetX1())
				>_x[1])
				_x[1]=BZK_FastMath::Fix32toInt32(World[a].GetSector((*groups[a][z])[y])->GetX1());

			if (BZK_FastMath::Fix32toInt32(World[a].GetSector((*groups[a][z])[y])->GetY0())
				<_y[0])
				_y[0]=BZK_FastMath::Fix32toInt32(World[a].GetSector((*groups[a][z])[y])->GetY0());

	 		if (BZK_FastMath::Fix32toInt32(World[a].GetSector((*groups[a][z])[y])->GetY1())
				>_y[1])
				_y[1]=BZK_FastMath::Fix32toInt32(World[a].GetSector((*groups[a][z])[y])->GetY1());

			if (BZK_FastMath::Fix32toInt32(World[a].GetSector((*groups[a][z])[y])->GetH0())
				<_z[0])
				_z[0]=BZK_FastMath::Fix32toInt32(World[a].GetSector((*groups[a][z])[y])->GetH0());

	 		if (BZK_FastMath::Fix32toInt32(World[a].GetSector((*groups[a][z])[y])->GetH1())
				>_z[1])
				_z[1]=BZK_FastMath::Fix32toInt32(World[a].GetSector((*groups[a][z])[y])->GetH1());
			///////////////////////////////////////////////////////////////////////////////////////////////////
			///set the son's parent
			World[a].GetSector((*groups[a][z])[y])->iParent=World[a].TotalSectors()-1;
			//cout <<(*groups[a][z])[y] << "-pai :"<<original+z<<endl;
		}
		///finally sets the fathers dimensions, so to hold its sons
		if (tmp3!=NULL)
		{
		tmp3->SetX0(BZK_FastMath::Int32toFix32(_x[0]));
		tmp3->SetX1(BZK_FastMath::Int32toFix32(_x[1]));
		tmp3->SetY0(BZK_FastMath::Int32toFix32(_y[0]));
		tmp3->SetY1(BZK_FastMath::Int32toFix32(_y[1]));
		tmp3->SetH0(BZK_FastMath::Int32toFix32(_z[0]));
		tmp3->SetH1(BZK_FastMath::Int32toFix32(_z[1]));
		}

	}

VAC_Sector *sec[2];

///still trying to figure out what happens here...
///for each parent sector
for (int h=0;h<nodos[0].TotalItems();h++)
	{
	///copy it to nodes[1]
	nodos[1].Add(nodos[0][h]);
	///for each already processed sector...
	for (int i=h;i<nodos[0].TotalItems();i++)
		{
		//if they're the same, skip
		if (i==h) continue;	

		//make shortcuts
		sec[0]=nodos[0][h];
		sec[1]=nodos[0][i];


		//if (!sec[1]->HasBeenDrawn()) continue;
		//if (!nodos[0][h]->HasBeenDrawn()) continue;
		//sec[0]->Visiting();
		//sec[1]->Visiting();
		
		///if they're the same in YZ...
		if (sec[0]->GetY0() ==sec[1]->GetY0() && sec[0]->GetY1() ==sec[1]->GetY1() && sec[0]->GetH0() ==sec[1]->GetH0() && sec[0]->GetH1() ==sec[1]->GetH1())
			{
				///copy the sons of the already processed to the newer
				for (int x2=0;x2<(*(groups[2])[i]).TotalItems();x2++)
					(*(groups[2])[h]).Add((*(groups[2])[i])[x2]);

				///make them ocupy the same place in space
				if (sec[0]->GetX0() > sec[1]->GetX0())					
					sec[0]->SetX0(sec[1]->GetX0());					

				if (sec[0]->GetX1() < sec[1]->GetX1())
					sec[0]->SetX1(sec[1]->GetX1());
				
			}
		}
}


//for (int h=0;h<nodos[1].TotalItems();h++)
//	nodos[1][h]->NotifyEndOfCicle();

for (int h=0;h<nodos[1].TotalItems();h++)
	{	
	nodos[2].Add(nodos[1][h]);
	///for each already processed parent sector...
	for (int i=h;i<nodos[1].TotalItems();i++)
		{
		///if they're the same, skip
		if (i==h) continue;		
		///make a shortcut
		sec[0]=nodos[1][h];
		sec[1]=nodos[1][i];


		//if (!sec[1]->HasBeenDrawn()) continue;
		//if (!sec[0]->HasBeenDrawn()) continue;
		//sec[0]->Visiting();
		//sec[1]->Visiting();

		///if they ocupy the same space in XZ (ignore the Y - its notation)...
		if (sec[0]->GetY0() ==sec[1]->GetY0() && sec[0]->GetY1() ==sec[1]->GetY1() && sec[0]->GetX0() ==sec[1]->GetX0() && sec[0]->GetX1() ==sec[1]->GetX1())
			{
				///copy the sons of the already processed sector
				for (int x2=0;x2<(*(groups[2])[i]).TotalItems();x2++)
					(*(groups[2])[h]).Add((*(groups[2])[i])[x2]);


				//// make them ocupy the same place in space 

				if (sec[0]->GetH0() > sec[1]->GetH0())					
					sec[0]->SetH0(sec[1]->GetH0());					

				if (sec[0]->GetH1() < sec[1]->GetH1())
					sec[0]->SetH1(sec[1]->GetH1());
				
			}
		}
	}

// writes input and output SVG files

ofstream svg("entrada.svg");
WriteSVGHeader(svg);
for (int i=1;i<World[a].TotalSectors();i++)
		{
		sec[0]=World[a].GetSector(i);
		svg << "<rect stroke='red' x='"<<BZK_FastMath::Fix32toInt32(sec[0]->GetX0())<<"' ";
		svg << "y='"<<BZK_FastMath::Fix32toInt32(sec[0]->GetY0())<<"' ";
		svg << "width='"<<BZK_FastMath::Fix32toInt32(sec[0]->GetX1())-BZK_FastMath::Fix32toInt32(sec[0]->GetX0())<<"' ";
		svg << "height='"<<BZK_FastMath::Fix32toInt32(sec[0]->GetY1())-BZK_FastMath::Fix32toInt32(sec[0]->GetY0())<<"' />"<<endl;
		}
svg<<"</svg>"<<endl;
svg.clear();
ofstream _svg("saida.svg");
WriteSVGHeader(_svg);
///for each surviving master sector
for (int i=0;i<nodos[2].TotalItems();i++)
		{
		sec[0]=nodos[2][i];
		///for each of its sons
		for (int x2=0;x2<(*(groups[2])[i]).TotalItems();x2++)
			///register the master as his father
			World[a].GetSector((*(groups[2])[i])[x2])->iParent=original+i;
		_svg << "<rect stroke='red' x='"<<BZK_FastMath::Fix32toInt32(sec[0]->GetX0())<<"' ";
		_svg << "y='"<<BZK_FastMath::Fix32toInt32(sec[0]->GetY0())<<"' ";
		_svg << "width='"<<BZK_FastMath::Fix32toInt32(sec[0]->GetX1())-BZK_FastMath::Fix32toInt32(sec[0]->GetX0())<<"' ";
		_svg << "height='"<<BZK_FastMath::Fix32toInt32(sec[0]->GetY1())-BZK_FastMath::Fix32toInt32(sec[0]->GetY0())<<"' />"<<endl;
		}
_svg<<"</svg>"<<endl;
World[a].SaveMap("./out.bzk2",People);



cout << "terminei" <<endl;
}
}
