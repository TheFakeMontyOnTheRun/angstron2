///@brief simple IO managed. Just video, keyboard, audio and mouse for now.
class BZK_DeviceIO {
public:
	BZK_DeviceInput *iInputDevice[1];
	BZK_DeviceOutput *iOutputDevice[1];
///@brief shortcut to video device manager
	BZK_DevVideo *STDOUT;
///@brief shortcut for keyboard device manager
	BZK_DevKB *STDIN;

//==============================================================================================
	BZK_DeviceIO() {
		////cout<<"deviceio: criando video\n";
		iOutputDevice[0] = new BZK_DevVideo(); //video
		//    iOutputDevice[1]=BZK_DevSound()::GetInstance(); //music
		////cout<<"deviceio: criando teclado\n"<<std::endl;;
		iInputDevice[0] = new BZK_DevKB(); //teclado
		//  iInputDevice[1]=new BZK_DevMouse(); //teclado

		////cout<<"deviceio: ajustando video como saida padrão\n";
		STDIN = ((BZK_DevKB *) iInputDevice[0]);
		////cout<<"deviceio: ajustando teclado como entrada padrão\n";
		STDOUT = ((BZK_DevVideo *) iOutputDevice[0]);
	}

	~BZK_DeviceIO() {
		///clean up the mess
		delete iOutputDevice[0];
		delete iInputDevice[0];

	}
};
