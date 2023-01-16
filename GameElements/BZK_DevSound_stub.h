///@brief null sound device. In case we dont have SDL_mixer available.
class Mix_Chunk;

class Mix_Music;

typedef Mix_Chunk BZK_Chunk;
typedef Mix_Music BZK_Music;

char *Mix_GetError() { return "this version of BZK was not compiled with sound support"; }

void Mix_FreeChunk(Mix_Chunk *aChunk) {}

void Mix_FreeMusic(Mix_Music *aChunk) {}

class BZK_DevSound : public BZK_DeviceOutput {
public:
	char *DeviceName() { return "NULL Music Device"; }

	bool SignalReady() { return true; }

	bool ready;


	static BZK_DevSound *iInstance;

	static BZK_DevSound *GetInstance() {
		if (BZK_DevSound::iInstance == NULL)
			BZK_DevSound::iInstance = new BZK_DevSound();
		return BZK_DevSound::iInstance;
	}


	BZK_DevSound() {
		ready = true;
	}

	~BZK_DevSound() {
	}

	void FadeOut() {
	}

	void StopMusic() {
	}

	Mix_Music *PlayMusic(std::string aFile) {
	}

	Mix_Chunk *LoadAndPlaySound(std::string aFile) {
		return NULL;
	}

	Mix_Chunk *LoadSound(std::string aFile) {
	}

	bool PlaySound(Mix_Chunk *aSound) {
		return true;
	}

	bool PlaySound(Mix_Chunk *aSound, int aChannel) {
		return true;
	}

	void StopAllSounds() {

	}

	void UnloadSound(Mix_Chunk *aSound) {

	}
};

BZK_DevSound *BZK_DevSound::iInstance = NULL;
