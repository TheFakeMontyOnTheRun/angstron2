class BZK_App
{
  BZK_Document *iCurrentDocument; 
  BZK_AppUI *iCurrentUI;
 std::string iAppName;
  int iPid;
 public:
  void SetDocument(BZK_Document *aDocument) {iCurrentDocument=aDocument;}
  BZK_Document *GetCurrentDocument(){return iCurrentDocument;}
  void SetUI(BZK_AppUI *aUI) {iCurrentUI=aUI;}
  BZK_AppUI *GetUI(){return iCurrentUI;}

void SetPid(int aPid)
{
iPid=aPid;
}

int GetPid()
{
return iPid;
}

  ///context functions
virtual int ResolveUrl(const char *aUrl)
{

}

virtual int Exec(const char *aUrl)
{

}
  
static  bool IsEmbedded()
  {
    return false; //standard answer
  }
  
static std::string GetPlataformName()
  {
    return std::string("POSIX compliant plataform"); //standard answer
  }
  
static  int GetTotalHeap() //in bytes
  {
    return 16*1024*1024; //16 MB RAM
  }
  
 std::string GetApplicationName()
  {
    return iAppName;
  }

  void SetApplicationName(std::string aName)
  {
    iAppName=aName;
  }

 static std::string GetStoragePath()
  {
    return std::string(STOREPATH);
  }
  
 std::string GetResourcesLocation()
  {
  return GetStoragePath()+GetApplicationName()+"/";
  }

 std::string GetCommonResourcesLocation()
  {
  return GetStoragePath()+"common/";
  }
  //TODO: add singleton

};
