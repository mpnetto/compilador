#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

int linha = 1;
int apontador = -1;
string texto;

void lerArquivo(char *nomeArquivo)
{
    FILE *arquivo;
    char letra;

    if ((arquivo = fopen (nomeArquivo , "r")) == NULL)
    {
        perror ("Erro ao abrir o arquivo: " + *nomeArquivo);
        exit(1);
    }

    while((letra = getc (arquivo)) && (letra != EOF))
        texto += letra;

    fclose (arquivo);
}

void escreverArquivo(char *nomeArquivo, string lex)
{

    ofstream arquivo;
    arquivo.open(nomeArquivo);

    if(arquivo.fail())
    {
        perror ("Erro ao abrir o arquivo: " + *nomeArquivo);
        exit(1);
    }

    arquivo << lex;

    arquivo.close();
}

char getChar()
{
  char c = texto[++apontador];

  if(c == '\n')
        linha++;
    
    return c;
}

char getProximoChar()
{
    return texto[apontador+1];
}
