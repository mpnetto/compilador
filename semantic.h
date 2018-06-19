#include "ast.h"
#include "scope.h"
#include <stack>
#include <iostream>
#include <utility>

using namespace std;

Scope* findScope(vector<Scope> scopes, string id)
{
	for(vector<Scope>::iterator it = scopes.begin(); it != scopes.end(); ++it)
	{
		if(it->id == id)
			return &(*it);
	}
	return NULL;
}

class Semantic
{
public:
	stack<AstNode> tree;
	vector<Scope> scopes;
	Scope globalScope;
	Scope localScope;
	Scope callScope;
	string out;
	bool error = false;
	bool call = false;
	int callCount=0;

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

	string run(AstNode node){

		if(!node.nodeType.compare("program"))
		{
			out += "[" + node.nodeType;
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
				run(*child);
			out += "]";
			return "";
		}
		else if(!node.nodeType.compare("declaration-list"))
		{
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if (node.hasChildren())
					run(*child);
			}
			return "";
		}
		else if(!node.nodeType.compare("declaration"))
		{
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if (node.hasChildren())
					run(*child);
			}
			return "";
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
				else if (!child->nodeToken.compare("["))
					type = "ARRAY";
			}

			Symbol sym(name, type, typeSpecifier);

			error = error || localScope.addSymbol(sym);
			out +="]";
			return "";
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
					scopes.push_back(localScope);
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
			return "";
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
			return "";
		}
		else if(!node.nodeType.compare("param-list"))
		{
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if (!child->nodeType.compare("param-list") || !child->nodeType.compare("param"))
					run(*child);
			}
			return "";
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
				{
					type = "ARRAY";
					out += "[\\[";
				}
				else if (!child->nodeToken.compare("]"))
				{
					out += "\\]]";
				}
			}

			Symbol sym(name, type, typeSpecifier);

			error = error || localScope.addSymbol(sym);

			out +="]";
			return "";
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
			return "";
		}
		else if(!node.nodeType.compare("local-declarations"))
		{
			vector<AstNode> children = node.getChildren();
			for (vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
					run(*child);
			}
			return "";
		}
		else if(!node.nodeType.compare("statement-list"))
		{
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
					run(*child);
			}
			return "";
		}
		else if(!node.nodeType.compare("statement"))
		{
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
					run(*child);
			}
			return "";
		}
		else if(!node.nodeType.compare("expression-stmt"))
		{
			if(call)
				callCount++;

			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
					run(*child);
			}

			if(node.onlyChild())
					out +="\n[;]";
			return "";
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
			return "";
		}
		else if(!node.nodeType.compare("selection-stmt'"))
		{
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
					run(*child);
			}
			return "";
		}
		else if(!node.nodeType.compare("selection-stmt''"))
		{
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
					run(*child);
			}
			return "";
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
			return "";
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
			return "";
		}
		else if(!node.nodeType.compare("expression"))
		{
			bool expr = false;
			string temp, type = "";
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(!child->nodeToken.compare("="))
				{
					out +="\n[=";
					expr = true;
				}
			}
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
				if(child->hasChildren())
				{
					temp = run(*child);
					if(temp!="")
					{
						if(type == "" || type == temp)
							type = temp;
						else
							error=true;
					}
				}

			if(expr)
				out += "]";
			return type;
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
				else if(node.hasChildren())
					run(*child);
			}
			string type = localScope.findScope(name, "VARIABLE");
			if(!call)
				error = error || localScope.checkScope(name, "VARIABLE");
			else
				error = error || callScope.checkScope(type, callCount);
			out += "]";
			return type;
		}
		else if(!node.nodeType.compare("simple-expression"))
		{
			bool relop = false;
			string temp, type = "";
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
				if(!child->nodeType.compare("relop"))
					{
						relop = true;
						out += "\n[" + child->getFirstChild().nodeToken;
					}
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if (child->hasChildren())
				{
										
					temp = run(*child);

					if(temp!="")
					{
						if(type == "" || type == temp)
						{
							type = temp;
						}
						else
							error=true;
					}


				}
			}

			if(relop)
				out += "]";
			return type;
		}
		else if(!node.nodeType.compare("additive-expression"))
		{
			string temp, type = "";
			bool addop = false;
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
				if(!child->nodeType.compare("addop"))
				{
					addop = true;
					out += "\n[" + child->getFirstChild().nodeToken;
				}
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if (child->hasChildren())
				{
										

					temp = run(*child);

					if(temp!="")
					{
						if(type == "" || type == temp)
						{
							type = temp;
						}
						else
							error=true;
					}


				}
			}

			if(addop)
				out += "]";
			return type;
		}
		else if(!node.nodeType.compare("term"))
		{
			bool mulop = false;
			string temp, type="";
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
				if(!child->nodeType.compare("mulop")){
					out += "\n[" + child->getFirstChild().nodeToken;
					mulop = true;
				}
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if (child->hasChildren())
				{
					temp = run(*child);
					if(temp!="")
					{
						if(type == "" || type == temp)
						{
							type = temp;
						}
						else
							error=true;
					}
				}
			}

			if(mulop)
				out += "]";
			return type;
		}
		else if(!node.nodeType.compare("factor"))
		{
			if(call)
				callCount++;
			string checktype;
			vector<AstNode> children = node.getChildren();
			string type = "";
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(!child->nodeType.compare("NUM"))
				{
					checktype = "int";

					out +=" [" +  child->nodeToken +"]";
				}
				else if(child->hasChildren())
					type = run(*child);
			}
			if(call && checktype != "")
			{
				error = error || callScope.checkScope(checktype, callCount);
			}
			return type;
		}
		else if(!node.nodeType.compare("call"))
		{
			call = true;
			callCount = -1;
			out += "\n[" + node.nodeType;
			string name;
			vector<AstNode> children = node.getChildren();

			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(!child->nodeType.compare("ID"))
				{

					name = child->nodeToken;
					Scope* sco = findScope(scopes, name);

					if(sco == NULL)
						error = true;
					else
						callScope = *sco;
					out +="\n[" +  child->nodeToken +"]";
				}
				if(child->hasChildren())
					run(*child);
			}
			string type = globalScope.findScope(name, "FUNCTION");
			error = error || globalScope.checkScope(name, "FUNCTION");
			call = false;
			out +="]";

			return type;
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
			return "";
		}
		else if(!node.nodeType.compare("arg-list"))
		{
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
					run(*child);
			}
			return "";
		}
		return "";
	}
};