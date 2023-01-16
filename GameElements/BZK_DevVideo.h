
//dispositivo de video. apenas faz os servi�os relativos a bitmaps mesmo.

class BZK_DevVideo : public BZK_DeviceOutput {
public:
	char *DeviceName() { return "Abstract Video"; }

	bool SignalReady() { return (1 == 1); }

	bool iNeedRedraw;
	BZK_Bitmap *iVideo;

//----------------------------------------------------------------------------------------------
	BZK_DevVideo() {


		iVideo = BZK_InitVideo();
	}

	BZK_Bitmap *GetRawSurface() {
		return iVideo;
	}

	void NeedRedraw() {
		iNeedRedraw = true;
	}


	void
	DrawUpmostLayer() {
		Refresh();
	}

	void
	Refresh() {
		BZK_SwapBuffers(iVideo);
	}


};  
