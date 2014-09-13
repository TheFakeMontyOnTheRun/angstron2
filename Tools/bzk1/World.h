//classes de mundo

//um locus
class BZK1_PETEntry
{
public:
//código de objeto
BZK_Sint32 ObjectType;

//alturas das texturas
float HeightFloor;
float HeightCeiling;
float HeightFloorRight;
float HeightCeilingRight;
float HeightFloorLeft;
float HeightCeilingLeft;

//flags de propriedades do locus
bool BlockActor;
bool BlockFire;
bool BlockRay;
bool Used;
};

//informações sobre o mundo
class BZK1_Info
 {
public:
char        header[3];
//SDL_Surface *loadpic;
//SDL_Surface  *mapname;
//SDL_Surface *pausephrase;

char        mapmusic[50];
char        geofile[50];
char        petfile[50];
};

class BZK1_PET
{
 public:
//a PET
BZK1_PETEntry        patterns[512];
 void Load(string aFilename)
   {
     ifstream pet(aFilename.c_str());
     int c4=0;
     //nomes das texturas ..eu poderia usar um só..eu é que fui idiota de usar um para cada
     char right[15];
     char left[15];
     char floor[15];
     char ceiling[15];
     //flags
     bool TF;
     //código de objeto
     BZK_Sint32 obj;
     //alturas
     float height;
     while(c4 < 512)
       {	 
	 pet >> right;	 
	 //direita inferior	 	 	 
	 pet >> right;	 
	 //superior direita	 	
	 pet >> left;
	 //inferior esquerda	 
	 pet >> left; 
	 pet >> floor;
	 pet >> ceiling;
	 //teto	
	 //código de objeto
	 pet >> obj;
	 patterns[c4].ObjectType=obj;	 
	 //altura do chão
	 pet >> height;	 
	 patterns[c4].HeightFloor=height/10;	 
	 //altura do teto
	 pet >> height;
	 patterns[c4].HeightCeiling=(height+10)/10;	 
	 //altura da parede inferior direita
	 pet >> height;	
	 patterns[c4].HeightFloorRight=height/10;	 
	 //altura da parede superior direita
	 pet >> height;	
	 patterns[c4].HeightCeilingRight=(height+10)/10;	 
	 //altura da parede inferior esquerda
	 pet >> height;
	 patterns[c4].HeightFloorLeft=height/10;
	 //altura da parede superior esquerda
	 pet >> height;
	 patterns[c4].HeightCeilingLeft=(height+10)/10;	 
	 //flag de bloqueio de atores
	 pet >> TF;
	 patterns[c4].BlockActor=TF;	 
	 //flag de bloqueio de balas
	 pet >> TF;
	 patterns[c4].BlockFire=TF;	 
	 //flag de bloqueio de visão
	 pet >> TF;
	 patterns[c4].BlockRay=TF;
	 //flag de retraçamento de raios (portal rendering) - desativado
	 //apagada!
	 //flag usada para indicar se o locus é de fato usado.
	 pet >> TF;
	 patterns[c4].Used=TF;	 
	 c4++;	 
       }   
     //fecha arquivo de pet ..o pior ja passou
     pet.close();   
     
   }
};

class BZK1_File
{
 public:
  BZK1_File(string aFilename,BZK1_PET &aPET ,BZK1_Info &aInfo )
    {
     // SDL_Surface Temp;
      //cabeçalho   
      ifstream infile(aFilename.c_str());
      //le o cabeçalho
      infile >> aInfo.header;     
      
      //le o nome do arquivo de musica
      infile >>  aInfo.mapmusic;
      
      //arquivo de geometria
      infile >> aInfo.geofile;      
      
      //arquivo de geometria
      infile >> aInfo.petfile;

      //fecha o stream do arquivo de definições
      infile.close();
      aPET.Load(aInfo.geofile);
    }


};

