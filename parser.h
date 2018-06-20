#include <stack>
#include <stdio.h>
#include <string.h>
#include <string>
#include <utility>
#include "ast.h"
#include "token.h"

#define SYM 59

using namespace std;

static const string tab_symbols[SYM] ={ "ID", ";", "[", "NUM", "]", "int", "void", "(", ")", ",", "{", "}", "if", "else", "while",	"return", "=", 	"<=", "<", ">",	">=", "==",	"!=", "+", "-", "*", "/", "$", "program", "declaration-list",	"declaration", "var-declaration", "fun-declaration", "type-specifier", "params", "compound-stmt", "param-list", "param", "local-declarations", "statement-list", "statement", 	"expression-stmt", "selection-stmt", "iteration-stmt", "return-stmt", "expression","selection-stmt'", "selection-stmt''", "var", "simple-expression", "additive-expression", "relop", "addop", "term", "mulop", "factor", "call", "args", "arg-list"};

string LRS[106][SYM];
string FF_SET[30][3];
stack<AstNode> tree;

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
	string getType(int i);
};

Parser::Parser(vector<Token> tokens)
{
	this->tokens = tokens;
	Token token;
	token.setToken(0, "$","$");
	this->tokens.push_back(token);
	readLRS((char*) "Tabela LRS.csv");
	readFF((char*) "FF-Sets.csv");
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

string Parser::getType(int i)
{
	Token t;
	string tipo;

	t = tokens[i];
	tipo = t.getType();
	if (tipo == "NUM" || tipo == "ID")
		return tipo;
	else
		return t.getLex();
}

void Parser::parse()
{
	int sym, action, n, i=0;
	int state = 0;
	string lookAhead = "dummy" ;
	stack<pair<int,string>> parse_table;
	parse_table.push(make_pair(state, lookAhead));



	while(true)
	{

		lookAhead = getType(i);
		state = parse_table.top().first;
		sym  = findSym(lookAhead);
		action = stoi(LRS[state][sym]);

		// cout << "state: " << state << " sym: " << sym << " lookAhead: " << lookAhead << " symbol: "<< parse_table.top().second << endl;
		if (action == 0 || action == 999999)
			 	break;
		else if(action > 0){
			state = action;
			parse_table.push(make_pair(state, lookAhead));
			AstNode node(tokens[i].getType(),tokens[i].getLex());
			tree.push(node);
			i++;
		}
		else
		{
			n = -action % 100 ;
			sym = (-action - n) / 100;
			for (int j = 0; j < n; ++j)
			   	parse_table.pop();
			state = parse_table.top().first;
			parse_table.push(make_pair(stoi(LRS[state][sym]), tab_symbols[sym]));
			AstNode node(tab_symbols[sym]);

			for (int j = 0; j < n; ++j)
			{
				// if(tree.top().onlyChild())
				// 	node.addChild(tree.top().getFirstChild());
				// else

				node.addChild(tree.top());
				tree.pop();
			}
			tree.push(node);
		}
	 }
	  // cout << "size: " << tree.size()<< endl;
	  // AstNode node = tree.top();
	  // node.print(0);
//	if(action == 0)
//		cout << "--Programa Finalizado--\n PARSER INCORRETO!!\n";
//	else{
//		cout << "--Programa Finalizado Corretamente--\n";
//	}
}

