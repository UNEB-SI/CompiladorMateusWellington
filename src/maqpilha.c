//
// Created by mateus e wellington on 05/12/2017.
//

#include "../include/maqpilha.h"

void configurarPilha(FILE *arq) {
    file = arq;
    topoPilha = 0;
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