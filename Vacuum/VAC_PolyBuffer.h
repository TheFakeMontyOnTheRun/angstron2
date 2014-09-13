class VAC_PolyBuffer:public BZK_Vector<VAC_Face*>
{
 public:
  string iResourceName;
 // BZK_FixP iHead;
 // BZK_FixP iTail;
  
/*
  BZK_Queue<VAC_Face*> iFaces;
 public:
  void Reset()
    {
      iFaces.EraseAll();
    }

  
  void Push(VAC_Face* aFace)
    {
      iFaces.Push(aFace);
    }
 
  void Push(VAC_PolyBuffer &aAnother)
    {
      while (!aAnother.Empty())	
	  Push(aAnother.Pop());	
    }*/
  
//TODO: make it actually order this mess
  void Order()
    {
      BZK_Stack<VAC_Face*> temp;
      BZK_Queue<VAC_Face*> final; //mais seguro assim...      
    }

  ~VAC_PolyBuffer()
    {      
      EraseAll();
    }

  static
 VAC_PolyBuffer *InternalizeFromScratch(ifstream &aObj)
 {  
   VAC_PolyBuffer *tmp=new VAC_PolyBuffer();
   tmp->Internalize(aObj);   
   return tmp;
 }

void Push(VAC_PolyBuffer &aAnother)
{
int qtd=aAnother.TotalItems();
for (int c=0;c<qtd;c++)
	Add(aAnother[c]);
aAnother.ReleaseAll();
}

void Flip()
{
BZK_Queue<VAC_Face*> tmp;
VAC_Face *tmp2;
BZK_FixP tmp3;
for (int d=0;d<TotalItems();d++)
	{
	tmp2=(*this)[d];
	for (int c=0;c<tmp2->GetTotalPoints();c++)
		{	
			tmp3=tmp2->GetPoint(c)->GetY();
			tmp2->GetPoint(c)->SetY(tmp2->GetPoint(c)->GetX());
			tmp2->GetPoint(c)->SetX(tmp3);
		}
	}
}


  void Internalize(ifstream &aObj)
  {
      if (!aObj.good())
	{
	  cout << "ERROR:resource not found!"<<endl;
	  exit(0);
	}
      //carrega um arquivo no formato Alias's Wavefront OBJ
      //cout << "iniciando leitura" <<endl;
      BZK_Vector<BZK_Vec3f*> Vertices;
      string token;
      int vert[255];
      float level;
      int count;
      float r,g,b,a;
      memset(vert,0,sizeof(int)*4);
	a=255.0;
      /*
      r=0.0;
      g=1.0;
      b=0.0;
      a=0.25;
      */
      while (!aObj.eof())
	{
	  getline(aObj,token);
	  if (token[0]=='f')
	    {
	      VAC_Face *handler=NULL;
	      //    //cout << "entrei" <<endl;
	      count=0;
	      while (BZK_ParsingTools::GetSubToken(token,count)!="")
		{
		vert[count]=BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(BZK_ParsingTools::GetSubToken(token,count+1)));
		count++;
		}
	      count--;

	      /*
	      vert[0]=BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(BZK_ParsingTools::GetSubToken(token,1)));   
	      vert[1]=BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(BZK_ParsingTools::GetSubToken(token,2)));
	      vert[2]=BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(BZK_ParsingTools::GetSubToken(token,3)));
	      vert[3]=BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(BZK_ParsingTools::GetSubToken(token,4)));
	      */
	      
	      for (int c=0;c<count;c++)		 if (vert[c]<=0) vert[c]=1;

	      /*
	      if (vert[0]<=0) vert[0]=1;
	      if (vert[1]<=0) vert[1]=1;
	      if (vert[2]<=0) vert[2]=1;
	      if (vert[3]<=0) vert[3]=1;
	      */
	      /*
	      for (int c=0;c<count;c++) 
		if (vert[c]-1>=Vertices.TotalItems())
		  {
		    //cout <<"** vertice fora de faixa **"<<endl;
		    exit(0);
		  }
	      */
		  //vert[c]=Vertices.TotalItems();
	      
	      /*
	      if (vert[0]>=Vertices.TotalItems()) vert[0]=Vertices.TotalItems();
	      if (vert[1]>=Vertices.TotalItems()) vert[1]=Vertices.TotalItems();
	      if (vert[2]>=Vertices.TotalItems()) vert[2]=Vertices.TotalItems();
	      if (vert[3]>=Vertices.TotalItems()) vert[3]=Vertices.TotalItems();
	      */
	     
	      handler=new VAC_Face();
	      // //cout << "count:" <<count<<endl;
	      for (int c=0;c<count;c++)
		handler->AddPoint(Vertices[vert[c]-1]);

	      //	      handler=new VAC_Face(Vertices[vert[0]-1],Vertices[vert[1]-1],Vertices[vert[2]-1],Vertices[vert[3]-1],0);


	      assert(handler!=NULL);
	      
	      handler->iFill.SetColor(BZK_FastMath::Real32toFix32(r*255),
				      BZK_FastMath::Real32toFix32(g*255),
				      BZK_FastMath::Real32toFix32(b*255),
				      BZK_FastMath::Real32toFix32(a*255));
	      handler->iOutLine.SetColor(0,
				      0,
				      0,
				      BZK_FastMath::Real32toFix32(a*255));
	     // handler->iFill.AddNoise();
	      
	      handler->DoNormal();
	      level=BZK_FastMath::Fix32toReal32(BZK_VecOps::DotProductFix(*handler->GetNormal(),BZK_Vec3f(1,1,1)))*255;
	      if (level <0)// 
		  level=-level;

		if (level > 128 && level <255)
		  handler->iFill.Darken(level);

	      Add (handler);
	      
	    }
	  
	  if(token[0]=='v') 
	    {
	      //    //cout << token << endl;
	      BZK_Vec3f *handler=new BZK_Vec3f();
	      handler->Internalize(token);
	      ////cout << BZK_VecOps::Print(*handler) <<endl;
	      Vertices.Add(handler);
	      
	    }  

	  if(token[0]=='u') //usemtl 
	    {	
	      string nomemtl=BZK_ParsingTools::GetSubToken(string(token),1);
/*
	      if (nomemtl=="Rock")
		{
		  r=0.5;g=0.5;b=0.5;a=1.0;	
		  //cout << "usando rocha" <<endl;
		}
		else
	      if (nomemtl=="Glass")
		{
		  r=0.6;g=0.6;b=0.9;a=0.3;	
		  //cout << "usando vidro" <<endl;
		}
		else
	      if (nomemtl=="Lava")
		{
		  r=1.0;g=0.28;b=0.078;a=0.7;	
		  //cout << "usando vidro" <<endl;
		}
		else
	      if (nomemtl=="Grass")
		{
		  r=0.142;g=0.57;b=0.07;a=1.0;	
		  //cout << "usando vidro" <<endl;
		}
		else

	      if (nomemtl=="Steel")
		{
		  r=0.65;g=0.65;b=0.65;a=1.0;	
		  //cout << "usando vidro" <<endl;
		}
		else
	      if (nomemtl=="Water")
		{
		  r=0.217;g=0.527;b=0.663;a=1.0;	
		  //cout << "usando vidro" <<endl;
		}
		else
	      if (nomemtl=="Dirt")
		{
		  r=0.45;g=0.46;b=0.27;a=1.0;	
		  //cout << "usando vidro" <<endl;
		}
		else
	      if (nomemtl=="Hologram")
		{
		  r=0.0;g=1.0;b=0.0;a=0.25;	
		  //cout << "usando vidro" <<endl;
		}
		else
	      if (nomemtl=="darkgrey"||nomemtl=="DarkGrey")
		{
		  r=0.4;g=0.4;b=0.4;a=1.0;	
		  //cout << "usando cinza escuro" <<endl;
		}
		else
	      if (nomemtl=="lightgrey"||nomemtl=="LightGrey")
		{
		  r=0.8;g=0.8;b=0.8;a=1.0;	
		  //cout << "usando cinza claro" <<endl;
		}
		else
	      if (nomemtl=="red"||nomemtl=="Red")
		{
		  r=1.0;g=0.0;b=0.0;a=1.0;	
		  //cout << "usando vermelho asbtrato" <<endl;
		}
		else
	      if (nomemtl=="green"||nomemtl=="Green")
		{
		  r=0.0;g=1.0;b=0.0;a=1.0;	
		  //cout << "usando verde asbtrato" <<endl;
		}
		else
	      if (nomemtl=="blue"||nomemtl=="Blue")
		{
		  r=0.0;g=0.0;b=1.0;a=1.0;	
		  //cout << "usando azul asbtrato" <<endl;
		}
		else
	      if (nomemtl=="white"||nomemtl=="White")
		{
		  r=1.0;g=1.0;b=1.0;a=1.0;	
		  //cout << "usando branco asbtrato" <<endl;
		}
		else
	      if (nomemtl=="yellow"||nomemtl=="Yellow")
		{
		  r=1.0;g=1.0;b=0.0;a=1.0;	
		  //cout << "usando amarelo asbtrato" <<endl;
		}
//	      if (nomemtl[0]=='#')
		else
*/
		{
		  a=1.0;
		  VAC_ColorRGBA tmp;
		  tmp.DecodeXMLColor(nomemtl);
		 // a=tmp.a();
		  b=tmp.b()/255.0;
		  g=tmp.g()/255.0;
		  r=tmp.r()/255.0;
//		  cout << "cor:"<<nomemtl<<"="<<r<<","<<g<<","<<b<<","<<a<<endl;
		  //cout << "cor HTML ou SVG"<<endl;
		}
	    }  

	}
      
      {//cleanup
	
	for (int c=0;c<Vertices.TotalItems();c++)	  delete Vertices[c];	  
      }
      //cout << "** "<<TotalItems()<<" poligono(s) importados" <<endl;
    }
 //---------------------------------------------------------------------------------------
/*
void  Externalize (string aFilename)
   {
     ofstream OBJ(aFilename.c_str());
     ostringstream vs;
     ostringstream fs;
     VAC_Face *handler;
    
     int where;
     int counter;
     int c=0;
     int counter2;
     int counter3=1;
     
		    //    //cout << "extraindo uma face" <<endl;
     while(!Empty())
       {
	 handler=Pop();
	 ////cout << "dumpando pontos"<<endl;
	 for (counter2=0;counter2<4;counter2++)
	   vs << "v "<<handler->GetPoint(counter2)->wvstring()<<endl;
	 fs << "f "<<counter3++<<" "<<counter3++<<" "<<counter3++<<" "<<counter3++<<endl;
       }     
   
	OBJ<< vs.str();
	OBJ<< fs.str()<<endl;
	OBJ.close();
   }
*/
/*
  VAC_Face *Pop()
    {
      //cout << "poppando" <<endl;
      return iFaces.Pop();
    }

  bool Empty()
    {
      return iFaces.TotalItems();
    }
*/

  long iDistance;
  
  VAC_PolyBuffer()
    {
    //  iHead=0;
      //iTail=0;
     
    }

  void Push(VAC_Face *aPtr)
	{
		Add(aPtr);
	}
/*
  VAC_PolyBuffer *MakeShadow(int aDirection,int aHeight)
    {
      //  //cout << "making a shadow" <<endl;
      VAC_PolyBuffer *toReturn=new VAC_PolyBuffer();
      BZK_Vector<BZK_Vec3f> tmp;
      BZK_Queue<VAC_Face*> tmp2;
      VAC_Face *face[2];
      int level;
      BZK_Vec3f *point[2];
      switch(aDirection)
	{
	case FLOOR:
	  
	 
	  while (!this->Empty())
	    {
	      //  //cout << "one more face" << endl;
	      face[0]=this->Pop();
	      face[1]=new VAC_Face();
	      face[1]->iFill.SetColor(0,0,0,BZK_FastMath::Int32toFix32(64));
	      for (int c=0;c<face[0]->GetTotalPoints();c++)
		{
		  //  //cout << "point...";
		  point[0]=face[0]->GetPoint(c);
		  point[1]=new BZK_Vec3f();
		  memcpy(point[1],point[0],sizeof(BZK_Vec3f));
		  		  point[1]->SetY(BZK_FastMath::Real32toFix32(0.6)+BZK_FastMath::Int32toFix32(-aHeight));//point[1]->GetY()-BZK_FastMath::Int32toFix32(1));
		  //  point[1]->SetY(BZK_FastMath::Real32toFix32(0.6)+point[1]->GetY());	  
		  face[1]->AddPoint(point[1]); 
		delete point[1];

		}
	      face[1]->SetNormal(*face[0]->GetNormal());
	      level=BZK_FastMath::Fix32toReal32(BZK_VecOps::DotProductFix(*face[1]->GetNormal(),BZK_Vec3f(0,1,0)))*255;
	      if (level <0)// level=-level;
		      // if (level==0)
		toReturn->Add(face[1]);
	      // //cout << endl;
	      tmp2.Push(face[0]);
	    }
	  
	  while (!tmp2.Empty())
	    this->Add(tmp2.Pop());


	  break;

	}
      return toReturn;;

    }
  */
  
};
