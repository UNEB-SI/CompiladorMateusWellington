//
// Created by mateus and wellington on 05/12/2017.
//

#ifndef COMPILADOR_MAQPILHA_H
#define COMPILADOR_MAQPILHA_H

#include "analisadorlexico.h"
#include "analisadorsintatico.h"

#define DIM 100

float pilha[DIM];
int topoPilha;
FILE *file;

void configurarPilha(FILE *arq);
void PUSH(Token tk);
void POP();
void COPY();
void STOR(Token tk);
void LOAD(Token tk);
/*Valores Inteiros*/
void ADD();
void SUB();
void MUL();
void DIV();
/*Valores Reais*/
void ADD();
void SUB();
void MUL();
void DIV();

int tDeclarados, tUsados;
int gerarLabel();
int pegarLabel();
void LABEL(int label);
void GOTO(int label);
void GOFALSE(int label);
void GOTRUE(int label);

void AMEM(int size);

void HALT();

#endif //COMPILADOR_MAQPILHA_H
