class VAC_Graphic:public BZK_Vector<VAC_Shape*>
{
 public:
  BZK_FixP iHead;
  BZK_FixP iTail;
  BZK_Vec2f iDimensions;
  string iResourceName;
  bool iOriginalAspectRatio;
  BZK_FixP ResX,ResY;
  ~VAC_Graphic()
    {      
	EraseAll();
    }


  void GetRect(BZK_DOMNode &aNode)
  {
    int x0,x1,y0,y1;
    BZK_DOMNode *node;
    node=aNode.GetSon();
    VAC_Shape *rect;
    BZK_Vec2f *point;
    rect=new VAC_Shape();
    while (node!=NULL)
      {
	//Visit(*node);
	if (node->GetName()=="style")
	  {
	    BZK_XMLReader::ParseStyle(node->GetContent(),aNode);
	    node->SetName("style_parsed");
	    GetRect(aNode);
	    delete rect;
	    return;
	  }

	if (node->GetName()=="x")
	  x0=BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(node->GetContent()));

	if (node->GetName()=="y")
	  y0=BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(node->GetContent()));

	if (node->GetName()=="width")
	  x1=BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(node->GetContent()));

	if (node->GetName()=="height")
	  y1=BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(node->GetContent()));
	

	if (node->GetName()=="fill")
	  rect->iFill.DecodeXMLColor(node->GetContent());
	
		if (node->GetName()=="opacity")
	  {
		BZK_FixP value=BZK_FastMath::Int32toFix32(1);
		value=BZK_ParsingTools::GetNumber(node->GetContent());
		value=BZK_FastMath::Mul(value,BZK_FastMath::Int32toFix32(255));
		rect->iFill.SetA(value);
  		
	  }
	node=node->GetNext();
      } 
    x1+=x0;
    y1+=y0;
 
    point=new BZK_Vec2f();
    point->SetX(BZK_FastMath::Mul(iDimensions.GetX(),BZK_FastMath::Int32toFix32(x0)));
    point->SetY(BZK_FastMath::Mul(iDimensions.GetY(),BZK_FastMath::Int32toFix32(y0)));
    rect->iPoint.Add(point);
    point=new BZK_Vec2f();
    point->SetX(BZK_FastMath::Mul(iDimensions.GetX(),BZK_FastMath::Int32toFix32(x1)));
    point->SetY(BZK_FastMath::Mul(iDimensions.GetY(),BZK_FastMath::Int32toFix32(y0)));
    rect->iPoint.Add(point);
    point=new BZK_Vec2f();
    point->SetX(BZK_FastMath::Mul(iDimensions.GetX(),BZK_FastMath::Int32toFix32(x1)));
    point->SetY(BZK_FastMath::Mul(iDimensions.GetY(),BZK_FastMath::Int32toFix32(y1)));
    rect->iPoint.Add(point);
    point=new BZK_Vec2f();
    point->SetX(BZK_FastMath::Mul(iDimensions.GetX(),BZK_FastMath::Int32toFix32(x0)));
    point->SetY(BZK_FastMath::Mul(iDimensions.GetY(),BZK_FastMath::Int32toFix32(y1)));
    rect->iPoint.Add(point);
    rect->iOutLine.SetColor(0,0,0,0);
    Push(rect);
    

    // //cout << "rect com "<<x0<<","<<y0<<"-"<<x1<<","<<y1<<endl;
  }

  void GetPath(BZK_DOMNode &aNode)
  {
    BZK_DOMNode *node;
    node=aNode.GetSon();
    VAC_Shape *path;
    BZK_Vec2f *point;
    string instructions;
    string data;   
    path=new VAC_Shape();
    path->iType=VAC_Shape::POLYGON;    
    path->iFill.SetColor(0,0,0,BZK_FastMath::Int32toFix32(255));
    path->iOutLine.SetColor(0,0,0,0);
    int backup;
    while (node!=NULL)
      {
	//Visit(*node);

	if (node->GetName()=="style")
	  {
	    BZK_XMLReader::ParseStyle(node->GetContent(),aNode);
	    node->SetName("style_parsed");
	    GetPath(aNode);
	    delete path;
	    return;
	  }

	if (node->GetName()=="opacity")
	  {
		BZK_FixP value=BZK_FastMath::Int32toFix32(1);
		value=BZK_ParsingTools::GetNumber(node->GetContent());
		value=BZK_FastMath::Mul(value,BZK_FastMath::Int32toFix32(255));
		path->iFill.SetA(value);
  		
	  }

	if (node->GetName()=="fill")
	  path->iFill.DecodeXMLColor(node->GetContent());
	if (node->GetName()=="d")
	  {	
	    char op;
	    // //cout << "content:"<<node->GetContent()<<endl;
	    instructions=node->GetContent();
	    for (int token=0;token<instructions.length();token++)
	      {
		op=instructions[token];
		switch (op)
		  {
		  case 'C':

		    //path->iNumberOfPoints++;
		    //    //cout << "ponto" <<endl;
		    //point=new BZK_Vec2f();
		   
		    for (int k=0;k<2;k++)
		    {
		      token++; //jump white space
		      data="";
		      while (token<instructions.length() && (instructions[token]!=','))
			data+=instructions[token++];
		      if (instructions[token]==' ')
			{cout<<"graphic image panic"<<endl;exit(0);} //premature end of coordenates?
		      // point->SetX(BZK_ParsingTools::GetNumber(data));
		      token++; //jump ','
		      // //cout << "coords:"<<data;
		      data="";
		      while (token<instructions.length() && instructions[token]!=' ')
			data+=instructions[token++];
		      // point->SetY(BZK_ParsingTools::GetNumber(data));
		      ////cout << ","<<data<<endl;
		      //path->iPoint.Add(point);
		      
		    }

		  case 'M':
		  case 'L':
		    ////cout << "ponto" <<endl;
		    point=new BZK_Vec2f();
		    token++; //jump white space
		    {
		      data="";
		      if (BZK_ParsingTools::Contains(instructions,","))
			{
		      while (token<instructions.length() && (instructions[token]!=',' ) )
			data+=instructions[token++];

		      if (instructions[token]==' ')
			{cout<<"graphic image panic"<<endl;exit(0);} //premature end of coordenates?
			}
			else
		      while (token<instructions.length() && (instructions[token]!=' ' ) )
			data+=instructions[token++];



		      point->SetX(BZK_FastMath::Mul(BZK_ParsingTools::GetNumber(data),iDimensions.GetX()));
		      token++; //jump ','
		      //    //cout << "coords:"<<data;
		      data="";
		      while (token<instructions.length() && instructions[token]!=' ')
			data+=instructions[token++];
		      point->SetY(BZK_FastMath::Mul(BZK_ParsingTools::GetNumber(data),iDimensions.GetY()));
		      ////cout << ","<<data<<endl;
		      path->iPoint.Add(point);
		      
		    }
		    break;
		  case 'z':
		    ////cout << "fim" <<endl;
		    token=instructions.length();
		    break;
		  }
	      }
	  }
	node=node->GetNext();
      }   
 
    Push(path);

  }

  void Visit(BZK_DOMNode &aNode)
  {
    BZK_DOMNode *node;
    if (aNode.GetNameSpace()!="svg")
      {
	//	//cout << "panic!" << aNode.GetNameSpace() <<endl;
      }
    //evaluate the node
    //  //cout << "nome:" << aNode.GetName() <<endl;
    if (aNode.GetName()=="svg" && aNode.GetNameSpace()=="XMLDoc")
	{
		BZK_DOMNode *sons=aNode.GetSon();
		while (sons!=NULL)
			{
//			if (sons->GetFather()!=NULL && sons->GetFather()->GetName()!="rect")
			{
			if (sons->GetName()=="width")	
				{
//					cout << sons->GetContent() <<endl;							
					iDimensions.SetX(BZK_ParsingTools::GetNumber(sons->GetContent()));
				}
			if (sons->GetName()=="height")
				{
					//cout << sons->GetContent() <<endl;
					iDimensions.SetY(BZK_ParsingTools::GetNumber(sons->GetContent()));
				}
			}
//			cout << iDimensions.GetX() <<endl;
//			cout << iDimensions.GetY() <<endl;
			sons=sons->GetNext();
			}
			iDimensions.SetX(BZK_FastMath::Div(BZK_FastMath::Int32toFix32(ResX),iDimensions.GetX()));
			iDimensions.SetY(BZK_FastMath::Div(BZK_FastMath::Int32toFix32(ResY),iDimensions.GetY()));
			//cout << iDimensions.GetX() <<endl;
		//	cout << iDimensions.GetY() <<endl;
	}
    if (aNode.GetName()=="rect")
      {
	GetRect(aNode);
	//Push(new VAC_Shape());
	////cout << "**** peguei um RECT!"<<endl;
      }
    if (aNode.GetName()=="path")
      {
	GetPath(aNode);
	//Push(new VAC_Shape());
	//	//cout << "**** peguei uma PATH!"<<endl;
      }
    node=aNode.GetSon();
    while (node!=NULL)
      {
	Visit(*node);
	node=node->GetNext();
      }
 
  }

	void Translate(BZK_Vec2f aTranslate)
  	{
	    VAC_Graphic tmp2;
	    VAC_Shape *handler;    
	//    while (!Empty())
	int qtd=TotalItems();
	for (int c=0;c<qtd;c++)
	      {
		handler=(*this)[c];//Pop();
		handler->Translate(aTranslate);
		//tmp2.Push(handler);	
	      }
	  //  while (!tmp2.Empty()) Push(tmp2.Pop());
	}


  void Internalize(std::string aFile,bool aKeepOriginalAspectRatio=false)
    {
	if (iOriginalAspectRatio=aKeepOriginalAspectRatio)
		{
		ResX=BZK_FastMath::Int32toFix32(800);
		ResY=BZK_FastMath::Int32toFix32(480);
		}
	else
		{
		ResX=MXRES;
		ResY=MYRES;
		}


	iResourceName=aFile;
//	cout << "file:"<<aFile<<endl;
      BZK_XMLReader reader;
      reader.Internalize(aFile);
      Visit(*reader.GetRoot());      
 			iDimensions.SetX(BZK_FastMath::Mul(MXRES,iDimensions.GetX()));
			iDimensions.SetY(BZK_FastMath::Mul(MYRES,iDimensions.GetY()));
	reader.GetRoot()->DeleteCascading();
	delete reader.GetRoot();
    }
 //---------------------------------------------------------------------------------------
void  Externalize (string aFilename)
   {
  
   }
  
static
  VAC_Graphic *InternalizeFromScratch(std::string aFile,bool aKeepOriginalAspectRatio=false)
 {
   VAC_Graphic *tmp=new VAC_Graphic();   
   tmp->Internalize(aFile,aKeepOriginalAspectRatio);
   return tmp;	
 }


  long iDistance;
  
  VAC_Graphic():iHead(0),iTail(0),iOriginalAspectRatio(false)
    {
/*
      iHead=0;
      iTail=0;
      iOriginalAspectRatio=false;  
*/
    }

///TODO: change the use of stack for another ADT
  VAC_PolyBuffer *toPolyBuffer()
  {
    VAC_PolyBuffer *tmp=new VAC_PolyBuffer();
    VAC_Graphic tmp2;
    VAC_Shape *handler;    
 //   while (!Empty())
   int qtd=TotalItems();
for (int c=0;c<qtd;c++)
      {
	//	//cout <<"totalitems:"<<TotalItems()<<","<<tmp->TotalItems()<<endl;
	handler=(*this)[c];//Pop();
	//handler->toFace();
	tmp->Add(handler->toFace());
	//tmp2.Push(handler);	
      }
   // while (!tmp2.Empty()) Push(tmp2.Pop());
    ////cout << "polybuffer construido" <<endl;
    return tmp;
  }

  VAC_PolyBuffer *toPolyBufferDisposable()
  {
    VAC_PolyBuffer *tmp=new VAC_PolyBuffer();
    VAC_Shape *handler;    
   int qtd=TotalItems();
for (int c=0;c<qtd;c++)
      {
	//	//cout <<"totalitems:"<<TotalItems()<<","<<tmp->TotalItems()<<endl;
	handler=(*this)[c];//Pop();
	tmp->Push(handler->toFace());
//	delete handler;
      }
     EraseAll();
    return tmp;
  }


  VAC_Graphic *Clone()
  {
    VAC_Graphic *tmp=new VAC_Graphic();
  //  VAC_Graphic tmp2;
    VAC_Shape *handler;    
   int qtd=TotalItems();
for (int c=0;c<qtd;c++)
      {
	//	//cout <<"totalitems:"<<TotalItems()<<","<<tmp->TotalItems()<<endl;
	handler=(*this)[c];//Pop();
	//handler->toFace();
	tmp->Push(handler->Clone());
//	tmp2.Push(handler);	
      }
  //  while (!tmp2.Empty()) Push(tmp2.Pop());
    ////cout << "polybuffer construido" <<endl;
    return tmp;
  }
 
  
};
