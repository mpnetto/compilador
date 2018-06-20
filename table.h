#ifndef _TABLE_H_
#define _TABLE_H_

#include <vector>
#include <string>

using namespace std;

// enum SymbolType
// {
// 	VARIABLE,
// 	FUNCTION
// };

// enum TypeSpecifier
// {
// 	INT,
// 	VOID
// };

struct Symbol
{
	string name;
	string type;
	string typeSpecifier;

	Symbol(string name)
	{
		this->name = name;
	}

	Symbol(string name, string type)
	{
		this->name = name;
		this->type = type;
	}

	Symbol(string name, string type, string typeSpecifier)
	{
		this->name = name;
		this->type = type;
		this->typeSpecifier = typeSpecifier;
	}

	bool equal(Symbol sym)
	{
		return this->name.compare(sym.name);
	}
};


class SymbolTable
{
public:

	vector <Symbol> table;

	SymbolTable()
	{
	}

	bool addSymbol(Symbol sym)
	{
		if(!hasSymbol(sym.name))
		{
			table.push_back(sym);
			return false;
		}
		return true;
	}

	bool hasSymbol(string name)
	{
		for(Symbol sym : table){
			if(!sym.name.compare(name))
			{
				return true;
			}
		}
		return false;
	}

	string findSymbol(string name, string typeSpecifier)
	{
		for(Symbol sym : table){
			if(!sym.name.compare(name) && !sym.typeSpecifier.compare(typeSpecifier))
			{
				return sym.type;
			}
		}
		return "";
	}

	bool hasSymbol(string name, string typeSpecifier)
	{
		// cout << table.size() << endl;
		for(Symbol sym : table){
				// cout <<" symbol: " << sym.name<<" type: " << sym.type << " typeSpecifier: " << sym.typeSpecifier  <<endl;
			if(!sym.name.compare(name) && !sym.typeSpecifier.compare(typeSpecifier))
			{

				return true;
			}
		}
		return false;
	}
	bool hasSymbol(string type, int i)
	{

		if(i < table.size())
		{
			Symbol sym = table[i];
			if(!sym.type.compare(type))
			return true;
		}

		return false;
	}
};

#endif