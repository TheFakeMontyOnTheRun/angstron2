enum
{
    BZK_DONTCARE, BZK_APROXIMATE, BZK_USERADIALBUFFER
};

enum
{
    BZK_PATHFINDING, BZK_TESSELATING, BZK_RASTERIZING
};

enum
{
    BZK_INACTIVE, BZK_ACTIVE
};
//focus

enum
{
    BZK_LOWER_LEFT, BZK_LOWER_CENTER, BZK_LOWER_RIGHT, BZK_LEFT, BZK_CENTER, BZK_RIGHT,
    BZK_UPPER_LEFT, BZK_UPPER_CENTER, BZK_UPPER_RIGHT
};

enum
{
BZK_RENDERING_TIME_BEGIN,
BZK_RENDERING_TIME_END,
BZK_RENDERING_TIME_BUDGET,
BZK_RENDERING_TIME_TOTAL
};
#define BRANCH_SIZE 512
#define TRUNK_SIZE 128
#define DESIRED_FRAMERATE 20

class VAC_Engine : public VAC_Display
{
    //======================================================================================
 	/////////////HACKS///////////////
 	BZK_Chunk *iSilence;
    ////////GAME LOGIC/////////////////////
    bool WroteLoading;
    VAC_World *iWorld;
    VAC_Render *_iRender; ///cache the casted render;
    VAC_ActorManager *iPeople;
    VAC_Graphic iWatermark;
    BZK_TypeSet *writer;

//    int iRadialBuffer[iBufferTrunkSize][iBufferBranchSize];
    int *iRadialBuffer;
    int *iTop;
    int iTime[4];
    int iBufferTrunkSize;
    int iBufferBranchSize;
    int iCameraLastSector;
    int iCameraLastDirection;
    int iNeedRendering;

    int iDynamicLightsActive;
    bool JustGotFocus;
    ///////////// RENDERING ///////////////////
    int iBigger;
    int iLast;
 
    int iRenderMode;
    int iOptimize;
    int iState;


    BZK_Queue<int> ActorsToDraw;


public:
   int lastkeycode;
	string RestartTo;
VAC_ActorManager *GetActors()
{
 return iPeople;
 }
 
 VAC_World *GetWorld()
 {
 return iWorld;
 }

    //=======================================================================================

public:

    VAC_Render *GetRender()
    {
        return (VAC_Render*) iRender;
    }
    //---------------------------------------------------------------------------------------
    // INIT CLASS
    //---------------------------------------------------------------------------------------
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    void PreAllocateResources()
    {
        int faces = iWorld->Polycount();
        //cout << "polycount:"<<faces<<endl;

        iRender->PreAllocateResources(faces);
	iWorld->PreAllocateResources();
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void GotFocus()
    {
        VAC_Display::GotFocus();
        if (iWorld->iCurrentFile == "")
        {
//            cout << "data not ready!" << endl;
            HandleSignal(BZK_GS_RESTART);
            WroteLoading = false;
        }
        cout << "focus!" << endl;
        JustGotFocus = true;
	iRender->FadeIn();
    }

    VAC_Engine(string aFilename, BZK_App *aParent) : VAC_Display(aFilename, aParent)
    {
	iBufferTrunkSize=TRUNK_SIZE;
	iBufferBranchSize=BRANCH_SIZE;
	iRadialBuffer=(int*)malloc(iBufferTrunkSize*iBufferBranchSize*sizeof(int));
	iTop=(int*)malloc(iBufferTrunkSize*sizeof(int));
    	iSilence=BZK_DevSound::GetInstance()->LoadSound(string(STOREPATH)+"common/silence2.wav");
	lastkeycode=0;
        SetGoto(-1);
        writer = new BZK_TypeSet((char*)(iParent->GetResourcesLocation()+"typesetb.bmp").c_str());
        RestartTo = aFilename;
        iLast = iBufferTrunkSize;
        iTime[BZK_RENDERING_TIME_BUDGET] = 1000 / BZK_DESIRED_FPS; //rendering budget
        JustGotFocus = false;        
        memset(iRadialBuffer, 0, sizeof (int) * iBufferTrunkSize * iBufferBranchSize);
        iDynamicLightsActive = BZK_APROXIMATE;
        iRenderMode = BZK_USERADIALBUFFER;
        iOptimize = BZK_DONTCARE;
        iQuit = false;
	delete iRender;
        iRender = new VAC_Render();
        _iRender=GetRender();
        Splash();
        _iRender->ClearScreen();
        iWorld = new VAC_World();
        iPeople = new VAC_ActorManager();
        WroteLoading = false;
        paused = false;
	iCameraLastSector=0;
	iCameraLastDirection=-1;
	iNeedRendering=true;
    }

    bool Quit()
    {
        return iQuit;
    }

    void HandleSignal(int aSignal)
    {
        switch (aSignal)
        {
        case BZK_GS_RESTART:
	#ifdef NETWORKSUPPORT
	NET_InitOK=0;
	#endif NETWORKSUPPORT
            WroteLoading = false;
            LoadMap(RestartTo);
            break;

        }

    }
    
    void LoadLogo(string aURL)
    {
    cout << "loading logo:"<<string(STOREPATH + aURL).c_str()<<endl;
    iWatermark.Internalize(string(STOREPATH + aURL).c_str());
    }

///TODO: change stack for something better
    bool HasActorClass(VAC_Sector *aSector, BZK_Sint32 aClass)
    {
        BZK_Stack<int> tmp;
        int id;
        bool toReturn = false;
	/*
        while (!aSector->iActors.Empty())
        {
            id = aSector->iActors.Pop();
            if (((VAC_Actor*) iPeople->GetActorPtr(id))->GetClass() == aClass)
                toReturn = true;
            tmp.Push(id);
        }
        while (!tmp.Empty())
            aSector->iActors.Push(tmp.Pop());
	*/
	int qtd=aSector->iActors.TotalItems();
	for (int c=0;c<qtd;c++)
        {
            id = aSector->iActors[c];
            if (((VAC_Actor*) iPeople->GetActorPtr(id))->GetClass() == aClass)
                toReturn = true;
        }
        return toReturn;
    }

    bool SenseClass(VAC_Sector *aStart, int aDirection, BZK_Sint32 aClass)
    {
        VAC_Sector *current = aStart;
        while (current != NULL)// && current->iParent == aStart->iParent)
            if (HasActorClass(current, aClass))
                return true;
            else
                if (current->Get_Connection(aDirection) > 0)
                current = iWorld->GetSector(current->Get_Connection(aDirection));
            else
                current = NULL;
        return false;
    }


    //---------------------------------------------------------------------------------------
    // SPACE-Time CALCULUS (pretty name for such a mundane stuff, huh?)
    //---------------------------------------------------------------------------------------
    // also GAME INTERNAL EVENTS

    virtual void Tick()
    {
	
        if (!iWorld->DataReady()) return;
        if (paused) return;
     
	//cout << iPeople->GetActivePlayerPtr()->GetCurrentSector() << endl;
       // cout << BZK_FastMath::GetDirection(((VAC_Actor*)iPeople->GetActivePlayerPtr())->GetAngle()) << endl;
       //SDL_Delay(100); //debug easier!
        int c = 0;
        int d=0;
        int sector = 0;
        int where = 0;
        BZK_Sint32 ActorClass;
        int ActorID;
	int ActorSector;
	int FaceConnection;
        VAC_Actor *_Actor;
        VAC_Sector *ThisSector;
        VAC_Actor *playerc;
        VAC_Actor *playerd;
	iNeedRendering=false;
	if (iPeople->GetActivePlayerPtr()->GetCurrentSector()!=iCameraLastSector)
		{
			iNeedRendering=true;
			iCameraLastSector=iPeople->GetActivePlayerPtr()->GetCurrentSector();
		}

	if (!iNeedRendering && BZK_FastMath::GetDirection(((VAC_Actor*)iPeople->GetActivePlayerPtr())->GetAngle())!=iCameraLastDirection)
		{
			iNeedRendering=true;
			iCameraLastDirection=BZK_FastMath::GetDirection(((VAC_Actor*)iPeople->GetActivePlayerPtr())->GetAngle());
		}
	
	if (iWorld->iChanged || iPeople->iChanged)
		iNeedRendering=true;
		

        for (c = 0 ; c< iPeople->GetActorsCount();  c++)
        {
            //obtem ponteiro para ator
            _Actor = (VAC_Actor*) (iPeople->GetActorPtr(c));

            //se ator é nulo, não esta vivo ou ja foi processado, pule
            if (_Actor == NULL || !_Actor->GetAlive())
                continue;
               
            ActorClass=_Actor->GetClass();
	    ActorID=_Actor->GetID();
	    ActorSector = _Actor->GetCurrentSector();        	    
	    
            //se sua classe é negativa, significa que ele esta forçando alguma outra visão do sistema
            if (ActorClass < 0)            
		{
		iPeople->UpdateActors(*iWorld);
                SetGoto(-ActorClass);		
            	}

            //varre a lista de aparências para determinar se a sua aparência atual é a correta (provavelmente pode ser otimizado)
            for (int d = 0; d < iPeople->appearance_class.TotalItems(); d++)
                if (iPeople->appearance_class[d] == ActorClass) //encontrou uma linha cuja classe bate
                {
                
                  if (_Actor->iAppearanceCache==d)
                    	continue;

                
                    // o campo accel_mesh diz se o recurso é um modelo 3D ou uma imagem SVG
                    int tmp = iPeople->appearance_accel_mesh[d];
                    if (tmp)
                    {
                        _Actor->SetBillBoard(NULL); //faz com que o billboard seja nulo, afinal agora temos um modelo 3D
                        //se o ator não tem aparência ou a aparência não é a mesma da determina para sua classe, este muda de aparência
                        //(repare  que coloquei a primeira checagem na frente, para otimizar)
                        if (_Actor->GetAppearance() == NULL || _Actor->GetAppearance()->iResourceName != iPeople->appearance_file[d])
                            _Actor->SetAppearance(iWorld->RequestMeshResource(iPeople->appearance_file[d]));
                    }
                    else //a aparência determinada para esta classe é um billboard ou uma imagem de HUD
                    {
                        _Actor->SetAppearance(NULL); //faz com que o modelo 3D seja nulo, ja que agora temos uma imagem SVG
                        if (_Actor->GetBillBoard() == NULL || _Actor->GetBillBoard()->iResourceName != iPeople->appearance_file[d])
                            _Actor->SetBillBoard(iWorld->RequestGraphicResource(iPeople->appearance_file[d]));
                    }
                    _Actor->iAppearanceCache=d;
                }

            ///////////////////////////////////


            //trata qualquer inconcistência no setor do ator
            if (ActorSector < 0)
            {
                cout << "warning: actor " << c << " has a invalid sector value = " << sector << ". hard setting it to sector 1" << endl;
                _Actor->SetCurrentSector(1);
                ActorSector = 1;
            }

            ThisSector = iWorld->GetSector(ActorSector);

	//Se o ator é fonte de luz válida...	
            if (
#ifndef BZK_DYNAMIC_LIGHTS
ActorClass!=BZK_STATIC_ACTOR &&
#endif
 _Actor->GetCandelas() > 0)           
                  Lit_APROXIMATE(ActorSector, _Actor->GetCandelas());


            //ideia por enquanto cancelada: fontes de luz direcionais...
            ///	if (_Actor->GetFlashlightCandelas()>0)    		 Flashlight(_Actor->GetCurrentSector(),_Actor->GetFlashlightCandelas(),BZK_FastMath::GetDirection(_Actor->GetAngle()));


            if (ThisSector->GetTrigger() != TRIG_NOP)
            {
                //se o setor contém algum sinal, despache ao ator.
//                _Actor->PushEvent(ThisSector->GetTrigger());
                //...e faça deste seu sinal corrente
  //              _Actor->SetTrigger(ThisSector->GetTrigger());
            }

            //se o setor contém algum sinal especificado pelo designer, mesmo procedimento acima
            if (ThisSector->iOriginalTrigger != TRIG_NOP)
            {
                _Actor->PushEvent(ThisSector->iOriginalTrigger);
                _Actor->SetTrigger(ThisSector->iOriginalTrigger);
            }

            //verifica se a classe encherga
            for (int e = 0; e < iPeople->sensor_myclass.TotalItems(); e++)
                if (iPeople->sensor_myclass[e] == ActorClass) //sendo o caso, passe para uma rotina que detecta uma determinada classe de atores
                    if (SenseClass(ThisSector, (iPeople->sensor_direction[e] + BZK_FastMath::GetDirection(_Actor->GetAngle())) % 4, iPeople->sensor_senseclass[e]))
                    	{
                    	ActorClass=iPeople->sensor_newclass[e];
                        _Actor->SetClass(ActorClass);
                        }

            ////////////////////////////////////////////////// detecção de colisão ATOR x MUNDO //////////////////////////////////////////////////////////////////
            //determina, no setor atual, se o ator esta dentro do setor ou por qual parede ele vaza.

	    if (_Actor->iMoved)
		{
	            where = ThisSector->WhereBSC(*_Actor->GetPosition());
            if (where != 100)
            {
            	FaceConnection=ThisSector->Get_Connection(where);
                ///////////////////////////// DE CARA COM A PAREDE //////////////////////////////////
                if (FaceConnection == 0 ||
                        iWorld->GetSector(FaceConnection)->Get_Connection(BZK_FastMath::GetOpositeDirection(where)) == 0)

                { //ator tendou vazar pela parede. Desfaça o movimento e diga a ele que ele colidiu.
                    _Actor->Undo();
                    _Actor->Action(TRIG_COLLIDE);
                }
                else
                {
                    ////////////////////////////// CHANGING SECTOR /////////////////////////////////////////
                    if (ThisSector->Get_Connection(where) > 0)
                    {
                        //ator vazou para outro setor.
                        //mesmo procedimento de inserção em outro setor.
                        _Actor->SetCurrentSector(FaceConnection);
                        ActorSector = FaceConnection;
                        ThisSector = iWorld->GetSector(ActorSector);
                        ThisSector->AddActor(c);
                    }
                    ////////////////////////////// MUDAN�A DE MAPA POR LINK /////////////////////////////////////////////
                    if ((FaceConnection < 0) || (ThisSector->Link != "") && (iPeople->GetActivePlayerPtr()->GetID() == ActorID))
                    {
                        string link;
                        if (ThisSector->Link != "")
                            link = ThisSector->Link;
                        else
                            link = iWorld->ExternalMaps(-(FaceConnection + 1));
                        iPeople->EraseAll();
                        iWorld->LoadMap((STOREPATH + link).c_str(), *iPeople, false);
                        ////////////////////
                        WroteLoading = false;
                        /////////////////////
			NoNeedRedraw();
                        return;
                    }

                    //////////////////////////////////////////////////////////////////////////////////
                }
            }
            else
                //diz ao setor que ele contém o ator atual, para que este seja processado e desenhado
                ThisSector->AddActor(c);
		_Actor->iMoved=false;
	}
	else
	  ThisSector->AddActor(c);


	    //mensagem dinâmica do setor passa a ser a "presença" do ator
            ThisSector->SetTrigger(TRIG_MAX + ActorClass);
        }

	// Colisão entre atores. Ainda não é exata. Talvez nunca seja.
	// não se se este é um método muito eficiente, mas tem funcionado.
        for (c = iPeople->GetActorsCount()-1; c >=0 ; c--)
        {
            playerc = (VAC_Actor*) iPeople->GetActorPtr(c);
	    //se ator c é estático, não o processe
	    if (playerc->GetClass() == BZK_STATIC_ACTOR) continue;

            for (d = iPeople->GetActorsCount()-1; d >=0; d--)
            {
                playerd = (VAC_Actor*) iPeople->GetActorPtr(d);
		//se ator d é estático, não o processe
	        if (playerd->GetClass() == BZK_STATIC_ACTOR) continue;

                if (c != d && playerc->GetCurrentSector() == playerd->GetCurrentSector()
                        //&& playerc->GetBoundingBox()->Inside(playerd->GetBoundingBox())
                             ) //a colisão "ocorreu". hora de determinar o que acontece agora.
                    for (int e = 0; e < iPeople->collision_class.TotalItems(); e++)
                        if (iPeople->collision_class[e] == playerc->GetClass() && iPeople->collided_class[e] == playerd->GetClass())
                            playerc->SetClass(iPeople->collision_newclass[e]);
            }
        }

	SetForRedraw();
        iPeople->UpdateActors(*iWorld);
    }
    //-----------------------------------------------------------------------------
    // Process external events
    //-----------------------------------------------------------------------------

    void LoadMap(string link)
    {
       	cout << "loading it:"<<link<<endl;
        iPeople->EraseAll();
	
        iWorld->LoadMap(link, *iPeople, false);
//	iWorld=VAC_WorldFactory::Build(link);

//	cout << "time to place "<< iPeople->GetActorsCount()-1 << "  actors"<<endl;
//        for (int c = iPeople->GetActorsCount()-1; c >=0; c--)            iPeople->PlaceActor(c, *iWorld->GetSector(iPeople->GetActorPtr(c)->GetCurrentSector()));
        WroteLoading = false;
    }

    virtual void FilterEvents(BZK_SystemEvent &aEvent)
    {
        iDeviceIO->FilterEvents(&aEvent);
	
        if (!iWorld->DataReady()) return;
        if (JustGotFocus)
        {
            JustGotFocus = false;
//            iRender->FadeIn();
            return;
        }
        VAC_Actor *_Player = (VAC_Actor*) (iPeople->GetActivePlayerPtr());
	SetForRedraw();

	lastkeycode=TRIG_NOP;
	
        if (iDeviceIO->STDIN->iKey[ACT_MOVE_E])
           { 
		lastkeycode=(ACT_MOVE_E);
		iPeople->iChanged=true;
           }

        if (iDeviceIO->STDIN->iKey[ACT_MOVE_W]){
            lastkeycode=(ACT_MOVE_W);
		iPeople->iChanged=true;
           }

        if (iDeviceIO->STDIN->iKey[ACT_MOVE_N]){
            lastkeycode=(ACT_MOVE_N);
		iPeople->iChanged=true;
           }

        if (iDeviceIO->STDIN->iKey[ACT_ACTION3]){
            lastkeycode=(ACT_ACTION3);
		iPeople->SetActivePlayer(1);
		iPeople->iChanged=true;
           }

        if (iDeviceIO->STDIN->iKey[ACT_ACTION4]){
            lastkeycode=(ACT_ACTION4);
		iPeople->SetActivePlayer(0);
		iPeople->iChanged=true;
           }

        if (iDeviceIO->STDIN->iKey[ACT_MOVE_S]){
            lastkeycode=(ACT_MOVE_S);
		iPeople->iChanged=true;
           }

        if (iDeviceIO->STDIN->iKey[ACT_MOVE_UP]){
            lastkeycode=(ACT_MOVE_UP);
		iPeople->iChanged=true;
           }

        if (iDeviceIO->STDIN->iKey[ACT_MOVE_DOWN]){
            lastkeycode=(ACT_MOVE_DOWN);
		iPeople->iChanged=true;
           }

        if (iDeviceIO->STDIN->iKey[ACT_TURN_L]){
            lastkeycode=(ACT_TURN_L);
		iPeople->iChanged=true;
           }

        if (iDeviceIO->STDIN->iKey[ACT_TURN_R]){
            lastkeycode=(ACT_TURN_R);
		iPeople->iChanged=true;
           }

        if (iDeviceIO->STDIN->iKey[ACT_ACTION1]){
            lastkeycode=(TRIG_ACTION1);
		iPeople->iChanged=true;
           }

	if (lastkeycode!=TRIG_NOP){
            _Player->PushEvent(lastkeycode, 1);
       		iPeople->iChanged=true;
           }
     
        if (iDeviceIO->STDIN->iKey[ACT_ACTION2]){
	  //  iRender->FadeIn();	
            SetGoto(0); //BZK_GS_BACK; //game main menu
		iPeople->iChanged=true;
           }


    }

    virtual void Splash()
    {
    }

    ~VAC_Engine()
    {
        delete iWorld;
        delete iPeople;
        delete _iRender; 
	_iRender=NULL;
	iRender=NULL; //dont worry, VAC_Display, I've already done the dirty work for ya!
	free(iRadialBuffer);
    }
    //---------------------------------------------------------------------------------------
    // UPDATE GAME LOGIC
    //---------------------------------------------------------------------------------------

    virtual void Update()
    {
        //	cout << "update"<<endl;
        if (!WroteLoading)
        {
            cout << "wrote" << endl;

            writer->BlitString(((BZK_DevVideo*) iDeviceIO->STDOUT)->iVideo, "Loading...", 0, 0);
            iDeviceIO->STDOUT->Refresh();
            WroteLoading = true;
        }

        if (!iWorld->DataReady())
        {
            iWorld->GetMoreData(*iPeople);

            if (iWorld->DataReady())
            {
		cout << "just finished it!"<<endl;
                if (iWorld->iSuggestedFar != 0 && iWorld->iSuggestedNear != 0)
                    _iRender->SetFrustum(iWorld->iSuggestedFar, iWorld->iSuggestedNear);
                    
	///         static lightning for slower devices
                    VAC_Actor* _Actor;
#ifndef BZK_DYNAMIC_LIGHTS
                    for (int c = iPeople->GetActorsCount()-1; c >=0 ; c--)
		        {
		        _Actor=(VAC_Actor*)iPeople->GetActorPtr(c);
		        if (_Actor->GetClass()==BZK_STATIC_ACTOR)
		        	Lit_STATIC(_Actor->GetCurrentSector(),_Actor->GetCandelas());
		        }
#endif                    
                PreAllocateResources();
		if (iWorld->iSugestedBufferSize!=-1)
		{
			free(iRadialBuffer);
			free(iTop);
			iBufferTrunkSize=iWorld->iSugestedBufferSize;
			iBufferBranchSize=iWorld->iSugestedBufferSize;
			iRadialBuffer=(int*)malloc(iBufferTrunkSize*iBufferBranchSize*sizeof(int));
			iTop=(int*)malloc(iBufferTrunkSize*sizeof(int));
		}
	        memset(iRadialBuffer, 0, sizeof (int) * iBufferTrunkSize * iBufferBranchSize);
	        memset(iTop, 0, sizeof (int) * iBufferTrunkSize);
                GotFocus();

            }

            return;

        }

        if (paused)
            return;
        VAC_Actor *Player = (VAC_Actor*) iPeople->GetActivePlayerPtr();

        if (NeedRedraw() && !iQuit)
        {
		VAC_Graphic *BillBoard;
		iRender->AddToImage(iWorld->iSkyBox);	
            if (Player != NULL) RenderView(*Player);
            _iRender->Update(iDeviceIO->STDOUT->iVideo);
            iWorld->NotifyEndOfCycle();
            iPeople->NotifyEndOfCycle();
	    NoNeedRedraw();

	    BillBoard=Player->GetBillBoard();
            if (BillBoard != NULL && !BillBoard->Empty()) iRender->AddToImage(*BillBoard);

		iRender->AddToImage(iWatermark);	
            _iRender->Update(iDeviceIO->STDOUT->iVideo);
            iDeviceIO->STDOUT->Refresh();

        }

    }
    //---------------------------------------------------------------------------------------
    // UPDATE VISUAL
    //---------------------------------------------------------------------------------------

    virtual void Render()
    {
	_iRender->ClearScreen();
        RenderView(*(VAC_Actor*) iPeople->GetActivePlayerPtr());

    }

    virtual void RenderView(VAC_Actor &aActor)
    {
        if (!iWorld->DataReady())
            return;

	   iTime[BZK_RENDERING_TIME_BEGIN] = BZK_GetSysTicks();
/*
	if (!iWorld->iChanged && !iPeople->iChanged)
		{
		cout << "nothing changed" <<endl;
		return;
		}
	cout << "changed" <<endl;
*/
        int x, y;
        int temp;
	int lodlevel=1;
        int lights[6];
        VAC_Actor *dummy;
        VAC_Sector *ThisSector;
        VAC_Sector *CameraSector;

//        
        ////////////// RESET RENDER ///////////////////////////  
        _iRender->LoadCannonical();
        _iRender->SetCamera(aActor);
        /// one would argue: why top[]? why not just test the values of iRadialBuffer? The answer is not that simple: in order to garantuee
        // integrity of the rendered image, I would have to zero all the buffer (or, in a smarter move, always zero the next position of a buffer write).
        // thats too much more writing that this and the check and indirection in the top buffer is not that bad. As a bonus, it always tell me how much
        // is there in the buffer to render...not that I really use it, but...
//        if (iNeedRendering)
	{
		memset(iTop, 0, sizeof (int) * iLast);
		iLast = -1;		
		RenderSector_RadialBuffer_PathFind_NoRecursive(aActor.GetCurrentSector(), aActor, 0, 0, 0);
		iWorld->camerasector = aActor.GetCurrentSector();
	}
  
	VAC_Sector *aux[2];
        for (y = iLast; y >= 0; y--)
            if (iTop[y] != 0)
                for (x = iTop[y]; x >=0 ; x--)	
	        	RenderSector_Aproximate_Tesselate(iRadialBuffer[(y*iBufferTrunkSize)+x], aActor);

       CameraSector=ThisSector;
        while (!ActorsToDraw.Empty())
        {

            temp = ActorsToDraw.Pop();
            if (iPeople->GetActorPtr(temp) != NULL && iPeople->GetActorPtr(temp) != iPeople->GetActivePlayerPtr())
            {
                dummy = (VAC_Actor*) iPeople->GetActorPtr(temp);
                ThisSector=iWorld->GetSector(dummy->GetCurrentSector());
		if (!dummy->HasBeenDrawn()) 
		{
	     //       cout << "adicionando pela caixa mestra" << endl;
		if (ThisSector==CameraSector)
			lodlevel=2;
		else
			lodlevel=1;

                if (dummy->GetAppearance() != NULL)

                    _iRender->AddToImage(*dummy->GetAppearance(), *dummy->GetPosition(), dummy->GetAngle(), ThisSector->iCandelas + ThisSector->iExtraCandelas,lodlevel);
                if (dummy->GetCachedBillBoard() != NULL)
                    _iRender->AddToImage(
                                            *dummy->GetCachedBillBoard(),
                                            *dummy->GetPosition(),
                                            aActor.GetAngle(),
                                            iWorld->GetSector(0)->iCandelas+ThisSector->iCandelas + ThisSector->iExtraCandelas,
				/* 0 b'cos a billboard doesnt need z-order */
					    0
                                            );
		dummy->Drawing();
		}
            }
        }
        iTime[BZK_RENDERING_TIME_END] = BZK_GetSysTicks();
        iTime[BZK_RENDERING_TIME_TOTAL] = iTime[BZK_RENDERING_TIME_END] - iTime[BZK_RENDERING_TIME_BEGIN];
	if (iTime[BZK_RENDERING_TIME_TOTAL] < iTime[BZK_RENDERING_TIME_BUDGET] ) 		
		if (iTime[BZK_RENDERING_TIME_TOTAL] - iTime[BZK_RENDERING_TIME_BUDGET] >0)
			BZK_Wait(iTime[BZK_RENDERING_TIME_TOTAL] - iTime[BZK_RENDERING_TIME_BUDGET]);
		else
			BZK_Wait(iTime[BZK_RENDERING_TIME_BUDGET]-iTime[BZK_RENDERING_TIME_TOTAL]);
    }
    //================================================================================================================

    inline void RenderSector_Aproximate_Tesselate(int aSector, VAC_Actor &aActor)
    {
        if (aSector <= 0) return;
        VAC_Sector *ThisSector = iWorld->GetSector(aSector);
	VAC_Face *faceptr;

   	int RenderingOrder[6];
    	int ordertop=0;
    	
	int rendered=0;
        int where = 0;
        int OffMod = 0;
        float angle = BZK_FastMath::Fix32toReal32(aActor.GetAngle());
        int offset = BZK_FastMath::GetDirection(aActor.GetAngle());
        int offmo = 0;
        int offpo = 0;
        int offback = 0;
        int ShadowHeight;
        int actorsector = aActor.GetCurrentSector();
	int lights=iWorld->GetSector(0)->iCandelas+ThisSector->iCandelas + ThisSector->iExtraCandelas;
        VAC_Sector *OtherSector ;
        VAC_Actor *dummy;
        int temp = 0;
        int backmod;

        //        if (!ThisSector->iActors.Empty())            ShadowHeight = ProjectShadow(aSector);


        offmo = BZK_FastMath::GetLeft(offset);
        offpo = BZK_FastMath::GetRight(offset);
        offback = BZK_FastMath::GetOpositeDirection(offset);



        //se não estamos num setor apenas para desenhar sua porta de offback
        /*
            RenderingOrder.Push(offpo);
            RenderingOrder.Push(offmo);
            RenderingOrder.Push(offset);
            RenderingOrder.Push(CEILING);
            RenderingOrder.Push(FLOOR);
	*/
	    RenderingOrder[ordertop++]=offpo;
            RenderingOrder[ordertop++]=(offmo);
            RenderingOrder[ordertop++]=(offset);
            RenderingOrder[ordertop++]=(CEILING);
            RenderingOrder[ordertop++]=(FLOOR);	    
	
        while (ordertop>0)//(!RenderingOrder.Empty())
        {
          //  OffMod = RenderingOrder.Pop();
            OffMod = RenderingOrder[--ordertop];
            where = ThisSector->Get_Connection(OffMod);


		//////////////////PORTA///////////////////////
	    //se o setor adjacente a face atual tem porta
	    if (where!=0 && iWorld->closeddoors)
	    {
            backmod = BZK_FastMath::GetOpositeDirection(OffMod);
            OtherSector=iWorld->GetSector(where);		

	    if (OffMod<4 && 
		/*OtherSector->iDoor[backmod]!=NULL && */
		OtherSector->Get_Connection(backmod)==0 &&
		OtherSector->iTextures[backmod] != NULL
		)
       	            _iRender->AddToImage(
                                            *OtherSector->iTextures[backmod],
                                            _iRender->GetTexturePosition(*OtherSector, backmod),
                                            backmod * 90
                                            );
		
	///cant allow this, since some parts of the said sector can be visible through some other sector.
//        if (/*ThisSector->iDoor[offback]!=NULL &&*/ ThisSector->Get_Connection(offback)==0) continue; 	        
/*

  # filled space
  = door
  - sector boundaries
           ____________________
	   |    B  |C |        |
	   |_______|  |________|
	   |       #==#        |
	   |       #A #        |
	   |_______#__#________|
	   |       |  |        |
	   | _0_   |  |        |
	   |__/\___|__|________|

While the sector C can be ocluded by the its door to A, its still visible from B
*/

	


	   }

            if (!where)
            {
                if (OffMod == offmo && (ThisSector->iHint == BZK_LOWER_LEFT || ThisSector->iHint == BZK_LEFT || ThisSector->iHint == BZK_UPPER_LEFT))
                    continue;

                if (OffMod == offpo && (ThisSector->iHint == BZK_LOWER_RIGHT || ThisSector->iHint == BZK_RIGHT || ThisSector->iHint == BZK_UPPER_RIGHT))
                    continue;

                if (OffMod == CEILING && (ThisSector->iHint == BZK_LOWER_LEFT || ThisSector->iHint == BZK_LOWER_CENTER || ThisSector->iHint == BZK_LOWER_RIGHT))
                    continue;

                if (OffMod == FLOOR && (ThisSector->iHint == BZK_UPPER_LEFT || ThisSector->iHint == BZK_UPPER_CENTER || ThisSector->iHint == BZK_UPPER_RIGHT))
                    continue;

		if (ThisSector->iColor[OffMod].GetA()==0)
			continue;

                //  if (iDynamicLightsActive == BZK_APROXIMATE)
                if (_iRender->AddToImageFinal(_iRender->ExtractFace(*ThisSector, OffMod),lights))
			rendered++;


                //comment for PocketPC

                if (ThisSector->iTextures[OffMod] != NULL)
                    _iRender->AddToImage(
                                            *ThisSector->iTextures[OffMod],
                                            _iRender->GetTexturePosition(*ThisSector, OffMod),
                                            OffMod * 90
                                            );

                //end comment here for pocketpc
            }
            else
                if (ThisSector->iForceRender[OffMod])
                	 _iRender->AddToImageFinal(_iRender->JustExtractFace(*ThisSector, OffMod));
			

        

        if (ThisSector->iMesh != NULL)
            _iRender->AddToImage(*ThisSector->iMesh, ThisSector->center(), 0);

        _iRender->FlushToSurface();

        if (ThisSector->iParent == iWorld->GetSector(actorsector)->iParent && ThisSector->iParent != -1)
	      for (int model=0;model<ThisSector->iActors.TotalItems();model++)
                ActorsToDraw.Push(ThisSector->iActors[model]);
        else
            for (int model=0;model<ThisSector->iActors.TotalItems();model++)
            {
                temp = ThisSector->iActors[model];
                ////////////////////////////////////
                if (temp != aActor.GetID())
                {
                    if (iPeople->GetActorPtr(temp) != NULL)
                    {
                        dummy = (VAC_Actor*) iPeople->GetActorPtr(temp);
			if (!dummy->HasBeenDrawn()) 
				{
				dummy->Drawing();
		                ///// MAKE AND RENDER SHADOW/////////////////////////////////////////////////
		                //     if (dummy->GetAppearance() != NULL && dummy->GetShadow() == NULL) dummy->SetShadow(dummy->GetAppearance()->MakeShadow(FLOOR, ShadowHeight));
		                // if (dummy->GetShadow() != NULL) _iRender->AddToImage(*dummy->GetShadow(), *dummy->GetPosition(), dummy->GetAngle());
		                ////////////////////////////////////////////////////////////////


		                    if (dummy->GetAppearance() != NULL)
		                        _iRender->AddToImage(*dummy->GetAppearance(), *dummy->GetPosition(), dummy->GetAngle(), ThisSector->iCandelas + ThisSector->iExtraCandelas);
		                    if (dummy->GetCachedBillBoard() != NULL)
		                        _iRender->AddToImage(
		                                                *dummy->GetCachedBillBoard(),
		                                                *dummy->GetPosition(),
		                                                aActor.GetAngle(),
		                                                lights
		                                                );


			}
                    }
                }
            }	
		ThisSector->iActors.ReleaseAll();
	}
	
    }

    void Lit_APROXIMATE(int aSector, int aCandelas)
    {
        int where = 0;
        VAC_Sector *ThisSector;
        ThisSector = iWorld->GetSector(aSector);
        int factor = (aCandelas - ThisSector->iVisibleFaces) / 2;
      //  ThisSector->iExtraCandelas += aCandelas % (255 - ThisSector->iCandelas - ThisSector->iExtraCandelas); //ok , I cheat on lightning...
	  ThisSector->iExtraCandelas += aCandelas;
	//  ThisSector->iExtraCandelas = ThisSector->iExtraCandelas % 256;
	if (ThisSector->iExtraCandelas+ThisSector->iCandelas>255)
		ThisSector->iExtraCandelas=255-ThisSector->iCandelas;
        if (factor < 8) return;
        // loop unrolled
        where = ThisSector->Get_Connection(0);
        if (where != 0 && iWorld->GetSector(where)->Get_Connection(2) ==aSector) Lit_APROXIMATE(where, factor);

        where = ThisSector->Get_Connection(1);
        if (where != 0 && iWorld->GetSector(where)->Get_Connection(3) ==aSector) Lit_APROXIMATE(where, factor);

        where = ThisSector->Get_Connection(2);
        if (where != 0 && iWorld->GetSector(where)->Get_Connection(0) ==aSector) Lit_APROXIMATE(where, factor);

        where = ThisSector->Get_Connection(3);
        if (where != 0 && iWorld->GetSector(where)->Get_Connection(1) ==aSector) Lit_APROXIMATE(where, factor);

        where = ThisSector->Get_Connection(4);
        if (where != 0 && iWorld->GetSector(where)->Get_Connection(5) ==aSector) Lit_APROXIMATE(where, factor);

        where = ThisSector->Get_Connection(5);
        if (where != 0 && iWorld->GetSector(where)->Get_Connection(4) ==aSector) Lit_APROXIMATE(where, factor);

/*
			if (iWorld->closeddoors)
			{
			 backmod=BZK_FastMath::GetOpositeDirection(OffMod);
			OtherSector=iWorld->GetSector(where);
			 if (
				OtherSector->iDoor[backmod]!=NULL &&
				OtherSector->Get_Connection(backmod) ==0 
				) continue;
			} 
*/
    }



    void Lit_STATIC(int aSector, int aCandelas)
    {
        int where = 0;
        VAC_Sector *ThisSector;
        ThisSector = iWorld->GetSector(aSector);
        int factor = (aCandelas - ThisSector->iVisibleFaces) / 2;
        ThisSector->iCandelas += aCandelas;
	//ThisSector->iCandelas=ThisSector->iCandelas%256;
	if (ThisSector->iCandelas>255)
		ThisSector->iCandelas=255;
        if (factor < 8) return;
        // loop unrolled
        where = ThisSector->Get_Connection(0);
        if (where != 0) Lit_STATIC(where, factor);

        where = ThisSector->Get_Connection(1);
        if (where != 0) Lit_STATIC(where, factor);

        where = ThisSector->Get_Connection(2);
        if (where != 0) Lit_STATIC(where, factor);

        where = ThisSector->Get_Connection(3);
        if (where != 0) Lit_STATIC(where, factor);

        where = ThisSector->Get_Connection(4);
        if (where != 0) Lit_STATIC(where, factor);

        where = ThisSector->Get_Connection(5);
        if (where != 0) Lit_STATIC(where, factor);
    }

    //==========================================================================================

    void RenderSector_RadialBuffer_PathFind_NoRecursive(int aSector, VAC_Actor &aActor, int aX, int aY, int aZ)
    {
        int where = 0;
        int OffMod = 0;
        int offset = BZK_FastMath::GetDirection(aActor.GetAngle());
        int offmo = 0;
        int offpo = 0;
        int offback = 0;
	int backmod;
        int Xoff = 0;
        int Yoff = 0;
        int Zoff = 0;
        int RelX, RelY, ReZ;
        int facecount = 0;
        int c = 0;        
       ///unlike the stack @ tesselate, this stack is really important
        BZK_Stack<int> Sectors;        
        BZK_Stack<int> Xs;
        BZK_Stack<int> Ys;
        BZK_Stack<int> Zs;
        Sectors.Push(aSector);
        Xs.Push(aX);
        Ys.Push(aY);
        Zs.Push(aZ);
        VAC_Sector *ThisSector;
	VAC_Sector *OtherSector;
        while (!Sectors.Empty())
        {
            aSector = Sectors.Pop();
            aX = Xs.Pop();
            aY = Ys.Pop();
            aZ = Zs.Pop();
            ThisSector = iWorld->GetSector(aSector);

            if (!ThisSector->HasBeenDrawn())
            {
                ThisSector->Visiting();
//		if (!

//)
//			continue
;
                facecount = ThisSector->iVisibleFaces;
                facecount += ThisSector->iVerticalOcludders;
                facecount += ThisSector->iHorizontalOcludders;


                if (facecount > 0 || aX == 0 || ThisSector->iHorizontalOcludders == 2 || !ThisSector->iActors.Empty())
                {
                    int radius = (float) sqrt /*BZK_FastMath::FastSqrt*/((float) (aX * aX + aZ * aZ + aY * aY));
                    if (iLast < radius)
                        iLast = radius;
/*
                    if (!_iRender->iFrustum._Inside(*ThisSector))
			cout << "vai pular" << endl;
*/
                    if (radius < iBufferTrunkSize )
                        if (iTop[radius] < iBufferBranchSize /*&& _iRender->iFrustum._Inside(*ThisSector)*/)
                            iRadialBuffer[(radius*iBufferTrunkSize)+iTop[radius]++] = aSector;
                }

                offmo = BZK_FastMath::GetLeft(offset);
                offpo = BZK_FastMath::GetRight(offset);
                offback = BZK_FastMath::GetOpositeDirection(offset);

	
                for (int OffMod = 5; OffMod >=0; OffMod--)
                {
                    where = ThisSector->Get_Connection(OffMod);
       /////////////////// needed to keep cohesion - dont touch this ///////////////
                    if (OffMod == offback) continue;
                    if (OffMod == offmo && aX > 0) continue;
                    if (OffMod == offpo && aX < 0) continue;
	/////////////////////////////////////////////////////////////
		    

                    if (OffMod == offset)
                        Zoff = -1;
                    else
                        Zoff = 0;

                    if (OffMod == offmo)
                        Xoff = -1;
                    else
                        if (OffMod == offpo)
                        Xoff = 1;
                    else Xoff = 0;

                    if (OffMod == CEILING)
                        Yoff = 1;
                    else
                        if (OffMod == FLOOR)
                        Yoff = -1;
                    else
                        Yoff = 0;

                    if (aX > 0)
                        RelX = -1;
                    else
                    {
                        if (aX < 0)
                            RelX = 1;
                        else
                            RelX = 0;
                    }

                    if (aY > 0)
                        RelY = 1;
                    else
                    {
                        if (aY < 0)
                            RelY = -1;
                        else
                            RelY = 0;
                    }

                    ThisSector->iHint = 3 + (3 * RelY) + RelX + 1;
                    //			cout << "aX:"<< aX<<" aY"<<aY<< " aZ"<<aZ<< " RelX"<<RelX<< " RelY"<<RelY<< " Hint:"<<ThisSector->iHint <<endl;
                    if (where != 0)
                    {

			if (iWorld->closeddoors)
			{
			///check if the sector ahead hasnt closed shut its connection to us...
			///need better thinking...
			 backmod=BZK_FastMath::GetOpositeDirection(OffMod);
			OtherSector=iWorld->GetSector(where);
			 if (
				/*OtherSector->iDoor[backmod]!=NULL &&*/
				OtherSector->Get_Connection(backmod) ==0 
				) 
				{
			//		OffMod=0;
				continue;
				}
			} 
//			else cout << "pulou no percorrimento" <<endl;

                        Sectors.Push(where);
                        Xs.Push(aX + Xoff);
                        Ys.Push(aY + Yoff);
                        Zs.Push(aZ + Zoff);
                    }

                }

            }
        }
    }


};
