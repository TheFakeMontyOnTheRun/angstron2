class BZK_AppUI
{
  BZK_Vector<BZK_View*> iView;
  BZK_View *iCurrentView;
 public:
  void AddView(BZK_View *aView) {iView.Add(aView);}
  BZK_View *GetCurrentView() {return iCurrentView;}
  bool SetView(int aViewNum) {iCurrentView=iView[aViewNum];}
  bool SetView(BZK_View *aView){iCurrentView=aView;}
  void ConsumeKey(long aKey)
  {
    int c=0;
    bool consumed=false;
    for (c=0;!consumed && c<iView.TotalItems();c++)
      consumed=iView[c]->ConsumeKey(aKey);
    if (consumed)
      SetView(c);    
  }

 ~BZK_AppUI()
	{
	iView.EraseAll();
	}
};
