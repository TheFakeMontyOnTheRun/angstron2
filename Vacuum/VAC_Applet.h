///TODO: fix music on a BZK_GS_BACK

extern void JoinGame();

extern void StartNetworkedGame();

///TODO: why is that VAC_Applet doesnt use BZK_AppUI at all?

class VAC_Applet : public BZK_Applet {
	VAC_Engine *iEngine;
	VAC_DeviceIO *iDev;
	int iActiveID;
#ifndef __EMSCRIPTEN__
	Mix_Music *music;
#endif
	bool iQuit;
	bool iReady;
	int iDefaultGameView;
	BZK_Vector<VAC_Display *> iDisplay;
	BZK_Stack<int> iHistory;
	BZK_DevKB *STDIN;
	bool iConnected;
public:

	VAC_DeviceIO *GetDeviceIO() {
		return iDev;
	}

	VAC_Engine *GetDefaultGameView() {
		return iEngine;
	}

	VAC_Display *GetCurrentView() {
		return iDisplay[iActiveID];
	}

	bool IsConnected() {
		return iConnected;
	}

	bool CheckId(int aId) {
		if (aId < 0) //triggers
			return true;

		for (int c = 0; c < iDisplay.TotalItems(); c++)
			if (iDisplay[c]->iID == aId) return true;
		return false;
	}

	int AddView(VAC_Display *aView) {
		if (aView == NULL) return -1;
		iDisplay.Add(aView);
		iDisplay[iDisplay.TotalItems() - 1]->iID = iDisplay.TotalItems() - 1;
		iDisplay[iDisplay.TotalItems() - 1]->iDeviceIO = iDev;
		return iDisplay[iDisplay.TotalItems() - 1]->iID;
	}

	void RegisterCard(VAC_WMLView *aCard, string aName) {

		iDisplay[AddView(aCard)]->iTitle = aName;
	}

	void ForceView(int aViewID) {
		iDisplay[iActiveID]->SetGoto(aViewID);
	}

	int GetIDByName(string aCard) {
		////cout << "nome procurado:"<<aCard<<endl;
		for (int c = 0; c < iDisplay.TotalItems(); c++)
			if (iDisplay[c]->iTitle == aCard) return iDisplay[c]->iID;
		return -1;
	}

	virtual int ResolveUrl(const char *aUrl) {
		//cout << "resolvendo "<<aUrl<<endl;
		string Url = aUrl;
		////cout << "resolvendo "<<aUrl<<" como "<<BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Url.substr(1)))<<endl;
		if (Url == "game.resume()") {
			cout << "resume game" << endl;
			return BZK_GS_RESUME;
		}

		if (Url == "game.restart()") {
			if (IsConnected()) {
				NET_SignalPlayer();
				NET_WaitForPlayer();
			}
			return BZK_GS_RESTART;
		}

		if (Url == "game.exit()") return BZK_GS_QUIT;

		if (Url == "game.startnetworkedgame()") {
			cout << "decoded start" << endl;
			return BZK_GS_STARTNETWORK;
		}

		if (Url == "game.joinnetworkedgame()") {
			cout << "decoded join" << endl;
			return BZK_GS_JOINNETWORK;
		}
		/*
		if (Url=="game.waitfornetworkedplayer()")
		{
		//NET_WaitForPlayer();
		iConnected=false;
		return 0;BZK_GS_RESUME;
		}

		if (Url=="game.signalfornetworkedplayer()")
		{
		//NET_SignalPlayer();
		iConnected=false;
		return 0;//BZK_GS_RESUME;
		}
		 */

		if (Url == "back();") {
			//cout << "caught back()" <<endl;
			return BZK_GS_BACK;
		}

		if (aUrl[0] == '#')
			return BZK_FastMath::Fix32toInt32(BZK_ParsingTools::GetNumber(Url.substr(1)));
		/* GetIDByName(Url.substr(1))*/;
		return -1;
	}

	virtual int Exec(const char *aUrl) {
		string Url = aUrl;
		////cout << "resolvendo "<<aUrl<<endl;

		if (Url == "game.resume()") return iDefaultGameView;
		if (Url == "game.exit()") return BZK_GS_QUIT;

		if (Url == "game.startnetworkedgame()") {
			cout << "decoded start" << endl;
			return BZK_GS_STARTNETWORK;
		}

		if (Url == "game.joinnetworkedgame()") {
			cout << "decoded join" << endl;
			return BZK_GS_JOINNETWORK;
		}


		if (Url.substr(0, 6) == "alert(") {
			/*
						Url=Url.substr(7);
						Url=Url.substr(0,Url.length()-3);
						VAC_GameInfoDisplay display("",this);
						display.TextAdd((char*)Url.c_str());
						display.GotFocus();
						display.Update();
						BZK_DevKB::WaitForKey();
			 */
		}
		return -1;
	}

	////////////////////////////////////////////////////////////

	void GenerateFromXML(string aFilename) {
		//cout << "pegando menu de:"<<aFilename<<endl;
		BZK_XMLReader reader;
		BZK_Stack<BZK_DOMNode *> aux;
		BZK_DOMNode *current;
		reader.Internalize(aFilename);
		reader.Visit(*reader.GetRoot());
		current = reader.GetRoot();
		while (current != NULL || !aux.Empty()) {
			if (current == NULL)
				current = aux.Pop();
			if (current->GetName() == "card" && current->GetSon() != NULL &&
				current->GetElementByName("title") != NULL) {
				////cout << "criando um card" <<endl;
				RegisterCard(new VAC_WMLView("", this), current->GetElementByName("title")->GetContent());
				((VAC_WMLView *) iDisplay[iDisplay.TotalItems() - 1])->CreateFromXML(current->GetSon());
				if (iActiveID == -1) iActiveID = iDisplay.TotalItems() - 1;
			} else if (current->GetName() == "object") {

				if (current->GetElementByName("mapfile") != NULL) {
					//cout << "iniciando em "<<current->GetElementByName("mapfile")->GetContent()<<endl;
					iEngine = new VAC_Engine(string(STOREPATH) + current->GetElementByName("mapfile")->GetContent(),
											 this);
				} else {
					cout << "no map file specified in card file. exiting..." << endl;
					exit(0);
				}

				if (current->GetElementByName("watermark") != NULL)
					iEngine->LoadLogo(current->GetElementByName("watermark")->GetContent());

				iDefaultGameView = AddView(iEngine);
			} else if (current->GetSon() != NULL)
				aux.Push(current->GetSon());
			current = current->GetNext();
		}
		reader.Delete(reader.GetRoot());
		//exit(0);

	}
	////////////////////////////////////////////////////////////

	void Start() {
		string _type;
		_type = GetResourcesLocation();
		_type += "typesetb.bmp";
		BZK_TypeSet *writer = new BZK_TypeSet((char *) _type.c_str());
		writer->BlitString(((BZK_DevVideo *) iDev->STDOUT)->iVideo, "Loading...", 0, 0);
		iDev->STDOUT->Refresh();
		delete writer;


		iDefaultGameView = -1;
		iActiveID = -1;

		{
			GenerateFromXML(string(STOREPATH) + GetApplicationName() + string("/menu.wml"));
			//cout << "menus construidos" <<endl;
#ifndef __EMSCRIPTEN__
			if (music != NULL) Mix_FreeMusic(music);
			cout << "music:" << iDisplay[iActiveID]->iMusicFile << endl;
			music = BZK_DevSound::GetInstance()->PlayMusic(iDisplay[iActiveID]->iMusicFile);
			// iDisplay[iActiveID]->GotFocus();
#endif
		}


		iActiveID = 0;
		iReady = true;
		STDIN = ((BZK_DevKB *) (iDev->iInputDevice[0]));
	}
	//----------------------------------------------------------------------------------------------

	void Refresh() {

		if (!iReady || iActiveID == -1) return;
		//perform IO - including 3D rendering,input handling and sound output
		if (iActiveID < iDisplay.TotalItems()) {
			iDisplay[iActiveID]->Tick();
			iDisplay[iActiveID]->Update();
		}
		/*
			  iEngine->SetRenderPlasma(iOptions->iStateOptions[0]);
			  iEngine->SetRenderRadar(iOptions->iStateOptions[1]);
			  iEngine->SetRenderReflex(iOptions->iStateOptions[2]);

			  iEngine->SetLightning(iOptions->iStateOptions[7]);

			  if (iOptions->iStateOptions[5])
				iEngine->SetRenderMode(BZK_APROXIMATE);
				else
				iEngine->SetRenderMode(BZK_DONTCARE);

			 if (iOptions->iStateOptions[6])
			  iEngine->SetOptimization(BZK_OPTIMIZE_FACEMERGING);
				else
			  iEngine->SetOptimization(BZK_DONTCARE);
		 */
	}
	//----------------------------------------------------------------------------------------------

	void FilterEvents(BZK_SystemEvent *aEvents) {

		if (!iReady || iActiveID == -1) return;

		STDIN->ResetAll();
		iDisplay[iActiveID]->FilterEvents(*aEvents);
		iQuit = iQuit || (aEvents->type == BZK_QUIT);
		// cout << "goto:"<<iDisplay[iActiveID]->GoTo()<<endl;
		if (iDisplay[iActiveID]->GoTo() == -1) return;

		int oldId = -1;
		string PreviousMusic = iDisplay[iActiveID]->iMusicFile;


		if (iDisplay[iActiveID]->GoTo() == BZK_GS_STARTNETWORK) {
			cout << "game.startnetworkedgame()" << endl;
			BZK_DevSound::GetInstance()->FadeOut();
			StartNetworkedGame();
			iConnected = true;
			iEngine->RestartTo = (string(STOREPATH) + GetApplicationName() + string("/arena.bzk2"));
			iDisplay[iActiveID]->SetGoto(BZK_GS_RESTART);
		}

		if (iDisplay[iActiveID]->GoTo() == BZK_GS_JOINNETWORK) {
			cout << "game.joinnetworkedgame()" << endl;
			BZK_DevSound::GetInstance()->FadeOut();
			JoinGame();
			iConnected = true;
			iEngine->RestartTo = (string(STOREPATH) + GetApplicationName() + string("/arena.bzk2"));
			iDisplay[iActiveID]->SetGoto(BZK_GS_RESTART);
		}


		if (iDefaultGameView == -1 &&
			(iDisplay[iActiveID]->GoTo() == BZK_GS_RESTART || iDisplay[iActiveID]->GoTo() == BZK_GS_PLAY))
			iDisplay[iActiveID]->SetGoto(-1);

		if (iDefaultGameView != -1 && iDisplay[iActiveID]->GoTo() == BZK_GS_RESTART) {
			//    iDisplay[iActiveID]->iRender->FadeToBlack();
			BZK_DevSound::GetInstance()->FadeOut();
			BZK_DevSound::GetInstance()->StopMusic();
			BZK_DevSound::GetInstance()->StopAllSounds();
			iEngine->HandleSignal(BZK_GS_RESTART);
			iDisplay[iActiveID]->SetGoto(-1);
			iActiveID = iDefaultGameView;
			iDisplay[iDefaultGameView]->SetGoto(-1);
			iDisplay[iDefaultGameView]->GotFocus();
			FilterEvents(aEvents);
			return;
		}

		if (iDefaultGameView != -1 && iDisplay[iActiveID]->GoTo() == BZK_GS_RESUME) {
			iDisplay[iActiveID]->SetGoto(iDefaultGameView);
			return;
		}

		if (iDisplay[iActiveID]->GoTo() == BZK_GS_BACK && !iHistory.Empty()) {
			cout << "caught BZK_GS_BACK" << endl;
			iDisplay[iActiveID]->SetGoto(iHistory.Pop());
			cout << "restored " << iDisplay[iActiveID]->GoTo() << endl;
		} else {
			iHistory.Push(iActiveID);
			cout << "push'd " << iActiveID << endl;
		}

		if (!CheckId(iDisplay[iActiveID]->GoTo())) {
			cout << "invalid card id:" << iDisplay[iActiveID]->GoTo() << " from:" << iActiveID << endl;
			exit(0);
		}

		//   iDisplay[iActiveID]->iRender->FadeToBlack();
		oldId = iActiveID;
		iActiveID = iDisplay[iActiveID]->GoTo();
		iDisplay[oldId]->SetGoto(-1);

		if (iActiveID == BZK_GS_BACK && !iHistory.Empty()) {
			cout << "caught BZK_GS_BACK right on time!" << endl;
			iActiveID = iHistory.Pop();
			cout << "restored right on time!" << iActiveID << endl;
		}

#ifndef __EMSCRIPTEN__
		if (CheckId(iActiveID) && (iActiveID > 0) &&
			(PreviousMusic != iDisplay[iActiveID]->iMusicFile || iDisplay[iActiveID]->iRestartMusic)) {
			cout << "restart music:" << iDisplay[iActiveID]->iRestartMusic << endl;
			cout << "previous music:" << PreviousMusic << endl;
			cout << "new music:" << iDisplay[iActiveID]->iMusicFile << endl;
			BZK_DevSound::GetInstance()->FadeOut();
			BZK_DevSound::GetInstance()->StopMusic();
			BZK_DevSound::GetInstance()->StopAllSounds();
			cout << "faded and stopped" << endl;

#ifndef __EMSCRIPTEN__
			if (music != NULL)
				Mix_FreeMusic(music);

			if (iDisplay[iActiveID]->iPlayMusic && iDisplay[iActiveID]->iMusicFile != "") {
				music = BZK_DevSound::GetInstance()->PlayMusic(iDisplay[iActiveID]->iMusicFile);
				PreviousMusic = iDisplay[iActiveID]->iMusicFile;
			} else
				music = NULL;
#endif

		}
#endif

		if (CheckId(iActiveID) && (iActiveID > 0)) ///sem essa leitura, há um erro
			iDisplay[iActiveID]->GotFocus();
		//      if (iActiveID!=iDefaultGameView)  		BZK_DevSound::GetInstance()->FadeOut();

		if (iActiveID == BZK_GS_QUIT)
			iQuit = true;
	}
	//----------------------------------------------------------------------------------------------

	bool WaitForNextCycle() {
#ifdef POCKETPC
		return false;
#endif

#ifdef EZX
		return false; //uncomment this for EZX
#else
		return iQuit; //you can safely use this for other plataforms
#endif
	}
	//----------------------------------------------------------------------------------------------

	VAC_Applet() {
		if (BZK_SysInit())
			iQuit = 0;
		else {
			iQuit = 1;
		}
		iDev = new VAC_DeviceIO;
		//TODO: middleware pode indicar em seu card um mapa de inicio

		iReady = false;
		iEngine = NULL;
		iQuit = false;
#ifndef __EMSCRIPTEN__
		music = NULL;
#endif
		iConnected = false;
	}
	//----------------------------------------------------------------------------------------------

	~VAC_Applet() {
#ifndef __EMSCRIPTEN__
		if (music != NULL)
			Mix_FreeMusic(music);
#endif
		for (int c = 0; c < iDisplay.TotalItems(); c++)
			if (c != iDefaultGameView)
				delete iDisplay[c];
		delete iEngine;
		while (!iHistory.Empty()) iHistory.Pop();


	}

};
