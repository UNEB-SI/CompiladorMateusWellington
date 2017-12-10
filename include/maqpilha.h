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
void LOADD(Token tk); //Teste
void LOAD(float var);
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

#endif //COMPILADOR_MAQPILHA_H
