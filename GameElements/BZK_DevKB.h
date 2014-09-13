///@brief abstract keyboard. Not a so bad idea after all. I map the keys I want and its states.
///Limiting the input for a multi-plataform project is generally a good idea. Creates a common-denominator
class BZK_DevKB:public BZK_DeviceInput
{
 public:
  ///@brief the keys
  bool iKey[18]; 
    SDL_Joystick *joystick;
 //----------------------------------------------------------------------------------------------
  ///@brief constructor
  BZK_DevKB()
    {
	std::cout << "construtor" <<std::endl;
    for(int i=0; i < SDL_NumJoysticks(); i++ ) 
    {
        printf("    %s\n", SDL_JoystickName(i));
    }

    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);
      ResetAll();
    }

   ~BZK_DevKB()
	{
    SDL_JoystickClose(joystick);
	}
  ///@brief give all the keys a neutral state
  void ResetAll()
  { 
      iKey[0]=0;
      iKey[1]=0;
      iKey[2]=0;
      iKey[3]=0;
      iKey[4]=0;
      iKey[5]=0;
      iKey[6]=0;
      iKey[7]=0;
      iKey[8]=0;
      iKey[9]=0;
      iKey[10]=0;
      iKey[11]=0;
      iKey[12]=0;
      iKey[13]=0;
      iKey[14]=0;
      iKey[15]=0;
      iKey[16]=0;
      iKey[17]=0;
      
    }
//----------------------------------------------------------------------------------------------
  ///@brief handle kbd events.
  virtual bool FilterEvents(BZK_SystemEvent &aEvent)
    {
	///por incrivel que pareça, para ARM, essa é a melhor forma.
      iKey[0]=0;
      iKey[1]=0;
      iKey[2]=0;
      iKey[3]=0;
      iKey[4]=0;
      iKey[5]=0;
      iKey[6]=0;
      iKey[7]=0;
      iKey[8]=0;
      iKey[9]=0;
      iKey[10]=0;
      iKey[11]=0;
      iKey[12]=0;
      iKey[13]=0;
      iKey[14]=0;
      iKey[15]=0;
      iKey[16]=0;
      iKey[17]=0;
        if (aEvent.key.type==BZK_KeyUpEvent)
	  {	  
	  if (aEvent.key.keysym.sym==BZK_KeyUP || aEvent.key.keysym.sym==SDLK_KP8 ) iKey[ACT_MOVE_N]=false;	
	  if (aEvent.key.keysym.sym==BZK_KeyRIGHT || aEvent.key.keysym.sym==SDLK_KP6) iKey[ACT_TURN_R]=false;
	  if (aEvent.key.keysym.sym==BZK_KeyDOWN || aEvent.key.keysym.sym==SDLK_KP2) iKey[ACT_MOVE_S]=false;
	  if (aEvent.key.keysym.sym==BZK_KeyLEFT || aEvent.key.keysym.sym==SDLK_KP4) iKey[ACT_TURN_L]=false;
	  if (aEvent.key.keysym.sym==BZK_KeyMinorA) iKey[ACT_MOVE_UP]=false;
	  if (aEvent.key.keysym.sym==BZK_KeyMinorZ) iKey[ACT_MOVE_DOWN]=false;
	  if (aEvent.key.keysym.sym==BZK_KeyCOMMA) iKey[ACT_MOVE_W]=false;
	  if (aEvent.key.keysym.sym==BZK_KeyPERIOD) iKey[ACT_MOVE_E]=false;
	  if (aEvent.key.keysym.sym==BZK_KeyENTER || aEvent.key.keysym.sym==SDLK_KP5) iKey[ACT_ACTION1]=false;	
	
	  ///note those SAPP_FUNCs - thats when we're at Maemo and there's ShadowLib. Kind of a glueware...
	  ///in this case, it remaps the action 2 key to the "fullscreen" of the 770 unit
#ifndef MAEMO22
	  if (aEvent.key.keysym.sym==BZK_KeyMinorQ) iKey[ACT_ACTION2]=false;
	  if (aEvent.key.keysym.sym==BZK_KeyMinorW) iKey[ACT_ACTION3]=false;
	  if (aEvent.key.keysym.sym==BZK_KeyPERIOD) iKey[ACT_ACTION4]=false;
#endif
	 
#ifdef MAEMO22	 
	  if (aEvent.key.keysym.sym==BZK_KeyF4) iKey[ACT_ACTION2]=false;
	  if (aEvent.key.keysym.sym==BZK_KeyF8) iKey[ACT_ACTION3]=false;
	  if (aEvent.key.keysym.sym==BZK_KeyF7) iKey[ACT_ACTION4]=false;

#endif
 	
	  }

      if (aEvent.key.type==BZK_KeyDownEvent)
	{
	  
	  if (aEvent.key.keysym.sym==BZK_KeyUP|| aEvent.key.keysym.sym==SDLK_KP8) iKey[ACT_MOVE_N]=true;	
	  if (aEvent.key.keysym.sym==BZK_KeyRIGHT|| aEvent.key.keysym.sym==SDLK_KP6) iKey[ACT_TURN_R]=true;
	  if (aEvent.key.keysym.sym==BZK_KeyDOWN|| aEvent.key.keysym.sym==SDLK_KP2) iKey[ACT_MOVE_S]=true;
	  if (aEvent.key.keysym.sym==BZK_KeyLEFT|| aEvent.key.keysym.sym==SDLK_KP4) iKey[ACT_TURN_L]=true;
	  if (aEvent.key.keysym.sym==BZK_KeyMinorA) iKey[ACT_MOVE_UP]=true;
	  if (aEvent.key.keysym.sym==BZK_KeyMinorZ) iKey[ACT_MOVE_DOWN]=true;
	  if (aEvent.key.keysym.sym==BZK_KeyCOMMA) iKey[ACT_MOVE_W]=true;
	  if (aEvent.key.keysym.sym==BZK_KeyPERIOD) iKey[ACT_MOVE_E]=true;
	  if (aEvent.key.keysym.sym==BZK_KeyENTER|| aEvent.key.keysym.sym==SDLK_KP5) iKey[ACT_ACTION1]=true;  

	 
#ifndef MAEMO22
	  if (aEvent.key.keysym.sym==BZK_KeyESCAPE) iKey[ACT_ACTION2]=true;
	  if (aEvent.key.keysym.sym==BZK_KeyCOMMA) iKey[ACT_ACTION3]=true;  
	  if (aEvent.key.keysym.sym==BZK_KeyPERIOD) iKey[ACT_ACTION4]=true;  
#endif
	  
#ifdef MAEMO22 
	  if (aEvent.key.keysym.sym==BZK_KeyF4) iKey[ACT_ACTION2]=true;
	  if (aEvent.key.keysym.sym==BZK_KeyF8) iKey[ACT_ACTION3]=true;  
	  if (aEvent.key.keysym.sym==BZK_KeyF7) iKey[ACT_ACTION4]=true;  
#endif	
	}

      if (aEvent.type==SDL_JOYAXISMOTION)
	    {
		if( aEvent.jaxis.axis == 0) 
		{
		 if (  aEvent.jaxis.value < -3200 )
		    iKey[ACT_TURN_L]=true;
		else
		if  (aEvent.jaxis.value > 3200 ) 
			iKey[ACT_TURN_R]=true;
		}

		if( aEvent.jaxis.axis == 1) 
		{
		 if (  aEvent.jaxis.value < -3200 )
		    iKey[ACT_MOVE_N]=true;
		else
		if  (aEvent.jaxis.value > 3200 ) 
		    iKey[ACT_MOVE_S]=true;

		}
	    }


 if (aEvent.type== SDL_JOYBUTTONDOWN)
    {
	if (aEvent.jbutton.button<4)
	iKey[ACT_ACTION1+aEvent.jbutton.button ]=true;
    }

 if (aEvent.type== SDL_JOYBUTTONUP)
    {
	if (aEvent.jbutton.button<4)
	iKey[ACT_ACTION1+aEvent.jbutton.button ]=false;
    }

      return true;
    }

  ///@brief sit waiting for keys until a key is pressed
  ///@note BUG: if the screen needs a repaint, it wont be serviced.
  static void WaitForKey()
    {
      BZK_SystemEvent respond;
      bool exit=false;
      while( !exit)
	{    
	  BZK_CheckEvents(&respond);
	  exit=(respond.type==BZK_KeyDownEvent);
	}
    }

  ///@brief wait for a key to be released.
  ///@note same bug from WaitForKey applies.
  static void WaitForUnpress()
    {
      BZK_SystemEvent respond;
      bool exit=false;
      while( !exit)
	{    
	  BZK_CheckEvents(&respond);
	  exit=(respond.type==BZK_KeyUpEvent);	 
	}
    }
//----------------------------------------------------------------------------------------------
};
