#include "../include/gerenciador.h"

void inserirVar(int tipo, char lexema[], int zombie, int escopo, int linha, int pos) {
    if(topo == 0){
        memset(tabela, 0, DIM);
    }
    strcpy(tabela[topo].lexema, lexema);
    tabela[topo].tipo = tipo;
    tabela[topo].escopo = escopo;
    tabela[topo].zombie = zombie;
    tabela[topo].linha = linha;
    char buffer[10];
    sprintf(buffer, "%d", escopo);
    strcpy(tabela[topo].posicao, buffer);
    strcpy(buffer, "");
    sprintf(buffer, "%d", ++pos);
    strcat(tabela[topo].posicao, ",");
    strcat(tabela[topo].posicao, buffer);
    topo++;
}

void inserirFuncao(int tipo, char lexema[], int zombie, int escopo, int linha, int pos) {
    char buffer[10];
    if(topo == 0){
        memset(tabela, 0, DIM);
    }
    strcpy(tabela[topo].lexema, lexema);
    tabela[topo].tipo = tipo;
    tabela[topo].escopo = escopo;
    tabela[topo].zombie = zombie;
    tabela[topo].linha = linha;
    tabela[topo].label = pos;
    if (pos != -1) {
        strcpy(tabela[topo].posicao, "L");
        sprintf(buffer, "%d", pos);
        strcat(tabela[topo].posicao, buffer);
    }
    topo++;
}

void alterarParamVar(int posParam) {
    int aux, aux2 = 0;
    char buffer[10];
    for (aux = topo - 1; aux >= topo - posParam; aux--) {
        aux2++;
        sprintf(buffer, "%d", tabela[aux].escopo);
        strcpy(tabela[aux].posicao, buffer);
        strcpy(buffer, "");
        sprintf(buffer, "%d", -(aux2 + 2));
        strcat(tabela[aux].posicao, ",");
        strcat(tabela[aux].posicao, buffer);
    }
}

CelulaTabela consultar(char lexema[]) {
    CelulaTabela cedula;
    cedula.tipo = -1;
    int aux;
    for (aux = 0; aux < topo; aux++) {
        if (strcmp(tabela[aux].lexema, lexema) == 0) {
            cedula = tabela[aux];
            if (cedula.zombie != ZOMBIE) break;
        }
    }
    return cedula;
}

CelulaTabela consultarLocais(char lexema[]) {
    CelulaTabela cedula;
    cedula.tipo = -1;
    int aux;
    for (aux = topo; aux > 0; aux--) {
        if (strcmp(tabela[aux].lexema, lexema) == 0 && tabela[aux].escopo == LOCAL) {
            cedula = tabela[aux];
            break;
        }
    }
    return cedula;
}

CelulaTabela consultarLocaisEGlobais(char lexema[]) {
    CelulaTabela cedula;
    cedula.tipo = -1;
    int aux;
    for (aux = topo; aux >= 0; aux--) {
        if (strcmp(tabela[aux].lexema, lexema) == 0) {
            if (tabela[aux].zombie == ZOMBIE && tabela[aux].escopo == GLOBAL) break;
            cedula = tabela[aux];
            break;
        }
    }
    return cedula;
}

CelulaTabela consultarTipoParametro(char lexema[], int posicao) {
    CelulaTabela cedula;
    cedula.tipo = -1;
    int aux, pos = 0;
    for (aux = 0; aux < topo + 1; aux++) {
        if (pos > 0 && tabela[aux].zombie != ZOMBIE) break;
        if (pos > 0 &&
            strcmp(tabela[aux].lexema, lexema) != 0 &&
            pos == posicao) {
            cedula = tabela[aux];
            break;
        }
        if (pos > 0) pos++;
        if (strcmp(tabela[aux].lexema, lexema) == 0/* && tabela[aux].zombie == ZOMBIE*/) {
            pos = 1;
        }
    }
    return cedula;
}

void excluirLocais(char* lexema, int param){
    int aux, pos;
    topo--;
    while(1) {
        if(tabela[topo].zombie != ZOMBIE && tabela[topo].escopo == LOCAL)
            topo--;
        else break;
    }
    for (aux = topo; aux > 0; aux--) {
        if (strcmp(tabela[aux].lexema, lexema) == 0) {
            break;
        }
    }
    for (pos = aux + param; pos >= aux; pos--) {
        if (tabela[pos].zombie == ZOMBIE) tabela[pos].escopo = GLOBAL;
    }
    topo++;
}

void mostrarTabela() {
    int i;
    printf("\n########## TABELA ##########");
    for (i = topo; i >= 0; i--) {
        printf("\nLexema: %s", tabela[i].lexema);
        switch (tabela[i].tipo) {
            case INTCON: printf("\nTipo: INTEIRO"); break;
            case REALCON: printf("\nTipo: REAL"); break;
            case CT_C: printf("\nTipo: CARACTER"); break;
            case CT_L: printf("\nTipo: CADEIA"); break;
            case BOOLEANO: printf("\nTipo: BOOLEANO"); break;
            case SEMRETORNO: printf("\nTipo: SEMRETORNO"); break;
            default: break;
        }
        switch (tabela[i].escopo) {
            case GLOBAL: printf("\nEscopo: GLOBAL"); break;
            case LOCAL: printf("\nEscopo: LOCAL"); break;
            default: break;
        }
        if (tabela[i].zombie != 0) printf("\nZombie: TRUE");
        printf("\nLinha: %d", tabela[i].linha);
        printf("\nPosicao: %s", tabela[i].posicao);
        printf("\n----------------------------");
    }
    printf("\n######## FIM TABELA ########\n");
}