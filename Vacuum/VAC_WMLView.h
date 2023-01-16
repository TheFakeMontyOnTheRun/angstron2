class VAC_WMLView : public VAC_Display {
	Mix_Chunk *SoundEvent[5];
	BZK_TypeSet *writer;
	BZK_TypeSet *writerselected;
	BZK_Vec2f iTextAllignment;
	VAC_VecFonts font;
	int iState;
	BZK_Vector<VAC_Display::VAC_DisplayLet *> iContent;
	int iOption;
public:


	//=======================================================================================
	int KeyName(string aName) {
		if (aName == "up" || aName == "Up" || aName == "UP") return 0;
		if (aName == "enter" || aName == "Enter" || aName == "ENTER") return 4;
		if (aName == "down" || aName == "Down" || aName == "DOWN") return 2;
		if (aName == "left" || aName == "Left" || aName == "LEFT") return 3;
		if (aName == "right" || aName == "Right" || aName == "RIGHT") return 1;
	}

	//---------------------------------------------------------------------------------------
	// INIT CLASS
	//---------------------------------------------------------------------------------------
	void ResetCursor() {
		for (iOption = 0; iOption < iContent.TotalItems() - 1; iOption++)
			if (iContent[iOption]->IsSelectable()) return;
	}

	void ResetCursorBackwards() {
		for (iOption = iContent.TotalItems() - 1; iOption > 0; iOption--)
			if (iContent[iOption]->IsSelectable()) return;
	}

	bool ResolveAsSelectable(BZK_DOMNode *aNode) {
		BZK_DOMNode *current = aNode;
		BZK_DOMNode *style = current->GetElementByName("style");
		if (style == NULL) return false;
		BZK_DOMNode *selectable = style->GetElementByName("enabled");
		if (selectable == NULL) return false;
		if (BZK_ParsingTools::Contains(selectable->GetContent(), "enabled"))
			return true;
		else return false;

	}

	void VisitNode(BZK_DOMNode *aNode) {
		BZK_DOMNode *current = aNode;

		if (current->GetName() == "br" || current->GetName() == "p")
			iContent.Add(new VAC_Display::VAC_DisplayLet("", -1, NULL, false, false));

		if (current->GetName() == "style") {
			//cout << "parse style:"<<current->GetContent() <<endl;
			BZK_XMLReader::ParseStyle(current->GetContent(), *aNode);
			current->SetName("style_parsed");
		}
		if (current->GetName() == "left") {
			iTextAllignment.SetX(BZK_ParsingTools::GetNumber(current->GetContent()));
			if (BZK_ParsingTools::Contains(current->GetContent(), "%"))
				iTextAllignment.SetX(BZK_ParsingTools::GetNumber(
						current->GetContent().substr(0, current->GetContent().length() - 1)));
			iTextAllignment.SetX(BZK_FastMath::Div(BZK_FastMath::Mul(iTextAllignment.GetX(), MXRES), 100));
		}

		if (current->GetName() == "img") {
			iContent.Add(
					new VAC_Display::VAC_DisplayLet("", -1, VAC_Image::InternalizeFromScratch(
							string(STOREPATH) + iParent->GetApplicationName() + string("/") +
							current->GetElementByName("src")->GetContent()), false, false));

		}
		//cout << current->GetName() <<endl;
		if (current->GetName() == "script")
			iContent.Add(
					new VAC_Display::VAC_DisplayLet(current->GetElementByName("_text")->GetContent(), -1, NULL, false,
													true));

		if (current->GetName() == "onload") {
			iParent->Exec(current->GetContent().c_str());
		}

		if (current->GetName() == "music") {
			iMusicFile = (string(STOREPATH) + current->GetContent()).c_str();
			iPlayMusic = true;
		}


		if (current->GetName() == "top") {
			iTextAllignment.SetY(BZK_ParsingTools::GetNumber(current->GetContent()));
			if (BZK_ParsingTools::Contains(current->GetContent(), "%"))
				iTextAllignment.SetY(BZK_ParsingTools::GetNumber(
						current->GetContent().substr(0, current->GetContent().length() - 1)));
			iTextAllignment.SetY(BZK_FastMath::Div(BZK_FastMath::Mul(iTextAllignment.GetY(), YRES), 100));
		}


		if (current->GetName() == "_text" && BZK_ParsingTools::TrimToken(current->GetContent()) != "")
			iContent.Add(new VAC_Display::VAC_DisplayLet(current->GetContent(), -1, NULL, false, false));
		else if (current->GetName() == "anchor" || current->GetName() == "a")
			iContent.Add(
					new VAC_Display::VAC_DisplayLet(
							current->GetElementByName("_text")->GetContent(),
							iParent->ResolveUrl(current->GetElementByName("href")->GetContent().c_str()), NULL, true,
							false
					)
			);
//					cout << "<link para card:"<<current->GetElementByName("href")->GetContent() <<"/>"<<endl;

		{
			if (current->GetName() != "script" && current->GetName() != "anchor" && current->GetName() != "a" &&
				current->GetSon() != NULL)
				VisitNode(current->GetSon());

			if (current->GetName() == "on") {
				Mix_Chunk *tmp = BZK_DevSound::GetInstance()->LoadSound(
						string(STOREPATH) + current->GetElementByName("sound")->GetContent());
				//cout << "key:"<<KeyName(current->GetElementByName("key")->GetContent())<<endl;
				//cout << "sound:"<<string(STOREPATH)+current->GetElementByName("sound")->GetContent()<<endl;
				SoundEvent[KeyName(current->GetElementByName("key")->GetContent())] = tmp;
				if (tmp == NULL)
					cout << "WMLview:" << Mix_GetError() << endl;
			}


			if (current->GetNext() != NULL)
				VisitNode(current->GetNext());
		}
	}


	void CreateFromXML(BZK_DOMNode *aNode) {
//cout << "card:" <<endl;
		VisitNode(aNode);
//cout << "fim card" <<endl;
		ResetCursor();
	}

	void CreateFromXML(string aDeck) {
		BZK_XMLReader reader;
		reader.Internalize(aDeck);
		CreateFromXML(reader.GetRoot()->GetElementByName("card"));
	}


	VAC_WMLView(string aFilename, BZK_App *aParent) : VAC_Display(aFilename, aParent) {
		iOption = 1;
		iTextAllignment.SetX(0);//BZK_FastMath::Int32toFix32((XRES/2.5)+(XRES/12)));
		iTextAllignment.SetY(0);//BZK_FastMath::Int32toFix32((YRES/2)+(YRES/7)));
		//memset(SoundEvent,NULL,sizeof(Mix_Chunk*)*5);
		ResetCursor();
		iPlaySounds = true;
		SoundEvent[0] = NULL;
		SoundEvent[1] = NULL;
		SoundEvent[2] = NULL;
		SoundEvent[3] = NULL;
		SoundEvent[4] = NULL;
		string _type;

		_type = iParent->GetResourcesLocation();
		_type += "typesetb.bmp";
		writer = new BZK_TypeSet((char *) _type.c_str());

		_type = iParent->GetResourcesLocation();
		_type += "typeseta.bmp";
		writerselected = new BZK_TypeSet((char *) _type.c_str());


		iRestartMusic = false;




		//-----

		iQuit = 0;
		//-----
		iRender = new VAC_Rasterer();
		Splash();
		iRender->ClearScreen();
		//----------
		paused = false;
		//cout << "id:"<<iID<<endl;
	}

	virtual bool Quit() { return iQuit; }

	//---------------------------------------------------------------------------------------
	// SPACE-TIME CALCULUS (pretty name for such a mundane stuff, huh?)
	//---------------------------------------------------------------------------------------
	// also GAME INTERNAL EVENTS
	virtual void Tick() {
		if (paused) return;
	}
	//-----------------------------------------------------------------------------
	// Process external events
	//-----------------------------------------------------------------------------

	virtual void SetGoto(int aCard) {
		VAC_Display::SetGoto(aCard);
		if (aCard != -1)
			iRender->FadeToBlack();
	}

	virtual void FilterEvents(BZK_SystemEvent &aEvent) {
//	if (iRender->GetFadeProgress()>0)
		SetForRedraw();
		iDeviceIO->FilterEvents(&aEvent);

		if (((BZK_DevKB *) (iDeviceIO->iInputDevice[0]))->iKey[ACT_ACTION1]) {
			if (iPlaySounds && SoundEvent[KeyName("enter")], EFFECTS_SOUND != NULL)
				BZK_DevSound::GetInstance()->PlaySound(SoundEvent[KeyName("enter")], EFFECTS_SOUND);

			SetGoto(iContent[iOption]->GetLink()); //game start
			//cout << "goto:"<<iGoto<<","<<BZK_GS_RESTART<<endl;
			SetForRedraw();
		}

		if (((BZK_DevKB *) (iDeviceIO->iInputDevice[0]))->iKey[ACT_MOVE_N]) {
			if (iPlaySounds && SoundEvent[KeyName("up")], EFFECTS_SOUND != NULL)
				BZK_DevSound::GetInstance()->PlaySound(SoundEvent[KeyName("up")], EFFECTS_SOUND);


			iOption--; //game start
			if (iOption < 0) ResetCursor();
			if (!iContent[iOption]->IsSelectable())
				ResetCursor();
			SetForRedraw();
		}

		if (((BZK_DevKB *) (iDeviceIO->iInputDevice[0]))->iKey[ACT_MOVE_S]) {
			if (iPlaySounds && SoundEvent[KeyName("down")], EFFECTS_SOUND != NULL)
				BZK_DevSound::GetInstance()->PlaySound(SoundEvent[KeyName("down")], EFFECTS_SOUND);

			iOption++;

			if (iOption >= iContent.TotalItems()) ResetCursorBackwards();

			if (!iContent[iOption]->IsSelectable())
				ResetCursorBackwards();
			SetForRedraw();
		}

		if (iOption >= iContent.TotalItems()) ResetCursorBackwards();
		if (iOption < 0) ResetCursor();


		((BZK_DevKB *) (iDeviceIO->iInputDevice[0]))->ResetAll();
		BZK_Wait(50);
	}

	virtual void Splash() {
	}


	~VAC_WMLView() {
		for (int c = 0; c < iContent.TotalItems(); c++)
			delete iContent[c];
		delete writer;
		delete writerselected;
		BZK_DevSound::GetInstance()->UnloadSound(SoundEvent[0]);
		BZK_DevSound::GetInstance()->UnloadSound(SoundEvent[1]);
		BZK_DevSound::GetInstance()->UnloadSound(SoundEvent[2]);
		BZK_DevSound::GetInstance()->UnloadSound(SoundEvent[3]);
		BZK_DevSound::GetInstance()->UnloadSound(SoundEvent[4]);
	}
	//---------------------------------------------------------------------------------------
	// UPDATE GAME LOGIC
	//---------------------------------------------------------------------------------------

	virtual void GotFocus() {
		VAC_Display::GotFocus();

		for (int c = 0; c < iContent.TotalItems(); c++)
			if (iContent[c]->IsExecutable())
				iParent->Exec(iContent[c]->GetTextContent().c_str());
//	iRender->FadeIn();

	}

	virtual void Update() {
		//cout << "id2:"<<iID<<endl;



		((BZK_DevKB *) (iDeviceIO->iInputDevice[0]))->ResetAll();
		if (paused)
			return;

		if (!NeedRedraw()) return;
		NoNeedRedraw();

		int nontextspace = 0;

		for (int c = 0; c < iContent.TotalItems(); c++) {

			if (iContent[c]->GetTextContent() != "" && !iContent[c]->IsExecutable()) {
				if (c != iOption)

#ifdef SUPPORT_VECTOR_FONTS
					iRender->AddToImage(*font.WriteString((char*)iContent[c]->GetTextContent().c_str(),BZK_FastMath::Fix32toInt32(iTextAllignment.GetX()),BZK_FastMath::Fix32toInt32(iTextAllignment.GetY())+((c-nontextspace)*(YRES/24)),"#FF0000"));
#endif

#ifdef SUPPORT_BITMAP_FONTS
					writer->BlitString(((BZK_DevVideo *) iDeviceIO->STDOUT)->iVideo,
									   (char *) iContent[c]->GetTextContent().c_str(),
									   BZK_FastMath::Fix32toInt32(iTextAllignment.GetX()),
									   BZK_FastMath::Fix32toInt32(iTextAllignment.GetY()) + ((c - nontextspace) * 20));
#endif


				else


#ifdef SUPPORT_VECTOR_FONTS
					iRender->AddToImage(*font.WriteString((char*)iContent[c]->GetTextContent().c_str(),BZK_FastMath::Fix32toInt32(iTextAllignment.GetX())-5,BZK_FastMath::Fix32toInt32(iTextAllignment.GetY())+((c-nontextspace)*(YRES/24)),"#FFFFFF"));
#endif

#ifdef SUPPORT_BITMAP_FONTS
					writerselected->BlitString(((BZK_DevVideo *) iDeviceIO->STDOUT)->iVideo,
											   (char *) iContent[c]->GetTextContent().c_str(),
											   BZK_FastMath::Fix32toInt32(iTextAllignment.GetX()),
											   BZK_FastMath::Fix32toInt32(iTextAllignment.GetY()) +
											   ((c - nontextspace) * 20));
#endif
				iRender->Update(iDeviceIO->STDOUT->iVideo);

			}
			if (iContent[c]->GetGraphicalContent() != NULL) {
				nontextspace++;
//			iContent[c]->GetGraphicalContent()->Translate();
				iRender->AddToImage(*iContent[c]->GetGraphicalContent());
				iRender->Update(iDeviceIO->STDOUT->iVideo);
			} else nontextspace = 0;


		}

/*
	iRender->AddToImage(*font.WriteString("ronaldo",1,1,"#00FF00"));
	iRender->AddToImage(*font.WriteString("rumo a tokio",40,20,"#FF0000"));
*/
		iRender->Update(iDeviceIO->STDOUT->iVideo);

		iDeviceIO->STDOUT->Refresh();
		//cout << "id3:"<<iID<<endl;
	}
	//---------------------------------------------------------------------------------------
	// UPDATE VISUAL
	//---------------------------------------------------------------------------------------

	virtual void Render() {

	}

	//==========================================================================================
};
