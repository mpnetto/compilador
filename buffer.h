#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

class Codigo
{

private:

  int linha;
  int apontador;
  string texto;
public:
  Codigo(char *nomeArquivo);

  void lerArquivo(char *nomeArquivo);
  char proximoChar();
  char getProximoChar();

};

Codigo::Codigo(char *nomeArquivo)
{
  linha = 1;
  apontador = 0;
}


void Codigo::lerArquivo(char *nomeArquivo)
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

char Codigo::proximoChar()
{
  char c = texto[apontador];
    if(c == '\n')
        linha++;
    return texto[++apontador];
}

char Codigo::getProximoChar()
{
    return texto[apontador+1];
}
