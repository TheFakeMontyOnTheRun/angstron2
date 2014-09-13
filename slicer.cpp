using namespace std;
#include "BZK_POSIX.h"
#include "BZK_BaseIncludes.h"
#include "Vacuum/VAC_BaseIncludes.h"
BZK_Vector<BZK_Vector<string>*> slices;
BZK_Vector<string> classes;
BZK_Vector<string> dict;



void InitDict()
{
    dict.Add("MOVE_N");
    dict.Add("MOVE_E");
    dict.Add("MOVE_S");
    dict.Add("MOVE_W");
    dict.Add("MOVE_DOWN");
    dict.Add("MOVE_UP");
    dict.Add("TURN_L");
    dict.Add("TURN_R");
    dict.Add("ATACK");
    dict.Add("DEFENSE");
    dict.Add("TELEPORT");
    dict.Add("DAMAGE");
    dict.Add("VANISH");
    dict.Add("CONTROL");
    dict.Add("COLLIDE");
    dict.Add("CONTACT");
    dict.Add("CONNECT");
    dict.Add("ACK");
    dict.Add("NAK");
    dict.Add("PING");
    dict.Add("HALT");
    dict.Add("JOIN");
    dict.Add("LEAVE");
    dict.Add("CLASS");
    dict.Add("CREATE_ENTITY");
    dict.Add("LOADMAP");
    dict.Add("CLOSEMAP");
    dict.Add("NOP");
    dict.Add("NEUTRAL");
    dict.Add("LOOK_N");
    dict.Add("LOOK_S");
    dict.Add("LOOK_W");
    dict.Add("LOOK_E");
    dict.Add("LOOK_FLOOR");
    dict.Add("LOOK_CEILING");
    dict.Add("ACTION1");
    dict.Add("ACTION2");
    dict.Add("ACTION3");
    dict.Add("ACTION4");
    dict.Add("OPEN_DOOR");
    dict.Add("CLOSE_DOOR");
    dict.Add("RANDOM_SMALL_DELAY");
}

int translate(string token)
{
	token=BZK_ParsingTools::toHighCase(token);
//	cout <<"token :"<<token << "" <<endl;
	for (int c=0;c<dict.TotalItems();c++)
		if (dict[c]==token)
			return c;

	return -1;
}

int TakeSlice(BZK_DOMNode *aNode)
{
	BZK_Vector<string> *slice=new BZK_Vector<string>();
	BZK_DOMNode *ptr=aNode;
	BZK_DOMNode *aux;
	string content="";


	while (ptr!=NULL)
	{
	content=ptr->GetContent();
	if (BZK_ParsingTools::TrimToken(ptr->GetContent())!="")		
	{
			content=ptr->GetContent();
	//		cout << "this:"<<content << endl;
			slice->Add(content);	
			ptr=ptr->GetNext();
			continue;
	}

	if (ptr->GetSon()!=NULL)
	{
		//content=ptr->GetName();
		//content+=" ";
		content=BZK_FastMath::Tostring(TakeSlice(ptr->GetSon()));
		slice->Add(content);	
	//	cout << "son:"<<content << endl;
		ptr=ptr->GetNext();
		continue;
	}

	ptr=ptr->GetNext();
	}


	if (slice->TotalItems()>1)
		{
			slices.Add(slice);
			return slices.TotalItems()-1;
		}
		else
		{
			return translate((*slice)[0]);
		}
}


int main(int argc, char *argv[])
{
	InitDict();

  BZK_XMLReader test;
  BZK_Vector<string> *slice;
  test.Internalize("./testactor.xml"); 
  
//  slice->Add(BZK_FastMath::Tostring(
	TakeSlice(test.GetRoot()->GetSon())
//	))
	;	
	
	slice=slices[slices.TotalItems()-1];
	for (int d=1;d<slice->TotalItems();d++)		
		cout << (*slice)[d] << endl;
	
		
	cout << "ANTES" << endl;		
	for (int c=1;c<slices.TotalItems();c++)
	{
		cout <<c<< " slice:" << endl;
		slice=slices[c];
		for (int d=0;d<slice->TotalItems();d++)	
		if (!(c==slices.TotalItems()-1 && d==0))
			if (translate((*slice)[d] )==-1)
				cout << (*slice)[d] << endl;
			else
				cout << translate((*slice)[d] ) << endl;				
			
	}
	cout << "DEPOIS"<<endl;
	for (int c=1;c<slices.TotalItems();c++)
	{
		cout <<"<!--class"<< c<< "-->" << endl;
		slice=slices[c];
		for (int d=0;d<slice->TotalItems();d++)		
		if (!(c==slices.TotalItems()-1 && d==0))
		if (translate((*slice)[d] )!=-1)
		{
		cout << "<Actions>" << endl;
		cout << c << " "<<27<<" ";
			
		//		cout << (*slice)[d];
		//	else
				cout << translate((*slice)[d] );				
		cout << endl<<"</Actions>"<<endl;
		}	
	}
  return 0;
}
