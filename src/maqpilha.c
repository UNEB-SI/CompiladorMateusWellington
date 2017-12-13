//
// Created by mateus e wellington on 05/12/2017.
//

#include "../include/maqpilha.h"

void configurarPilha() {
    file = fopen("codigo.cmp", "w+");;
}

void excluirPilha() {
    fclose(file);
}

void PUSH(Token tk) {
    char str[20] = "PUSH ";
    strcat(str, tk.lexema);
    strcat(str, "\n");
    fwrite(str, 1, sizeof(str), file);
}

void POP() {
    char str[10] = "POP\n";
    fwrite(str, 1, sizeof(str), file);
}

void COPY() {
    char str[10] = "COPY\n";
    fwrite(str, 1, sizeof(str), file);
}

void STOR(CelulaTabela celulaTabela) {
    char str[20] = "STOR ";
    strcat(str, celulaTabela.posicao);
    strcat(str, "\n");
    fwrite(str, 1, sizeof(str), file);
}

void LOAD(CelulaTabela celulaTabela){
    char str[20] = "LOAD ";
    strcat(str, celulaTabela.posicao);
    strcat(str, "\n");
    fwrite(str, 1, sizeof(str), file);
}

void MUL() {
    char str[5] = "MUL\n";
    fwrite(str, 1, sizeof(str), file);
}

void DIV() {
    char str[5] = "DIV\n";
    fwrite(str, 1, sizeof(str), file);
}

void ADD() {
    char str[5] = "ADD\n";
    fwrite(str, 1, sizeof(str), file);
}
void SUB() {
    char str[5] = "SUB\n";
    fwrite(str, 1, sizeof(str), file);
}

void MULF() {
    char str[6] = "MULF\n";
    fwrite(str, 1, sizeof(str), file);
}

void DIVF() {
    char str[6] = "DIVF\n";
    fwrite(str, 1, sizeof(str), file);
}

void ADDF() {
    char str[6] = "ADDF\n";
    fwrite(str, 1, sizeof(str), file);
}
void SUBF() {
    char str[6] = "SUBF\n";
    fwrite(str, 1, sizeof(str), file);
}

int gerarLabel() {
    return ++tDeclarados;
}

int pegarLabel() {
    return ++tUsados;
}

void LABEL(int label) {
    char str[20] = "LABEL L";
    char buffer[10];
    sprintf(buffer, "%d", label);
    strcat(str, buffer);
    strcat(str, "\n");
    fwrite(str, 1, sizeof(str), file);
}

void GOTO(int label) {
    char str[20] = "GOTO L";
    char buffer[10];
    sprintf(buffer, "%d", label);
    strcat(str, buffer);
    strcat(str, "\n");
    fwrite(str, 1, sizeof(str), file);
}

void GOFALSE(int label) {
    char str[20] = "GOFALSE L";
    char buffer[10];
    sprintf(buffer, "%d", label);
    strcat(str, buffer);
    strcat(str, "\n");
    fwrite(str, 1, sizeof(str), file);
}

void GOTRUE(int label) {
    char str[20] = "GOTRUE L";
    char buffer[10];
    sprintf(buffer, "%d", label);
    strcat(str, buffer);
    strcat(str, "\n");
    fwrite(str, 1, sizeof(str), file);
}

void DMEM(int size) {
    char str[20] = "DMEM ";
    char buffer[10];
    sprintf(buffer, "%d", size);
    strcat(str, buffer);
    strcat(str, "\n");
    fwrite(str, 1, sizeof(str), file);
}


void AMEM(int size) {
    char str[20] = "AMEM ";
    char buffer[10];
    sprintf(buffer, "%d", size);
    strcat(str, buffer);
    strcat(str, "\n");
    fwrite(str, 1, sizeof(str), file);
}

void HALT() {
    char* str = "HALT";
    fwrite(str, 1, sizeof(str), file);
}
