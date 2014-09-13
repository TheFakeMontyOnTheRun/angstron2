
class BZK_View:public BZK_GraphicalElement
{

  ///temp
 public:
  ////////
  BZK_Vector<BZK_GraphicalElement*> iElements;
 public:
  //  virtual void Render(BZK_Bitmap *aDst)=0;
  virtual BZK_View * ConsumeKey(long aKeyCode)=0;
  
};
