#ifndef GERENCIADOR_H_INCLUDED
#define GERENCIADOR_H_INCLUDED

#define GLOBAL -1
#define LOCAL -2
#define PARAM -5
#include "sintatico.h"

typedef struct t{
        char lexema[DIM];
        int zombie;
        int tipo;
        int escopo;
        int linha;
}CelulaTabela;

enum{INSERIR = -10, IGNORE = -11, EXCLUIR = -12, CONSULTAR = -13} operacao;

CelulaTabela tabela[DIM];
int topo;

void gerenciador(int operacao, int tipo, int zombie, int classe);
void inserir(int tipo, int zombie, int classe);
void excluir();
void mostrarTabela();

int consultar(char lexema[DIM], int esc);


#endif // GERENCIADOR_H_INCLUDED
