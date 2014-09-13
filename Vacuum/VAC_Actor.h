///@brief 3D actor with full game logic
class VAC_Actor : public BZK_Actor {

//actor logic fields
    int iDropTrigger;  
    BZK_Sint32 iClass;
    bool iAlive;
    int iTrigger;
    float iTurnStep;
//actor appearance fields 
    VAC_Graphic *iBillBoard;
    VAC_PolyBuffer *iCachedBillBoard;
    int iFlashlightCandelas;
    int iCandelas;

    VAC_PolyBuffer *iAppearance;
    VAC_PolyBuffer *iShadow;
//actor position and volume
    int iDirection;
    BZK_FixP iAngle;
    BZK_Vec3f iLastValidPosition;
    BZK_VolumetricSector iBoundingBox;
    BZK_Vec3f iPosition;
    BZK_FixP doispi;
	bool iHasBeenDrawn;

public:
    int iLastAction;	
	bool iMoved;
    	int iAppearanceCache;
	void Drawing()
	{
	iHasBeenDrawn=true;
	}

	bool HasBeenDrawn()
	{
	return iHasBeenDrawn;
	}

	void NotifyEndOfCycle()
	{
	iHasBeenDrawn=false;
	}

    void SetTurnStep(float aStep) {
        iTurnStep = aStep / 2.0;
    }


    void SetFlashlightCandelas(int aCandelas) {
        if (aCandelas > 127) aCandelas = 127; //protect against overflow
        iFlashlightCandelas = aCandelas;
    }

    int GetFlashlightCandelas() {
        return iFlashlightCandelas;
    }
    void SetCandelas(int aCandelas) {
        if (aCandelas > 127) aCandelas = 127; //protect against overflow
        iCandelas = aCandelas;
    }

    int GetCandelas() {
        return iCandelas;
    }

 
	
	int GetTrigger()
	{
	return iTrigger;
	}

	void SetTrigger(int aTrigger)
	{
	iTrigger=aTrigger;
	}

    void SetBillBoard(VAC_Graphic* aBillBoard) {
        iBillBoard = aBillBoard;
        delete iCachedBillBoard;
        iCachedBillBoard = NULL;
    }

    VAC_Graphic* GetBillBoard() {
        return iBillBoard;
    }

    void SetShadow(VAC_PolyBuffer* aShadow) {
        iShadow = aShadow;
    }

    VAC_PolyBuffer* GetShadow() {
        return iShadow;
    }

    void SetAppearance(VAC_PolyBuffer* aAppearance) {
        iAppearance = aAppearance;
	if (aAppearance==NULL)
		iShadow=NULL; //probavly leaks
    }

    VAC_PolyBuffer* GetAppearance() {
        return iAppearance;
    }

    void SetBoundingBox(BZK_VolumetricSector *aSector) {
        memcpy(&iBoundingBox, aSector, sizeof (BZK_VolumetricSector));
    }

    BZK_VolumetricSector *GetBoundingBox() {
        return iBoundingBox.Clone();
    }

    VAC_PolyBuffer *GetCachedBillBoard() {
        if (iCachedBillBoard == NULL  && iBillBoard!=NULL) 
            iCachedBillBoard = iBillBoard->toPolyBuffer();
        return iCachedBillBoard;
    }

    void SetAlive(bool aAlive) {
	cout << "set alive:"<<aAlive<<endl;
        iAlive = aAlive;
    }

    bool GetAlive() {
        return iAlive;
    }

    BZK_Sint32 GetClass() {
        return iClass;
    }

    void SetClass(BZK_Sint32 aClass) {
        iClass = aClass;
    }

    void SetPosition(BZK_Vec3f aVec) {
        BZK_VecOps::Copy(iPosition, aVec);
    }

    BZK_Vec3f *GetPosition() {
        return &iPosition;
    }

    int GetDropTrigger() {
        return iDropTrigger;
    }

    void SetDropTrigger(int aDropTrigger) {
        iDropTrigger = aDropTrigger;
    }

    BZK_Actor *Base() {
        return (BZK_Actor*)this;
    }

    ~VAC_Actor() {
        delete iShadow;
        delete iAppearance;
        delete iBillBoard;
        delete iCachedBillBoard;
    }


    virtual void BeHere(BZK_VolumetricSector &aPlace) {

        iPosition.SetX(aPlace.center().GetX());
        iPosition.SetY(aPlace.center().GetY());
        iPosition.SetZ(aPlace.center().GetZ());
        iPosition.SetSpace(BSC);
        SetCurrentSector(aPlace.GetID());
    }

    virtual void Update() {

        while (iAngle < 0) iAngle = doispi + iAngle;
        while (iAngle >= doispi) iAngle -= doispi;
        while (iDirection < 0) iDirection += 4;
        while (iDirection >= 4) iDirection -= 4;
	SetState(TRIG_NOP);
    }
    //------------------------------------------------------

    VAC_Actor() : BZK_Actor(0) ,
	doispi (BZK_FastMath::Int32toFix32(360)),
    	iAppearanceCache(0),
	iHasBeenDrawn(false),
        iAppearance(NULL),
        iBillBoard(NULL),
        iCachedBillBoard(NULL),
        iShadow(NULL),
        iLastAction(TRIG_NOP),
        iCandelas(0),
	iFlashlightCandelas(0),
        iClass(0),
        iDropTrigger(TRIG_NOP),
        iAngle(BZK_FastMath::Real32toFix32(0)),        
        iAlive(true),
        iDirection(0),
	iMoved(true),
        iPosition(0,0,0)
	{
//        Init();
	SetCurrentSector(0);
    }

    VAC_Actor(BZK_Vec3f aStartPoint, int aStartLocation) : BZK_Actor(0)
    {
        Init(aStartPoint, aStartLocation);
        iTurnStep = 90.0;
    }

    void Init() {
        Init(BZK_Vec3f(0, 0, 0), 0);
    }

    void Init(BZK_Vec3f aStartPoint, int aStartLocation) {
	doispi = BZK_FastMath::Int32toFix32(360);    
    	iAppearanceCache=0;
	iHasBeenDrawn=false;
        iAppearance = NULL;
	iMoved=true;
        iBillBoard = NULL;
        iCachedBillBoard = NULL;
        iShadow = NULL;
        iLastAction=TRIG_NOP;
        iCandelas = 0;
	iFlashlightCandelas=0;
        iClass = 0;
        SetDropTrigger(TRIG_NOP);
        iAngle = BZK_FastMath::Real32toFix32(0);
        BZK_VecOps::Copy(*GetPosition(), aStartPoint);
        SetCurrentSector(aStartLocation);
        iAlive = true;
        iDirection = 0;
    }

    bool
    PushEvent(int aEvent) {	
	Action(aEvent);
      return true;
    }

    bool
    PushEvent(int aEvent, int aParm) {
        while (aParm--)
            Action(aEvent);
	return true;
    }

    virtual void
    Undo() {
        BZK_Actor::Undo();
        memcpy(&iPosition,&iLastValidPosition,sizeof (BZK_Vec3f));
    }

    void BackupPosition() {
        memcpy(&iLastValidPosition,&iPosition,sizeof (BZK_Vec3f));
    }

public:

    virtual bool
    Action(int aAction) {
        return Action(aAction, 1);
    }

    virtual bool
    Action(int aAction, int parm) {	
    	iLastAction=aAction;
        BZK_Actor::Action(aAction);
        SetDropTrigger(aAction);
        float angle = BZK_FastMath::Fix32toReal32(iAngle)*(3.1415925727 / 180.0); //frente e tras
        BZK_FixP Cos = BZK_FastMath::Real32toFix32(cos(angle));
        BZK_FixP Sin = BZK_FastMath::Real32toFix32(sin(angle));
        angle += (3.1415925727 / 2); //strafe
        BZK_FixP Cos90 = BZK_FastMath::Real32toFix32(cos(angle));
        BZK_FixP Sin90 = BZK_FastMath::Real32toFix32(sin(angle));
	if (aAction<=TRIG_MAX)
		SetState(aAction);



        switch (aAction) {

	    case TRIG_COLLIDE:
		iMoved=true;
		Undo();
		break;

            case ACT_MOVE_N:
		iMoved=true;
                BackupPosition();
                GetPosition()->SetX(GetPosition()->GetX() + Sin);
                GetPosition()->SetY(GetPosition()->GetY() - Cos);

                break;

            case ACT_MOVE_S:
		iMoved=true;
                BackupPosition();
                GetPosition()->SetX(GetPosition()->GetX() - Sin);
                GetPosition()->SetY(GetPosition()->GetY() + Cos);
                break;

            case ACT_MOVE_W:
		iMoved=true;
                BackupPosition();
                GetPosition()->SetX(GetPosition()->GetX() - Sin90);
                GetPosition()->SetY(GetPosition()->GetY() + Cos90);
                break;

            case ACT_MOVE_E:
		iMoved=true;
                BackupPosition();
                GetPosition()->SetX(GetPosition()->GetX() + Sin90);
                GetPosition()->SetY(GetPosition()->GetY() - Cos90);
                break;

            case TRIG_VANISH:
                SetAlive(false);
                break;
            case ACT_MOVE_UP:
		iMoved=true;
                BackupPosition();
                GetPosition()->SetZ(GetPosition()->GetZ() + BZK_FastMath::Int32toFix32(1));
                break;

            case ACT_MOVE_DOWN:
		iMoved=true;
                BackupPosition();
                GetPosition()->SetZ(GetPosition()->GetZ() - BZK_FastMath::Int32toFix32(1));
                break;


           case ACT_TURN_L:
                iAngle -= BZK_FastMath::Real32toFix32(iTurnStep);
		if (iAngle<0)
			iAngle += BZK_FastMath::Int32toFix32(360);
                iDirection = BZK_FastMath::GetDirection(iAngle);
                break;

            case ACT_TURN_R:
                iAngle += BZK_FastMath::Real32toFix32(iTurnStep);
		if (iAngle>BZK_FastMath::Int32toFix32(360))
			iAngle -= BZK_FastMath::Int32toFix32(360);
                iDirection = BZK_FastMath::GetDirection(iAngle);
                break;
        };
        /*
//        not used yet
        iBoundingBox.Rect()->SetX0(GetPosition()->GetX());
        iBoundingBox.Rect()->SetY0(GetPosition()->GetY());
        iBoundingBox.Rect()->SetX1(GetPosition()->GetX() + BZK_FastMath::Real32toFix32(2.0));
        iBoundingBox.Rect()->SetY1(GetPosition()->GetY() + BZK_FastMath::Real32toFix32(2.0));
        iBoundingBox.SetH1(GetPosition()->GetZ());
        iBoundingBox.SetH1(GetPosition()->GetZ()-BZK_FastMath::Real32toFix32(2.0));
	*/
        return true;
    }

    void SetAngle(BZK_FixP aAngle) {
        iAngle = aAngle;
    };

    BZK_FixP GetAngle() {
        return iAngle;
    }


};
