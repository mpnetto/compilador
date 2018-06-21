#include "ast.h"
#include "scope.h"
#include <stack>
#include <iostream>
#include <utility>

using namespace std;

string out = "";
bool semError = false;

Scope findScope(vector<Scope> scopes, string id)
{
	for(vector<Scope>::iterator it = scopes.begin(); it != scopes.end(); ++it)
	{
		if(it->id == id)
			return *it;
	}
	Scope scope;
	scope.setScopeId("0");
	return scope;
}

void escreverArquivo(char *nomeArquivo)
{
    ofstream arquivo;
    arquivo.open(nomeArquivo);

    if(arquivo.fail())
    {
        perror ("Erro ao abrir o arquivo: " + *nomeArquivo);
        exit(1);
    }

    if(!semError && out != "")
    	arquivo << out;

    arquivo.close();
}


class Semantic
{
public:
	stack<AstNode> tree;
	vector<Scope> scopes;
	Scope globalScope;
	Scope localScope;
	Scope callScope;
	bool call = false;
	int callCount=0;

	Semantic(stack<AstNode> tree)
	{
		this->tree = tree;
	}

	void semantic(AstNode node)
	{
		localScope =localScope = new Scope(globalScope);
		run(node);

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
			out += "[" + node.nodeType;

			string name;
			string type;
			string typeSpecifier = "VARIABLE";
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(!child->nodeType.compare("ID"))
				{
					name = child->nodeToken;
					out += "[" + name +"]";
				}
				else if (!child->nodeType.compare("type-specifier"))
				{
					type = child->getFirstChild().nodeToken;
					out +="["+ type + "]";
				}
				else if (!child->nodeType.compare("NUM"))
					out +="["+ child->nodeToken + "]";
				else if (!child->nodeToken.compare("["))
					type = "ARRAY";
			}

			Symbol sym(name, type, typeSpecifier);

			semError = semError || localScope.addSymbol(sym);
			out +="]";
			return "";
		}
		else if(!node.nodeType.compare("fun-declaration"))
		{
			out += "[" + node.nodeType;

			string name;
			string type;
			string typeSpecifier = "FUNCTION";

			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(!child->nodeType.compare("ID"))
				{
					name = child->nodeToken;
					out += "[" + name + "]";
					localScope = new Scope(localScope);
					localScope.setScopeId(name);

				}
				else if (!child->nodeType.compare("type-specifier"))
				{
					type = child->getFirstChild().nodeToken;
					out += "[" + type + "]";
				}
			}

			Symbol sym(name, type, typeSpecifier);


			semError = semError || globalScope.addSymbol(sym);

			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{

				if (!child->nodeType.compare("params") || !child->nodeType.compare("compound-stmt"))
					run(*child);
			}


			scopes.push_back(localScope);
			localScope = *localScope.exitScope();
			out += "]";
			return "";
		}
		else if(!node.nodeType.compare("params"))
		{
			out += "[" + node.nodeType;
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
			out += "[" + node.nodeType;
			string name;
			string type;
			string typeSpecifier = "VARIABLE";
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(!child->nodeType.compare("ID"))
				{
					name = child->nodeToken;
					out += "["+ name + "]";
				}
				else if (!child->nodeType.compare("type-specifier"))
				{
					type = child->getFirstChild().nodeToken;
					out += "[" + type+ "]";
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

			semError = semError || localScope.addSymbol(sym);

			out +="]";
			return "";
		}
		else if(!node.nodeType.compare("compound-stmt"))
		{
			out += "[" + node.nodeType;
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
			string temp, type = "";
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
				{
					temp = run(*child);
					if(temp!="")
					{
						if(type == "" || type == temp)
							type = temp;
						else
							semError=true;
					}
				}
			}
			return type;
		}
		else if(!node.nodeType.compare("statement"))
		{

			string temp, type = "";
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
				{

					temp = run(*child);
					if(temp!="")
					{
						if(type == "" || type == temp)
							type = temp;
						else
							semError=true;
					}
				}
			}

			return type;
		}
		else if(!node.nodeType.compare("expression-stmt"))
		{
			if(call)
				callCount++;

			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
				{
					run(*child);
				}

			}

			if(node.onlyChild())
					out +="[;]";
			return "";
		}
		else if(!node.nodeType.compare("selection-stmt"))
		{
			out += "[" + node.nodeType;
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
			out += "[" + node.nodeType;
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
				{

					run(*child);
				}
			}

			out += "]";
			return "";
		}
		else if(!node.nodeType.compare("return-stmt"))
		{
			string temp, type = "";
			out += "[" + node.nodeType;
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
				{
					temp = run(*child);
					if(temp!="")
					{
						if(type == "" || type == temp)
							type = temp;
						else
							semError=true;
					}
				}
			}

			string global =  globalScope.findScope(localScope.id, "FUNCTION");

			if((global == "void" && type != "") || (global != "void" && global != type))
				semError = true;

			out += "]";
			return type;
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
					out +="[=";
					expr = true;
				}
			}
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(child->hasChildren())
				{
					temp = run(*child);
					if(temp!="")
					{
						if(type == "" || type == temp)
							type = temp;
						else
							semError=true;
					}
				}
			}

			if(expr)
				out += "]";

			return type;
		}
		else if(!node.nodeType.compare("var"))
		{
			string name, array="", temp = "";
			out += "[" + node.nodeType;
			vector<AstNode> children = node.getChildren();
			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(!child->nodeType.compare("ID"))
				{
					name = child->nodeToken;
					out += "[" + child->nodeToken + "]";
				}
				else if(child->nodeToken.compare("["))
					array = "ARRAY";

				if(child->hasChildren())
					temp = run(*child);
			}
			string type = localScope.findScope(name, "VARIABLE");

			if (type == "ARRAY" && temp == "int")
				type = "int";
			else if(array!= "" && array != type)
				semError = true;

			if(!call)
				semError = semError || localScope.checkScope(name, "VARIABLE");
			else
			{
				semError = semError || callScope.checkScope(type, callCount);
			}

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
						out += "[" + child->getFirstChild().nodeToken;
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
							semError=true;
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
					out += "[" + child->getFirstChild().nodeToken;
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
							semError=true;
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
					out += "[" + child->getFirstChild().nodeToken;
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
							semError=true;
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

					out +="[" +  child->nodeToken +"]";
				}
				else if(child->hasChildren())
					type = run(*child);
			}
			if(call && checktype != "")
			{
				semError = semError || callScope.checkScope(checktype, callCount);
			}
			if(type == "")
				return checktype;
			return type;
		}
		else if(!node.nodeType.compare("call"))
		{
			call = true;
			callCount = -1;
			out += "[" + node.nodeType;
			string name;
			vector<AstNode> children = node.getChildren();

			for ( vector<AstNode>::reverse_iterator child = children.rbegin(); child != children.rend(); ++child)
			{
				if(!child->nodeType.compare("ID"))
				{

					name = child->nodeToken;
					Scope sco = findScope(scopes, name);


					if(sco.id == "0")
						semError = true;
					else
						callScope = sco;

					out +="[" +  child->nodeToken +"]";
				}
				if(child->hasChildren())
					run(*child);
			}
			string type = globalScope.findScope(name, "FUNCTION");
			semError = semError || globalScope.checkScope(name, "FUNCTION");
			call = false;
			out +="]";

			return type;
		}
		else if(!node.nodeType.compare("args"))
		{
			out += "[" + node.nodeType;
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