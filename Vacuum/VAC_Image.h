class VAC_Image : public VAC_Graphic {
///@brief Encapsulates both bitmap and vector graphics (while vectors are prefered)
public:
	BZK_Bitmap *iBitmap;

	static
	VAC_Image *InternalizeFromScratch(std::string aFile) {
		VAC_Image *tmp = new VAC_Image();
		if (BZK_ParsingTools::Contains(aFile, ".svg"))
			tmp->Internalize(aFile);
		else if (BZK_ParsingTools::Contains(aFile, ".bmp"))
			tmp->iBitmap = BZK_LoadBitmap(aFile.c_str());
		else return NULL;
		return tmp;
	}

	~VAC_Image() {
		BZK_UnloadBitmap(iBitmap);
	}
};
