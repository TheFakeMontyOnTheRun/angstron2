/*
statements
while
for
vetores
string?
*/

#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <stdlib.h>
using namespace std;

enum {OP_NOP,OP_ADD,OP_SUB};

enum {
	WML_FUNCTION,
	WML_EXTERN,
	WML_PRIVATE,
	WML_VAR,
	WML_PUBLIC,
	};


class funcnode
{

public:
string name;
funcnode *next;
funcnode *son;

funcnode()
{
next=NULL;
son=NULL;
name="no name";
}

};

funcnode *currentlambda;
stack<funcnode*> otherlambda;

class funcdef
{
public:
//vector<int> body;
funcnode* lambda;
int qualifiers;
string name;
int numparms;
funcdef()
{
lambda=new funcnode();
lambda->name="lambda";
currentlambda=lambda;
}
};

vector<funcdef*> funcs;
funcdef *currentfunction;

void exec(funcnode *aFunc)
{
	cout << aFunc->name << endl;
	if (aFunc->son!=NULL)
	{
	cout << "{" << endl;
		exec(aFunc->son);
	cout << "}" << endl;
	}

	if (aFunc->next!=NULL)
	{
	cout << "|" << endl;
		exec(aFunc->next);
	}


}

void exec(funcdef *aFunc)
{
	cout << aFunc->name << endl;
	cout << "{" << endl;
	exec(aFunc->lambda);
	cout << "}" << endl;
}

void getExpression(ifstream &file);
void getFuncCall(ifstream &file);
stack<int> _stack;

void do_push(int aData)
{
_stack.push(aData);
}

int do_pop()
{
int tmp=_stack.top();
_stack.pop();
return tmp;
}

void putback(string str,ifstream &file)
{
for (int c=0;c<str.length();c++)
	file.unget();
}

string Peek(ifstream &file)
{
string toReturn;
if (!file.good()) return "";
file >> toReturn;
if (toReturn!="")
	putback(toReturn,file);
return toReturn;
}


void expect(string str,ifstream &file)
{
string tmp;
file >> tmp;
if (tmp!=str)
	{
	cout << "expected:'"<<str<<"' got '"<< tmp<<"'"<< endl;
	exit(0);
	}
}

///function header
void getFunctionParmList(ifstream &file)
{
string token;
bool end=false;
file >> token;
if (token!="(")
{
cout << "synthax error: parameter list opener not found in "<<currentfunction->name << endl;
exit(0);
}

while (file.good())
{
file >> token;
if (token==")") break;
currentfunction->numparms++;
file >> token;
if (token==")") break;
if (token!=",")
	{
	cout << "expected: parameter separator" <<endl;
	exit(0);
	}
}

}

/// function call
void getParmList(ifstream &file)
{
string token;
bool end=false;
file >> token;
if (token!="(")
{
cout << "synthax error: parameter list opener not found in "<<currentfunction->name << endl;
exit(0);
}

while (file.good())
{
file >> token;
if (token==")") return;

if (Peek(file)=="(")
   {
   currentlambda->son=new funcnode();
   otherlambda.push(currentlambda);
   currentlambda=currentlambda->son;
   currentlambda->name=token;
   getParmList(file);
   }


file >> token;

if (token==")") 
{
	if (!otherlambda.empty())
	{
	currentlambda=otherlambda.top();
	otherlambda.pop();
	}
return;
}


if (token!=",")
	{
	cout << "expected: parameter separator" <<endl;
	exit(0);
	}
}

}

void getFunctionName(ifstream &file)
{
string token;
file >> token;

currentfunction->name=token;
if (token!="(")
	return;
cout << "synthax error: expected function name, but you seem to forget it" << endl;
exit(0);
}


void getFunctionHeader(ifstream &file)
{
//getFunctionQualifier(file);
expect("function",file);
getFunctionName(file);
getFunctionParmList(file);
}
////////////////////////////////////////////
void getVarsDeclaration(ifstream &file)
{ //var ja foi consumido
string token;
//----------------------------
file >> token; // "name?"
file.putback(token.length()); // retorna "name?"
if (token==";")
	{
	cout << "expected: variable identifier" <<endl;
	exit(0);
	}

while (token!=";" && file.good())
{
file >> token; // name
file >> token; // separador ou terminador
if (token!="," && token!=";")
	{
	cout << "expected: variable identifier separator or declaration terminator" <<endl;
	exit(0);
	}
file.unget(); //putback the ;	
}

}


void getStatement(ifstream &file);

void getFuncCall(ifstream &file)
{
string token;
file >> token;
currentlambda->next=new funcnode();
currentlambda->name=token;
currentlambda=currentlambda->next;
getParmList(file);	
}


void getStatement(ifstream &file)
{
string token;
file >> token;
}

void getAttrib(ifstream &file)
{
string token;
file >> token;
expect("=",file);
getFuncCall(file);
}

void getStatements(ifstream &file)
{
string token,tmp;

tmp=Peek(file);
if (tmp=="}" || tmp==")") return;

	while (true)
	{
	file >> token;
	if (token=="}")
		{
		file.unget();
		return;
		}
	if (token=="var")
	getVarsDeclaration(file);	
		else
		{
		//token eh nome
		tmp=token;
		file >> token; // pega o segundo token depois do nome

		putback(token,file); // retorna ponteiro para o nome
		do file.putback(1); while (file.peek()==' ');
		putback(tmp,file); //retorna ponteiro para antes do token	
	
		if (token=="(")
			getFuncCall(file);

		cout << endl;
		}
	expect(";",file);
	}
}

void getFunctionBody(ifstream &file)
{
expect("{",file);
getStatements(file);
expect("}",file);
}


void getFunction(ifstream &file)
{
if (Peek(file)=="") 
	return;
currentfunction=new funcdef();
getFunctionHeader(file);
getFunctionBody(file);
funcs.push_back(currentfunction);
cout << "got function "<<currentfunction->name<<endl;
}

void parse(ifstream &file)
{
while (file.good())
	getFunction(file);
}

int main(int argc,char *argv[])
{
ifstream script("script.lis");
parse(script);
funcnode *ptr;
funcnode *ptr2;
for (int c=0;c<funcs.size();c++)
	exec(funcs[c]);
return 0;
for (int c=0;c<funcs.size();c++)
    {
    cout  << funcs[c]->name<<":" <<endl;
    ptr=funcs[c]->lambda;
    while (ptr!=NULL)
            {
                cout << ptr->name << "-*";
                /*
               ptr2=ptr->son;
               while (ptr2!=NULL)
               {
               cout << '\t';
               cout << ptr2->name;               
               ptr2=ptr2->next;
               }  */       
               //cout << endl;
               ptr=ptr->next;
            }
            cout << endl;
    }
return 0;
}
