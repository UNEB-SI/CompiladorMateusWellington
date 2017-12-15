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
    int label;
    char posicao[DIM];
} CelulaTabela;

enum Escopo {GLOBAL, LOCAL};

CelulaTabela tabela[DIM];
int topo;

void inserirVar(int tipo, char* lexema, int zombie, int escopo, int pos);
void inserirFuncao(int tipo, char* lexema, int zombie, int escopo, int pos);
void alterarParamVar(int posParam);
CelulaTabela consultar(char lexema[]);
CelulaTabela consultarLocais(char lexema[]);
CelulaTabela consultarLocaisEGlobais(char lexema[]);
CelulaTabela consultarTipoParametro(char lexema[], int posicao);
void excluirLocais(char* lexema, int param);
void mostrarTabela();

#endif // GERENCIADOR_H_INCLUDED
