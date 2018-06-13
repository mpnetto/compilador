#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include "lexico.h"

using namespace std;

static const string  terminais[29] ={ "ID", ";", "[", "NUM", "]", "int", "void", "(", ")" ",", "{", "}", "if", "else", "while",	"return", "=", 	"<=", "<", ">",	">=", "==",	"!=", "+", "-", "*", "/", "empty","$"};
static const string naoTerminais[29] = {"program", "declaration-list",	"declaration", "var-declaration", "fun-declaration", "type-specifier", "params", "compound-stmt", "param-list", "param", "local-declarations", "statement-list", "statement", 	"expression-stmt", "selection-stmt", "iteration-stmt", "return-stmt", "expression",	"var", "simple-expression", "additive-expression", "relop", "addop", "term", "mulop", "factor", "call", "args", "arg-list"};

string LRS[100][57];

void readCSV(char *csv)
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

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
    	readCSV("Tabela LRS.csv");
    	lexico(argv[1]);
    }
    return 0;
}