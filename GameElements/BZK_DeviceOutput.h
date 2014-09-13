///@brief abstract output device. Same comments from BZK_DeviceInput applies here.
///@note in fact. This class looks TOO SIMILAR to BZK_DeviceInput!!!
class BZK_DeviceOutput
{
 public:
  virtual char *DeviceName(){return "Abstract Output Device";}
  virtual bool SignalReady(){return false;}
  virtual bool FilterEvents(BZK_SystemEvent *aEvent){return true;}
};
