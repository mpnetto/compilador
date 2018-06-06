#include <stdio.h>
#include <string>
#include "lexico.h"
#include "parser.h"
using namespace std;

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
    	lexico(argv[1]);
        Parser parser(tokens);
        parser.parse();
    }
    return 0;
}