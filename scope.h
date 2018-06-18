#ifndef _scope_H_
#define _scope_H_

#include <stack>
#include <string>
#include "table.h"

using namespace std;


class Scope
{
	stack<Scope> scopeList;
	Scope* parent;
	SymbolTable symbolTable;

public:

	bool addSymbol(Symbol sym)
	{
		return symbolTable.addSymbol(sym);
	}

	Scope enterScope()
	{
		Scope scope;
		scope.parent = this;
		scopeList.push(scope);

		return scope;
	}

	Scope* exitScope()
	{
		return parent;
	}

	void checkScope();
};

#endif