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
void PUSH(float var);
void POP();
void COPY();
void STOR(char* var);
void LOAD(float var);
void ADD();
void SUB();
void MUL();
void DIV();

#endif //COMPILADOR_MAQPILHA_H
