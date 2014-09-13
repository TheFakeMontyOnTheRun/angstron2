class VAC_Display:public BZK_AppUI
{
  int iGoto; 
 public:
	void SetGoto(int aGoto)
		{
		iGoto=aGoto;
		}
	int GetGoto()
		{
		return iGoto;
		}
  class VAC_DisplayLet
	{
	int iLink;
	string iTextContent;	
	VAC_Image *iGraphicalContent;
	bool iSelectable;
	bool iExec;
	public:
	~VAC_DisplayLet()
	{
		delete iGraphicalContent;
	}

	bool IsSelectable()
	{
	return iSelectable;	
	}

	bool IsExecutable()
	{
	return iExec;	
	}

	string GetTextContent()
	{return iTextContent;}

	VAC_Image *GetGraphicalContent()
	{return iGraphicalContent;}

	int GetLink()
	{
	return iLink;
	}


	VAC_DisplayLet(string aText,int aLink,VAC_Image *aGraphicalContent,bool aSelectable,bool aExec):
			iTextContent(aText),
			iLink(aLink),
			iGraphicalContent(aGraphicalContent),
			iSelectable(aSelectable),
			iExec(aExec)
		{
/*
			iTextContent=aText;
			iLink=aLink;
			iGraphicalContent=aGraphicalContent;
			iSelectable=aSelectable;
			iExec=aExec;
*/
		}
	};
  
  bool iQuit;
  bool paused;
//////////// GENERAL IO /////////////////
  VAC_DeviceIO *iDeviceIO;
///////////// RENDERING ///////////////////
  VAC_Rasterer *iRender;
  BZK_App *iParent;
  int iState;
  int iID;
  bool iRestartMusic;
  bool iPlayMusic;
  bool iPlaySounds;
  bool iNeedRedraw;
  string iTitle;
  string iMusicFile;
virtual VAC_Rasterer *GetRender()
{
return iRender;
}

 //=======================================================================================
 virtual void GotFocus()
  {
  	iNeedRedraw=true;
    ((BZK_DevKB*)iDeviceIO->STDIN)->ResetAll();
    iRender->currentalpha=0;
    //cout <<"ID "<<iID<<" ativa!" << endl;
    //cout <<"goto:"<<iGoto<<" se torna -1"<<endl;
    SetGoto(-1);
    
  }

  //---------------------------------------------------------------------------------------
  // INIT CLASS
  //---------------------------------------------------------------------------------------
  VAC_Display(string aFilename,BZK_App *aParent):iParent(aParent),
      iNeedRedraw(true),
      iRestartMusic(true),
      iMusicFile(""),
      iPlayMusic(false),
      iPlaySounds(false),
      iID(BZK_GS_NOVIEW),
      iQuit(false),
      iRender(VAC_Rasterer::GetInstance()),
       paused(false),
      iDeviceIO(NULL)
    {
/*
    	iNeedRedraw=true;
      iRestartMusic=true;
      iMusicFile="";
      iPlayMusic=false;
      iPlaySounds=false;

      iID=BZK_GS_NOVIEW;
      iQuit = false;
       //--------------
      //iDeviceIO=new VAC_DeviceIO;
      //-----
      iRender=new VAC_Rasterer();
       paused=false;
      iDeviceIO=NULL;
*/
      SetGoto(-1);
      iRender->ClearScreen();
      Splash();
      //----------

    }

  VAC_Display() {printf("error 1\n");exit(0);}//VAC_Display("",NULL);}
  bool Quit()    {return iQuit;}
  //---------------------------------------------------------------------------------------
  // SPACE-TIME CALCULUS (pretty name for such a mundane stuff, huh?)
  //---------------------------------------------------------------------------------------
  // also GAME INTERNAL EVENTS
virtual  void Tick()
    {
      if (paused) return;      
    }
  //-----------------------------------------------------------------------------
  // Process external events
  //-----------------------------------------------------------------------------

  
virtual  void FilterEvents(BZK_SystemEvent &aEvent)
    {
      iDeviceIO->FilterEvents(&aEvent);

   
    }

virtual  void  Splash()
    {
     } 

virtual  void  Cmd(string aCmd)
    {
	
     } 

  int GoTo(){return iGoto;}

  ~VAC_Display()
    {  
//	cout <<iID<<": iRender &:"<< iRender << endl;
	delete iRender;
    }
  //---------------------------------------------------------------------------------------
  // UPDATE GAME LOGIC
  //---------------------------------------------------------------------------------------
virtual  void Update()
    {   
      if (paused)
	return;
      iDeviceIO->STDOUT->Refresh();      
    }
  //---------------------------------------------------------------------------------------
  // UPDATE VISUAL
  //---------------------------------------------------------------------------------------

 virtual void Render()
  {
  
  }
  
  void SetForRedraw()
  {
  iNeedRedraw=true;
  }
  
  bool NeedRedraw()
  {
  return iNeedRedraw;
  }
  
  void NoNeedRedraw()
  {
  iNeedRedraw=false;
  }

  VAC_DeviceIO *GetIO()
	{
		return iDeviceIO;
	}

 
   //==========================================================================================
 };
