#include <stack>
#include <stdio.h>
#include <string.h>
#include <string>
#include <utility>
#include "token.h"

#define SYM 57

using namespace std;

static const string tab_symbols[SYM] ={ "ID", ";", "[", "NUM", "]", "int", "void", "(", ")", ",", "{", "}", "if", "else", "while",	"return", "=", 	"<=", "<", ">",	">=", "==",	"!=", "+", "-", "*", "/", "$", "program", "declaration-list",	"declaration", "var-declaration", "fun-declaration", "type-specifier", "params", "compound-stmt", "param-list", "param", "local-declarations", "statement-list", "statement", 	"expression-stmt", "selection-stmt", "iteration-stmt", "return-stmt", "expression",	"var", "simple-expression", "additive-expression", "relop", "addop", "term", "mulop", "factor", "call", "args", "arg-list"};

string LRS[101][SYM];
string FF_SET[30][3];

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

int findSym(string tipo)
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
	void readLRS(char *csv);
	void readFF(char *csv);
	void parse();
	void getTypes();
};

Parser::Parser(vector<Token> tokens)
{
	this->tokens = tokens;
	readLRS((char*) "Tabela LRS.csv");
	readFF((char*) "FF-Sets.csv");
	getTypes();
}

void Parser::readLRS(char *csv)
{
	int i, j;
    ifstream  data(csv);
    string line;

    i=0;
    while(getline(data,line))
    {
        stringstream lineStream(line);
        string cell;
        j=0;
        while(getline(lineStream,cell,';'))
        {
        	LRS[i][j] = cell;
            j++;
        }
        i++;
    }
}

void Parser::readFF(char *csv)
{
	int i, j;
    ifstream  data(csv);
    string line;

    i=0;
    while(getline(data,line))
    {
        stringstream lineStream(line);
        string cell;
        j=0;
        while(getline(lineStream,cell,';'))
        {
        	FF_SET[i][j] = cell;
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
	int sym, action, n, i=0;
	int state = 0;
	string lookAhead = "dummy" ;
	stack<pair<int,string>> parse_table;
	vector<string> vec;
	parse_table.push(make_pair(state, lookAhead));

	 while(true)
	{
		lookAhead = types[i];
		state = parse_table.top().first;
		sym  = findSym(lookAhead);
		action = stoi(LRS[state][sym]);

		if (action == 0 || action == 9999)
			 	break;
		else if(action > 0){
			state = action;
			parse_table.push(make_pair(state, lookAhead));
			i++;
		}
		else
		{
			n = -action % 100;
			sym = (-action - n) / 100;
			for (int j = 0; j < n; ++j)
			   	parse_table.pop();
			state = parse_table.top().first;
			parse_table.push(make_pair(stoi(LRS[state][sym]), tab_symbols[sym]));
		}
	 }

	if(action == 0)
		cout << "--Programa Finalizado--\n PARSER INCORRETO!!\n";
	else{
		cout << "--Programa Finalizado Corretamente--\n";
	}
}

