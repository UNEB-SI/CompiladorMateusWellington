//
// Created by mateus e wellington on 05/12/2017.
//

#include "../include/maqpilha.h"

void configurarPilha(FILE *arq) {
    file = arq;
    topoPilha = 0;
}

void PUSH(Token tk){
    char str[20] = "PUSH ";
    strcat(str, tk.lexema);
    strcat(str, "\n");
    fwrite(str, 1, sizeof(str), file);
}

void STOR(Token tk){
    char str[20] = "STOR ";
    strcat(str, tk.lexema);
    strcat(str, "\n");
    fwrite(str, 1, sizeof(str), file);
}

void LOADD(Token tk){
    char str[20] = "LOAD ";
    strcat(str, tk.lexema);
    strcat(str, "\n");
    fwrite(str, 1, sizeof(str), file);
}

void LOAD(float var) {
    pilha[topoPilha] = var;
    topoPilha++;
    char buffer[10];
    snprintf(buffer, sizeof buffer, "%f", var);
    char str[20] = "LOAD ";
    strcat(str, buffer);
    strcat(str, "\n");
    fwrite(str, 1, sizeof(str), file);
}

void MUL() {
    char* str = "MUL\n";
    fwrite(str, 1, sizeof(str), file);
}

void DIV() {
    char* str = "DIV\n";
    fwrite(str, 1, sizeof(str), file);
}

void ADD() {
    char* str = "ADD\n";
    fwrite(str, 1, sizeof(str), file);
}
void SUB() {
    char* str = "SUB\n";
    fwrite(str, 1, sizeof(str), file);
}

void MULF() {
    char* str = "MUL\n";
    fwrite(str, 1, sizeof(str), file);
}

void DIVF() {
    char* str = "DIV\n";
    fwrite(str, 1, sizeof(str), file);
}

void ADDF() {
    char* str = "ADD\n";
    fwrite(str, 1, sizeof(str), file);
}
void SUBF() {
    char* str = "SUB\n";
    fwrite(str, 1, sizeof(str), file);
}
