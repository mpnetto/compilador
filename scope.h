#ifndef _scope_H_
#define _scope_H_

#include <vector>
#include <string>
#include "table.h"

using namespace std;

class Scope
{
	vector<Scope> scopeList;
	Scope* parent = NULL;
	SymbolTable* symbolTable;
	string id;

public:

	Scope()
	{
		symbolTable = new SymbolTable();
		parent = NULL;
	}
	Scope(Scope* par)
	{
		symbolTable = new SymbolTable();
		this->parent = par;
	}
	void setScopeId(string id)
	{
		this->id = id;
	}

	bool addSymbol(Symbol sym)
	{
		return symbolTable->addSymbol(sym);
	}

	void addScope(Scope scope)
	{
		scopeList.push_back(scope);
	}

	Scope enterScope()
	{
		Scope scope;
		scopeList.push_back(scope);
		scope.parent = this;

		return scope;
		// Scope* scope = new Scope();
		// scope->parent = this;


		// scopeList.push_back(scope);

		// return scope;
	}

	Scope* exitScope()
	{
		return parent;
	}

	bool checkScope(string name)
	{
		if(symbolTable->hasSymbol(name))
			return false;
		if(parent != NULL)
			return parent->checkScope(name);
		return true;
	}
};

#endif