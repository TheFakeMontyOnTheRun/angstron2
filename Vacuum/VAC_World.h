
class VAC_World : public Partial_VAC_World {
	BZK_Vector<string> iExternalMaps;


	BZK_Vector<std::string *> iPreAlloc;
	BZK_Vector<VAC_PolyBuffer *> iMeshes;
	BZK_Vector<VAC_Graphic *> iGraphics;
	BZK_Vector<VAC_Sector *> *iSector;
	BZK_Vector<BZK_Chunk *> iChunks;
	BZK_Vector<string> iChunkNames;
	VAC_Sector *iMasterSector;
	VAC_Sector *CurrentSector;

public: //temp
	VAC_Graphic iSkyBox;
	bool iChanged;
	bool stop;
	BZK_FixP iSuggestedFar, iSuggestedNear, iSuggestedHeight;
	int iSugestedBufferSize;

	BZK_Vector<BZK_Vec3f *> iVertexBuffer;
	int SectorLoadingDetails;
	int SectorsToGet;
	int originalsectors;
	int camerasector;
	int peersector;
	int opendoors;
	int closeddoors;
	string iCurrentFile;
//notify
//timeout
	string Echo;
	string Header;
	string NumberOfSectors;
	string Author;
	string Version;
	string End;
	string SectorTag;
	string Name;
	string StyleSheet;
	string EndName;
	string Location;
	string EndLocation;
	string Class;
	string EndClass;
	string Actor;
	string EndActor;
	string Actors;
	string EndActors;
	string Camera;
	string EndCamera;
	string External;
	string EndExternal;

	string DropTableTag;
	string GeneratorTableTag;

	string Rules;
	string EndRules;

	string TriggersList;
	string EndTriggersList;

	ifstream Datafile;
	BZK_Vector<int> TriggerRule;
	bool iKeepExternals;

	virtual int GetSectorByPos(BZK_Vec3f &aPos, int aHint = 1, int aTriggerHint = TRIG_NOP) {
		VAC_Sector *ThisSector = GetSector(aHint);
/*
   switch (aTriggerHint)
	{
	case N:
		if (ThisSector->iRect.GetY0() > aPos.GetY())
		{
		int toReturn=ThisSector->Get_Connection(N);
                    ////////////////////////////// CHANGING SECTOR /////////////////////////////////////////
                    if (toReturn > 0 && !ThisSector->iDoor[N] && !GetSector(toReturn)->iDoor[S] )
			return toReturn;
		else 
			return aHint;
                    /////////////////////////////////////////////////////////////////////////////////
		} else return aHint;
	break;
	case S:
		if (ThisSector->iRect.GetY1() < aPos.GetY())
		{
		int toReturn=ThisSector->Get_Connection(S);
                    ////////////////////////////// CHANGING SECTOR /////////////////////////////////////////
                    if (toReturn > 0 && !ThisSector->iDoor[S] && !GetSector(toReturn)->iDoor[N] )
			return toReturn;
		else 
			return aHint;
                    /////////////////////////////////////////////////////////////////////////////////
		} else return aHint;
	break;
	case W:
		if (ThisSector->iRect.GetX1() < aPos.GetX())
		{
		int toReturn=ThisSector->Get_Connection(W);
                    ////////////////////////////// CHANGING SECTOR /////////////////////////////////////////
                    if (toReturn > 0 && !ThisSector->iDoor[W] && !GetSector(toReturn)->iDoor[E] )
			return toReturn;
		else 
			return aHint;
                    /////////////////////////////////////////////////////////////////////////////////
		} else return aHint;
	break;
	case E:
		if (ThisSector->iRect.GetX0() > aPos.GetX())
		{
		int toReturn=ThisSector->Get_Connection(E);
                    ////////////////////////////// CHANGING SECTOR /////////////////////////////////////////
                    if (toReturn > 0 && !ThisSector->iDoor[E] && !GetSector(toReturn)->iDoor[W] )
			return toReturn;
		else 
			return aHint;
                    /////////////////////////////////////////////////////////////////////////////////
		} else return aHint;
	break;
	case FLOOR:
		if (ThisSector->GetH0() > aPos.GetZ())
		{
		int toReturn=ThisSector->Get_Connection(FLOOR);
                    ////////////////////////////// CHANGING SECTOR /////////////////////////////////////////
                    if (toReturn > 0 && !ThisSector->iDoor[FLOOR] && !GetSector(toReturn)->iDoor[BZK_FastMath::GetOpositeDirection(CEILING)] )
			return toReturn;
		else 
			return aHint;
                    /////////////////////////////////////////////////////////////////////////////////
		} else return aHint;
	break;
	case CEILING:
		if (ThisSector->GetH1() < aPos.GetZ())
		{
		int toReturn=ThisSector->Get_Connection(CEILING);
                    ////////////////////////////// CHANGING SECTOR /////////////////////////////////////////
                    if (toReturn > 0 && !ThisSector->iDoor[CEILING] && !GetSector(toReturn)->iDoor[FLOOR] )
			return toReturn;
		else 
			return aHint;
                    /////////////////////////////////////////////////////////////////////////////////
		} else return aHint;
	break;

	}
*/
		int where = ThisSector->WhereBSC(aPos);
		if (where != 100) {
			int toReturn = ThisSector->Get_Connection(where);
			////////////////////////////// CHANGING SECTOR /////////////////////////////////////////
			if (toReturn > 0 && !ThisSector->iDoor[where] &&
				!GetSector(toReturn)->iDoor[BZK_FastMath::GetOpositeDirection(where)])
				return toReturn;
			else
				return aHint;
			/////////////////////////////////////////////////////////////////////////////////

		} else
			return aHint;
	}

	string GetNextToken() {
		string tmp, tmp2;
		tmp = BZK_ParsingTools::GetNextToken(&Datafile);
		/*
		cout << "stream:"<<tmp <<endl;
		tmp2="";
		if (tmp.substr(1,1)!="/")
		{
		while (BZK_ParsingTools::TrimToken(tmp2)=="" ||iCurrent->GetName()=="_text"|| BZK_ParsingTools::TrimToken(tmp2)=="\n"||BZK_ParsingTools::TrimToken(tmp2)=="\t");
		{
		if (iCurrent->GetSon()!=NULL)
			{
			iXMLContext.Push(iCurrent);
			iCurrent=iCurrent->GetSon();
			}

		if (iCurrent->GetName()!="_text")
		 tmp2= iCurrent->GetName();
		if (BZK_ParsingTools::Contains(tmp,"Sector")) exit(0);
		//else
		   //tmp = iCurrent->GetContent();
		iCurrent=iCurrent->GetNext();
		if (iCurrent==NULL)
			iCurrent=iXMLContext.Pop();
		}

		cout << " xml:"<<tmp2<<endl;
		}
		return  tmp;//BZK_ParsingTools::toLowerCase(tmp);
		*/
		return tmp;
	}


public:

	long AddToVertexBuffer(BZK_Vec3f *aVertex) {/*
    for (int c=0;c<iVertexBuffer.TotalItems();c++)
      if ((*iVertexBuffer[c])==(*aVertex))
	return c;
    iVertexBuffer.Add(aVertex);
   */
		return iVertexBuffer.TotalItems() - 1;
	}

///TODO: for the love of god, strip this string cmp
	BZK_Chunk *RequestChunk(string aName) {
//	cout << "request chunk:"<<aName<<endl;
		for (int c = iChunkNames.TotalItems() - 1; c >= 0; c--)
			if (aName == iChunkNames[c])
				return iChunks[c];
		BZK_Chunk *tmp = BZK_DevSound::GetInstance()->LoadSound((string(STOREPATH) + aName).c_str());
//	cout << "carregando from scratch:"<<tmp<<endl;
		if (tmp == NULL) {
			cout << Mix_GetError() << endl;
//		exit(0);
		}
		iChunks.Add(tmp);
		iChunkNames.Add(aName);
		return tmp;
	}

///TODO: for the love of god, strip this string cmp
	VAC_PolyBuffer *RequestMeshResource(string aName, bool aKeepAspectRatio = false) {
		//retrieve existant mesh
		{
			for (int c = iMeshes.TotalItems() - 1; c >= 0; c--)
				if (iMeshes[c]->iResourceName == aName) {
					// //cout << "aproveitei alocacao de "<<aName<<endl;
					return iMeshes[c];
				}
		}
		//doesnt exist
		if (BZK_ParsingTools::Contains(aName, ".obj")) {
			VAC_PolyBuffer *toReturn = new VAC_PolyBuffer();
			ifstream file((STOREPATH + aName).c_str());
			toReturn->Internalize(file);
			iMeshes.Add(toReturn);
			toReturn->iResourceName = aName;
			return toReturn;
		} else if (BZK_ParsingTools::Contains(aName, ".svg")) {
			VAC_Graphic tmp;
			tmp.Internalize(STOREPATH + aName, aKeepAspectRatio);
			VAC_PolyBuffer *toReturn = tmp.toPolyBuffer();
			iMeshes.Add(toReturn);
			toReturn->iResourceName = aName;
			return toReturn;
		}
		return NULL;
	}

	VAC_Graphic *RequestGraphicResource(string aName) {
		//retrieve existant mesh
		{
			for (int c = iGraphics.TotalItems() - 1; c >= 0; c--)
				if (iGraphics[c]->iResourceName == aName) {
					// //cout << "aproveitei alocacao de "<<aName<<endl;
					return iGraphics[c];
				}
		}
		//doesnt exist

		if (BZK_ParsingTools::Contains(aName, ".svg")) {
			VAC_Graphic *tmp = new VAC_Graphic();
			tmp->Internalize(STOREPATH + aName);
			iGraphics.Add(tmp);
			tmp->iResourceName = aName;
			return tmp;

		}
	}

	VAC_World() {
		iSugestedBufferSize = -1;
		camerasector = 0;
		peersector = -1;
		opendoors = 0;
		closeddoors = 0;
		iCurrentFile = "";
		stop = false;
		iSuggestedFar = 0;
		iSuggestedNear = 0;
		iKeepExternals = false;
		iSector = new BZK_Vector<VAC_Sector *>;
	}

	~VAC_World() {
		CleanWorld();
		delete iSector;
	}

	void CleanWorld() {
		iChanged = true;
		//I must delete the meshes too...
		if (iExternalMaps.TotalItems() > 0)
			iKeepExternals = true;


		if (!iKeepExternals)
			iExternalMaps.EraseAll();
		TriggerRule.EraseAll();
		int c;
		for (c = 0; c < iSector->TotalItems(); c++) delete GetSector(c);
		for (c = 0; c < iMeshes.TotalItems(); c++) delete iMeshes[c];
		for (c = 0; c < iGraphics.TotalItems(); c++) delete iGraphics[c];
		for (c = 0; c < iChunks.TotalItems(); c++) Mix_FreeChunk(iChunks[c]);
		for (c = 0; c < iPreAlloc.TotalItems(); c++) delete iPreAlloc[c];
		iGraphics.ReleaseAll();
		iChunkNames.ReleaseAll();
		iChunks.ReleaseAll();
		iMeshes.ReleaseAll();
		iPreAlloc.ReleaseAll();
		//delete iSector;
	}

	void PreAllocateResources() {
		for (int c = 0; c < iPreAlloc.TotalItems(); c++) {
			cout << "pre-allocating " << *iPreAlloc[c] << endl;
			if (BZK_ParsingTools::Contains(*iPreAlloc[c], ".wav"))
				RequestChunk(*iPreAlloc[c]);
			else {
				RequestGraphicResource(*iPreAlloc[c]);
				RequestMeshResource(*iPreAlloc[c]);
			}
		}
	}

	int TotalSectors() {
		return iSector->TotalItems();
	}

	VAC_Sector *GetSector(int ID) {
		return (*iSector)[ID];
	}

	std::string
	ShowSector(BZK_VolumetricSector &aSector) {
		return aSector.ToString();
	}

	bool
	DataReady() {
		return stop;
	}

	string
	ExternalMaps(int aMap) {
		return iExternalMaps[aMap];
	}

	void
	GetColorTag(ifstream &Datafile, VAC_Sector &aSector, int aIndex) {

		string echo = "";
		echo = GetNextToken(); //<RGB>


		BZK_FixP temp1, temp2, temp3, temp4;
		temp1 = BZK_ParsingTools::GetNumber(GetNextToken());
		//aSector.iColorRGBA[aIndex].SetR(temp);
		temp2 = BZK_ParsingTools::GetNumber(GetNextToken());
		//  aSector.iColorRGBA[aIndex].SetG(temp);
		temp3 = BZK_ParsingTools::GetNumber(GetNextToken());
		//  aSector.iColorRGBA[aIndex].SetB(temp);
		//  temp4=BZK_ParsingTools::GetNumber(GetNextToken());
		if (echo == "<RGBA>")
			temp4 = BZK_ParsingTools::GetNumber(GetNextToken());
		else
			temp4 = BZK_FastMath::Int32toFix32(255);
		aSector.GetColor(aIndex)->SetColor(temp1, temp2, temp3, temp4);
		//  aSector.iColor[aIndex].AddNoise();
		//  aSector.iColor[aIndex].SetColor(0,255,0,255);




		//  aSector.iColorRGBA[aIndex].SetA(255);
		GetNextToken(); // </RGB>

	}

	void
	GetColorsTag(ifstream &Datafile, VAC_Sector &aSector) {
		int c;
		for (c = 0; c < 6; c++) {

			GetColorTag(Datafile, aSector, c);
		}
		GetNextToken(); //</Colors>
	}

	void
	GetVolumeTag(ifstream &Datafile, VAC_Sector &aSector) {

		BZK_FixP Dx, Dy, Dz;
		BZK_Rect SectorRect;
		GetNextToken(); //<Vec3f>
		BZK_Vec3f FirstPoint;
		FirstPoint.SetX(BZK_ParsingTools::GetNumber(GetNextToken()));
		FirstPoint.SetY(BZK_ParsingTools::GetNumber(GetNextToken()));
		FirstPoint.SetZ(BZK_ParsingTools::GetNumber(GetNextToken()));
		GetNextToken(); //</Vec3f>

		Dx = BZK_ParsingTools::GetNumber(GetNextToken());
		Dy = BZK_ParsingTools::GetNumber(GetNextToken());
		Dz = BZK_ParsingTools::GetNumber(GetNextToken());

		SectorRect.SetX0(FirstPoint.GetX());
		SectorRect.SetY0(FirstPoint.GetY());
		SectorRect.SetW(Dx);
		SectorRect.SetH(Dy);
		aSector.SetRect(SectorRect);
		aSector.SetH0(FirstPoint.GetZ());
		aSector.SetH1(FirstPoint.GetZ() + Dz);
		/*
		for (int c=0;c<6;c++)
		  aSector.SetVertexBuffer(c,AddToVertexBuffer(aSector.Point(c)));

		//cout << "-----------"<<endl;
		for (int c=0;c<6;c++)
		  //cout << "VA:"<<c<<":"<<aSector.GetVertexBuffer(c)<<endl;
		 */
	}

	void
	GetConnectionsTag(ifstream &Datafile, VAC_Sector &aSector) {
		int c;
		for (c = 0; c < 6; c++) {

			aSector.SetConnection(c, BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken())));
			if (aSector.Get_Connection(c) == 0)
				aSector.SetVisibleFaces(aSector.GetVisibleFaces() + 1);
		}
		GetNextToken(); //</Connections>
	}


	VAC_Sector *
	GetSectorDetails(ifstream &Datafile, int aSectorsToGet) {
		VAC_Sector *ThisSector = new VAC_Sector();

		string Echo = "";
		string *ptr;
		string Volume = "<Volume>";
		string Connections = "<Connections>";


		string RGB = "<RGB>";
		string RGBA = "<RGBA>";
		string Texture = "<texture>";
		string EndTexture = "</texture>";
		string Color = "<Color>";
		string End = "</Sector>";;
		string Link = "<Link>";
		string EndLink = "</Link>";
		string light = "<light>";
		string endlight = "</light>";

		string mesh = "<mesh>";
		string endmesh = "</mesh>";

		string Triggers = "<Trigger>";
		string EndTriggers = "</Trigger>";
		string ForceRender = "<Force_Render_Face>";
		string EndForceRender = "</Force_Render_Face>";

		bool stop = false;
		ThisSector->SetOriginalTrigger(TRIG_NOP);
		while (!stop) {
			Echo = GetNextToken();

			if (Echo == End) stop = true;
			if (Echo == Volume) GetVolumeTag(Datafile, *ThisSector);
			if (Echo == Connections) GetConnectionsTag(Datafile, *ThisSector);
			if (Echo == Color) GetColorsTag(Datafile, *ThisSector);
			if (Echo == "<facecolor>") {
				int face = 0;
				Echo = GetNextToken();
				face = BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo));
				GetColorTag(Datafile, *ThisSector, face);
				Echo = GetNextToken();
			}

			if (Echo == "<Door") {
				VAC_Sector::VAC_Sector_Door *tmp_door = new VAC_Sector::VAC_Sector_Door();
				tmp_door->openonpresence = true;
				opendoors++;
				while (Echo != "/>") {
					if (Echo == "face") {
						GetNextToken();
						Echo = GetNextToken();
						int face = BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo));
						if (face == 1 || face == 3) {
							face += 2;
							face = face % 4;
						}
						ThisSector->iDoor[face] = tmp_door;
						cout << "criei uma porta em " << face << endl;
					}

					if (Echo == "timeout") {
						GetNextToken();
						Echo = GetNextToken();
						int timeout = BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo));
						tmp_door->timeout = timeout;
					}

					if (Echo == "opener") {
						GetNextToken();
						Echo = GetNextToken();
						int action = BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo));
						tmp_door->action = action;
					}

					if (Echo == "opensound") {
						GetNextToken();
						Echo = GetNextToken();
						Echo = Echo.substr(1);
						Echo = Echo.substr(0, Echo.length() - 1);
						tmp_door->OpenSound = RequestChunk(Echo);
					}


					if (Echo == "closesound") {
						GetNextToken();
						Echo = GetNextToken();
						Echo = Echo.substr(1);
						Echo = Echo.substr(0, Echo.length() - 1);
						tmp_door->CloseSound = RequestChunk(Echo);
					}

					Echo = GetNextToken();
				}
			}

			if (Echo == "<parent>") {
				Echo = GetNextToken();
				ThisSector->iParent = BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo));
				Echo = GetNextToken();
			}
			if (Echo == Link) {
				Echo = GetNextToken();

				{
					ThisSector->Link = Echo;
					//cout << "peguei o seguinte:"<<Echo<<endl;
				}
				GetNextToken();
			}


			if (Echo == mesh) {
				Echo = GetNextToken();

				{
					ThisSector->SetMeshName(Echo);
					//cout << "peguei o seguinte:"<<Echo<<endl;
					ThisSector->SetMesh(RequestMeshResource(Echo));
					if (ThisSector->GetMesh() == NULL) {
						//cout << "recurso não carregado!"<<Echo<<endl;
						{
							cout << "world1" << endl;
							exit(0);
						}
					}
				}
				GetNextToken();
			}


			if (Echo == Texture) {
				VAC_Graphic *SVG;
				int face;

				Echo = GetNextToken();
				while (Echo != EndTexture) {
					face = BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo));
					if ((face == 5 || face == 4)) {
						Echo = GetNextToken();
						Echo = GetNextToken();
						continue;
						//cout << "face de textura:"<<face<<endl;
					} else if ((face == 1 || face == 3)) {
						face += 2;
						face = face % 4;
					}

					Echo = GetNextToken();
					//cout << "svg:"<<STOREPATH+Echo<<endl;
					//SVG=new VAC_Graphic();
					//SVG->Internalize(STOREPATH+Echo);

					ThisSector->iTextures[face] = RequestMeshResource(Echo, true); //SVG->toPolyBuffer();
					if ((face == 5 || face == 4)) {
						ThisSector->iTextures[face]->Flip();
						cout << "flip" << endl;
					}
					//flip
					Echo = GetNextToken();
					//cout << "echo:" <<Echo<<endl;
					// {cout<<"world"<<endl;exit(0);}


				}
			}

			if (Echo == "<RenderMode>") {
				//cout << "** peguei o modo de renderiza��o ** "<<endl;
				Echo = GetNextToken();
				ThisSector->iRenderMode = BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo));
				GetNextToken(); //</RenderMode>
			}

			if (Echo == light) {
				//cout << "** luz ** "<<endl;
				Echo = GetNextToken();
				ThisSector->iCandelas = BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo));
				GetNextToken(); //</RenderMode>
			}

			if (Echo == Triggers) {
				Echo = GetNextToken();
				//while (Echo!=EndTriggers)
				{
					//ThisSector->iLocalTriggers.Add
					ThisSector->//SetTrigger
							iOriginalTrigger = (atoi(Echo.c_str()));
					Echo = GetNextToken(); //</trigger>
				}
			}

			if (Echo == ForceRender) {

				Echo = GetNextToken();
				//while (Echo!=EndTriggers)
				{
					//ThisSector->iLocalTriggers.Add
					ThisSector->iForceRender[atoi(Echo.c_str())] = true;;
					//	    //cout << "trigger! "<<ThisSector->GetTrigger() <<endl;
					Echo = GetNextToken(); //end force render;
				}
			}

			if (Echo == RGBA || Echo == RGB) {

				//       Echo=*GetNextToken();
				//while (Echo!=EndTriggers)
				{
					//ThisSector->iLocalTriggers.Add
					ThisSector->iHasInternalColor = true;


					BZK_FixP temp1, temp2, temp3, temp4;
					temp1 = BZK_ParsingTools::GetNumber(GetNextToken());
					//aSector.iColorRGBA[aIndex].SetR(temp);
					temp2 = BZK_ParsingTools::GetNumber(GetNextToken());
					//  aSector.iColorRGBA[aIndex].SetG(temp);
					temp3 = BZK_ParsingTools::GetNumber(GetNextToken());
					//  aSector.iColorRGBA[aIndex].SetB(temp);
					//  temp4=BZK_ParsingTools::GetNumber(GetNextToken());
					if (Echo == "<RGBA>")
						temp4 = BZK_ParsingTools::GetNumber(GetNextToken());
					else
						temp4 = BZK_FastMath::Int32toFix32(255);
					ThisSector->iInternalColor.SetColor(temp1, temp2, temp3, temp4);
					//  aSector.iColor[aIndex].SetColor(0,255,0,255);




					//	    //cout << "trigger! "<<ThisSector->GetTrigger() <<endl;
					Echo = GetNextToken(); //end force render;
				}
			}


		}


		return ThisSector;
	}


	int
	LoadMap(string aFile, VAC_ActorManager &aPeople, bool iAllForOnce = true) {
		CleanWorld();
		//cout << "iniciando leitura de arquivo"<<endl;
		iCurrentFile = aFile;
		opendoors = 0;
		SectorsToGet = 0;
		originalsectors = 0;

		Header = "<BZK2>";
		NumberOfSectors = "<NumberofSectors>";
		Author = "<Author>";
		Version = "<Version>";
		End = "</BZK2>";
		SectorTag = "<Sector>";
		Name = "<Name>";
		//  string StyleSheet=
		EndName = "</Name>";
		Location = "<Location>";
		EndLocation = "</Location>";
		Class = "<Class>";
		EndClass = "</Class>";
		Actor = "<Actor>";
		EndActor = "</Actor>";
		Actors = "<Actors>";
		EndActors = "</Actors>";
		Camera = "<Camera>";
		EndCamera = "</Camera>";
		External = "<External>";
		EndExternal = "</External>";
		TriggersList = "<TriggerRules>";
		EndTriggersList = "</TriggerRules>";
		Rules = "<Rules>";
		EndRules = "</Rules>";
		DropTableTag = "<DropTable>";
		GeneratorTableTag = "<Generators>";

		TriggerRule.EraseAll();
		Datafile.clear();
		Datafile.open(aFile.c_str());

		cout << "abrindo " << aFile << endl;

		if (!Datafile.is_open()) {
			cout << "file not found!Make sure the file is in the storage area - Exiting." << endl;
			{
				cout << "world3" << endl;
				exit(0);
			}
		}
		iSector->EraseAll();
		aPeople.ResetRules();
		while ((!Datafile.eof()) && (Echo != Header))
			Echo = GetNextToken();

		if ((Echo) != Header) exit(ERR_INVALID_FILENAME);
		// std:://cout << "\nachou o header:"<<Echo<<endl;
		stop = false;
		if (iAllForOnce)
			while (!stop)
				GetMoreData(aPeople);
	}


	void GetMoreData(VAC_ActorManager &aPeople) {
		string BackDrop = "<backdrop>";
		string EndBackDrop = "</backdrop>";
		string Frustum = "<frustum>";
		string EndFrustum = "</frustum>";

		if (!stop) {
			stop = stop && Datafile.good();
			///       //cout << "getting more data! "<<SectorsToGet<< endl;
			//    std:://cout<<"\nlooping!";
			Echo = GetNextToken();
			//    std:://cout<<"\necho:"<<Echo<<endl;

			if ((Echo) == "<preallocate>") {
				while ((Echo) != "</preallocate>") {
					iPreAlloc.Add(new std::string(Echo));
					Echo = GetNextToken(); // </number>
				}
				//	std:://cout << "\nnumero de setores:"<<SectorsToGet;
			}


			if (Echo == Frustum) {
				iSuggestedFar = BZK_ParsingTools::GetNumber(GetNextToken());
				iSuggestedNear = BZK_ParsingTools::GetNumber(GetNextToken());
				GetNextToken(); // </number>
				//	std:://cout << "\nnumero de setores:"<<SectorsToGet;
			}

			if (Echo == BackDrop) {
				Echo = GetNextToken();
				iSkyBox.Internalize((string(STOREPATH) + Echo).c_str());
				Echo = GetNextToken(); //</backdrop>

			}

			if (Echo == End) {
				stop = true;

				//	std:://cout << "\nfim de arquivo\n";
				//	if (!SectorsToGet) throw ERR_UNEXPECTED_END_OF_FILE;
				if (iSector->TotalItems() > 1) {
					iMasterSector = (*iSector)[0];
					//   std:://cout << iMasterSector->ToString()<<endl;
				}
				Datafile.close();

				//std:://cout << iSector->TotalItems()<< " setores lidos"<<endl;

			}
			/*
				if (Echo==Author)
				  {
							AuthorName=*GetNextToken();
							GetNextToken();  //</author>
							//	std:://cout << "\nautor:"<<AuthorName;
				  }

				if (Echo==Version)
				  {
							VersionNumber=BZK_ParsingTools::GetNumber(GetNextToken());
							GetNextToken();   // </version>
							//	std:://cout << "\nvers�o:"<<VersionNumber;
				  }
			 */


			if (Echo == "<SugestedBufferSize>") {
				iSugestedBufferSize = BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken()));
				GetNextToken();
			}


			if (Echo == NumberOfSectors || Echo == "<Number.of.Sectors>") {
				SectorsToGet = BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken()));
				originalsectors = SectorsToGet;
				GetNextToken(); // </number>
				//	std:://cout << "\nnumero de setores:"<<SectorsToGet;
			}

			if (Echo == Rules) {
				while (Echo != EndRules) {

					if (Echo == "<TriggerTransitions>") {
						cout << "triggered" << endl;
						Echo = GetNextToken();
						while (Echo != "</TriggerTransitions>") {
							aPeople.triggered_class.Add(BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo)));
							aPeople.triggered_state.Add(
									BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken())));
							aPeople.triggered_trigger.Add(
									BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken())));
							aPeople.triggered_newstate.Add(
									BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken())));
							Echo = GetNextToken();
						}
					}

					if (Echo == "<ClassTransitions>") {
						Echo = GetNextToken();
						while (Echo != "</ClassTransitions>") {
							cout << "transition" << endl;
							cout << Echo << endl;
							if (Echo == "") exit(0);
							int valor = BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo));
							aPeople.transition_class.Add(valor);
							valor = BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken()));
							aPeople.transition_state.Add(valor);
							valor = BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken()));
							aPeople.transition_newclass.Add(valor);
							Echo = GetNextToken();
						}
						cout << Echo << endl;
					}

					if (Echo == "<Spawner>") {
						cout << "spawner" << endl;
						Echo = GetNextToken();
						while (Echo != "</Spawner>") {
							aPeople.spawner_class.Add(BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo)));
							aPeople.spawner_state.Add(
									BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken())));
							aPeople.spawner_spawnedclass.Add(
									BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken())));
							aPeople.spawner_spawnedstate.Add(
									BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken())));
							Echo = GetNextToken();
						}
					}

					if (Echo == "<ActionSounds>") {
						cout << "sound" << endl;
						Echo = GetNextToken();
						while (Echo != "</ActionSounds>") {
							aPeople.sound_class.Add(BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo)));
							aPeople.sound_action.Add(
									BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken())));
							Echo = GetNextToken();
							aPeople.sound_chunk.Add(RequestChunk(Echo));
							aPeople.sound_file.Add(Echo);
							Echo = GetNextToken();
						}
					}

					if (Echo == "<Lights>") {
						Echo = GetNextToken();
						while (Echo != "</Lights>") {
							aPeople.light_class.Add(BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo)));
							aPeople.light_candelas.Add(
									BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken())));
							Echo = GetNextToken();
						}
					}


					if (Echo == "<Actions>") {
						cout << "droppers" << endl;
						Echo = GetNextToken();
						while (Echo != "</Actions>") {
							aPeople.droppers_class.Add(BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo)));
							aPeople.droppers_state.Add(
									BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken())));
							aPeople.droppers_newdroptrigger.Add(
									BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken())));
							Echo = GetNextToken();
						}
					}

					if (Echo == "<Collisions>") {
						cout << "collision" << endl;
						Echo = GetNextToken();
						while (Echo != "</Collisions>") {
							aPeople.collision_class.Add(BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo)));
							aPeople.collided_class.Add(
									BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken())));
							aPeople.collision_newclass.Add(
									BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken())));
							Echo = GetNextToken();
						}
					}

					if (Echo == "<Sensor>") {
						cout << "sensor" << endl;
						Echo = GetNextToken();
						while (Echo != "</Sensor>") {
							aPeople.sensor_myclass.Add(BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo)));
							aPeople.sensor_direction.Add(
									BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken())));
							aPeople.sensor_senseclass.Add(
									BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken())));
							aPeople.sensor_newclass.Add(
									BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken())));
							Echo = GetNextToken();
						}
					}

					if (Echo == "<Appearance>") {
						cout << "appearance" << endl;
						Echo = GetNextToken();
						while (Echo != "</Appearance>") {
							aPeople.appearance_class.Add(BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo)));
							Echo = GetNextToken();
							aPeople.appearance_file.Add(Echo);
							aPeople.appearance_accel_mesh.Add(BZK_ParsingTools::Contains(Echo, ".obj"));
							Echo = GetNextToken();
						}
					}

					Echo = GetNextToken(); // </Rules>
					cout << Echo << endl;
				}
				//exit(0);
//                aPeople.Optimize();
			}
			if (Echo == SectorTag) {

				//	std:://cout << "\nsetor!";
				AddSector(*GetSectorDetails(Datafile, originalsectors));

				//	if (SectorLoadingDetails==ERR_NONE)
				SectorsToGet--;
				//	else throw SectorLoadingDetails;

			}

			if (Echo == Actors) {
				//cout << "lendo lista de atores"<<endl;
				Echo = GetNextToken();
				while (Echo != EndActors) {
					if (Echo == Actor) aPeople.AddActor(*GetActorDetails(Datafile, aPeople));
					//cout << "sujeito adicionado" <<endl;
					Echo = GetNextToken();
				}
			}

			if (Echo == TriggersList) {
				//cout << "lendo lista de atores"<<endl;
				Echo = GetNextToken();
				int tmp;
				while (Echo != EndTriggersList) {
					tmp = (int) BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo));
					TriggerRule.Add(tmp);
					Echo = GetNextToken();
				}
			}


			if (Echo == External && !iKeepExternals) {
				Echo = GetNextToken();
				while (Echo != EndExternal) {
					iExternalMaps.Add(Echo);
					//cout << iExternalMaps[iExternalMaps.TotalItems()-1] << endl;
					Echo = GetNextToken();
				}
			}


			if (Echo == Camera) {
				Echo = GetNextToken(); //fica sendo o /Camera por enquanto

				//	if (BZK_ParsingTools::IsNumber(Echo))
				{
					aPeople.SetActivePlayer(BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo)));

					//ajusta camera como o ator Echo
					//cout << "ator ativo determinado:" <<aPeople.GetActivePlayerPtr()->GetID() << endl;
				}
				//else
				{
					//busca ator pelo nome e ajusta ele como camera
				}
			}


		}
	}

	int GetSectorExactByPos(BZK_Vec3f aPos, int aHint = 1) {
		for (int c = 1; c < TotalSectors(); c++) {
			cout << "sector " << c << endl;
			if (GetSector(c)->WhereBSC(aPos) == 100)
				return c;
		}

		return 0;
	}


	VAC_Actor *GetActorDetails(ifstream &aDatafile, VAC_ActorManager &aPeople) {
		//cout << "lendo detalhes do ator" <<endl;
		string Echo;
		VAC_Actor *DummyGuy;
		VAC_Actor *DummyGuy2;
		BZK_Vec3f Pos;
		bool PosSetted = false;
		string Location = "<Location>";
		string EndLocation = "</Location>";

		string flashlight = "<flashlight>";
		string EndFlashLight = "</flashlight>";

		string Kind = "<Kind>";
		string EndKind = "</Kind>";

		string Class = "<Class>";
		string EndClass = "</Class>";

		string billboard = "<billboard>";
		string endbillboard = "</billboard>";

		string light = "<light>";
		string endlight = "</light>";

		string Actor = "<Actor>";
		string Volume = "<Sector>";
		string EndActor = "</Actor>";
		string Name = "<Name>";
		string EndName = "</Name>";
		string Angle = "<angle>";
		string EndAngle = "</angle>";
		string mesh = "<mesh>";
		string TriggerList = "<Triggers>";
		string EndTriggerList = "</Triggers>";
		string Endmesh = "</mesh>";
		string Behavior = "<behavior>";

		string EndStep = "</TurnStep>";
		string Step = "<TurnStep>";


		string EndBehavior = "</behavior>";
		int templocation = 1;
		int tempclass = 0; //unique actor (camera for testing levels);
		int candelas = 0;
		int flashcandelas = 0;
		float tempstep = 22.5;
		string tempmodifiers = "camera_dummy";
		string tempname = "annonymous";
		string tempangle = "0";
		string tempmesh = "";
		string tmpbillboard = "";
		string tempscript = "std.logo";
		VAC_Sector *tmp = NULL;


		Echo = GetNextToken();
		while (Echo != EndActor) {
			if (Echo == Location) {
				//cout << "localiza��o" <<endl;
				templocation = BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken()));
				GetNextToken(); //</Location>
				//cout << "/localiza��o" <<endl;

			}

			if (Echo == light) {
				//cout << "** luz ** "<<endl;
				Echo = GetNextToken();
				candelas = BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo));
				GetNextToken(); //</RenderMode>
			}

			if (Echo == flashlight) {

				cout << "** luz direcionada ** " << endl;
				Echo = GetNextToken();
				flashcandelas = BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo));
				GetNextToken(); //</RenderMode>
			}

			if (Echo == "<Position>") {

				cout << "** luz direcionada ** " << endl;
				Pos.SetX(BZK_ParsingTools::GetNumber(GetNextToken()));
				Pos.SetY(BZK_ParsingTools::GetNumber(GetNextToken()));
				Pos.SetZ(BZK_ParsingTools::GetNumber(GetNextToken()));
				PosSetted = true;
				flashcandelas = BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Echo));
				GetNextToken(); //</Position>
			}


			if (Echo == Name) {
				//cout << "name" <<endl;
				tempname = GetNextToken();

				GetNextToken(); //</Name>
				//cout << "/name" <<endl;
			}
			if (Echo == mesh) {
				//cout << "mesh" <<endl;
				tempmesh = GetNextToken();

				GetNextToken(); //</Name>
				//cout << "/mesh" <<endl;
			}

			if (Echo == billboard) {
				//cout << "billboard" <<endl;
				tmpbillboard = GetNextToken();

				GetNextToken(); //</Name>
				//cout << "/billboard" <<endl;
			}

			if (Echo == Step) {
				//cout << "behave" <<endl;
				tempstep = BZK_FastMath::Fix32toReal32(BZK_ParsingTools::GetNumber(GetNextToken()));

				GetNextToken(); //</behavior>
				//cout << "/behave" <<endl;
			}

			if (Echo == Behavior) {
				//cout << "behave" <<endl;
				tempscript = GetNextToken();

				GetNextToken(); //</behavior>
				//cout << "/behave" <<endl;
			}


			if (Echo == Kind) {
				//cout << "kind" <<endl;
				tempclass = BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(GetNextToken()));
				GetNextToken(); //</Class>
				//cout << "/kind" <<endl;
			}

			if (Echo == Class) {
				//cout << "class" <<endl;
				tempmodifiers = GetNextToken();
				GetNextToken(); //</Class>
				//cout << "/class" <<endl;
			}
			if (Echo == Angle) {
				//cout << "angle" <<endl;
				tempangle = GetNextToken();
				GetNextToken(); //</Class>
				//cout << "/angle" <<endl;
			}
			if (Echo == Volume) {
				//cout << "volume" <<endl;
				tmp = GetSectorDetails(aDatafile, 1);
				//	  GetNextToken(); //</volume>
				//cout << "/volume" <<endl;
			}

			Echo = GetNextToken();
		}


		//cout << "efetivando os detalhes" <<endl;
		VAC_Sector *location = GetSector(templocation);
		//cout << "templocation:"<<templocation<<" ponteiro:"<< location<<endl;
		if (iSector->TotalItems() > 1)
			DummyGuy2 = new VAC_Actor(GetSector(templocation)->center(), templocation);
		else
			DummyGuy2 = new VAC_Actor();

		/*
		DummyGuy2=aPeople.RequestInstance();
		memcpy(DummyGuy2,DummyGuy,sizeof(VAC_Actor));

		if (iSector->TotalItems()>1)
		  DummyGuy2->Init(GetSector(templocation)->center(),templocation);
		else
		  DummyGuy2->Init();
		 */
		ifstream Mesh;
		if (tempmesh != "") {
			//cout << "obtendo mesh em"<<(STOREPATH+tempmesh) <<endl;
			Mesh.open((STOREPATH + tempmesh).c_str());
			//cout << "arquivo aberto" <<endl;

			// DummyGuy->iBehavior=tempscript;
			//cout << "mesh internalizada" <<endl;
			DummyGuy2->SetName(tempname);
		}

		if (tmpbillboard != "")
			DummyGuy2->SetBillBoard(VAC_Graphic::InternalizeFromScratch(STOREPATH + tmpbillboard));
		DummyGuy2->SetTurnStep(tempstep);
		if (PosSetted) {
			DummyGuy2->SetPosition(Pos);
			DummyGuy2->SetCurrentSector(GetSectorExactByPos(Pos));
		}
		DummyGuy2->SetCandelas(candelas);
		DummyGuy2->SetFlashlightCandelas(flashcandelas);
		DummyGuy2->SetAngle(BZK_ParsingTools::GetNumber(tempangle));
		cout << "classe:" << tempclass << endl;
		DummyGuy2->SetClass(tempclass);
		//cout << "copiando bounding box" <<endl;



		if (tmp != NULL)
			DummyGuy2->SetBoundingBox(tmp);
		delete tmp;

		//name
		//class
		//cout << "retornando" <<endl;
		if (tempmesh != "")
			DummyGuy2->SetAppearance(VAC_PolyBuffer::InternalizeFromScratch(Mesh));
		Mesh.close();
		//  delete DummyGuy;
		return DummyGuy2;
	}

	void NotifyEndOfCycle() {
		int counter;
		int other;
		int g;
		bool prox = false;
		int backh;

		iChanged = false;
		VAC_Sector *ThisSector;
		VAC_Sector *OtherSector;
		VAC_Sector::VAC_Sector_Door *door;
		ThisSector = GetSector(camerasector);
		for (int h = 5; h >= 0; h--) {
			backh = BZK_FastMath::GetOpositeDirection(h);
			OtherSector = GetSector(ThisSector->Get_Connection(h));
			door = ThisSector->iDoor[h];
			if (door != NULL && ThisSector->iVisibleFaces && door->openonpresence && door->currenttimeout == 0) {
				iChanged = true;
				cout << "abri" << endl;
				BZK_DevSound::GetInstance()->PlaySound(door->OpenSound, EFFECTS_SOUND);
				ThisSector->SetConnection(h, door->link);
				door->currenttimeout = door->timeout;
				ThisSector->iVisibleFaces--;
				opendoors++;
				closeddoors--;
			}
			door = OtherSector->iDoor[backh];
			if (door != NULL && OtherSector->iVisibleFaces && door->currenttimeout == 0) {
				iChanged = true;
				opendoors++;
				closeddoors--;
				cout << "do outro lado" << endl;
				ThisSector->iVisibleFaces--;
				BZK_DevSound::GetInstance()->PlaySound(door->OpenSound, EFFECTS_SOUND);
				OtherSector->SetConnection(backh, camerasector);
				door->currenttimeout = door->timeout;
			}
		}

		if (peersector != -1) {
			ThisSector = GetSector(peersector);
			for (int h = 5; h >= 0; h--) {
				backh = BZK_FastMath::GetOpositeDirection(h);
				OtherSector = GetSector(ThisSector->Get_Connection(h));
				door = ThisSector->iDoor[h];
				if (door != NULL && ThisSector->iVisibleFaces && door->openonpresence && door->currenttimeout == 0) {
					iChanged = true;
					cout << "abri" << endl;
					BZK_DevSound::GetInstance()->PlaySound(door->OpenSound, EFFECTS_SOUND);
					ThisSector->SetConnection(h, door->link);
					door->currenttimeout = door->timeout;
					ThisSector->iVisibleFaces--;
					opendoors++;
					closeddoors--;
				}
				door = OtherSector->iDoor[backh];
				if (door != NULL && OtherSector->iVisibleFaces && door->currenttimeout == 0) {
					iChanged = true;
					opendoors++;
					closeddoors--;
					cout << "do outro lado" << endl;
					ThisSector->iVisibleFaces--;
					BZK_DevSound::GetInstance()->PlaySound(door->OpenSound, EFFECTS_SOUND);
					OtherSector->SetConnection(backh, camerasector);
					door->currenttimeout = door->timeout;
				}
			}
		}
		for (counter = iSector->TotalItems() - 1; counter >= 0; counter--) {
			ThisSector = (*iSector)[counter];
			if (opendoors)
				for (int h = 5; h >= 0; h--) {
					door = ThisSector->iDoor[h];
					if (door != NULL)
						if (door->currenttimeout == 0) {
							if (ThisSector->Get_Connection(h) != 0) {
								iChanged = true;
								door->link = ThisSector->Get_Connection(h);
								ThisSector->SetConnection(h, 0);
								//				if (GetSector(camerasector)->iParent==ThisSector->iParent)
								BZK_DevSound::GetInstance()->PlaySound(door->CloseSound, EFFECTS_SOUND);
								ThisSector->iVisibleFaces++;
								opendoors--;
								closeddoors++;
								cout << "fechou" << endl;
							}
						} else door->currenttimeout--;

				}
			if (ThisSector->HasBeenDrawn())
				(*iSector)[counter]->NotifyEndOfCicle();
		}
	}

	bool
	SaveMap(char *aFile, VAC_ActorManager &aPeople) {
		if (!strcmp(aFile, "")) strcpy(aFile, "out.bzk2");

		string Header = "<BZK2>";
		string NumberOfSectors = "<Number.of.Sectors>";
		string Author = "<Author>";
		string Version = "<Version>";
		string SectorTag = "<Sector>";
		string EndSector = "</Sector>";
		string EndBZK2 = "</BZK2>";


		//cout << "cabecalho do XML"<<endl;

		int SectorsToWrite;
		string Echo = "";

		ofstream Datafile(aFile);
		Datafile << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << endl;
		//dummy
		Datafile << "<?xml-stylesheet type=\"text/xsl\" href=\"bzkmap.xsl\" ?>" << endl;
		Datafile << Header << endl;
		//dummy
		Datafile << "<Version>" << endl;
		Datafile << "0.5" << endl;
		Datafile << "</Version>" << endl;

		Datafile << NumberOfSectors << endl;
		Datafile << iSector->TotalItems() - 1 << "\n";
		Datafile << "</Number.of.Sectors>" << endl;
		//cout <<"number of sectors..."<<endl;
		/*
		if (AuthorName != "")
		  {
		Datafile << Author<<" ";
		Datafile << AuthorName<<"\n";
		  }

		if (VersionNumber !=0)
		  {
		Datafile << Version<<" ";
		Datafile << VersionNumber<<"\n";
		  }
		 */
		if (iSugestedBufferSize != -1) {
			Datafile << "<SugestedBufferSize>" << endl;
			Datafile << iSugestedBufferSize << endl;
			Datafile << "</SugestedBufferSize>" << endl;
		}

		//setores
		for (SectorsToWrite = 0; SectorsToWrite < iSector->TotalItems(); SectorsToWrite++) {
			Datafile << "<!-- sector " << SectorsToWrite << " -->" << endl;
			Datafile << SectorTag << "\n";
			WriteSectorDetails(Datafile, SectorsToWrite);
			Datafile << EndSector << "\n";
		}

		//cout << "actors:"<<aPeople.GetActorsCount()<<endl;
		if (aPeople.GetActorsCount() > 0) {
			Datafile << "<Actors>" << endl;
			int c;
			for (c = 0; c < aPeople.GetActorsCount(); c++) {
				Datafile << "<Actor>" << endl;
				Datafile << "<TurnStep>" << endl;
				Datafile << 22.5 << endl;
				Datafile << "</TurnStep>" << endl;
				//cout << "posicionado em:"<<aPeople.GetActorPtr(c)->GetCurrentSector()<<endl;

				Datafile << "<Location>" << endl;

				Datafile << aPeople.GetActorPtr(c)->GetCurrentSector() << endl;

				Datafile << "</Location>" << endl;
/*
		Datafile << "<Position>"<< endl;
		Datafile << BZK_FastMath::Fix32toInt32( ((VAC_Actor*)aPeople.GetActorPtr(c))->GetPosition()->GetX()) << endl;
		Datafile << BZK_FastMath::Fix32toInt32( ((VAC_Actor*)aPeople.GetActorPtr(c))->GetPosition()->GetY()) << endl;
		Datafile << BZK_FastMath::Fix32toInt32( ((VAC_Actor*)aPeople.GetActorPtr(c))->GetPosition()->GetZ()) << endl;
		Datafile << "</Position>"<< endl;
*/

				Datafile << "<light>" << endl;
				Datafile << ((VAC_Actor *) aPeople.GetActorPtr(c))->GetCandelas() << endl;
				Datafile << "</light>" << endl;

				/*
				if (((VAC_Actor*)aPeople.GetActorPtr(c))->GetModifiers()!="")
				  {
					Datafile << "<Class>"<<endl;
					Datafile << ((VAC_Actor*)aPeople.GetActorPtr(c))->GetModifiers()<<endl;
					Datafile << "</Class>"<<endl;
				  }
				 */
				Datafile << "<Kind>" << endl;
				Datafile << ((VAC_Actor *) aPeople.GetActorPtr(c))->GetClass() << endl;
				Datafile << "</Kind>" << endl;


				Datafile << "</Actor>" << endl;
			}


			Datafile << "</Actors>" << endl;
			aPeople.ExternalizeRules(Datafile);
			Datafile << "<Camera>" << endl;
			Datafile << aPeople.GetActivePlayerPtr()->GetID() << endl;
			Datafile << "</Camera>" << endl;

		} else {
			Datafile << "<Actors>" << endl;
			Datafile << "<Actor>" << endl;
			Datafile << "<TurnStep>" << endl;
			Datafile << 22.5 << endl;
			Datafile << "</TurnStep>" << endl;
			Datafile << "<light>" << endl;
			Datafile << 128 << endl;
			Datafile << "</light>" << endl;
			Datafile << "<Location>" << endl;
			Datafile << 10 << endl;
			Datafile << "</Location>" << endl;
			Datafile << "</Actor>" << endl;
			Datafile << "</Actors>" << endl;
			aPeople.ExternalizeRules(Datafile);
			Datafile << "<Camera>" << endl;
			Datafile << 0 << endl;
			Datafile << "</Camera>" << endl;
		}


		Datafile << EndBZK2;

		Datafile.close();
		return true;

	}

	void AddSector(VAC_Sector &aSector) {
//	cout << "adding "<<BZK_FastMath::Fix32toInt32(aSector.GetX0())<<","<<BZK_FastMath::Fix32toInt32(aSector.GetX1())<<"-"<<BZK_FastMath::Fix32toInt32(aSector.GetY0())<<","<<BZK_FastMath::Fix32toInt32(aSector.GetY1())<<"-"<<BZK_FastMath::Fix32toInt32(aSector.GetH0())<<","<<BZK_FastMath::Fix32toInt32(aSector.GetH1())<<endl;



		VAC_Sector *ptr = &aSector;
		//  //cout << "ponteiro recebido:" <<(unsigned int)ptr <<endl;
		iSector->Add(ptr);
		aSector.SetID(iSector->TotalItems());
	}

	int
	WriteSectorDetails(ofstream &Datafile, int aSector) {
		//	//cout << "iniciando gravacao de setor" <<endl;
		string Volume = "<Volume>";
		string Vec = "<Vec3f>";
		string EndVec = "</Vec3f>";
		string EndVolume = "</Volume>";
		string Connections = "<Connections>";
		string EndConnections = "</Connections>";
		string Color = "<Color>";
		string RGB = "<RGBA>";
		string EndRGB = "</RGBA>";
		string EndColor = "</Color>";
		string Name = "<Name>";
		string EndName = "</Name>";
		string Trigger = "<Trigger>";
		string EndTrigger = "</Trigger>";
		int closedportals = 0;

		int c, d;
		VAC_Sector *Sector = NULL;
		Sector = GetSector(aSector);

		if (Sector == NULL) {
			cout << "world4" << endl;
			exit(0);
		}
		//cout << "erro de sanidade!!! tentando gravar setor nulo!"<<endl;
		if (Sector->Link != "") {
			Datafile << "<Link>" << endl;
			Datafile << Sector->Link << endl;
			Datafile << "</Link>" << endl;
		}

		if (Sector->iMeshName != "") {
			Datafile << "<mesh>" << endl;
			Datafile << Sector->iMeshName << endl;
			Datafile << "</mesh>" << endl;
		}
		if (Sector->iParent != -1) {
			Datafile << "<parent>" << endl;
			Datafile << Sector->iParent << endl;
			Datafile << "</parent>" << endl;
		}

		Datafile << "<light>" << endl;
		Datafile << Sector->iCandelas + Sector->iExtraCandelas << endl;
		Datafile << "</light>" << endl;

		for (c = 0; c < 6; c++)
			if (Sector->iDoor[c]) {
				d = c;
				if (d == 1 || d == 3) {
					d += 2;
					d = d % 4;
				}
				Datafile << "<Door face = " << d;
				Datafile
						<< " timeout = 150 opener = 36 opensound = \"angstron2/dooropen.wav\" closesound = \"angstron2/doorclose.wav\"  openonpresence = true />"
						<< endl;
			}


		for (c = 0; c < 6; c++)
			if (Sector->iForceRender[c])
				Datafile << "<Force_Render_Face>" << c << "<Force_Render_Face>" << endl;

		int face;
		for (c = 0; c < 6; c++)
			if (Sector->iTextures[c] != NULL) {
				Datafile << "<texture>" << endl;
				face = c;
				if (face == 1 || face == 3) {
					face += 2;
					face = face % 4;
				}
				Datafile << face << " " << Sector->iTextures[c]->iResourceName << endl;
				Datafile << "</texture>" << endl;
			}


		if (Sector->iOriginalTrigger != -1) {
			//cout << "gravando trigger de valor " <<Sector->GetTrigger() <<endl;
			Datafile << Trigger << endl;
			Datafile << Sector->iOriginalTrigger << endl;
			Datafile << EndTrigger << endl;
		}
		Datafile << Volume << "\n";
		BZK_Vec3f *FirstPoint = NULL;
		FirstPoint = new BZK_Vec3f(Sector->iRect.GetX0(), Sector->iRect.GetY0(), Sector->GetH0());

		if (FirstPoint == NULL) {
			cout << "world5" << endl;
			exit(0);
		}
		//cout << "erro de sanidade!!! vetor nao foi alocado!" <<endl;



		Datafile << Vec << "\n";
		Datafile << BZK_FastMath::Fix32toReal32(FirstPoint->GetX()) << "\n";
		Datafile << BZK_FastMath::Fix32toReal32(FirstPoint->GetY()) << "\n";
		Datafile << BZK_FastMath::Fix32toReal32(FirstPoint->GetZ()) << "\n";
		Datafile << EndVec << "\n";
		Datafile << BZK_FastMath::Fix32toReal32(Sector->iRect.GetW()) << "\n";
		Datafile << BZK_FastMath::Fix32toReal32(Sector->iRect.GetH()) << "\n";
		Datafile << BZK_FastMath::Fix32toReal32(Sector->GetH1() - Sector->GetH0()) << "\n";
		Datafile << EndVolume << "\n";
		if (Sector->GetName() != "") {
			Datafile << Name << endl;
			Datafile << Sector->GetName() << endl;
			Datafile << EndName << endl;
		}
		// WritePVSTag(Datafile,*Sector,aSector);
		Datafile << Connections << "\n";
		for (c = 0; c < 6; c++) {
			Datafile << Sector->Get_Connection(c) << "\n";
			if (Sector->Get_Connection(c) == 0) closedportals++;
		}
		Datafile << EndConnections << "\n";
		if (closedportals != 0) {
			for (c = 0; c < 6; c++) {
				if (Sector->Get_Connection(c) != 0) continue;
				Datafile << "<facecolor>" << "\n";
				Datafile << c << "\n";
				Datafile << RGB << "\n";
				d = Sector->iColor[c].r();
				Datafile << d << "\n";
				d = Sector->iColor[c].g();
				Datafile << d << "\n";
				d = Sector->iColor[c].b();
				Datafile << d << "\n";
				d = Sector->iColor[c].a();
				Datafile << d << "\n";
				Datafile << EndRGB << "\n";
				Datafile << "</facecolor>" << "\n";
			}
		}
		delete FirstPoint;
		return ERR_NONE;
	}

	int
	CreateSector() {
		CurrentSector = new VAC_Sector;
		CurrentSector->iOriginalTrigger = (TRIG_NOP);
		AddSector(*CurrentSector);
		//cout << "\nsetor criado";
		return TotalSectors() - 1;
	}

	int
	CreateSector(BZK_Rect &aOrigin, int h1, int h2) {
		CreateSector();
		BZK_Rect *newrect = new BZK_Rect;
		memcpy(newrect, &aOrigin, sizeof(BZK_Rect));
		CurrentSector->SetRect(*newrect);
		CurrentSector->SetH0(BZK_FastMath::Int32toFix32(h1));
		CurrentSector->SetH1(BZK_FastMath::Int32toFix32(h2));
		BZK_FixP Color = BZK_FastMath::Int32toFix32(255);
		BZK_FixP Color2 = BZK_FastMath::Int32toFix32(128);
		BZK_FixP Color3 = BZK_FastMath::Int32toFix32(64);

		CurrentSector->iColor[N].SetR(Color);
		CurrentSector->iColor[N].SetG(0);
		CurrentSector->iColor[N].SetB(0);

		CurrentSector->iColor[S].SetR(0);
		CurrentSector->iColor[S].SetG(Color);
		CurrentSector->iColor[S].SetB(0);

		CurrentSector->iColor[W].SetR(0);
		CurrentSector->iColor[W].SetG(0);
		CurrentSector->iColor[W].SetB(Color);

		CurrentSector->iColor[E].SetR(Color);
		CurrentSector->iColor[E].SetG(Color);
		CurrentSector->iColor[E].SetB(Color);

		CurrentSector->iColor[CEILING].SetR(Color2);
		CurrentSector->iColor[CEILING].SetG(Color2);
		CurrentSector->iColor[CEILING].SetB(Color2);

		CurrentSector->iColor[FLOOR].SetR(Color3);
		CurrentSector->iColor[FLOOR].SetG(Color3);
		CurrentSector->iColor[FLOOR].SetB(Color3);

		//  CurrentSector->Set_Connection


		return TotalSectors() - 1;
	}


	long Polycount() {
		int where;
		int polycount = 0;
		int counter;
		int c = 0;
		BZK_VolumetricSector *ThisSector = NULL;

		for (c = 0; c < TotalSectors(); c++) {
			ThisSector = GetSector(c);
			if (ThisSector == NULL) {
				//cout << "NULL pointer!"<<endl;
				return -1;
			}
			for (counter = 0; counter < 6; counter++)
				if (ThisSector->Get_Connection(counter) == 0)
					polycount++;
		}

		return polycount;
	}
};
