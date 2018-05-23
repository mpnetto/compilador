#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

static const int keywordsSize = 6;
static const int symbolsSize = 20;
static const string keywords[keywordsSize] = {"else", "if", "int", "return", "void", "while"};
static const string symbols[symbolsSize] = {"+",  "-",  "*",  "/",  "<",  "<=",  ">",  ">=",  "==",  "!=",  "=",  ";",  ",",  "(",  ")",  "[",  "]",  "{",  "}",  "/*"};

class Lexema
{

private:
	string lexema;

public:
	Lexema();

	void add(char c);
	void clear();
	bool isKeyword();
	bool isIdentifier();
	bool isSymbol();
	bool isNumber();
	bool isWhiteSpace();
	bool isError();
	bool isKeyword(char c);
	bool isIdentifier(char c);
	bool isSymbol(char c);
	bool isNumber(char c);
	bool isWhiteSpace(char c);
	bool isOpenComment();
	bool isCloseComment();
	bool isCloseComment(char c);
	bool isError(char c);
	string get();
	string getType();
};

Lexema::Lexema()
{
	this->lexema = "";
}

void Lexema::add(char c)
{
	lexema+=c;
}

void Lexema::clear()
{
	lexema = "";
}

string Lexema::get()
{
	return lexema;
}

bool Lexema::isKeyword()
{
    for(int i = 0; i < keywordsSize; i++){
         if(lexema.compare(keywords[i]) == 0)
             return true;
    }

    return false;
}

bool Lexema::isKeyword(char c)
{
	string s = lexema + c;
    for(int i = 0; i < keywordsSize; i++){
         if(s.compare(keywords[i]) == 0)
             return true;
    }

    return false;
}

bool Lexema::isSymbol()
{
    for(int i = 0; i < symbolsSize; i++){
         if(lexema.compare(symbols[i]) == 0)
             return true;
    }

    return false;
}

bool Lexema::isSymbol(char c)
{
	string s = lexema + c;
    for(int i = 0; i < symbolsSize; i++){
         if(s.compare(symbols[i]) == 0)
             return true;
    }

    return false;
}

bool Lexema::isIdentifier(){
	if(!isalpha(lexema[0]))
		return false;
	for(int i = 1; i < lexema.size(); i++)
		if(!isalnum(lexema[i]))
			return false;

	return true;
}

bool Lexema::isIdentifier(char c){
	string s = lexema + c;
	if(!isalpha(s[0]))
		return false;
	for(int i = 1; i < s.size(); i++)
		if(!isalnum(s[i]))
			return false;

	return true;
}

bool Lexema::isNumber()
{
	for(int i = 0; i < lexema.size(); i++)
		if(!isdigit(lexema[i]))
			return false;

	return true;
}

bool Lexema::isNumber(char c)
{
	string s = lexema + c;
	for(int i = 0; i < s.size(); i++)
		if(!isdigit(s[i]))
			return false;

	return true;
}

bool Lexema::isWhiteSpace()
{
    if (lexema == "\n" || lexema == " " || lexema == "\t" || lexema == "")
        return true;
    else
        return false;
}

bool Lexema::isWhiteSpace(char c)
{
    string s = lexema+c;

    if (s == "\n" || s == " " || s == "\t" || s == "")
        return true;
    else
        return false;
}

bool Lexema::isOpenComment()
{
	return (lexema == "/*");
}

bool Lexema::isCloseComment()
{
	return (lexema == "*/");
}

bool Lexema::isCloseComment(char c)
{
	string str = lexema + c;
	return (str == "*/");
}

string Lexema::getType()
{
    if(isKeyword())
        return "KEY";
    else if(isIdentifier())
        return "ID";
    else if(isSymbol())
        return "SYM";
    else if (isNumber())
        return "NUM";
    else if (isWhiteSpace())
        return "WHITESPACE";
    else    
        return "ERROR";
}

bool Lexema::isError(){
	return !isKeyword() && !isIdentifier() && !isSymbol() && !isNumber() && !isWhiteSpace();
}

bool Lexema::isError(char c){
	return !isKeyword(c) && !isIdentifier(c) && !isSymbol(c) && !isNumber(c) && !isWhiteSpace(c);
}