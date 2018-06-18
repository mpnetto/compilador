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
		cout <<" name: " << name <<endl;
		for(Symbol sym : table){
			if(!sym.name.compare(name))
			{
				cout <<" symbol: " << sym.name << " name: " << name <<endl;
				return true;
			}
		}
		return false;
	}

	string findSymbol(string name, string typeSpecifier)
	{
		cout <<" name: " << name <<endl;
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
		// cout <<" name: " << name <<endl;
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
		cout << "CHECK:\n";

		Symbol sym = table[i];

		cout <<  " i: " << i << " Symbol: "<< sym.type << " type: " << type<< endl;

		if(!sym.type.compare(type))

			return true;
		return false;
	}
};

#endif