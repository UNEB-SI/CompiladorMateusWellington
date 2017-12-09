#ifndef GERENCIADOR_H_INCLUDED
#define GERENCIADOR_H_INCLUDED

#include "analisadorlexico.h"
#include "analisadorsintatico.h"

#define ZOMBIE 1
#define DIM 100

typedef struct tabela {
        char lexema[DIM];
        int zombie;
        int tipo;
        int escopo;
        int linha;
} CelulaTabela;

enum Escopo {GLOBAL, LOCAL};

CelulaTabela tabela[DIM];
int topo;

void inserir(int tipo, char* lexema, int zombie, int escopo, int linha);
void alterar(char lexema[], int escopo);
CelulaTabela consultar(char lexema[]);
CelulaTabela consultarUltimoGlobal();
void excluirLocais();
void mostrarTabela();

#endif // GERENCIADOR_H_INCLUDED
