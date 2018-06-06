#include <stack>
#include <stdio.h>
#include <string.h>
#include <string>
#include "token.h"

#define SYM 57

using namespace std;

static const string  tab_symbols[SYM] ={ "ID", ";", "[", "NUM", "]", "int", "void", "(", ")", ",", "{", "}", "if", "else", "while",	"return", "=", 	"<=", "<", ">",	">=", "==",	"!=", "+", "-", "*", "/", "$", "program", "declaration-list",	"declaration", "var-declaration", "fun-declaration", "type-specifier", "params", "compound-stmt", "param-list", "param", "local-declarations", "statement-list", "statement", 	"expression-stmt", "selection-stmt", "iteration-stmt", "return-stmt", "expression",	"var", "simple-expression", "additive-expression", "relop", "addop", "term", "mulop", "factor", "call", "args", "arg-list"};

string LRS[101][SYM];

vector<string> splitString(string str){

	char *dup, *pch;
	vector<string> vec;

	dup = strdup(str.c_str());
	pch = strtok(dup, ",");

	while (pch != NULL)
	{
		vec.push_back(pch);
		pch = strtok (NULL, ",");
	}
	return vec;
}


int find(string tipo)
{
    for (int i = 0; i < SYM; ++i)
    {
        if (!tipo.compare(tab_symbols[i])) return i;
    }
    return -1;
}

class Parser
{
private:
	vector<Token> tokens;
	int **SLR;
	vector<string> types;
public:
	Parser(vector<Token> tokens);
	void readCSV(char *csv);
	void parse();
	void getTypes();
};

Parser::Parser(vector<Token> tokens)
{
	this->tokens = tokens;
	readCSV((char*) "Tabela LRS.csv");
	getTypes();
}

void Parser::readCSV(char *csv)
{
	int i = 0;
    ifstream  data(csv);
    string line;
    while(getline(data,line))
    {
        stringstream lineStream(line);
        string cell;

        int j=0;
        while(getline(lineStream,cell,';'))
        {
            LRS[i][j] = cell;
            j++;
        }
        i++;
    }
}

void Parser::getTypes()
{
	vector<Token>::iterator it;
	Token t;
	string tipo;

	 for (it = tokens.begin(); it != tokens.end(); ++it)
	{
		t = *it;
		tipo = t.getType();

		if (tipo == "NUM" || tipo == "ID")
			types.push_back(tipo);
		else
			types.push_back(t.getLex());

	}

	types.push_back("$");
}

void Parser::parse()
{
	int col;
	int i = 0;
	int state = 0;
	string tipo, str;
	vector<string> vec;
	stack<int> states;
	stack<string> pars;

	states.push(state);
	pars.push(types[i]);
	tipo = pars.top();

	while(true)
	{
		col = find(tipo);
		state=states.top();
		vec=splitString(LRS[state][col]);

		if (vec[0] == "0" || vec[0] == "accept")
			break;
		if(vec[0] == "shift"){
			states.push(stoi(vec[1]));
			i++;

			tipo = types[i];
			pars.push(tipo);
		}
		else if (find(vec[0]) != -1)
		{
			int red = stoi(vec[1]);
			string temp = pars.top();
			pars.pop();
			for (int i = 0; i < red; ++i)
			{
				pars.pop();
				states.pop();
			}
			pars.push(vec[0]);
			tipo = pars.top();
			pars.push(temp);
		}
		else
		{
			states.push(stoi(vec[0]));
			tipo = types[i];
		}

	}

	if(vec[0] == "0")
		cout << "--Programa Finalizado--\n PARSER INCORRETO!!\n";
	else{
		cout << "--Programa Finalizado Corretamente--\n";
	}
}

