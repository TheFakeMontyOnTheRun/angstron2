///@brief sound device. Grew out of the music device. Hence the filename.
typedef Mix_Chunk BZK_Chunk;
typedef Mix_Music BZK_Music;

class BZK_DevSound:public BZK_DeviceOutput
{
 public:
  char* DeviceName(){return "Abstract Music Device";}
  bool SignalReady(){return true;}  
	bool ready;

  static BZK_DevSound*iInstance;
 
  static BZK_DevSound *GetInstance()
	{
	if (BZK_DevSound::iInstance==NULL)
		BZK_DevSound::iInstance=new BZK_DevSound();
	return BZK_DevSound::iInstance;
	} 

//----------------------------------------------------------------------------------------------
  BZK_DevSound()
    {      
	std::cout <<"sound...";
      int audio_rate = 44100;
      Uint16 audio_format = AUDIO_S16; 
      int audio_channels = 2;
      int audio_buffers = 4096;
      if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
	//printf("Unable to open audio!\n");
	std::cout << "not ok!"<<std::endl;
		ready=false;
	//	exit(1);
      }	
      else 
	{
	ready=true;
      std::cout << "ok!"<<std::endl;
	}
    }

  ~BZK_DevSound()
    {if (ready)
      Mix_CloseAudio();
    }

  ///@brief starts fading the music out until silence in the music channel is achieved
  void
    FadeOut()
  {
    Mix_FadeOutMusic(200);
  }

///@brief stops the music instantly.  
  void
   StopMusic()
    {
      Mix_HaltMusic();
    }

 ///@brief plays a music file (given in absolute path. not relative to the stoage area)
  Mix_Music* PlayMusic(std::string aFile)
    {
	if (!ready) return NULL;
      //std::cout << "trying to play "<<aFile<<std::endl;
      Mix_Music *music = Mix_LoadMUS(aFile.c_str());
	//std::cout << Mix_GetError() <<std::endl;
      if (Mix_PlayMusic(music, -1)==-1)
	{
	  //std::cout << "couldnt start music!"<<std::endl;
	//std::cout << Mix_GetError() <<std::endl;
	}
      //  iCurrentMusic=music;
      return music;
    }

  Mix_Chunk* LoadAndPlaySound(std::string aFile)
  {
	if (!ready) return NULL;
    Mix_Chunk *sound = LoadSound(aFile);
    PlaySound(sound);
    return sound;
  }

   Mix_Chunk* LoadSound(std::string aFile)
  { 
    return  Mix_LoadWAV(aFile.c_str());  
  }

   bool PlaySound(Mix_Chunk *aSound)
  {
    return PlaySound(aSound,-1);
  }

   bool PlaySound(Mix_Chunk *aSound,int aChannel)
  {
  	if (!Mix_Playing(aChannel))
	    return  (Mix_PlayChannel(aChannel, aSound, 0)!=-1);
	else
	return false;
  }
  
	void StopAllSounds()
	{
	Mix_HaltChannel(-1);
	}
 
 void UnloadSound(Mix_Chunk *aSound)
{
Mix_FreeChunk(aSound);
}
};  

BZK_DevSound *BZK_DevSound::iInstance=NULL;
