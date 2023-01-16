class VAC_ColorRGBA : public BZK_Color4f {
public:


	void GetColor(ifstream &origin) {
		float temp;
		origin >> temp;
		SetR(BZK_FastMath::Real32toFix32(temp));
		origin >> temp;
		SetG(BZK_FastMath::Real32toFix32(temp));
		origin >> temp;
		SetB(BZK_FastMath::Real32toFix32(temp));
		origin >> temp;
		SetA(BZK_FastMath::Real32toFix32(temp));
	}

	int r() {
		return BZK_FastMath::Fix32toInt32(GetC0());
	}

	int g() {
		return BZK_FastMath::Fix32toInt32(GetC1());
	}

	int b() {
		return BZK_FastMath::Fix32toInt32(GetC2());
	}

	int a() {
		return BZK_FastMath::Fix32toInt32(GetC3());
	}

	void SetR(BZK_FixP aC0) {
		SetC0(aC0);
	}


	void SetG(BZK_FixP aC1) {
		SetC1(aC1);
	}

	void SetB(BZK_FixP aC2) {
		SetC2(aC2);
	}

	void SetA(BZK_FixP aC3) {
		SetC3(aC3);
	}

	void Set(BZK_FixP aC0, BZK_FixP aC1, BZK_FixP aC2, BZK_FixP aC3) {
		SetC0(aC0);
		SetC1(aC1);
		SetC2(aC2);
		SetC3(aC3);
	}

	VAC_ColorRGBA() : BZK_Color4f(BZK_FastMath::Int32toFix32(255), BZK_FastMath::Int32toFix32(255),
								  BZK_FastMath::Int32toFix32(255), BZK_FastMath::Int32toFix32(255)) {
/*
	SetR(BZK_FastMath::Int32toFix32(255));
	SetG(BZK_FastMath::Int32toFix32(255));
	SetB(BZK_FastMath::Int32toFix32(255));
	SetA(BZK_FastMath::Int32toFix32(255));
*/
	}

	VAC_ColorRGBA(BZK_FixP aR, BZK_FixP aG, BZK_FixP aB, BZK_FixP aA) : BZK_Color4f(aR, aG, aB, aA) {}

	BZK_FixP GetR() { return GetC0(); }

	BZK_FixP GetG() { return GetC1(); }

	BZK_FixP GetB() { return GetC2(); }

	BZK_FixP GetA() { return GetC3(); }

	void SetColor(BZK_FixP aC0, BZK_FixP aC1, BZK_FixP aC2, BZK_FixP aC3) {
		SetR(aC0);
		SetG(aC1);
		SetB(aC2);
		SetA(aC3);
		//  //cout <<"color-c0:"<<aC0<<" c1:"<<aC1<<" c2:"<<aC2<<" c3:"<<aC3<<endl;
	}

	void Copy(/*const*/ VAC_ColorRGBA &aAnother) {
		SetR(aAnother.GetR());
		SetG(aAnother.GetG());
		SetB(aAnother.GetB());
		SetA(aAnother.GetA());
		//     SetA(BZK_FastMath::Real32toFix32(200.0));
	}

	void DecodeXMLColor(string aColor) {
		////cout << "decoding:"<<aColor<<endl;
		int r, g, b, a;
		string tmp;
		a = BZK_FastMath::Fix32toInt32(GetA());
		SetA(BZK_FastMath::Int32toFix32(255));
		if (aColor[0] == '#') {
			if (aColor.length() == 7) {
				tmp = aColor.substr(1, 2);
				r = BZK_ParsingTools::DecodeHexStr(tmp);
				tmp = aColor.substr(3, 2);
				g = BZK_ParsingTools::DecodeHexStr(tmp);
				tmp = aColor.substr(5, 2);
				b = BZK_ParsingTools::DecodeHexStr(tmp);

				SetR(BZK_FastMath::Int32toFix32(r));
				SetG(BZK_FastMath::Int32toFix32(g));
				SetB(BZK_FastMath::Int32toFix32(b));
				SetA(BZK_FastMath::Int32toFix32(a));

			} else if (aColor.length() == 9) {
				tmp = aColor.substr(1, 2);
				r = BZK_ParsingTools::DecodeHexStr(tmp);
				tmp = aColor.substr(3, 2);
				g = BZK_ParsingTools::DecodeHexStr(tmp);
				tmp = aColor.substr(5, 2);
				b = BZK_ParsingTools::DecodeHexStr(tmp);
				tmp = aColor.substr(7, 2);
				a = BZK_ParsingTools::DecodeHexStr(tmp);
				SetR(BZK_FastMath::Int32toFix32(r));
				SetG(BZK_FastMath::Int32toFix32(g));
				SetB(BZK_FastMath::Int32toFix32(b));
				SetA(BZK_FastMath::Int32toFix32(a));
			} else if (aColor.length() == 4) {
				tmp = aColor.substr(1, 1);
				SetR(/*BZK_FastMath::Fix32toInt32*/(BZK_ParsingTools::GetNumber(tmp)) * 25);
				tmp = aColor.substr(2, 1);
				SetG(/*BZK_FastMath::Fix32toInt32*/(BZK_ParsingTools::GetNumber(tmp)) * 25);
				tmp = aColor.substr(3, 1);
				SetB(/*BZK_FastMath::Fix32toInt32*/(BZK_ParsingTools::GetNumber(tmp)) * 25);
			}
			return;
		}
		//parse the colornames
		if (aColor == "AliceBlue") {
			DecodeXMLColor("#F0F8FF");
			return;
		}
		if (aColor == "AntiqueWhite") {
			DecodeXMLColor("#FAEBD7");
			return;
		}
		if (aColor == "Aqua") {
			DecodeXMLColor("#00FFFF");
			return;
		}
		if (aColor == "Aquamarine") {
			DecodeXMLColor("#7FFFD4");
			return;
		}
		if (aColor == "Azure") {
			DecodeXMLColor("#F0FFFF");
			return;
		}
		if (aColor == "Beige") {
			DecodeXMLColor("#F5F5DC");
			return;
		}
		if (aColor == "Bisque") {
			DecodeXMLColor("#FFE4C4");
			return;
		}
		if (aColor == "Black") {
			DecodeXMLColor("#000000");
			return;
		}
		if (aColor == "BlanchedAlmond") {
			DecodeXMLColor("#FFEBCD");
			return;
		}
		if (aColor == "Blue") {
			DecodeXMLColor("#0000FF");
			return;
		}
		if (aColor == "BlueViolet") {
			DecodeXMLColor("#8A2BE2");
			return;
		}
		if (aColor == "Brown") {
			DecodeXMLColor("#A52A2A");
			return;
		}
		if (aColor == "BurlyWood") {
			DecodeXMLColor("#DEB887");
			return;
		}
		if (aColor == "CadetBlue") {
			DecodeXMLColor("#5F9EA0");
			return;
		}
		if (aColor == "Chartreuse") {
			DecodeXMLColor("#7FFF00");
			return;
		}
		if (aColor == "Chocolate") {
			DecodeXMLColor("#D2691E");
			return;
		}
		if (aColor == "Coral") {
			DecodeXMLColor("#FF7F50");
			return;
		}
		if (aColor == "CornflowerBlue") {
			DecodeXMLColor("#6495ED");
			return;
		}
		if (aColor == "Cornsilk") {
			DecodeXMLColor("#FFF8DC");
			return;
		}
		if (aColor == "Crimson") {
			DecodeXMLColor("#DC143C");
			return;
		}
		if (aColor == "Cyan") {
			DecodeXMLColor("#00FFFF");
			return;
		}
		if (aColor == "DarkBlue") {
			DecodeXMLColor("#00008B");
			return;
		}
		if (aColor == "DarkCyan") {
			DecodeXMLColor("#008B8B");
			return;
		}
		if (aColor == "DarkGoldenRod") {
			DecodeXMLColor("#B8860B");
			return;
		}
		if (aColor == "DarkGray") {
			DecodeXMLColor("#A9A9A9");
			return;
		}
		if (aColor == "DarkGrey") {
			DecodeXMLColor("#A9A9A9");
			return;
		}
		if (aColor == "DarkGreen") {
			DecodeXMLColor("#006400");
			return;
		}
		if (aColor == "DarkKhaki") {
			DecodeXMLColor("#BDB76B");
			return;
		}
		if (aColor == "DarkMagenta") {
			DecodeXMLColor("#8B008B");
			return;
		}
		if (aColor == "DarkOliveGreen") {
			DecodeXMLColor("#556B2F");
			return;
		}
		if (aColor == "Darkorange") {
			DecodeXMLColor("#FF8C00");
			return;
		}
		if (aColor == "DarkOrchid") {
			DecodeXMLColor("#9932CC");
			return;
		}
		if (aColor == "DarkRed") {
			DecodeXMLColor("#8B0000");
			return;
		}
		if (aColor == "DarkSalmon") {
			DecodeXMLColor("#E9967A");
			return;
		}
		if (aColor == "DarkSeaGreen") {
			DecodeXMLColor("#8FBC8F");
			return;
		}
		if (aColor == "DarkSlateBlue") {
			DecodeXMLColor("#483D8B");
			return;
		}
		if (aColor == "DarkSlateGray") {
			DecodeXMLColor("#2F4F4F");
			return;
		}
		if (aColor == "DarkSlateGrey") {
			DecodeXMLColor("#2F4F4F");
			return;
		}
		if (aColor == "DarkTurquoise") {
			DecodeXMLColor("#00CED1");
			return;
		}
		if (aColor == "DarkViolet") {
			DecodeXMLColor("#9400D3");
			return;
		}
		if (aColor == "DeepPink") {
			DecodeXMLColor("#FF1493");
			return;
		}
		if (aColor == "DeepSkyBlue") {
			DecodeXMLColor("#00BFFF");
			return;
		}
		if (aColor == "DimGray") {
			DecodeXMLColor("#696969");
			return;
		}
		if (aColor == "DimGrey") {
			DecodeXMLColor("#696969");
			return;
		}
		if (aColor == "DodgerBlue") {
			DecodeXMLColor("#1E90FF");
			return;
		}
		if (aColor == "FireBrick") {
			DecodeXMLColor("#B22222");
			return;
		}
		if (aColor == "FloralWhite") {
			DecodeXMLColor("#FFFAF0");
			return;
		}
		if (aColor == "ForestGreen") {
			DecodeXMLColor("#228B22");
			return;
		}
		if (aColor == "Fuchsia") {
			DecodeXMLColor("#FF00FF");
			return;
		}
		if (aColor == "Gainsboro") {
			DecodeXMLColor("#DCDCDC");
			return;
		}
		if (aColor == "GhostWhite") {
			DecodeXMLColor("#F8F8FF");
			return;
		}
		if (aColor == "Gold") {
			DecodeXMLColor("#FFD700");
			return;
		}
		if (aColor == "GoldenRod") {
			DecodeXMLColor("#DAA520");
			return;
		}
		if (aColor == "Gray") {
			DecodeXMLColor("#808080");
			return;
		}
		if (aColor == "Grey") {
			DecodeXMLColor("#808080");
			return;
		}
		if (aColor == "Green") {
			DecodeXMLColor("#008000");
			return;
		}
		if (aColor == "GreenYellow") {
			DecodeXMLColor("#ADFF2F");
			return;
		}
		if (aColor == "HoneyDew") {
			DecodeXMLColor("#F0FFF0");
			return;
		}
		if (aColor == "HotPink") {
			DecodeXMLColor("#FF69B4");
			return;
		}
		if (aColor == "IndianRed") {
			DecodeXMLColor("#CD5C5C");
			return;
		}
		if (aColor == "Indigo") {
			DecodeXMLColor("#4B0082");
			return;
		}
		if (aColor == "Ivory") {
			DecodeXMLColor("#FFFFF0");
			return;
		}
		if (aColor == "Khaki") {
			DecodeXMLColor("#F0E68C");
			return;
		}
		if (aColor == "Lavender") {
			DecodeXMLColor("#E6E6FA");
			return;
		}
		if (aColor == "LavenderBlush") {
			DecodeXMLColor("#FFF0F5");
			return;
		}
		if (aColor == "LawnGreen") {
			DecodeXMLColor("#7CFC00");
			return;
		}
		if (aColor == "LemonChiffon") {
			DecodeXMLColor("#FFFACD");
			return;
		}
		if (aColor == "LightBlue") {
			DecodeXMLColor("#ADD8E6");
			return;
		}
		if (aColor == "LightCoral") {
			DecodeXMLColor("#F08080");
			return;
		}
		if (aColor == "LightCyan") {
			DecodeXMLColor("#E0FFFF");
			return;
		}
		if (aColor == "LightGoldenRodYellow") {
			DecodeXMLColor("#FAFAD2");
			return;
		}
		if (aColor == "LightGray") {
			DecodeXMLColor("#D3D3D3");
			return;
		}
		if (aColor == "LightGrey") {
			DecodeXMLColor("#D3D3D3");
			return;
		}
		if (aColor == "LightGreen") {
			DecodeXMLColor("#90EE90");
			return;
		}
		if (aColor == "LightPink") {
			DecodeXMLColor("#FFB6C1");
			return;
		}
		if (aColor == "LightSalmon") {
			DecodeXMLColor("#FFA07A");
			return;
		}
		if (aColor == "LightSeaGreen") {
			DecodeXMLColor("#20B2AA");
			return;
		}
		if (aColor == "LightSkyBlue") {
			DecodeXMLColor("#87CEFA");
			return;
		}
		if (aColor == "LightSlateGray") {
			DecodeXMLColor("#778899");
			return;
		}
		if (aColor == "LightSlateGrey") {
			DecodeXMLColor("#778899");
			return;
		}
		if (aColor == "LightSteelBlue") {
			DecodeXMLColor("#B0C4DE");
			return;
		}
		if (aColor == "LightYellow") {
			DecodeXMLColor("#FFFFE0");
			return;
		}
		if (aColor == "Lime") {
			DecodeXMLColor("#00FF00");
			return;
		}
		if (aColor == "LimeGreen") {
			DecodeXMLColor("#32CD32");
			return;
		}
		if (aColor == "Linen") {
			DecodeXMLColor("#FAF0E6");
			return;
		}
		if (aColor == "Magenta") {
			DecodeXMLColor("#FF00FF");
			return;
		}
		if (aColor == "Maroon") {
			DecodeXMLColor("#800000");
			return;
		}
		if (aColor == "MediumAquaMarine") {
			DecodeXMLColor("#66CDAA");
			return;
		}
		if (aColor == "MediumBlue") {
			DecodeXMLColor("#0000CD");
			return;
		}
		if (aColor == "MediumOrchid") {
			DecodeXMLColor("#BA55D3");
			return;
		}
		if (aColor == "MediumPurple") {
			DecodeXMLColor("#9370D8");
			return;
		}
		if (aColor == "MediumSeaGreen") {
			DecodeXMLColor("#3CB371");
			return;
		}
		if (aColor == "MediumSlateBlue") {
			DecodeXMLColor("#7B68EE");
			return;
		}
		if (aColor == "MediumSpringGreen") {
			DecodeXMLColor("#00FA9A");
			return;
		}
		if (aColor == "MediumTurquoise") {
			DecodeXMLColor("#48D1CC");
			return;
		}
		if (aColor == "MediumVioletRed") {
			DecodeXMLColor("#C71585");
			return;
		}
		if (aColor == "MidnightBlue") {
			DecodeXMLColor("#191970");
			return;
		}
		if (aColor == "MintCream") {
			DecodeXMLColor("#F5FFFA");
			return;
		}
		if (aColor == "MistyRose") {
			DecodeXMLColor("#FFE4E1");
			return;
		}
		if (aColor == "Moccasin") {
			DecodeXMLColor("#FFE4B5");
			return;
		}
		if (aColor == "NavajoWhite") {
			DecodeXMLColor("#FFDEAD");
			return;
		}
		if (aColor == "Navy") {
			DecodeXMLColor("#000080");
			return;
		}
		if (aColor == "OldLace") {
			DecodeXMLColor("#FDF5E6");
			return;
		}
		if (aColor == "Olive") {
			DecodeXMLColor("#808000");
			return;
		}
		if (aColor == "OliveDrab") {
			DecodeXMLColor("#6B8E23");
			return;
		}
		if (aColor == "Orange") {
			DecodeXMLColor("#FFA500");
			return;
		}
		if (aColor == "OrangeRed") {
			DecodeXMLColor("#FF4500");
			return;
		}
		if (aColor == "Orchid") {
			DecodeXMLColor("#DA70D6");
			return;
		}
		if (aColor == "PaleGoldenRod") {
			DecodeXMLColor("#EEE8AA");
			return;
		}
		if (aColor == "PaleGreen") {
			DecodeXMLColor("#98FB98");
			return;
		}
		if (aColor == "PaleTurquoise") {
			DecodeXMLColor("#AFEEEE");
			return;
		}
		if (aColor == "PaleVioletRed") {
			DecodeXMLColor("#D87093");
			return;
		}
		if (aColor == "PapayaWhip") {
			DecodeXMLColor("#FFEFD5");
			return;
		}
		if (aColor == "PeachPuff") {
			DecodeXMLColor("#FFDAB9");
			return;
		}
		if (aColor == "Peru") {
			DecodeXMLColor("#CD853F");
			return;
		}
		if (aColor == "Pink") {
			DecodeXMLColor("#FFC0CB");
			return;
		}
		if (aColor == "Plum") {
			DecodeXMLColor("#DDA0DD");
			return;
		}
		if (aColor == "PowderBlue") {
			DecodeXMLColor("#B0E0E6");
			return;
		}
		if (aColor == "Purple") {
			DecodeXMLColor("#800080");
			return;
		}
		if (aColor == "Red") {
			DecodeXMLColor("#FF0000");
			return;
		}
		if (aColor == "RosyBrown") {
			DecodeXMLColor("#BC8F8F");
			return;
		}
		if (aColor == "RoyalBlue") {
			DecodeXMLColor("#4169E1");
			return;
		}
		if (aColor == "SaddleBrown") {
			DecodeXMLColor("#8B4513");
			return;
		}
		if (aColor == "Salmon") {
			DecodeXMLColor("#FA8072");
			return;
		}
		if (aColor == "SandyBrown") {
			DecodeXMLColor("#F4A460");
			return;
		}
		if (aColor == "SeaGreen") {
			DecodeXMLColor("#2E8B57");
			return;
		}
		if (aColor == "SeaShell") {
			DecodeXMLColor("#FFF5EE");
			return;
		}
		if (aColor == "Sienna") {
			DecodeXMLColor("#A0522D");
			return;
		}
		if (aColor == "Silver") {
			DecodeXMLColor("#C0C0C0");
			return;
		}
		if (aColor == "SkyBlue") {
			DecodeXMLColor("#87CEEB");
			return;
		}
		if (aColor == "SlateBlue") {
			DecodeXMLColor("#6A5ACD");
			return;
		}
		if (aColor == "SlateGray") {
			DecodeXMLColor("#708090");
			return;
		}
		if (aColor == "SlateGrey") {
			DecodeXMLColor("#708090");
			return;
		}
		if (aColor == "Snow") {
			DecodeXMLColor("#FFFAFA");
			return;
		}
		if (aColor == "SpringGreen") {
			DecodeXMLColor("#00FF7F");
			return;
		}
		if (aColor == "SteelBlue") {
			DecodeXMLColor("#4682B4");
			return;
		}
		if (aColor == "Tan") {
			DecodeXMLColor("#D2B48C");
			return;
		}
		if (aColor == "Teal") {
			DecodeXMLColor("#008080");
			return;
		}
		if (aColor == "Thistle") {
			DecodeXMLColor("#D8BFD8");
			return;
		}
		if (aColor == "Tomato") {
			DecodeXMLColor("#FF6347");
			return;
		}
		if (aColor == "Turquoise") {
			DecodeXMLColor("#40E0D0");
			return;
		}
		if (aColor == "Violet") {
			DecodeXMLColor("#EE82EE");
			return;
		}
		if (aColor == "Wheat") {
			DecodeXMLColor("#F5DEB3");
			return;
		}
		if (aColor == "White") {
			DecodeXMLColor("#FFFFFF");
			return;
		}
		if (aColor == "WhiteSmoke") {
			DecodeXMLColor("#F5F5F5");
			return;
		}
		if (aColor == "Yellow") {
			DecodeXMLColor("#FFFF00");
			return;
		}
		if (aColor == "YellowGreen") {
			DecodeXMLColor("#9ACD32");
			return;
		};

		SetR(BZK_ParsingTools::GetNumber(BZK_ParsingTools::GetSubToken(aColor, 0)));
		SetG(BZK_ParsingTools::GetNumber(BZK_ParsingTools::GetSubToken(aColor, 1)));
		SetB(BZK_ParsingTools::GetNumber(BZK_ParsingTools::GetSubToken(aColor, 2)));
		if (BZK_ParsingTools::GetSubToken(aColor, 0) != "")
			SetA(BZK_ParsingTools::GetNumber(BZK_ParsingTools::GetSubToken(aColor, 3)));
		else
			SetA(BZK_FastMath::Int32toFix32(255));
		//cout << "cor:"<<aColor<<"="<<r<<","<<g<<","<<b<<","<<a<<endl;

	}

	void AddNoise() {
		srand(time(NULL));
		float Secret = rand();
		cout << "rand:" << (int) Secret << endl;
		BZK_FixP _C = BZK_FastMath::Real32toFix32((Secret / 100.0f) + 0.9f);
		SetR(BZK_FastMath::Mul(GetR(), _C));
		SetG(BZK_FastMath::Mul(GetG(), _C));
		SetB(BZK_FastMath::Mul(GetB(), _C));

	}


	void Darken(int level) {
		BZK_FixP doiscincocinco = BZK_FastMath::Int32toFix32(255);
		BZK_FixP _C = BZK_FastMath::Div(BZK_FastMath::Int32toFix32(level), doiscincocinco);
		SetR(BZK_FastMath::Mul(GetR(), _C));
		SetG(BZK_FastMath::Mul(GetG(), _C));
		SetB(BZK_FastMath::Mul(GetB(), _C));


		if (GetR() > doiscincocinco) SetR(doiscincocinco);
		if (GetG() > doiscincocinco) SetG(doiscincocinco);
		if (GetB() > doiscincocinco) SetB(doiscincocinco);


	}

	bool Equal(VAC_ColorRGBA &aAnother) {
		return GetR() == aAnother.GetR() &&
			   GetG() == aAnother.GetG() &&
			   GetB() == aAnother.GetB() &&
			   GetA() == aAnother.GetA();
		//     SetA(BZK_FastMath::Real32toFix32(200.0));
	}
	//essa fun��o vai ter que passar a entender os diferentes sistemas de cores...
};
