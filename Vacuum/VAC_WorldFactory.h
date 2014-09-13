class VAC_WorldFactory
{
public:
class FaceColor:public VAC_ColorRGBA
{
public:
int iSlot;

void Copy(FaceColor aFaceColor)
{
SetR(aFaceColor.GetR());
SetG(aFaceColor.GetG());
SetB(aFaceColor.GetB());
SetA(aFaceColor.GetA());
iSlot=aFaceColor.iSlot;
}
};

class ConnectionList
{
public:
int iConnections[6];
};

static VAC_ColorRGBA GetRGBA(BZK_DOMNode *aNode)
{
VAC_ColorRGBA rgba;
string nodename;
BZK_DOMNode *child=aNode->GetSon();
while (child!=NULL)
	{
	nodename=child->GetName();	

	if (BZK_ParsingTools::toLowerCase(nodename)=="_text" && BZK_ParsingTools::TrimToken(child->GetContent())!="")		
		rgba.DecodeXMLColor(child->GetContent());		

	child=child->GetNext();
	}
return rgba;
}

static FaceColor GetFaceColor(BZK_DOMNode *aNode)
{
FaceColor fc;
string nodename;
BZK_DOMNode *child=aNode->GetSon();

while (child!=NULL)
	{
	nodename=child->GetName();	

	if (BZK_ParsingTools::toLowerCase(nodename)=="rgba" || BZK_ParsingTools::toLowerCase(nodename)=="rgb")
		{
		fc.Copy(GetFaceColor(child));
		}
		
	if (BZK_ParsingTools::toLowerCase(nodename)=="_text" && BZK_ParsingTools::TrimToken(child->GetContent())!="")		
		fc.iSlot=BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(child->GetContent()));	


	child=child->GetNext();
	}

return fc;
}

static ConnectionList GetConnections(BZK_DOMNode *aNode)
{
ConnectionList con;
string nodename;
BZK_DOMNode *child=aNode->GetSon();

while (child!=NULL)
	{
	nodename=child->GetName();	

	if (BZK_ParsingTools::toLowerCase(nodename)=="_text" && BZK_ParsingTools::TrimToken(child->GetContent())!="")		
		for (int c=0;c<6;c++)
		con.iConnections[c]=BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(BZK_ParsingTools::GetSubToken(child->GetContent(),c)));

	child=child->GetNext();
	}

return con;	
}

static int GetTrigger(BZK_DOMNode *aNode)
{
string nodename;
BZK_DOMNode *child=aNode->GetSon();

while (child!=NULL)
	{
	nodename=child->GetName();	

		
	if (BZK_ParsingTools::toLowerCase(nodename)=="_text" && BZK_ParsingTools::TrimToken(child->GetContent())!="")		
		return BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(child->GetContent()));	

	child=child->GetNext();
	}
}

static BZK_VolumetricSector GetVolume(BZK_DOMNode *aNode)
{
VAC_Vec3f dest;
BZK_VolumetricSector vol;

string nodename;
BZK_DOMNode *child=aNode->GetSon();

while (child!=NULL)
	{
	nodename=child->GetName();	

	if (BZK_ParsingTools::toLowerCase(nodename)=="_text" && BZK_ParsingTools::TrimToken(child->GetContent())!="")		
		{
		vol.SetX0(BZK_ParsingTools::GetNumber(BZK_ParsingTools::GetSubToken(child->GetContent(),0)));
		vol.SetY0(BZK_ParsingTools::GetNumber(BZK_ParsingTools::GetSubToken(child->GetContent(),1)));
		vol.SetH0(BZK_ParsingTools::GetNumber(BZK_ParsingTools::GetSubToken(child->GetContent(),2)));
		}

	if (BZK_ParsingTools::toLowerCase(nodename)=="vec3")
		{
		dest.Internalize(child->GetContent());
		vol.SetX1(dest.GetX());
		vol.SetY1(dest.GetY());
		vol.SetH1(dest.GetZ());
		}
	child=child->GetNext();
	}

return vol;
}

static VAC_Sector *GetSector(BZK_DOMNode *aNode)
{
string nodename;
BZK_DOMNode *child=aNode->GetSon();
VAC_Sector *ptr=new VAC_Sector();
BZK_VolumetricSector vol;
ConnectionList con;
int trigger;
FaceColor fc;
cout << "get sector" <<endl;
while (child!=NULL)
	{
	nodename=child->GetName();	

	if (BZK_ParsingTools::toLowerCase(nodename)=="facecolor")
		{
		fc=GetFaceColor(child);
		ptr->iColor[fc.iSlot]=fc;
		}
		
	if (BZK_ParsingTools::toLowerCase(nodename)=="connections")
		con=GetConnections(child);

	if (BZK_ParsingTools::toLowerCase(nodename)=="trigger")
		trigger=GetTrigger(child);

	if (BZK_ParsingTools::toLowerCase(nodename)=="volume")
		vol=GetVolume(child);


	child=child->GetNext();
	}

memcpy(ptr,&vol,sizeof(BZK_VolumetricSector));
for (int c=0;c<6;c++)
	ptr->SetConnection(0,con.iConnections[c]);

ptr->SetTrigger(trigger);

return ptr;
}

static void Visit(BZK_DOMNode *aNode,VAC_World &World)
{
string name=aNode->GetName();
if (BZK_ParsingTools::toLowerCase(name)=="sector")
	World.AddSector(*GetSector(aNode));

BZK_DOMNode *child=aNode->GetSon();
if (child!=NULL)
	Visit(child,World);
child=aNode->GetNext();
if (child!=NULL)
	Visit(child,World);
}

static VAC_World *Build(string aUrl)
{
VAC_World *world=new VAC_World();
BZK_XMLReader reader;
reader.Internalize(aUrl);
Visit(reader.GetRoot(),*world);
world->stop=true;
return world;
}
};

