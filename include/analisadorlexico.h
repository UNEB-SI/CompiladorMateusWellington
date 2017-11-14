//
// Created by mateus on 07/10/2017.
//

#ifndef COMPILADOR_ANALISADORLEXICO_H
#define COMPILADOR_ANALISADORLEXICO_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "erro.h"
#define SIZE 1000
#define PR_TAM 13
#define SN_TAM 20

struct tk_struct {
    int categoria;
    union {
        int codigo;
        char lexema[SIZE];
    };
    int linha;
};

typedef struct tk_struct Token;

int linha, linhaAux;
int tksize, tkpos;
Token *tokens;
int idpos;
char ids[SIZE][SIZE];
int literalpos;
char literal[SIZE][SIZE];

enum Categoria {INTCON, REALCON, CT_C, CT_L, SN, PR, ID};
enum PalavraR {INTEIRO, REAL, CARACTER, CADEIA, BOOLEANO,
    SEMRETORNO, RETORNE, SE, SENAO, ENQUANTO, PARA, SEMPARAM, PROTOTIPO};
enum Sinal {AND, OR, NOT, ADICAO, SUBTRACAO, MULTIPLICACAO, DIVISAO, MENOR, MAIOR, IGUAL, MENORIGUAL, MAIORIGUAL,
        ATRIBUICAO, VIRGULA, PONTOEVIRGULA, ABREPARENTESES, FECHAPARENTESE, DIFERENTE, ABRECHAVES, FECHACHAVES};

int analisadorLexico(FILE *file);
void analisePRouID(char valor[]);
void criarToken(int categoria, char* valor, int codigo);
void armazenarToken(Token tk);

#endif //COMPILADOR_ANALISADORLEXICO_H
