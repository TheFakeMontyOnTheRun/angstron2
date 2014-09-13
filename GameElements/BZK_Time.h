
class BZK_Time
{
    public:
  BZK_Time()
{
Reset();
}

void Reset()
    {
      iStartTime = 0;
      iThisTime = 0;
      iFPS = 0;
      iFrame = 0;
    }
  
  void Start()
    {
      //cout << "Iniciando Timer" << endl;
      iStartTime = BZK_GetSysTicks();
    }
  
  void Refresh()
    {
      iThisTime = BZK_GetSysTicks();
      iFPS = (int)((float)iFrame / (iThisTime - iStartTime) * 1000.0);
      iFrame++;
    }
  
  long FPS()
    {
      return iFPS;
    }
 private:
  long iFPS;
  long iFrame;
  long iThisTime;
  long iStartTime;
};
