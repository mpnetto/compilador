#include <stdio.h>
#include <string>
#include "lexico.h"
#include "parser.h"
#include "semantic.h"
using namespace std;

int main(int argc, char** argv)
{
    if(argc > 1)
    {
    	lexico(argv[1]);
        Parser parser(tokens);
        parser.parse();
        if(!tree.empty())
        {
            Semantic semantic(tree);
            semantic.semantic(tree.top());
        }
        escreverArquivo(argv[2]);
        
    }
    return 0;
}