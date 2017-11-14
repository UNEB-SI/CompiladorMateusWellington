#ifndef GERENCIADOR_H_INCLUDED
#define GERENCIADOR_H_INCLUDED

#include "analisadorlexico.h"
#include "analisadorsintatico.h"

#define GLOBAL -1
#define LOCAL -2
#define PARAM -5
#define DIM 100

typedef struct tabela {
        char lexema[DIM];
        int zombie;
        int tipo;
        int escopo;
        int linha;
} CelulaTabela;

enum{INSERIR = -10, IGNORE = -11, EXCLUIR = -12, CONSULTAR = -13} operacao;

CelulaTabela tabela[DIM];
int topo;

void gerenciador(int operacao, int tipo, char lexema[], int zombie, int escopo, int linha);
void inserir(int tipo, char* lexema, int zombie, int escopo, int linha);
void excluir();
void mostrarTabela();

int consultar(char lexema[DIM], int esc);


#endif // GERENCIADOR_H_INCLUDED
