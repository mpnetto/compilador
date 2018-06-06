#include <iostream>
#include <stdio.h>
#include "scanner.h"
#include "lexema.h"
#include "token.h"

using namespace std;

bool error = false;
char c;
Lexema lexema;
Token token;
vector<Token> tokens;

void criaToken(Token token)
{
    tokens.push_back(token);
}

string lex()
{
    vector<Token>::iterator it;
    string str = "";

    for (it = tokens.begin(); it != tokens.end(); ++it)
    {
        str = str + it->print();
    }

    return str;
}

void isComment()
{
    token.setToken(linha, "ERROR", lexema.get());
    error = true;
    lexema.clear();

    while (c != '\0')
    {
        if(lexema.isCloseComment(c))
        {
            lexema.clear();
            c = getChar();
            error = false;
            return;
        }

        lexema.clear();
        lexema.add(c);

        c = getChar();
    }
}

void lexico(char *arq){

    lerArquivo(arq);
    c = getChar();

    while (c != '\0')
    {
        if(!lexema.isWhiteSpace() && lexema.isError(c))
        {
            token.setToken(linha, lexema.getType(), lexema.get());
            criaToken(token);
            lexema.clear();
        }

        if(!lexema.isWhiteSpace(c))
            lexema.add(c);

        c = getChar();

        if(lexema.isOpenComment())
            isComment();

        if(error)
            break;

        if(lexema.isError())
        {
            if(!lexema.isError(c))
            {
                lexema.add(c);
                c = getChar();
            }
            else
                break;
        }
    }

    if(error)
        criaToken(token);
    else if(!lexema.isWhiteSpace())
    {
        token.setToken(linha, lexema.getType(), lexema.get());
        criaToken(token);
    }
}