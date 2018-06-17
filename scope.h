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
	SymbolTable* symbolTable;

public:

	void addSymbol(Symbol sym)
	{
		symbolTable->addSymbol(sym);
	}

	Scope enterScope()
	{
		Scope scope;
		scope.parent = this;
		scopeList.push(scope);

		return scope;

	}

	void exitScope()
	{
		scopeList.pop();
	}

	void checkScope();


	void removeSymbol()
	{
		table.pop();
	}

};

#endif