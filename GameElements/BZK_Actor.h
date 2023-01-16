//implementa uma maquina de estados em cima das entidades b�sicas.
enum {
	SPECTACTOR, WAITING, DONE
};

//==============================================================================================
///@brief An entity with a FSM layer on top of it.
class BZK_Actor : public BZK_Entity {
	///@brief maps the relationship between internal states and external states
	BZK_Vector<int> iInternalToExternal;
public:

//--------------------------------------------------------------------------------------------
	BZK_Actor(int aState) : iUserState(TRIG_NOP), iInternalState(aState), iExternalState(TRIG_NOP) {/*
      //standard user defined state is a neutral one
      iUserState=TRIG_NOP;
      //starting state.
      iInternalState=aState;
	iExternalState=TRIG_NOP;
	*/
	}
	//--------------------------------------------------------------------------------------------

	///@brief actor heartbeat
	virtual void Update() {
		///if we do have any mapping, go on with it.
//      if (iInternalToExternal.TotalItems()>0)	iExternalState=iInternalToExternal[iInternalState];


	}

///@brief handle events (BZK events)
	bool PushEvent(int aEvent) {///forwards events to the entity layer
		return BZK_Entity::PushEvent(aEvent);
	}


	//============================================================================================

public:
	///getters and setters
	int GetState() { return iInternalState; }

	void SetState(int aState) { iInternalState = aState; }

	int GetUserState() { return iUserState; }

	void SetUserState(int aState) { iUserState = aState; }

private:
	///@brief user defined state.
	int iUserState;
	///@brief current external state
	int iExternalState;
	///@brief current internal state
	int iInternalState;

};
