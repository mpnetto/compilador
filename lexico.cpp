#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>

using namespace std;

typedef struct token
{
    int linha;
    string tipo;
    string lexema;
}token;

bool erro = false;
int linha = 1;
int apontador = 0;
char c;
string texto;
string tipo = "";
string lexema = "";
token tok;

static const int keywordsSize = 6;
static const int symbolsSize = 19;

static const string keywords[keywordsSize] = {"else", "if", "int", "return", "void", "while"};
static const string symbols[symbolsSize] = {"+", "-", "*", "/", "<", ">", "<=", ">=", "==", "=", "!=", ";", ",", "(", ")", "[", "]", "{", "}"};

vector<token> tokens;

bool isWhiteSpace()
{
    if (c == '\n' ||c== ' ' || c == '\t')
        return true;
    else
        return false;
}



void setTipo(string tipo)
{
    tok.tipo = tipo;
}

void isError()
{
    erro = true;
    setTipo("ERROR");
}

void geraToken(int linha, string tipo, string lexema)
{
    tok.linha = linha;
    tok.tipo = tipo;
    tok.lexema = lexema;
}

void salvaToken()
{
    if (tok.lexema != "")
    {
        tokens.push_back(tok);
    }
    lexema = ""; 
    tipo = "";
}

bool testSymbol()
{
    for(int i = 0; i < symbolsSize; i++){
        if(lexema.compare(symbols[i]) == 0)
            return true;
    }

    return false;
}

bool isKeyword()
{
    for(int i = 0; i < keywordsSize; i++){
         if(lexema.compare(keywords[i]) == 0)
             return true;
    }

    return false;
}

void isIdentifier()
{
    lexema += c;
    proximoChar();

    while(isalnum(c)){
        lexema +=c;
        proximoChar();
    }

    if(isKeyword())
        tipo = "KEY";
    else
        tipo = "ID";

    geraToken(linha,tipo,lexema);
}

void isComment()
{

    if ((lexema.compare("/") == 0) && (apontador+1 < texto.length()) && (c == '*')){
        proximoChar();

        while((apontador+1 < texto.length()) && ((c != '*') ||  (getProximoChar() != '/')))
            proximoChar();


        if((apontador+1 == texto.length()) || (c != '*') && (getProximoChar() != '/')){
            isError();
        }
        else    
            geraToken(linha,"","");
            proximoChar();
            proximoChar();
    }
}

bool isSymbol()
{
    lexema = c;
    string antlexema = lexema;

    tipo = "SYM";

    bool symbol = false;


    if (testSymbol())
    {
        symbol = true;

        proximoChar();
        geraToken(linha,tipo,lexema);

        isComment();
    }

    lexema += getProximoChar();


    if(testSymbol())
    {
        symbol = true;
        proximoChar();

        geraToken(linha,tipo,lexema);
        proximoChar();
    
    }

    if(!symbol)
        geraToken(linha,tipo,lexema);

    return symbol;

}

void isNumber()
{
    tipo = "NUM";

    lexema += c;
    proximoChar();

    while(isdigit(c)){
        lexema +=c;
        proximoChar();
    }
    

    if (isalpha(c)){
        lexema += c;
        geraToken(linha,tipo,lexema);
        isError();
    }
    else
         geraToken(linha,tipo,lexema);
        
}

void getType()
{
    if (isWhiteSpace())
    {
        proximoChar();
        return;
    }
    else if(isalpha(c))
        isIdentifier();
    else if(isdigit(c))
        isNumber();
    else if(isSymbol());
    else
        isError();

    salvaToken();

    return ;
}

void lerArquivo(char *nomeArquivo)
{
    FILE *arquivo;
    char letra;

    if ((arquivo = fopen (nomeArquivo , "r")) == NULL)
    {
        perror ("Erro ao abrir o arquivo: " + *nomeArquivo);
        exit(1);
    }

    while((letra = getc (arquivo)) && (letra != EOF))
        texto += letra;

    fclose (arquivo);
}

void escreverArquivo(char *nomeArquivo)
{
    vector<token>::iterator it;

    ofstream arquivo;
    arquivo.open(nomeArquivo);

    if(arquivo.fail())
    {
        perror ("Erro ao abrir o arquivo: " + *nomeArquivo);
        exit(1);
    }

    for (it = tokens.begin(); it != tokens.end(); ++it)
    {
        arquivo << "(";
        arquivo << it->linha;
        arquivo << ",";
        arquivo << it->tipo;
        arquivo << ",\"";
        arquivo << it->lexema;
        arquivo << "\")";
        arquivo << endl;
    };

    arquivo.close();
}

int main(int argc, char *argv[]){

    if(argc > 1)
    {

        lerArquivo(argv[1]);

        c=texto[apontador];

        while (c != '\0' && !erro)
        {
            getType();
        }

        escreverArquivo(argv[2]);
    }

    return 0;
}