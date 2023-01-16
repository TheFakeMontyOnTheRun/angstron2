class BZK_TypeSet {

	BZK_Bitmap *typeset;
	BZK_LightRect rect, rect2;
	BZK_Uint32 iColorkey;
public:


	BZK_TypeSet(char *aBitmapFile) {
		typeset = BZK_LoadBitmap(aBitmapFile);
		iColorkey = *((BZK_Uint32 *) typeset->pixels);
		BZK_SetTransparentColor(typeset, iColorkey);
		rect.y = 0;
		rect.w = 13;//(int)((typeset->w/13)/1.09);
		rect.h = 20;//typeset->h;
		rect2.w = 13;//(int)((typeset->w/13)/1.09);
		rect2.h = typeset->h;
	}

	~BZK_TypeSet() {
		BZK_UnloadBitmap(typeset);
	}

	void BlitLetter(BZK_Bitmap *aDst, char letter, int x, int y, int carry) {
		if (letter == ' ') return;
		letter -= '!';
		rect.x = (letter * rect.w) - 2;//+letter;
		rect2.y = y;
		rect2.x = x + (rect.w * carry);
		BZK_BitBlockTransfer(typeset, &rect, aDst, &rect2);
	}

	void BlitString(BZK_Bitmap *aDst, char *aStr, int x, int y) {
		int c = 0;
		for (c = 0; c < strlen(aStr); c++)
			BlitLetter(aDst, aStr[c], x, y, c);
	}
};
