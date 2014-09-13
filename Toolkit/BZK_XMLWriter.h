///@brief ultra-lightweight-inneficient-makeshift-poor-man XML writer (to a given DOM-like tree)
///@author Daniel "NeoStrider" Monteiro
class BZK_XMLWriter
{
void Write(BZK_DOMNode *aNode,std::ofstream &aFileOut)
{
BZK_DOMNode *head;



if (aNode->GetType()==BZK_DOMNode::PROPERTY)
	{
	aFileOut << ' ';
	if (aNode->GetNameSpace()!="XMLDoc")
		aFileOut << aNode->GetNameSpace() <<':';
	aFileOut << aNode->GetName()<<"=\""<<aNode->GetContent()<<"\"";
	return;
	}



if (aNode->GetType()==BZK_DOMNode::SPECIAL)
{

		
		aFileOut << "<";
		aFileOut << aNode->GetName();
		head=aNode->GetSon();
		while (head!=NULL)
		{
			Write(head,aFileOut);			
			head=head->GetNext();
		}
		aFileOut  << "?>";
}


if (aNode->GetType()!=BZK_DOMNode::SPECIAL)
if (aNode->GetContent()=="" && aNode->GetSon()==NULL)
	{
		if (aNode->GetName()!="_text" && aNode->GetName()!="DOCROOT")
		{
		aFileOut << '<';
		if (aNode->GetNameSpace()!="XMLDoc")
			aFileOut<< aNode->GetNameSpace() << ':';
		aFileOut<< aNode->GetName() << "/>";
		}
	}
	else
	{
		if (aNode->GetName()!="_text" && aNode->GetName()!="DOCROOT")
		{
		aFileOut << '<';
		if (aNode->GetNameSpace()!="XMLDoc")
			aFileOut<< aNode->GetNameSpace() << ':';



		aFileOut<< aNode->GetName()<<' ';
		
		head=aNode->GetSon();
		
		while (head!=NULL)
		{
			if (head->GetType()==BZK_DOMNode::PROPERTY)	
				Write(head,aFileOut);			
			head=head->GetNext();
		}

		aFileOut << '>';
		}

		aFileOut <<aNode->GetContent();
		head=aNode->GetSon();

		while (head!=NULL)
		{
			if (head->GetType()!=BZK_DOMNode::PROPERTY)	
				Write(head,aFileOut);			
				head=head->GetNext();
		}

		if (aNode->GetName()!="_text" && aNode->GetName()!="DOCROOT")
		{

		aFileOut << "</";
		if (aNode->GetNameSpace()!="XMLDoc")
			aFileOut<< aNode->GetNameSpace() << ':';
		aFileOut<< aNode->GetName() << '>';
		}
	}
}
public:
void Externalize(BZK_DOMNode *aNode,std::string aFile)
{
	BZK_DOMNode *head=aNode;
	std::ofstream FileOut(aFile.c_str());
	Write(head,FileOut);
}

};
