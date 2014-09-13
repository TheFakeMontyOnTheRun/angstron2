class BZK_View;

class BZK_PainelLink:public BZK_GraphicalElement
{
 public:
  BZK_PainelLink():iType(LINK)
    {
//      iType=LINK;
    }
  BZK_View *iDestination;
  void Render()
  {
    if (Selected()) iEmbedded->SetSelected();
    else  iEmbedded->UnSetSelected();
    std::cout << ">";
    /*
    if (Selected())
      {
    //desenha quadrado em volta do contorno do link
      }
   */
    iEmbedded->Render();
    std::cout << "<";
  }
};
