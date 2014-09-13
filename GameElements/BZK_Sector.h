//aqui, pode-se pensar que isso representa uma região 3D.isso é um engano. isso
//é apenas uma interface pra casos onde temos mapas por tiles com mapas feitos
//em 3D.Afinal, um mapa em 3D pode ser feito por vários setores contendo pontos
//assim como um mapa de tiles pode ser um unico setor contendo uma matriz.
//em ambos os casos,mandamos desenhar o setor.e de acordo com o tipo dele, ele
// é desenhado a sua maneira

//abstract representation of space. should never be used directly ,as the =0 
//states
class BZK_Sector //:public BZK_Entity
{
 public:
 
 std::string Link;
 std::string iName;
 int iID; //to pensando em trocar isso por uma string
 int iTrigger;


 virtual  void NotifyEndOfCicle()=0;
 virtual  bool HasBeenDrawn()=0;
 
  std::string GetName(){return iName;}
  void SetName(std::string aName){iName=aName;}
  int GetID(){return iID;}
  void SetID(int aID) {iID=aID;}
 
  ~BZK_Sector(){}
  
  BZK_Sector():Link("")
   {
     Link="";
   }

  int GetTrigger()
    {
      return iTrigger;
    }
  void SetTrigger(int aCandidate)
    {
      iTrigger=aCandidate;
    }
};
