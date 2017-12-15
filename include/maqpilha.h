//
// Created by mateus and wellington on 05/12/2017.
//

#ifndef COMPILADOR_MAQPILHA_H
#define COMPILADOR_MAQPILHA_H

#include "analisadorlexico.h"
#include "analisadorsintatico.h"

#define DIM 100

FILE *file;

void configurarPilha();
void excluirPilha();
void INIP();
void PUSH(int val);
void PUSH2(Token tk);
void POP();
void COPY();
void STOR(CelulaTabela celulaTabela);
void STOR2(char* posicao);
void LOAD(CelulaTabela celulaTabela);
/*Valores Inteiros*/
void ADD();
void SUB();
void MUL();
void DIV();
/*Valores Reais*/
void ADDF();
void SUBF();
void MULF();
void DIVF();
int tDeclarados, tUsados;
int gerarLabel();
int pegarLabel();
void LABEL(int label);
void GOTO(int label);
void GOFALSE(int label);
void GOTRUE(int label);
void DMEM(int size);
void AMEM(int size);
void CALL(CelulaTabela celulaTabela);
void INIPR(int size);
void RET(int size);
void HALT();

#endif //COMPILADOR_MAQPILHA_H
