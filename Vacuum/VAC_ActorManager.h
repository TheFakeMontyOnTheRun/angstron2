//class VAC_LogoLang;
//Gerenciador de bandos de atores. aqui surgiu a ideia doida
//=============================================================================================

class VAC_ActorManager : public BZK_ActorManager {
    ///                                     standard value
public: //just for now
	bool iChanged;
    BZK_Vector<BZK_Sint32> triggered_class; // class
    BZK_Vector<int> triggered_state; // 9999 (same applies here)
    BZK_Vector<int> triggered_trigger; // 9999 (same applies here)     
    BZK_Vector<int> triggered_newstate; // 9999 (same applies here)

    BZK_Vector<BZK_Sint32> transition_class; //        class
    BZK_Vector<int> transition_state; //          nop
    BZK_Vector<BZK_Sint32> transition_newclass; //       class (same)

    BZK_Vector<BZK_Sint32> spawner_class; // class
    BZK_Vector<int> spawner_state; // 9999 (any high value - if the user needs this number, just override it)
    BZK_Vector<BZK_Sint32> spawner_spawnedclass; // class
    BZK_Vector<int> spawner_spawnedstate; // class

    BZK_Vector<BZK_Sint32> droppers_class; // class
    BZK_Vector<int> droppers_state; //9999
    BZK_Vector<int> droppers_newdroptrigger; //nop

    BZK_Vector <string> appearance_file;
    BZK_Vector <BZK_Sint32> appearance_class;
    BZK_Vector <int> appearance_accel_mesh;

    BZK_Vector<BZK_Sint32> collision_class; // class
    BZK_Vector<BZK_Sint32> collided_class; // class
    BZK_Vector<int> collision_newclass; //9999    

    BZK_Vector<BZK_Sint32> sensor_myclass; // class
    BZK_Vector<int> sensor_direction; // class
    BZK_Vector<BZK_Sint32> sensor_senseclass; // class
    BZK_Vector<BZK_Sint32> sensor_newclass; //9999    


    BZK_Vector<BZK_Sint32> sound_class;    
    BZK_Vector<int> sound_action;    
    BZK_Vector<BZK_Chunk*> sound_chunk;
    BZK_Vector<std::string> sound_file;
  
    BZK_Stack <VAC_Actor*> iAvailableActors;

    BZK_Vector<BZK_Sint32> light_class; //        class
    BZK_Vector<int> light_candelas; //          nop

    //============================================================================================
public:

VAC_Actor *RequestActor()
{
if (iAvailableActors.Empty())
	return new VAC_Actor();
else
	{
	cout << "actor recycled"<<endl;
	return iAvailableActors.Pop();
	}
}

void DisposeActor(VAC_Actor *aActor)
{
	iAvailableActors.Push(aActor);
}


void	ExternalizeRules(ofstream &aOutStream)
{
	aOutStream << "<Rules>"<<endl;
    if (triggered_class.TotalItems()>0)
	{
	aOutStream << "<TriggerTransitions>" <<endl;
	for (int c=0;c<triggered_class.TotalItems();c++)
		{
		aOutStream << triggered_class[c] <<  " ";
		aOutStream << triggered_state[c] <<  " ";
		aOutStream << triggered_trigger[c] <<  " ";
		aOutStream << triggered_newstate[c] <<  endl;
		}
	aOutStream << "</TriggerTransitions>" <<endl;
	}


    if (transition_class.TotalItems()>0)
	{
	aOutStream << "<ClassTransitions>" <<endl;
	for (int c=0;c<transition_class.TotalItems();c++)
		{
		aOutStream << transition_class[c] <<  " ";
		aOutStream << transition_state[c] <<  " ";
		aOutStream << transition_newclass[c] <<  endl;
		}
	aOutStream << "</ClassTransitions>" <<endl;
	}

    if (spawner_class.TotalItems()>0)
	{
	aOutStream << "<Spawner>" <<endl;
	for (int c=0;c<spawner_class.TotalItems();c++)
		{
		aOutStream << spawner_class[c] <<  " ";
		aOutStream << spawner_state[c] <<  " ";
		aOutStream << spawner_spawnedclass[c] <<  " ";
		aOutStream << spawner_spawnedstate[c] <<  endl;
		}
	aOutStream << "</Spawner>" <<endl;
	}


    if (droppers_class.TotalItems()>0)
	{
	aOutStream << "<Actions>" <<endl;
	for (int c=0;c<droppers_class.TotalItems();c++)
		{
		aOutStream << droppers_class[c] << " ";
		aOutStream << droppers_state[c] <<  " ";
		aOutStream << droppers_newdroptrigger[c] <<  endl;
		}
	aOutStream << "</Actions>" <<endl;
	}


    if (appearance_class.TotalItems()>0)
	{
	aOutStream << "<Appearance>" <<endl;
	for (int c=0;c<appearance_class.TotalItems();c++)
		{
		aOutStream << appearance_class[c] <<  " ";
		aOutStream << appearance_file[c] <<  endl;
		}
	aOutStream << "</Appearance>" <<endl;
	}


    if (collision_class.TotalItems()>0)
	{
	aOutStream << "<Collisions>" <<endl;
	for (int c=0;c<collision_class.TotalItems();c++)
		{
		aOutStream << collision_class[c] << " ";
		aOutStream << collided_class[c] <<  " ";
		aOutStream << collision_newclass[c] <<  endl;
		}
	aOutStream << "</Collisions>" <<endl;
	}


    if (sensor_myclass.TotalItems()>0)
	{
	aOutStream << "<Sensor>" <<endl;
	for (int c=0;c<sensor_myclass.TotalItems();c++)
		{
		aOutStream << sensor_myclass[c] <<  " ";
		aOutStream << sensor_direction[c] <<  " ";
		aOutStream << sensor_senseclass[c] <<  " ";
		aOutStream << sensor_newclass[c] <<  endl;
		}
	aOutStream << "</Sensor>" <<endl;
	}


    if (sound_class.TotalItems()>0)
	{
	aOutStream << "<ActionSounds>" <<endl;
	for (int c=0;c<sound_class.TotalItems();c++)
		{
		aOutStream << sound_class[c] <<  " ";
		aOutStream << sound_action[c] <<  " ";
		aOutStream << sound_file[c] <<  endl;
		}
	aOutStream << "</ActionSounds>" <<endl;
	}

    if (light_class.TotalItems()>0)
	{
	aOutStream << "<Lights>" <<endl;
	for (int c=0;c<sensor_myclass.TotalItems();c++)
		{
		aOutStream << light_class[c] <<  " ";
		aOutStream << light_candelas[c] <<  endl;
		}
	aOutStream << "</Lights>" <<endl;
	}

	aOutStream << "</Rules>"<<endl;

}


    BZK_Vector<VAC_Actor*> toRemove;

    void ResetRules() {
        triggered_class.ReleaseAll();
        triggered_state.ReleaseAll();
        ; // 9999 (same applies here)
        triggered_trigger.ReleaseAll();
        ; // 9999 (same applies here)
        triggered_newstate.ReleaseAll();
        ; // 9999 (same applies here)

        transition_class.ReleaseAll();
        ; //        class
        transition_state.ReleaseAll();
        ; //          nop
        transition_newclass.ReleaseAll();
        ; //       class (same)

        spawner_class.ReleaseAll();
        ; // class
        spawner_state.ReleaseAll();
        ; // 9999 (any high value - if the user needs this number, just override it)
        spawner_spawnedclass.ReleaseAll();
        ; // class
        spawner_spawnedstate.ReleaseAll();
        ; // class


        droppers_class.ReleaseAll();
        ; // class
        droppers_state.ReleaseAll();
        ; //9999
        droppers_newdroptrigger.ReleaseAll();
        ; //nop

        appearance_file.ReleaseAll();
        ;
        appearance_class.ReleaseAll();
        ;
        appearance_accel_mesh.ReleaseAll();
        ;

        collision_class.ReleaseAll();
        ; // class
        collided_class.ReleaseAll();
        ; // class
        collision_newclass.ReleaseAll();
        ; //9999


	sound_class.ReleaseAll();
	sound_action.ReleaseAll();
	sound_chunk.ReleaseAll();
    }

    void SpawnActor(int aAngle, int aSector, VAC_Actor &aOrigin, BZK_Sint32 aClass, int aState) {
        VAC_Actor *ptr = RequestActor();
        ptr->SetClass(aClass);
        ptr->SetState(aState);
        ptr->SetCurrentSector(aSector);
        ptr->SetPosition(*aOrigin.GetPosition());
        ptr->SetAngle(aAngle);
        AddActor(*ptr->Base());
    }

    VAC_ActorManager() {
	iChanged=true;
    }

    ~VAC_ActorManager() {
	ResetRules();
	for (int c=0;c<iAvailableActors.TotalItems();c++)
		delete iAvailableActors.Pop();
	}

virtual  void EraseAll()
  {
	for (int c=0;c<GetActorsCount();c++)
		delete GetActorPtr(c);

	BZK_ActorManager::ReleaseAll();
    }
    //--------------------------------------------------------------------------------------------

    int AddActor(BZK_Actor &aActor) {
        return BZK_ActorManager::AddActor(aActor);
    }
    //-------------------------------------------------------------------------------------------

    void PlaceActor(int aID, BZK_VolumetricSector &aSector) {
        // ((VAC_Actor*)GetActorPtr(aID))->iPosition.iZ+=BZK_FastMath::Real32toFix32(5.5);
    }
    //-------------------------------------------------------------------------------------------

	void NotifyEndOfCycle()
	{
	VAC_Actor *_Actor;
		for (int c = 0; c < GetActorsCount(); c++)
		{
		_Actor = (VAC_Actor*) GetActorPtr(c);
		    if (_Actor != NULL) 
			_Actor->NotifyEndOfCycle();
		
		}
	}
    void UpdateActors(Partial_VAC_World &aWorld) //faz mundo funcionar - implementa��o tempor�ria
    {
	iChanged=false;
        int c;
	BZK_Sint32 ActorClass;
	int Trigger;
	int ActorSector;
	int ActorState;
	int ActorTrigger;
        int channel;
	int PlayerSector=GetActivePlayerPtr()->GetCurrentSector();	
        //   //cout << "update na camada Vacuum" <<endl;

        for (c = 0; c < GetActorsCount(); c++)
		{
                VAC_Actor *_Actor = (VAC_Actor*) GetActorPtr(c);
                if (_Actor == NULL) continue;
                
		ActorClass=_Actor->GetClass();
		if (ActorClass == BZK_STATIC_ACTOR) continue;
		ActorSector=_Actor->GetCurrentSector();
		ActorState=_Actor->GetState();
		ActorTrigger=_Actor->GetTrigger();
//		cout <<c<< ":----------------" <<endl;
                for (int e = 0; e < transition_class.TotalItems() /*&& transition_class[e] <= _Actor->GetClass()*/ ; e++) { 
                    if (transition_class[e] == ActorClass) {                         
                        if (transition_state[e] == ActorState) //e se ele é do estado determinado
                        {
                            ActorClass=transition_newclass[e];
                            _Actor->SetClass(ActorClass); //ele ganha uma nova classe
			    iChanged=true;
                          //  break;                          
                        }
                    }
                }

                channel=PEER_SOUND;
                for (int e = sound_class.TotalItems()-1; e >=0; e--) {
                    if (sound_class[e] == ActorClass && sound_action[e] == ActorState) { //se ele é da classe
                     	if (GetActivePlayerPtr()==_Actor)
                     		channel=PLAYER_SOUND;
                     	else
	                     	if (PlayerSector==ActorSector)
	                     		channel=PEER_SOUND;
		                     		else
		                     		channel=FAR_SOUND;
			BZK_DevSound::GetInstance()->PlaySound(sound_chunk[e],channel);
			    iChanged=true;
                    }
                }

                for (int e = 0; e < light_class.TotalItems()  /* && triggered_class[e] <= _Actor->GetClass()*/; e++) {
                    if (light_class[e] == ActorClass) { //se ele é da classe
                                _Actor->SetCandelas(light_candelas[e]); //ele ganha um novo estado
				iChanged=true;				
                            }
                    }
                

                //------------------
                for (int e = 0; e < triggered_class.TotalItems()  /* && triggered_class[e] <= _Actor->GetClass()*/; e++) {
                    if (triggered_class[e] == ActorClass && triggered_state[e] == ActorState) { //se ele é da classe
                        cout << "achei kind e agora procuro pelo trigger:" << triggered_trigger[e] << endl;
                        if (triggered_trigger[e] == ActorTrigger) //e estiver droppando trigger
                            if (triggered_state[e] == ActorState) //e tiver o determinado estado, 
                            {
				iChanged=true;
                           //     cout << "estado mudado" << endl;
                                _Actor->Action(triggered_newstate[e]); //ele ganha um novo estado
				
				if (_Actor->iMoved && _Actor->GetAlive())
					{
					_Actor->SetCurrentSector(aWorld.GetSectorByPos(*_Actor->GetPosition(),ActorSector,triggered_newstate[e]));
				//	std::cout << "atualizou com "<<_Actor->GetCurrentSector() << std::endl;
			    
					}
                            }
                    }
                }
                //-------------------
                for (int e = spawner_class.TotalItems()-1; e >=0; e--) {
                
                    if (spawner_class[e] == ActorClass) { //se ele é da classe
                        if (spawner_state[e] == ActorState) //e estiver em determinado estado
				{
                            SpawnActor(_Actor->GetAngle(), ActorSector, *_Actor, spawner_spawnedclass[e], spawner_spawnedstate[e]);
				    iChanged=true;
				}
                        // ele cria um novo ator com determinada classe e estado
                        // isso provavelmente deveria trocar de ordem com a de cima...
                    }
                }
                //-------------------
                for (int e = 0; e < droppers_class.TotalItems()/*&& droppers_class[e] <= _Actor->GetClass()*/; e++) {
			//
                	
//			cout << droppers_class[e]<<","<<_Actor->GetClass() <<endl;
                    if (droppers_class[e] == ActorClass) { //se ele pertence a uma determinada classe

//cout << _Actor->GetClass()<<","<<_Actor->GetState() <<endl;
                        if (droppers_state[e] == ActorState) //e tem determinado estado
				{
//				if (droppers_class[e] != BZK_STATIC_ACTOR)
				//	cout << droppers_class[e]<<","<<droppers_state[e]<<","<<droppers_newdroptrigger[e]<<endl;
				     Trigger=droppers_newdroptrigger[e];
	                            _Actor->Action(Trigger); //ele deve ter um certo drop trigger...
					if (Trigger!=27 && _Actor->iMoved && _Actor->GetAlive())
					{
					_Actor->SetCurrentSector(aWorld.GetSectorByPos(*_Actor->GetPosition(),ActorSector,Trigger));
			//		std::cout << "atualizou com "<<_Actor->GetCurrentSector() << std::endl;
				    
					}
				iChanged=true;
				}
                    }
                }




                if (!_Actor->GetAlive()) {
                    _Actor->SetCurrentSector(0);
                  //  DisposeActor(_Actor);
		//	cout <<"disposing"<< endl;
		 iChanged=true;
                } else
                    _Actor->Update();


              
            }

      


    }

    void SetActivePlayer(int aID) {
   // cout << "VAC: camera agora em "<<aID<<endl;
//    	cout << "setei" <<endl;
        BZK_ActorManager::SetActivePlayer(aID);
 iChanged=true;
  //      ((VAC_Actor*) GetActivePlayerPtr())->SetActive(true);
        //
    }
    
    void Optimize()
    {
	    {
	    		int c;

			    BZK_Vector<int> _triggered_class; // class
			    BZK_Vector<int> _triggered_state; // 9999 (same applies here)
			    BZK_Vector<int> _triggered_trigger; // 9999 (same applies here)     
			    BZK_Vector<int> _triggered_newstate; // 9999 (same applies here)
    	    		int *classes=(int*)malloc(triggered_class.TotalItems()*sizeof(int));
    	    		int *indexes=(int*)malloc(triggered_class.TotalItems()*sizeof(int));
			for (c=0;c<triggered_class.TotalItems();c++)			
			{
			//	cout << triggered_class[c] <<endl;
			    classes[c]=triggered_class[c];
			    indexes[c]=c;
			}
			BZK_FastMath::Order(&classes,&indexes,triggered_class.TotalItems());
			//cout << "order..."<<endl;
			for (c=0;c<triggered_class.TotalItems();c++)			
			{
				_triggered_class.Add(triggered_class[c]);
				_triggered_state.Add(triggered_state[c]);			
				_triggered_trigger.Add(triggered_trigger[c]);			
				_triggered_newstate.Add(triggered_newstate[c]);
			}			
			triggered_class.ReleaseAll();
			triggered_state.ReleaseAll();
			triggered_trigger.ReleaseAll();
			triggered_newstate.ReleaseAll();
			for (c=0;c<_triggered_class.TotalItems();c++)			
			{

				triggered_class.Add(_triggered_class[indexes[c]]);
			//	cout << triggered_class[c] <<endl;				
				triggered_state.Add(_triggered_state[indexes[c]]);			
				triggered_trigger.Add(_triggered_trigger[indexes[c]]);			
				triggered_newstate.Add(_triggered_newstate[indexes[c]]);
			}	
						free(classes);
			free(indexes);		
			
	    }
    /*
    BZK_Vector<int> transition_class; //        class
    BZK_Vector<int> transition_state; //          nop
    BZK_Vector<int> transition_newclass; //       class (same)
*/

   {
   			//cout << "class transission" <<endl;
	    		int c;

			    BZK_Vector<int> _transition_class; // class
			    BZK_Vector<int> _transition_state; // 9999 (same applies here)
			    BZK_Vector<int> _transition_newclass; // 9999 (same applies here)
    	    		int *classes=(int*)malloc(transition_class.TotalItems()*sizeof(int));
    	    		int *indexes=(int*)malloc(transition_class.TotalItems()*sizeof(int));
			for (c=0;c<transition_class.TotalItems();c++)			
			{
			//	cout << transition_class[c] <<endl;
			    classes[c]=transition_class[c];
			    indexes[c]=c;
			}
			BZK_FastMath::Order(&classes,&indexes,transition_class.TotalItems());
			//cout << "order..."<<endl;
			for (c=0;c<transition_class.TotalItems();c++)			
			{
				_transition_class.Add(transition_class[c]);
				_transition_state.Add(transition_state[c]);			
				_transition_newclass.Add(transition_newclass[c]);
			}			
			transition_class.ReleaseAll();
			transition_state.ReleaseAll();
			transition_newclass.ReleaseAll();
			for (c=0;c<_transition_class.TotalItems();c++)			
			{

				transition_class.Add(_transition_class[indexes[c]]);
			//	cout << transition_class[c] <<endl;				
				transition_state.Add(_transition_state[indexes[c]]);			
				transition_newclass.Add(_transition_newclass[indexes[c]]);
			}			
			//exit(0);
						free(classes);
			free(indexes);
	    }
/*
    BZK_Vector<int> spawner_class; // class
    BZK_Vector<int> spawner_state; // 9999 (any high value - if the user needs this number, just override it)
    BZK_Vector<int> spawner_spawnedclass; // class
    BZK_Vector<int> spawner_spawnedstate; // class
*/
    {
	    		int c;

			    BZK_Vector<int> _spawner_class; // class
			    BZK_Vector<int> _spawner_state; // 9999 (same applies here)
			    BZK_Vector<int> _spawner_spawnedclass; // 9999 (same applies here)     
			    BZK_Vector<int> _spawner_spawnedstate; // 9999 (same applies here)
    	    		int *classes=(int*)malloc(spawner_class.TotalItems()*sizeof(int));
    	    		int *indexes=(int*)malloc(spawner_class.TotalItems()*sizeof(int));
			for (c=0;c<spawner_class.TotalItems();c++)			
			{
			//	cout << triggered_class[c] <<endl;
			    classes[c]=spawner_class[c];
			    indexes[c]=c;
			}
			BZK_FastMath::Order(&classes,&indexes,spawner_class.TotalItems());
			//cout << "order..."<<endl;
			for (c=0;c<spawner_class.TotalItems();c++)			
			{
				_spawner_class.Add(spawner_class[c]);
				_spawner_state.Add(spawner_state[c]);			
				_spawner_spawnedclass.Add(spawner_spawnedclass[c]);			
				_spawner_spawnedstate.Add(spawner_spawnedstate[c]);
			}			
			spawner_class.ReleaseAll();
			spawner_state.ReleaseAll();
			spawner_spawnedclass.ReleaseAll();
			spawner_spawnedstate.ReleaseAll();
			for (c=0;c<_spawner_class.TotalItems();c++)			
			{

				spawner_class.Add(_spawner_class[indexes[c]]);
			//	cout << triggered_class[c] <<endl;				
				spawner_state.Add(_spawner_state[indexes[c]]);			
				spawner_spawnedclass.Add(_spawner_spawnedclass[indexes[c]]);			
				spawner_spawnedstate.Add(_spawner_spawnedstate[indexes[c]]);
			}	
						free(classes);
			free(indexes);		
			
	    }

/*
    BZK_Vector<int> droppers_class; // class
    BZK_Vector<int> droppers_state; //9999
    BZK_Vector<int> droppers_newdroptrigger; //nop
*/
 {
   			//cout << "class transission" <<endl;
	    		int c;

			    BZK_Vector<int> _droppers_class; // class
			    BZK_Vector<int> _droppers_state; // 9999 (same applies here)
			    BZK_Vector<int> _droppers_newdroptrigger; // 9999 (same applies here)
    	    		int *classes=(int*)malloc(droppers_class.TotalItems()*sizeof(int));
    	    		int *indexes=(int*)malloc(droppers_class.TotalItems()*sizeof(int));
			for (c=0;c<droppers_class.TotalItems();c++)			
			{
			//	cout << transition_class[c] <<endl;
			    classes[c]=droppers_class[c];
			    indexes[c]=c;
			}
			BZK_FastMath::Order(&classes,&indexes,droppers_class.TotalItems());
			//cout << "order..."<<endl;
			for (c=0;c<droppers_class.TotalItems();c++)			
			{
				_droppers_class.Add(droppers_class[c]);
				_droppers_state.Add(droppers_state[c]);			
				_droppers_newdroptrigger.Add(droppers_newdroptrigger[c]);
			}			
			droppers_class.ReleaseAll();
			droppers_state.ReleaseAll();
			droppers_newdroptrigger.ReleaseAll();
			for (c=0;c<_droppers_class.TotalItems();c++)			
			{

				droppers_class.Add(_droppers_class[indexes[c]]);
			//	cout << transition_class[c] <<endl;				
				droppers_state.Add(_droppers_state[indexes[c]]);			
				droppers_newdroptrigger.Add(_droppers_newdroptrigger[indexes[c]]);
			}			
			//exit(0);
						free(classes);
			free(indexes);
	    }
/*

    BZK_Vector <int> appearance_class;
    BZK_Vector <int> appearance_accel_mesh;
    BZK_Vector <string> appearance_file;
*/
 {
   			//cout << "class transission" <<endl;
	    		int c;

			    BZK_Vector<int> _appearance_class; // class
			    BZK_Vector<int> _appearance_accel_mesh; // 9999 (same applies here)
			    BZK_Vector<string> _appearance_file; // 9999 (same applies here)
    	    		int *classes=(int*)malloc(appearance_class.TotalItems()*sizeof(int));
    	    		int *indexes=(int*)malloc(appearance_class.TotalItems()*sizeof(int));
			for (c=0;c<appearance_class.TotalItems();c++)			
			{
			//	cout << transition_class[c] <<endl;
			    classes[c]=appearance_class[c];
			    indexes[c]=c;
			}
			BZK_FastMath::Order(&classes,&indexes,appearance_class.TotalItems());
			//cout << "order..."<<endl;
			for (c=0;c<appearance_class.TotalItems();c++)			
			{
				_appearance_class.Add(appearance_class[c]);
				_appearance_accel_mesh.Add(appearance_accel_mesh[c]);			
				_appearance_file.Add(appearance_file[c]);
			}			
			appearance_class.ReleaseAll();
			appearance_accel_mesh.ReleaseAll();
			appearance_file.ReleaseAll();
			for (c=0;c<_appearance_class.TotalItems();c++)			
			{

				appearance_class.Add(_appearance_class[indexes[c]]);
			//	cout << transition_class[c] <<endl;				
				appearance_accel_mesh.Add(_appearance_accel_mesh[indexes[c]]);			
				appearance_file.Add(_appearance_file[indexes[c]]);
			}			
			//exit(0);
						free(classes);
			free(indexes);
	    }
/*
    BZK_Vector<int> collision_class; // class
    BZK_Vector<int> collided_class; // class
    BZK_Vector<int> collision_newclass; //9999    
*/

 {
   			//cout << "class transission" <<endl;
	    		int c;

			    BZK_Vector<int> _collision_class; // class
			    BZK_Vector<int> _collided_class; // 9999 (same applies here)
			    BZK_Vector<int> _collision_newclass; // 9999 (same applies here)
    	    		int *classes=(int*)malloc(collision_class.TotalItems()*sizeof(int));
    	    		int *indexes=(int*)malloc(collision_class.TotalItems()*sizeof(int));
			for (c=0;c<collision_class.TotalItems();c++)			
			{
			//	cout << transition_class[c] <<endl;
			    classes[c]=collision_class[c];
			    indexes[c]=c;
			}
			BZK_FastMath::Order(&classes,&indexes,collision_class.TotalItems());
			//cout << "order..."<<endl;
			for (c=0;c<collision_class.TotalItems();c++)			
			{
				_collision_class.Add(collision_class[c]);
				_collided_class.Add(collided_class[c]);			
				_collision_newclass.Add(collision_newclass[c]);
			}			
			collision_class.ReleaseAll();
			collided_class.ReleaseAll();
			collision_newclass.ReleaseAll();
			for (c=0;c<_collision_class.TotalItems();c++)			
			{

				collision_class.Add(_collision_class[indexes[c]]);
			//	cout << transition_class[c] <<endl;				
				collided_class.Add(_collided_class[indexes[c]]);			
				collision_newclass.Add(_collision_newclass[indexes[c]]);
			}			
			//exit(0);
						free(classes);
			free(indexes);
	    }

/*
    BZK_Vector<int> sensor_myclass; // class
    BZK_Vector<int> sensor_direction; // class
    BZK_Vector<int> sensor_senseclass; // class
    BZK_Vector<int> sensor_newclass; //9999    
*/
 {
	    		int c;

			    BZK_Vector<int> _sensor_myclass; // class
			    BZK_Vector<int> _sensor_direction; // 9999 (same applies here)
			    BZK_Vector<int> _sensor_senseclass; // 9999 (same applies here)     
			    BZK_Vector<int> _sensor_newclass; // 9999 (same applies here)
    	    		int *classes=(int*)malloc(sensor_myclass.TotalItems()*sizeof(int));
    	    		int *indexes=(int*)malloc(sensor_myclass.TotalItems()*sizeof(int));
			for (c=0;c<sensor_myclass.TotalItems();c++)			
			{
			//	cout << triggered_class[c] <<endl;
			    classes[c]=sensor_myclass[c];
			    indexes[c]=c;
			}
			BZK_FastMath::Order(&classes,&indexes,sensor_myclass.TotalItems());
			//cout << "order..."<<endl;
			for (c=0;c<sensor_myclass.TotalItems();c++)			
			{
				_sensor_myclass.Add(sensor_myclass[c]);
				_sensor_direction.Add(sensor_direction[c]);			
				_sensor_senseclass.Add(sensor_senseclass[c]);			
				_sensor_newclass.Add(sensor_newclass[c]);
			}			
			sensor_myclass.ReleaseAll();
			sensor_direction.ReleaseAll();
			sensor_senseclass.ReleaseAll();
			sensor_newclass.ReleaseAll();
			for (c=0;c<_sensor_myclass.TotalItems();c++)			
			{

				sensor_myclass.Add(_sensor_myclass[indexes[c]]);
			//	cout << triggered_class[c] <<endl;				
				sensor_direction.Add(_sensor_direction[indexes[c]]);			
				sensor_senseclass.Add(_sensor_senseclass[indexes[c]]);			
				sensor_newclass.Add(_sensor_newclass[indexes[c]]);
			}	
						free(classes);
			free(indexes);		
			
	    }
/*
    BZK_Vector<int> sound_class;    
    BZK_Vector<int> sound_action;    
    BZK_Vector<BZK_Chunk*> sound_chunk;
    */
    
 {
   			//cout << "class transission" <<endl;
	    		int c;

			    BZK_Vector<int> _sound_class; // class
			    BZK_Vector<int> _sound_action; // 9999 (same applies here)
			    BZK_Vector<BZK_Chunk*> _sound_chunk; // 9999 (same applies here)
    	    		int *classes=(int*)malloc(sound_class.TotalItems()*sizeof(int));
    	    		int *indexes=(int*)malloc(sound_class.TotalItems()*sizeof(int));
			for (c=0;c<sound_class.TotalItems();c++)			
			{
			//	cout << transition_class[c] <<endl;
			    classes[c]=sound_class[c];
			    indexes[c]=c;
			}
			BZK_FastMath::Order(&classes,&indexes,sound_class.TotalItems());
			//cout << "order..."<<endl;
			for (c=0;c<sound_class.TotalItems();c++)			
			{
				_sound_class.Add(sound_class[c]);
				_sound_action.Add(sound_action[c]);			
				_sound_chunk.Add(sound_chunk[c]);
			}			
			sound_class.ReleaseAll();
			sound_action.ReleaseAll();
			sound_chunk.ReleaseAll();
			for (c=0;c<_sound_class.TotalItems();c++)			
			{

				sound_class.Add(_sound_class[indexes[c]]);
			//	cout << transition_class[c] <<endl;				
				sound_action.Add(_sound_action[indexes[c]]);			
				sound_chunk.Add(_sound_chunk[indexes[c]]);
			}
			free(classes);
			free(indexes);			
			//exit(0);
	    }
    }

};
