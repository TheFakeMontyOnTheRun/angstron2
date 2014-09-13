class BZK_GraphicalElement
{
  bool iSelected;
 
 public:
  BZK_GraphicalElement *iEmbedded;
  long iType;
  BZK_GraphicalElement():iSelected(false)
    {
//      UnSetSelected();
    }

  bool Selected(){return iSelected;}
  void SetSelected(){iSelected=true;}
  void UnSetSelected(){iSelected=false;}
  BZK_Rect iBounds;
  virtual void Render()=0;
};
