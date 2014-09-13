
//classe que gerencia entrada e saída,passando os eventos para os dispositivos.
class VAC_DeviceIO:public BZK_DeviceIO
{
  
 public:
  VAC_DeviceIO()
    {
     }
void FilterEvents(BZK_SystemEvent *aIOEvents)
  {
    
switch  (aIOEvents->type)
  {
  case SDL_JOYAXISMOTION:
  case SDL_JOYBUTTONDOWN: 
  case SDL_JOYBUTTONUP: 
  case BZK_KeyDownEvent:
  case BZK_KeyUpEvent:    
	////cout << "enviando evento de tecla pro pro teclado"<<endl;
    STDIN->FilterEvents(*aIOEvents);
    break;
  default:
    STDOUT->NeedRedraw();

  }

  }
};
