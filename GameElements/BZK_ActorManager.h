//==============================================================================================
///@brief Manager for game actors. Regular CRUD + gaming related stuff.
class BZK_ActorManager
{
  //============================================================================================
  ///@brief active player. Will receive all the input events.Not always the camera.
  BZK_Actor *iPlayer;
  ///@brief list of the actors currently in game and actors scheduled to be deleted.
  BZK_Vector<BZK_Actor*> iActors;
  
  //===========================================================================================
 public:
  //--------------------------------------------------------------------------------------------
  ///@brief constructor
  BZK_ActorManager():iPlayer(NULL)
    {
      //we start with no active player at all
//      iPlayer=NULL;
    }
  
  ~BZK_ActorManager()
    {
      //at the end, we delete it all. Thats our responsability
      EraseAll();
    }
  
  //--------------------------------------------------------------------------------------------
  ///@brief Not only add an actor to the list, but set it's ID and modes.
  int AddActor(BZK_Actor &aActor)
  {   
    BZK_Actor *ptr=&aActor;
    iActors.Add(ptr);
    aActor.SetID(iActors.TotalItems()-1);   
   // aActor.SetState(SPECTACTOR);
    std::cout << "ID "<<iActors.TotalItems()-1 <<" adicionado em "<< (ptr->GetCurrentSector())<<std::endl;
    return aActor.GetID();
  }
  
  ///@brief schedule an actor to be removed.
  ///@deprecated. 
  ///Its leaking!!!!
  int RemoveActor(int aID)
  {	
    int c;
    BZK_Actor *handler;
    int OldID;
    BZK_Queue<BZK_Actor*> tmp;
    for (c=0;c<GetActorsCount();c++)
      {
	handler=GetActorPtr(c);
	tmp.Push(handler);
      }
    
    iActors.ReleaseAll();    
    while (!tmp.Empty())
      {
	handler=tmp.Pop();
	OldID=handler->GetID();
	if (aID!=OldID)
	  {	  
	    AddActor(*handler);
	    handler->SetID(OldID);
	  }
      }
  }
  
  //-------------------------------------------------------------------------------------------
  ///@brief updates every actor in the list except for those scheduled to be deleted.
  ///@deprecated
  ///se the function body for details.
  void UpdateActors()
  {  
    int c;
    for(c=0;c<GetActorsCount();c++)
      {
	//actor scheduled to be deleted. (??) 
	if (GetActorPtr(c)->GetID()==-1) continue;
	GetActorPtr(c)->Update();	
      }
  } 
  //--------------------------------------------------------------------------------------------
  ///@brief defines a new active actor (!=camera actor)
  void SetActivePlayer(int aID)
  {
    iPlayer=iActors[aID];    
  }
  //--------------------------------------------------------------------------------------------
  
  ///@brief get the active actor (!=camera actor)
  BZK_Actor *GetActivePlayerPtr()
  {
    return iPlayer;
  }
  ///@brief retrieve pointer to a given actor. Tries to use the ID as an accelerator.
  BZK_Actor *GetActorPtr(int aID)
  {   
    BZK_Actor*handler=iActors[aID];
    //if no actor has been deleted, the ID should be compatible with the index in the list. So, lets try it!
    if (handler->GetID()==aID)
      return handler;
    ///no deal. traverse the intire list
    for (int c=0;c<GetActorsCount();c++)
      if (iActors[c]->GetID()==aID)
	return iActors[c];
    //ERROR!
    return NULL;
  }
  
  int GetActorsCount() ///this name should be changed...
  {
    return iActors.TotalItems();
  }

  ///@brief cleanup everything
  virtual void EraseAll()
  {
    iActors.EraseAll();
  }

  ///@brief just release the actors
  virtual void ReleaseAll()
  {
    iActors.ReleaseAll();
  }
};
