#include "ast.h"
#include "scope.h"
#include <stack>
#include <iostream>

using namespace std;

class Semantic
{
public:
	stack<AstNode> tree;
	Scope globalScope;
	Scope localScope;
	string out;
	bool error = false;

	Semantic(stack<AstNode> tree)
	{
		this->tree = tree;
	}

	void semantic(AstNode node)
	{
		localScope = globalScope.enterScope();
		run(node);
		if(!error)
			cout << out<< endl;
	}

	void run(AstNode node){

		if(!node.nodeType.compare("program"))
		{
			out += "[" + node.nodeType;
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
				run(*child);

			out += "]";
		}
		else if(!node.nodeType.compare("declaration-list"))
		{
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if (node.hasChildren())
					run(*child);
			}
		}
		else if(!node.nodeType.compare("declaration"))
		{
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if (node.hasChildren())
					run(*child);
			}
		}
		else if(!node.nodeType.compare("var-declaration"))
		{
			out += "\n[" + node.nodeType;

			string name;
			string type;
			string typeSpecifier = "VARIABLE";
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(!child->nodeType.compare("ID"))
				{
					name = child->nodeToken;
					out += " [" + name +"]";
				}
				else if (!child->nodeType.compare("type-specifier"))
				{
					type = child->getFirstChild().nodeToken;
					out +=" ["+ type + "]";
				}
				else if (!child->nodeType.compare("NUM"))
					out +=" ["+ child->nodeToken + "]";
			}

			Symbol sym(name, type, typeSpecifier);

			error = error || localScope.addSymbol(sym);

			out +="]";
		}
		else if(!node.nodeType.compare("fun-declaration"))
		{
			out += "\n[" + node.nodeType;

			string name;
			string type;
			string typeSpecifier = "FUNCTION";



			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(!child->nodeType.compare("ID"))
				{
					name = child->nodeToken;
					out += "\n[" + name + "]";
					localScope = new Scope(localScope);
					localScope.setScopeId(name);
				}
				else if (!child->nodeType.compare("type-specifier"))
				{
					type = child->getFirstChild().nodeToken;
					out += "\n[" + type + "]";
				}
				else if (!child->nodeType.compare("params") || !child->nodeType.compare("compound-stmt"))
					run(*child);
			}

			localScope = *localScope.exitScope();

			Symbol sym(name, type, typeSpecifier);

			error = error || globalScope.addSymbol(sym);

			out += "]";
		}
		else if(!node.nodeType.compare("params"))
		{
			out += "\n[" + node.nodeType;
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(!child->nodeType.compare("param-list"))
					run(*child);
			}
			out += "]";
		}
		else if(!node.nodeType.compare("param-list"))
		{
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if (!child->nodeType.compare("param-list") || !child->nodeType.compare("param"))
					run(*child);
			}
		}
		else if(!node.nodeType.compare("param"))
		{
			out += "\n[" + node.nodeType;
			string name;
			string type;
			string typeSpecifier = "VARIABLE";
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(!child->nodeType.compare("ID"))
				{
					name = child->nodeToken;
					out += " ["+ name + "]";
				}
				else if (!child->nodeType.compare("type-specifier"))
				{
					type = child->getFirstChild().nodeToken;
					out += " [" + type+ "]";
				}
				else if (!child->nodeToken.compare("["))
					out += "[\\[";
				else if (!child->nodeToken.compare("]"))
					out += "\\]]";
			}

			Symbol sym(name, type, typeSpecifier);

			error = error || localScope.addSymbol(sym);

			out +="]";
		}
		else if(!node.nodeType.compare("compound-stmt"))
		{
			out += "\n[" + node.nodeType;
			vector<AstNode> children = node.getChildren();
			for (vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if (!child->nodeType.compare("local-declarations") || !child->nodeType.compare("statement-list"))
					run(*child);
			}
			out += "]";
		}
		else if(!node.nodeType.compare("local-declarations"))
		{
			vector<AstNode> children = node.getChildren();
			for (vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
					run(*child);
			}
		}
		else if(!node.nodeType.compare("statement-list"))
		{
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
					run(*child);
			}
		}
		else if(!node.nodeType.compare("statement"))
		{
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
					run(*child);
			}
		}
		else if(!node.nodeType.compare("expression-stmt"))
		{
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
					run(*child);
			}

			if(node.onlyChild())
					out +="\n[;]";
		}
		else if(!node.nodeType.compare("selection-stmt"))
		{
			out += "\n[" + node.nodeType;
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
					run(*child);
			}
			out += "]";
		}
		else if(!node.nodeType.compare("selection-stmt'"))
		{
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
					run(*child);
			}

		}
		else if(!node.nodeType.compare("selection-stmt''"))
		{
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
					run(*child);
			}
		}
		else if(!node.nodeType.compare("iteration-stmt"))
		{
			out += "\n[" + node.nodeType;
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
					run(*child);
			}
			out += "]";
		}
		else if(!node.nodeType.compare("return-stmt"))
		{
			out += "\n[" + node.nodeType;
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
					run(*child);
			}
			out += "]";
		}
		else if(!node.nodeType.compare("expression"))
		{
			bool expr = false;
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
				if(!child->nodeToken.compare("="))
				{
					out +="\n[=";
					expr = true;
				}
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
				if(child->hasChildren())
					run(*child);

			if(expr)
				out += "]";
		}
		else if(!node.nodeType.compare("var"))
		{
			string name;
			out += " [" + node.nodeType;
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(!child->nodeType.compare("ID"))
				{
					name = child->nodeToken;
					out += " [" + child->nodeToken + "]";
				}
				if(node.hasChildren())
					run(*child);
			}
			error = error || localScope.checkScope(name);
			out += "]";
		}
		else if(!node.nodeType.compare("simple-expression"))
		{
			bool relop = false;
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
				if(!child->nodeType.compare("relop"))
					{
						relop = true;
						out += "\n[" + child->getFirstChild().nodeToken;
					}
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
				if(node.hasChildren())
					run(*child);

			if(relop)
				out += "]";
		}
		else if(!node.nodeType.compare("additive-expression"))
		{
			bool addop = false;
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
				if(!child->nodeType.compare("addop"))
				{
					addop = true;
					out += "\n[" + child->getFirstChild().nodeToken;
				}
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
				if (child->hasChildren())
					run(*child);

			if(addop)
				out += "]";
		}
		else if(!node.nodeType.compare("term"))
		{
			bool mulop = false;
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
				if(!child->nodeType.compare("mulop")){
					out += "\n[" + child->getFirstChild().nodeToken;
					mulop = true;
				}
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
				if (child->hasChildren())
					run(*child);

			if(mulop)
				out += "]";
		}
		else if(!node.nodeType.compare("factor"))
		{
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(!child->nodeType.compare("NUM"))
					out +=" [" +  child->nodeToken +"]";
				else if(child->hasChildren())
					run(*child);
			}
		}
		else if(!node.nodeType.compare("call"))
		{
			out += "\n[" + node.nodeType;
			string name;
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(!child->nodeType.compare("ID"))
				{
					name = child->nodeToken;
					out +="\n[" +  child->nodeToken +"]";
				}
				if(child->hasChildren())
					run(*child);
			}
			error = error || globalScope.checkScope(name);
			out +="]";
		}
		else if(!node.nodeType.compare("args"))
		{
			out += "\n[" + node.nodeType;
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
					run(*child);
			}
			out +="]";
		}
		else if(!node.nodeType.compare("arg-list"))
		{
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
					run(*child);
			}
		}
	}
};