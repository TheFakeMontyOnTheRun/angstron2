class BZK_TextLabel:public BZK_GraphicalElement
{
 public:
  string iString;
  BZK_TextLabel():iType(LABEL)
    {
//      iType=LABEL;
    }

  void Render()
  {
    if (Selected())
      cout <<"[";
    cout << iString;
    if (Selected())
      cout <<"]";
      
  }
};
