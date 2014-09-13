/**
@brief class for general purpose parsing.
@author Daniel Monteiro and Carlos Muniz

*/

namespace BZK_ParsingTools
{
///@brief trim token - eliminate whitespaces from the beggining and the end of a string,BUT NOT IN THE MIDDLE
static
std::string
TrimToken(std::string Father)
{
   if (((Father)!="")&&((Father)!=" "))
    {
      int pos1=0;
      int pos2=Father.length();
      while ((Father[pos1]==' ')&&(pos1!=(Father.length()))) pos1++;
      while ((Father[pos2]==' ')&&(pos2!=pos1)) pos2--;
      return Father.substr(pos1,pos2-pos1); //mexi nisso...e deu certo  delete newstring;
    }
   else
     return "";
  
}
 ///@brief test std::string for a substring, returning true or false.
  static bool Contains(std::string aSource,std::string aSub)
  {
    int sizesub=aSub.length();
    for (int c=0;c<aSource.length()-sizesub+1;c++)
	if (aSub==aSource.substr(c,sizesub))
	  return true;
    return false;
  }
///@brief returns the position of a given character in a string
static
int
  GetTokenDivider(std::string Father,std::string Separator)
{
  int pos1=0;

     while ((Father.substr(pos1,1)!=Separator)&&(pos1!=Father.length())) pos1++;
     return pos1;
}

///deprecated
static
bool 
IsNumeric(std::string *Token)
{   
  char**endptr=(char**)malloc(1);
  bool logical;
  int number=strtol(Token->c_str(),endptr,10);
  logical=!((!strcmp(Token->c_str(),*endptr))&&(number==0));
  delete endptr;
  return logical;
}

///@brief extracts the number contained in a std::string and returns as a math unit
static
BZK_FixP
GetNumber(std::string Token)
{
  if (Token==""||Token==" ") return 0;

	float number;
	std::istringstream ss(Token);
	ss >> number;
  return BZK_FastMath::Real32toFix32(number);
}

///@deprecated
///@brief should be deleted. Use BZK_XMLReader instead
static
bool 
IsTag(std::string *Token)
{
  std::string symbol("<");
  std::string symbol2(">");

  if ((Token->substr(0,1)==symbol)&&(Token->substr(Token->length()-1,1)==symbol2))
    return true;
  return false;
}

///@deprecated
///@brief should be deleted. Use BZK_XMLReader instead

static
bool 
IsCloseTag(std::string *Token)
{
  std::string symbol3("/");

       if (Token->substr(1,1)==symbol3)
	 return true;
 return false;
}



///@brief retrieve the next token from a file stream
static
std::string
GetNextToken(std::ifstream *Datafile)
{
  // if (Datafile.eof()) throw ERR_END_OF_FILE
  //  std::string *Token=new std::string();
  std::string Token;
  *Datafile >> Token;
  return Token;
}

///@brief get the substring of a string, all the way before a separator
static
std::string
GetStringUntilSep(std::string aString,std::string aSeparator)
{
return aString.substr(0,GetTokenDivider(aString,aSeparator));
}

///@brief get the substring of a string, all the way after a separator
static
std::string
GetStringFromSep(std::string aString,std::string aSeparator)
{
  //+1 to skip the separator
return aString.substr(GetTokenDivider(aString,aSeparator)+1,aString.length());
}


///@brief get the n-esimal substring from a bigger string, separated by white spaces
static
std::string
GetSubToken(std::string aFather,int aWhere,std::string aDivider=" ")
{


  std::string Father=TrimToken(aFather);    
  
  

  if (Father=="") return "";

  if (aWhere==0)
    	return Father.substr(0,GetTokenDivider(Father,aDivider));
    else
    {
      	//if (GetTokenDivider(Father," ")==strlen(Father.c_str()))  return "";
      	return GetSubToken(Father.substr(GetTokenDivider(Father,aDivider),Father.length()),aWhere-1);
    }
}



///@deprecated
///@brief should be deleted. Use BZK_XMLReader instead
static
std::string
GetTagName(std::string &Father)
{
  int pos1=1;
  int pos2=Father.length()-2;
  if (IsCloseTag(&Father)) pos1=2;
  return Father.substr(pos1,pos2-pos1+1);
}


///@brief convert all the std::string to lower case
 static std::string
  toLowerCase(std::string &aOriginal)
{
  std::string newstring;
  for (int c=0;c<aOriginal.length();c++)
    if (aOriginal[c]>65 && aOriginal[c]<90 )
      newstring+=aOriginal[c]+32;
    else
      newstring+=aOriginal[c];
  return newstring;
}

///@brief convert all the std::string to upper case
 static std::string
  toHighCase(std::string &aOriginal)
{
  std::string newstring;
  for (int c=0;c<aOriginal.length();c++)
    if (aOriginal[c]>=97 && aOriginal[c]<122)
      newstring+=aOriginal[c]-32;
    else
      newstring+=aOriginal[c];
  return newstring;
}

///@brief decode a char to its hexadecimal meaning
static int
   DecodeHexStr(char aOriginal)
{
  if (aOriginal>=65)
    return aOriginal-65+10;
  else return aOriginal-48;
}
 
///@brief decode a full std::string of hexa chars
 static int
   DecodeHexStr(std::string &aOriginal)
 {
   std::string unified= toHighCase(aOriginal);
   int currentDigit;
   int power=0;
   int pos=aOriginal.length()-1;
   int result=0;
   while (pos!=-1)
     {
       currentDigit=DecodeHexStr(unified[pos])*pow(16.0,(double)power);
       power++;
       pos--;
       result+=currentDigit;
     }
   return result;
 }
 
};
