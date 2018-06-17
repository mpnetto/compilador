#ifndef _TABLE_H_
#define _TABLE_H_

#include <vector>
#include <string>

using namespace std;

enum SymbolType
{
	VARIABLE,
	FUNCTION
};

enum TypeSpecifier
{
	INT,
	VOID
}

struct Symbol
{
	string name;
	SymbolType type;
	TypeSpecifier TypeSpecifier;

	Symbol(string name)
	{
		this->name = name;
	}

	bool equal(Symbol sym)
	{
		return this->type.compare(sym.type);
	}

};


class SymbolTable
{
public:

	vector <Symbol> table;

	SymbolTable()
	{
	}

	void addSymbol(Symbol sym)
	{

		if(!hasSymbol(sym.name))
		{
			table.push_back(sym);
		}
		// else			IMPLEMENTAR SOLUCAO DE ERRO
		// 	ERRO
	}

	bool hasSymbol(string name)
	{

		for(Symbol sym : table)
			if(!sym.name.compare(name));
				return true;

		return false;
	}

	
};

#endif