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
    char posicao[DIM];
} CelulaTabela;

enum Escopo {GLOBAL, LOCAL};

CelulaTabela tabela[DIM];
int topo;

void inserir(int tipo, char* lexema, int zombie, int escopo, int linha, int pos);
void alterar(char lexema[], int escopo);
void  alterarFuncao(char lexema[]);
void alterarParametro(char funcao[], char lexema[], int escopo, int posicao);
CelulaTabela consultar(char lexema[]);
CelulaTabela consultar2(char lexema[]);
CelulaTabela consultarTipoParametro(char lexema[], int posicao);
void excluirLocais();
void mostrarTabela();

#endif // GERENCIADOR_H_INCLUDED
