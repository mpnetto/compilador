#ifndef _AST_H_
#define _AST_H_

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class AstNode
{
public:
	AstNode	*parent;
	vector<AstNode> children;
	string nodeType;
	string nodeToken;

	AstNode()
	{
	}

	AstNode(string type)
	{
		nodeType = type;
	}

	AstNode(string type, string token)
	{
		nodeType = type;
		nodeToken = token;
	}

	void setType(string type)
	{
		nodeType = type;
	}

	void addChild(AstNode child)
	{
		child.parent = this;
		children.push_back(child);
	}

	vector<AstNode> getChildren()
	{
		return children;
	}

	AstNode getFirstChild()
	{
		return children[0];
	}

	bool onlyChild()
	{
		return children.size() == 1;
	}

	bool hasChildren()
	{
		return !children.empty();
	}

	bool hasGrandChildren(){
		for ( vector<AstNode>::iterator it = children.begin(); it != children.end(); ++it)
		{
			AstNode node = *it;

			if(node.hasChildren())
				return true;
		}
		return false;
	}

	void print(int n) {
		int size;
		AstNode node;
		cout << "[" << nodeType;

		bool grandFather = hasGrandChildren();
		n++;

		// if (n < 2)
			for(vector<AstNode>::reverse_iterator it = children.rbegin(); it != children.rend(); ++it)
			{
				node = *it;

				if(grandFather)
				{
					cout << endl;
					for(int i = 0; i < n; ++i)
				 		cout << "  ";
				}
				string str = node.nodeType;
				// if (str.compare(";") && str.compare("(") && str.compare(")"))
				// {
					node.print(n);
				// }


			}
		cout << "]";
	}

};
#endif