//vetor tridimensional

class VAC_VecFonts {
public:
	VAC_Graphic iChar[256];

	VAC_VecFonts() {
		string path;
		string extension = ".svg";
		for (int c = 'a'; c <= 'z'; c++) {
			path = STOREPATH;
			path += "common/";
			path += (char) c;
			iChar[c].Internalize(path + extension);
		}
	}

	VAC_Graphic *WriteString(string str, int x, int y, string aForceColor = "#000000") {
		str = BZK_ParsingTools::toLowerCase(str);
		VAC_Graphic *tmpchar;
		VAC_Graphic backup;
		VAC_Shape *tmppath;
		VAC_Shape *tmppath2;
		BZK_Vec2f pos;
		VAC_Graphic *finalbuffer = new VAC_Graphic();
//		cout << "escreva:"<<endl;
		pos.SetY(BZK_FastMath::Int32toFix32(y));
		pos.SetX(BZK_FastMath::Int32toFix32(x));
		for (int c = 0; c < str.length(); c++) {
			tmpchar = iChar[str[c]].Clone();
			//cout << str[c] << endl;
			pos.SetX(pos.GetX() + BZK_FastMath::Int32toFix32(16));
			if (tmpchar == NULL) continue;
			//while (!tmpchar->Empty())
			int qtd = tmpchar->TotalItems();
			for (int d = 0; d < qtd; d++) {
//				cout << "path" <<endl;
				tmppath = (*tmpchar)[d];//->Pop();
				//	backup.Push(tmppath);
				tmppath2 = tmppath->Clone();
				tmppath2->iFill.DecodeXMLColor(aForceColor);
				tmppath2->Translate(pos);
				finalbuffer->Push(tmppath2);
			}

			//while (!backup.Empty())
			//	tmpchar->Push(backup.Pop());	
		}
		return finalbuffer;
	}
};
