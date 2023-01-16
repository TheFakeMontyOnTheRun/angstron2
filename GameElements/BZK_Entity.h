//implementa uma entidade, com toda sua relação de parentesco, localidade e ações.
class BZK_Entity {
private:
	BZK_Queue<int> iEventQueue; //saida
	int iDropTrigger;
	int iCurrentSector; //setor onde ele esta
	std::string iName;
	std::string iDescription;
	int iID; //to pensando em trocar isso por uma std::string
public:
	//--------------------------------------------------------------------------------------------
	void SetDropTrigger(int aTrigger) {
		iDropTrigger = aTrigger;
	}

	int DropTrigger() {
		return iDropTrigger;
	}

//--------------------------------------------------------------------------------------------
	bool NothingLeftToDo() {
		return iEventQueue.Empty();
	}

	void NotifyEndOfCicle() {
		iEventQueue.EraseAll();
	}

	void Undo() {
	}

	virtual bool Action(int aEvent) {
		iDropTrigger = TRIG_NOP;
	}

	void PushEvents(BZK_Vector<int> aTriggers) {
		int c;
		for (c = 0; c < aTriggers.TotalItems(); c++) {
			PushEvent(aTriggers[c]);
		}
	}

	bool
	PushEvent(int aEvent) {
		iEventQueue.Push(aEvent);
		return true; //for now, we go TRUE.
	}

//--------------------------------------------------------------------------------------------
	BZK_Entity() : iCurrentSector(0), iID(0), iName("") {
//	while (!iEventQueue.Empty()) iEventQueue.Pop(); ///?
//      iCurrentSector=0;
		//    iID=0;
		//  iName="";
	}

	~BZK_Entity() {
		//
//	while (!iEventQueue.Empty()) iEventQueue.Pop();
		//iEventQueue.ReleaseAll();//EraseAll();
	}

	std::string GetDescription() { return iDescription; }

	void SetDescription(std::string aDescription) { iDescription = aDescription; }

	std::string GetName() { return iName; }

	void SetName(std::string aName) { iName = aName; }

	int GetID() { return iID; }

	void SetID(int aID) { iID = aID; }

	int GetCurrentSector() { return iCurrentSector; }

	void SetCurrentSector(int aNewSector) { iCurrentSector = aNewSector; }
};
