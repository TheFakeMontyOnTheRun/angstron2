//o mundo, com seu setor de come�o e o setor que contem os outros

class BZK_World {
public:
	BZK_Sector *FirstSector;
	BZK_Sector *MasterSector;

//  BZK_List<BZK_VolumetricSector*> *iSector;
	virtual int GetSectorByPos(BZK_Vec3f &aVec, int aHint = 1) {
		std::cout << "incomplete class-method called" << std::endl;
		return 0;
	}
};
