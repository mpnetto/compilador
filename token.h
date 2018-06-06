#ifndef _Token_H_
#define _Token_H_

#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>

using namespace std;


class Token
{
private:
  int linha;
  string tipo;
  string lexema;

public:
    Token();

    void setToken(int linha, string tipo, string lexema);
    string getType();
    string getLex();
    string print();
};

Token::Token(){

}

void Token::setToken(int linha, string tipo, string lexema)
{
    this->linha = linha;
    this->tipo = tipo;
    this->lexema = lexema;
}

string Token::getType()
{
  return tipo;
}

string Token::getLex()
{
  return lexema;
}

string Token::print()
{
	stringstream ss;
	ss << "(" << linha << "," << tipo << ",\"" << lexema << "\")\n";

	return ss.str();
}

#endif