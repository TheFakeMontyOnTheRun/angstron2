///@brief ultra-lightweight-inneficient XML parser
///@author Daniel "NeoStrider" Monteiro
///this is a mess! ...but works quite well!

class BZK_XMLReader {
	BZK_DOMNode *iRoot;
	bool iFinished;
	int line, col;
public:

	BZK_XMLReader() : iFinished(false), iRoot(new BZK_DOMNode()) {
//      iFinished=false;
		//    iRoot=new  BZK_DOMNode();
		iRoot->SetName("DOCROOT");
		iRoot->SetNameSpace("XMLDoc");
		iRoot->SetContent("");
	}

	BZK_DOMNode *GetRoot() { return iRoot; }

	char GetChar(std::ifstream &aFile) {
		char head = aFile.get();
		//    if (head=='\\')      head=aFile.get();

		if (head == '\n') {
			line++;
			col = 0;
		}

		if (!Good(aFile) || head == '\n' || head == '\t' || head == '\r')
			return ' ';

		col++;
		if (head == '\\')
			//if (false)
		{
			aFile.get();
			return ' ';
		}
		return head;
	}

	void UnGet(std::ifstream &aFile) {
		aFile.unget();
	}

	bool Good(std::ifstream &aFile) {
		return aFile.good();
	}

	void JumpWhiteSpaces(std::ifstream &aFile) {
		char head = ' ';
		while (Good(aFile) && (head == ' ' || head == '\n' || head == '\t' || head == '\r'))
			head = GetChar(aFile);
		UnGet(aFile);
	}

	void JumpComment(std::ifstream &aFile) {
		char head;
		Expect(aFile, '<');
		head = Peek(aFile);
		if (head != '!')
			AlertError("! apenas valido para comentarios, quando dentro de uma tag");
		Expect(aFile, '!');
		Expect(aFile, '-');
		Expect(aFile, '-');
		head = Peek(aFile);

		while (Good(aFile)) {
			head = GetChar(aFile);
			if (head == '-')
				if (Peek(aFile) == '-') {
					Expect(aFile, '-');
					Expect(aFile, '>');
					return;
				}

		}
	}

	std::string GetCDATA(std::ifstream &aFile) {
		char head = Peek(aFile);
		std::string content = "";
		std::string cdata;
		if (head != '!')
			AlertError("! apenas valido para comentarios, quando dentro de uma tag");
		Expect(aFile, '[');
		if (GetToken(aFile) != "CDATA[")
			AlertError("CDATA");

		head = Peek(aFile);

		while (Good(aFile)) {
			head = GetChar(aFile);
			if (head == ']') {
				if (Peek(aFile) == ']') {
					Expect(aFile, ']');
					Expect(aFile, '>');
					return content;
				}
			} else
				content += head;
		}
	}

	char Peek(std::ifstream &aFile) {
		char head = GetChar(aFile);
		UnGet(aFile);
		return head;
	}


	void Expect(std::ifstream &aFile, char what) {
		char head = ' ';
		while (Good(aFile) && head != what && (head == ' ' || head == '\n' || head == '\t' || head == '\r'))
			head = GetChar(aFile);


		if (head != what)
			if (Good(aFile)) {
				char msg[25];
				sprintf(msg, "esperado %c , encontrado %c", what, head);
				AlertError(std::string(msg));
			} else
				AlertError("fim de arquivo prematuro ou arquivo problematico");

		//     if (Good(aFile))       UnGet(aFile);
		//     if (Good(aFile)) return true;
		//return false;

	}

	char ExpectReverse(std::ifstream &aFile, char what) {
		char head = ' ';
		while (Good(aFile) && head != what) {
			UnGet(aFile);
			head = GetChar(aFile);
			UnGet(aFile);
		}


		//if (Good(aFile)) UnGet(aFile);
		return head;

	}

	void AlertError(std::string aErro) {
		std::cout << "[" << line << "," << col << "]" << "erro:" << aErro << std::endl;
		exit(0);
	}


	std::string GetQuotedToken(std::ifstream &aFile) {
		////cout << "get quoted"<<endl;
		std::string token = "";
		char opener = GetChar(aFile);
		////cout << "with "<<opener<< " as opener"<<endl;
		char head = Peek(aFile);
		while (Good(aFile) && head != opener) {
			//	//cout << "token:" << token<<endl;
			head = GetChar(aFile);
			if (head != opener)
				token += head;
		}
		if (head == '"' && token.length() == 0) GetChar(aFile); //protects against ""
		////cout << "token:"<<BZK_ParsingTools::TrimToken(token)<<endl;
		////cout << "head ao sair do quoted:"<<Peek(aFile)<<endl;
		//    UnGet(aFile);
		return BZK_ParsingTools::TrimToken(token);
	}

	std::string GetToken(std::ifstream &aFile) {
		std::string token = "";
		char head = Peek(aFile);
		while (Good(aFile) && head != ':' && head != '=' && head != '>' && head != '/' && head != ' ' && head != '"' &&
			   head != '\'') {
			//	//cout << "token:" << token<<endl;
			head = GetChar(aFile);
			if (head != ':' && head != '=' && head != '>' && head != '/' && head != ' ' && head != '"' && head != '\'')
				token += head;
		}
		//    //cout << "token:"<<BZK_ParsingTools::TrimToken(token)<<endl;
		UnGet(aFile);
		return BZK_ParsingTools::TrimToken(token);
	}

	static void ParseStyle(std::string source, BZK_DOMNode &aCurrent) {
		// //cout << "get style" << endl;
		char head;
		int token;
		std::string data;
		std::string instructions;
		BZK_DOMNode *son;
		//    while (head!='>'&&head!='/'&&head!='?')
		instructions = source;
		for (int token = 0; token < instructions.length(); token++) {
			son = new BZK_DOMNode();
			data = "";
			while (token < instructions.length() && (instructions[token] != ':'))
				if (instructions[token] != ' ')
					data += instructions[token++];
				else
					token++;

			token++; //jump ':'
			////cout << "style="<<data;
			son->SetName(data);
			data = "";
			while (token < instructions.length() && instructions[token] != ';')
				if (instructions[token] != ' ')
					data += instructions[token++];
				else
					token++;
			////cout << ":"<<data<<endl;
			son->SetContent(data);
			////cout << "propriedade:"<<son->GetName()<<"->"<<son->GetContent()<<endl;
			aCurrent.Append(son);
		}

	}

	void GetProperties(std::ifstream &aFile, BZK_DOMNode &aCurrent) {
		////cout << "get properties" << endl;
		char head;
		std::string token;
		std::string ns = "XMLDoc";
		BZK_DOMNode *son;
		head = Peek(aFile);
		while (head != '>' && head != '/' && head != '?') {
			////cout << "head:"<<head<<endl;
			son = new BZK_DOMNode();

			JumpWhiteSpaces(aFile);
			token = GetToken(aFile);
			////cout << "token obtido para a prop:"<<token<<endl;
			if (Peek(aFile) == ':') {
				ns = token;
				Expect(aFile, ':');
				token = GetToken(aFile);
				////cout << "propriedade com namespace" <<endl;
			}
			son->SetName(token);
			Expect(aFile, '=');

			if (Peek(aFile) == '"' || Peek(aFile) == '\'')
				token = GetQuotedToken(aFile);
			else
				token = GetToken(aFile);
			son->SetContent(token);
			son->SetNameSpace(ns);
			son->SetType(BZK_DOMNode::PROPERTY);
			aCurrent.Append(son);
			JumpWhiteSpaces(aFile);
			//cout << "propriedade:"<<son->GetName()<<"->"<<son->GetContent()<<endl;
			head = Peek(aFile);
		}

	}

	//--------------------------------------------------------------------------------------------
	void GetContent(std::ifstream &aFile, BZK_DOMNode &aCurrent) {
		std::string content;
		BZK_DOMNode *textnode = new BZK_DOMNode();
		textnode->SetName("_text");
		textnode->SetNameSpace("XMLDoc");
		char head = ' ';
		while (Good(aFile) &&/*head!='<'*/!aCurrent.closed) {
			//	//cout << "content:\""<<content<<"\""<<endl;
			head = GetChar(aFile);
			if (head != '<')
				content += head;

			// }
			//   UnGet(aFile);
			if (head == '<') {
				textnode->SetContent(content);
				aCurrent.Append(textnode);
				// //cout << "enquanto buscando conteudo, achei tag" <<endl;
				UnGet(aFile);
				GetTag(aFile, aCurrent);
				////cout << "retornando de recursividade" <<endl;



				///prepara para iniciar um novo nó de texto.
				content = "";
				textnode = new BZK_DOMNode();
				textnode->SetName("_text");
				textnode->SetNameSpace("XMLDoc");
				head = Peek(aFile);
				////cout << "preparar pra mais uma iteração" <<endl;
			}
		}
		aCurrent.Append(textnode);
		////cout <<aCurrent.GetNameSpace() <<":" <<aCurrent.GetName() <<"->content:"<<content<<endl;
		textnode->SetContent(content);
		////cout << "trabalho finalizado por aqui" <<endl;
	}

	//-------------------------------------------------------------------------------------------
	bool GetTagClosure(std::ifstream &aFile, BZK_DOMNode &aCurrent) {
		////cout << "closetag:"<<aCurrent.GetName() <<endl;
		std::string token;
		JumpWhiteSpaces(aFile);
		Expect(aFile, '<');
		JumpWhiteSpaces(aFile);
		Expect(aFile, '/');
		JumpWhiteSpaces(aFile);

		token = GetToken(aFile);
		////cout << "gettoken:"<<token<<endl;
		if (aCurrent.GetNameSpace() != "XMLDoc") {
			////cout << "namespace nao padrao" <<endl;
			if (token != aCurrent.GetNameSpace())
				AlertError("namespace errado(" + token + " contra " + aCurrent.GetNameSpace() + ")");
			JumpWhiteSpaces(aFile);
			Expect(aFile, ':');
			token = GetToken(aFile);
			if (token != aCurrent.GetName())
				AlertError("nome errado(|" + token + "| contra |" + aCurrent.GetName() + "|)");

		}
		JumpWhiteSpaces(aFile);
		Expect(aFile, '>');
		////cout << "tag "<< aCurrent.GetName()<<" fechou bem </>" <<endl;
		aCurrent.closed = true;
		return true;
	}

	bool GetTag(std::ifstream &aFile, BZK_DOMNode &aCurrent) {
		bool special = false;
		BZK_DOMNode *newnode;
		////cout << "gettag" <<endl;
		char head = ' ';
		std::string token;
		Expect(aFile, '<');
		//now, we got a tag, or the file is not good anymore;
		if (!Good(aFile)) AlertError("this is no good");
		//ok, we got a tag:
		JumpWhiteSpaces(aFile);
		head = Peek(aFile);
		if (head == '/') {

			////cout << "expecting tag, got closure" <<endl;
			ExpectReverse(aFile, '<');
			GetTagClosure(aFile, aCurrent);
			return false;
		}

		if (head == '?') {
			////cout << "we got a special tag" <<endl;
			special = true;
		}
		if (head == '!') {
			Expect(aFile, '!');
			head = Peek(aFile);
			if (head == '-') {
				ExpectReverse(aFile, '<');
				////cout << "comentario ("<<Peek(aFile)<<")" <<endl;
				JumpComment(aFile);
			} else if (head == '[') {
				BZK_DOMNode *newnode;
				newnode = new BZK_DOMNode();
				newnode->SetName("CDATA");
				newnode->SetContent(GetCDATA(aFile));
			}
			return true;
		}
		// else
		////cout << "head de gettag:"<<head<<endl;
		///////////////////////////////////////////////////////////////////////////////////////////
		newnode = new BZK_DOMNode();
		if (special) newnode->SetType(BZK_DOMNode::SPECIAL);
		aCurrent.Append(newnode);
		////cout << "assumiu o filho" <<endl;
		if (!Good(aFile)) AlertError("this is no good"); //after the white space, nothing.
		token = GetToken(aFile);
		// //cout << "token:"<<token<<endl;;
		JumpWhiteSpaces(aFile);
		//////////////////// <token_ ??? //////////////////////////////////////
		head = Peek(aFile);
		////cout << "peek:"<<head<<endl;
		switch (head) {
			case '>':
				//    if (head=='>')
			{
				Expect(aFile, '>'); //consume
				newnode->SetName(token); //token é nome
				////cout << "fim da obtencao da <tag> diretamente:"<<newnode->GetName()<<endl;
				GetContent(aFile, *newnode);
				//GetTagClosure(aFile,*newnode);
				return true;
			}
				break;
			case '/':
				//    else
				//if (head=='/')
			{
				Expect(aFile, '/');
				newnode->SetName(token);
				////cout << "fim da obtencao da tag ,fechada na abertura:"<<token<<endl;
				Expect(aFile, '>');
				return true;
			}
				break;
			case ':':
				//else
				//if (head==':')
			{
				////cout << "tag com namespace" <<endl;

				//	      token=GetToken(aFile);
				////cout << "dados da tag-"<<token;
				newnode->SetNameSpace(token);
				Expect(aFile, ':');
				token = GetToken(aFile);
				////cout << newnode->GetNameSpace();
				newnode->SetName(token);
				// //cout <<":"<<token<<endl;
				JumpWhiteSpaces(aFile);
				head = Peek(aFile);
				////cout << "peek:"<<head<<endl;
				if (head == '>') {
					////cout << "fechei" <<endl;
					Expect(aFile, '>');
					GetContent(aFile, *newnode);
					return true;
				} else if (head == '/') {
					Expect(aFile, '/');
					////cout << "é uma tag sem filhos. Hora de catar o proximo caractere valido"<<endl;
					JumpWhiteSpaces(aFile);
					////cout << "eh hora de fechar o buteco" <<endl;
					Expect(aFile, '>');
					return true;
				} else {
					GetProperties(aFile, *newnode);
					//caso da tag que não tem conteudo, mas que tem propriedades
					if (Peek(aFile) == '/') {
						Expect(aFile, '/');
						Expect(aFile, '>');
						return true;
					}


					Expect(aFile, '>');
					GetContent(aFile, *newnode);
					return true;
				}
			}
				break;
		}
		/////////////////////////////////caso em que não há namespaces mas há propriedades///////////////////////////////////
		newnode->SetName(token);
		GetProperties(aFile, *newnode);
		JumpWhiteSpaces(aFile);
		head = Peek(aFile);
		if (head == '/') {
			////cout << "é uma tag sem conteudo,mas com propriedades" <<endl;
			Expect(aFile, '/');
			JumpWhiteSpaces(aFile);
			Expect(aFile, '>');
			return true;
		}

		if (head == '?' && special) {
			////cout << "finishing the special tag" <<endl;
			Expect(aFile, '?');
			Expect(aFile, '>');
			return true;
		}
		JumpWhiteSpaces(aFile);
		Expect(aFile, '>');
		GetContent(aFile, *newnode);
		//   GetTagClosure(aFile,*newnode);
		return true;
	}

	void Visit(BZK_DOMNode &current) {
		Visit(current, 0);
	}

	void Visit(BZK_DOMNode &current, int level) {
		//  for (int c=0;c<level;c++)
		//cout<<" ";

		//  if (current.GetName()!="_text")
		//cout << current.GetNameSpace()<<":"<<current.GetName()<<"=";
		//cout <<current.GetContent()<<endl;
		BZK_DOMNode *sibling;


		if (current.GetSon() != NULL)
			Visit(*current.GetSon(), level++);

		if (current.GetNext() != NULL)
			Visit(*current.GetNext(), level++);
	}


	void Delete(BZK_DOMNode *current) {
		if (current->GetSon() != NULL)
			Delete(current->GetSon());

		if (current->GetNext() != NULL)
			Delete(current->GetNext());
		delete current;
	}

	void Internalize(std::string aFile) {
		line = 0;
		col = 0;
		std::ifstream xmlfile(aFile.c_str());
		GetContent(xmlfile, *iRoot);
		iFinished = true;
	}

	bool IsFinished() {
		return iFinished;
	}
};
