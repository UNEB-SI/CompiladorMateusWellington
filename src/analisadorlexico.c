//
// Created by mateus on 07/10/2017.
//
#include "../include/analisadorlexico.h"

const char *Pr[PR_TAM] = {"inteiro", "real", "caracter", "cadeia", "booleano",
                          "semretorno", "retorne", "se", "senao", "enquanto", "para", "semparam", "prototipo"};
char *Sn[SN_TAM] = {"AND", "OR", "NOT", "ADICAO", "SUBTRACAO", "MULTIPLICACAO", "DIVISAO", "MENOR", "MAIOR", "IGUAL", "MENORIGUAL", "MAIORIGUAL",
                    "ATRIBUICAO", "VIRGULA", "PONTOEVIRGULA", "ABREPARENTESES", "FECHAPARENTESE", "DIFERENTE", "ABRECHAVES", "FECHACHAVES"};

int analisadorLexico(FILE *file) {
    char c = ' ';
    char caux[100] = "";
    int estado = 0;
    int tk_pos = 0;
    linha = 0;
    printf("Codigo Lido:\n");
    while (1) {
        if (estado == 0 && feof(file)) {
            break;
        }
        switch (estado) {
            case 0:
                c = (char) getc(file);
                strcpy(caux, "");
                if (c == '\n' || c == ' ') {
                    putchar(c);
                    if (c == '\n') {
                        linha++;
                    }
                    break;
                }
                else if (c == EOF) estado = 1;
                else if (isdigit(c)) estado = 2;
                else if (islower(c) || isupper(c)) estado = 7;
                else if (c == '\'') estado = 9; //APOSTROFO
                else if (c == '\"') estado = 12; //ASPAS
                else if (c == '/') estado = 14; //BARRA
                else if (c == '>') estado = 18; //MAIOR
                else if (c == '<') estado = 21; //MENOR
                else if (c == '=') estado = 24; //IGUAL
                else if (c == '!') estado = 27; //DIFERENTE
                else if (c == '(') estado = 30; //ABREPARENTESES
                else if (c == ')') estado = 31; //FECHAPARENTESES
                else if (c == '{') estado = 32; //ABRE CHAVES
                else if (c == '}') estado = 33; //FECHA CHAVES
                else if (c == '&') estado = 34; //E
                else if (c == '|') estado = 37; //OU
                else if (c == '+') estado = 40; //ADICAO
                else if (c == '-') estado = 42; //SUBTRACAO
                else if (c == '*') estado = 44; //MULTIPLICACAO
                else if (c == ',') estado = 47; //VIRGULA
                else if (c == ';') estado = 49; //PONTOEVIRGULA
                else logErro(AL_CARACINVALIDO, 1, linha);
                break;
            case 1: //ESTADODEACEITACAO
                estado = 0;
                break;
            case 2:
                while (isdigit(c)) {
                    strcat(caux, &c);
                    c = (char) getc(file);
                }
                if (c == '.') {
                    estado = 4;
                } else {
                    ungetc(c, file);
                    estado = 3;
                }
                break;
            case 3: //ESTADODEACEITACAO INTEIRO
                printf("%s", caux);
                criarToken(INTCON, caux, -1);
                estado = 0;
                break;
            case 4:
                strcat(caux, &c);
                c = (char) getc(file);
                if (isdigit(c)) {
                    strcat(caux, &c);
                    estado = 5;
                } else {
                    logErro(AL_CARACINVALIDO, 1, linha);
                }
                break;
            case 5:
                while (isdigit(c)) {
                    strcat(caux, &c);
                    c = (char) getc(file);
                }
                if (c == '.') {
                    logErro(AL_CARACINVALIDO, 1, linha);
                }
                ungetc(c, file);
                estado = 6;
                break;
            case 6: //ESTADODEACEITACAO REAL
                printf("%s", caux);
                criarToken(REALCON, caux, -1);
                estado = 0;
                break;
            case 7:
                while (islower(c) || isupper(c) ||
                        isdigit(c) || c == '_') {
                    strcat(caux, &c);
                    c = (char) getc(file);
                }
                ungetc(c, file);
                estado = 8;
                break;
            case 8: //ESTADODEACEITACAO PR ou ID
                printf("%s", caux);
                analisePRouID(caux);
                estado = 0;
                break;
            case 9:
                c = (char) getc(file);
                if (isprint(c)) {
                    strcat(caux, &c);
                    estado = 10;
                } else if (c == '\\' ) {
                    strcat(caux, &c);
                    c = (char) getc(file);
                    if (c == 'n' || c == '0') {
                        strcat(caux, &c);
                        estado = 10;
                    } else {
                        logErro(AL_CARACINVALIDO, 1, linha);
                    }
                } else if (c == '\'') {
                    estado = 11;
                } else {
                    logErro(AL_CARACINVALIDO, 1, linha);
                }
                break;
            case 10:
                c = (char) getc(file);
                if (c == '\'') {
                    estado = 11;
                }
                break;
            case 11: //ESTADODEACEITACAO CARACTER
                printf("%s", caux);
                criarToken(CT_C, caux, -1);
                estado = 0;
                break;
            case 12:
                c = (char) getc(file);
                while ((isprint(c) || c == '_' ||
                       c == ' ' || c == '\n' || c == ':') && c != '\"') {
                    //printf("OI");
                    //putchar(c);
                    strcat(caux, &c);
                    c = (char) getc(file);
                }
                if (c == '\"') {
                    estado = 13;
                } else {
                    logErro(AL_CARACINVALIDO, 1, linha);
                }
                break;
            case 13: //ESTADODEACEITACAO CADEIA CARACTER
                printf("%s", caux);
                strcpy(literal[literalpos], caux);
                criarToken(CT_L, literal[literalpos], literalpos);
                literalpos++;
                estado = 0;
                break;
            case 14:
                c = (char) getc(file);
                if (c == '*') {
                    estado = 15;
                } else {
                    ungetc(c, file);
                    estado = 46;
                }
                break;
            case 15:
                c = (char) getc(file);
                linhaAux = linha;
                while (c != '*') {
                    if (c == '\n') {
                        linha++;
                        c = ' ';
                    }
                    if (feof(file)) {
                        logErro(AL_COMENTARIO, 0, linhaAux);
                    } else {
                        c = (char) getc(file);
                    }
                }
                c = (char) getc(file);
                if (c != '*') {
                    estado = 16;
                }
                ungetc(c, file);
                break;
            case 16:
                c = (char) getc(file);
                if (c == '/' ) {
                    estado = 17;
                } else {
                    ungetc(c, file);
                    estado = 15;
                }
                break;
            case 17: //ESTADODEACEITACAO COMENTARIO
                printf("%s", caux);
                estado = 0;
                break;
            case 18:
                strcat(caux, &c);
                c = (char) getc(file);
                if (c == '=') {
                    strcat(caux, &c);
                    estado = 19;
                } else {
                    ungetc(c, file);
                    estado = 20;
                }
                break;
            case 19: //ESTADODEACEITACAO MAIOR IGUAL
                printf("%s", caux);
                criarToken(SN, caux, MAIORIGUAL);
                estado = 0;
                break;
            case 20: //ESTADODEACEITACAO MAIOR
                printf("%s", caux);
                criarToken(SN, caux, MAIOR);
                estado = 0;
                break;
            case 21:
                strcat(caux, &c);
                c = (char) getc(file);
                if (c == '=') {
                    strcat(caux, &c);
                    estado = 22;
                } else {
                    ungetc(c, file);
                    estado = 23;
                }
                break;
            case 22: //ESTADODEACEITACAO MENOR IGUAL
                printf("%s", caux);
                criarToken(SN, caux, MENORIGUAL);
                estado = 0;
                break;
            case 23: //ESTADODEACEITACAO MENOR
                printf("%s", caux);
                criarToken(SN, caux, MENOR);
                estado = 0;
                break;
            case 24:
                strcat(caux, &c);
                c = (char) getc(file);
                if (c == '=') {
                    strcat(caux, &c);
                    estado = 25;
                } else {
                    ungetc(c, file);
                    estado = 26;
                }
                break;
            case 25: //ESTADODEACEITACAO IGUAL
                printf("%s", caux);
                criarToken(SN, caux, IGUAL);
                estado = 0;
                break;
            case 26: //ESTADODEACEITACAO ATRIBUICAO
                printf("%s", caux);
                criarToken(SN, caux, ATRIBUICAO);
                estado = 0;
                break;
            case 27:
                strcat(caux, &c);
                c = (char) getc(file);
                if (c == '=') {
                    strcat(caux, &c);
                    estado = 28;
                } else {
                    ungetc(c, file);
                    estado = 29;
                }
                break;
            case 28: //ESTADODEACEITACAO DIFERENTE
                printf("%s", caux);
                criarToken(SN, caux, DIFERENTE);
                estado = 0;
                break;
            case 29: //ESTADODEACEITACAO NOT
                printf("%s", caux);
                criarToken(SN, caux, NOT);
                estado = 0;
                break;
            case 30: //ESTADODEACEITACAO ABRE PARENTESES
                strcat(caux, &c);
                printf("%s", caux);
                criarToken(SN, caux, ABREPARENTESES);
                estado = 0;
                break;
            case 31: //ESTADODEACEITACAO FECHA PARENTESES
                strcat(caux, &c);
                printf("%s", caux);
                criarToken(SN, caux, FECHAPARENTESE);
                estado = 0;
                break;
            case 32: //ESTADODEACEITACAO ABRE CHAVES
                strcat(caux, &c);
                printf("%s", caux);
                criarToken(SN, caux, ABRECHAVES);
                estado = 0;
                break;
            case 33: //ESTADODEACEITACAO FECHA CHAVES
                strcat(caux, &c);
                printf("%s", caux);
                criarToken(SN, caux, FECHACHAVES);
                estado = 0;
                break;
            case 34:
                strcat(caux, &c);
                c = (char) getc(file);
                if (c == '&') {
                    estado = 35;
                } else {
                    logErro(AL_CARACINVALIDO, 1, linha);
                }
                break;
            case 35:
                strcat(caux, &c);
                c = (char) getc(file);
                if (c != '&') {
                    estado = 36;
                }
                break;
            case 36: //ESTADODEACEITACAO AND
                printf("%s", caux);
                criarToken(SN, caux, AND);
                estado = 0;
                break;
            case 37:
                strcat(caux, &c);
                c = (char) getc(file);
                if (c == '|') {
                    estado = 38;
                } else {
                    logErro(AL_CARACINVALIDO, 1, linha);
                }
                break;
            case 38:
                strcat(caux, &c);
                c = (char) getc(file);
                if (c != '|') {
                    estado = 39;
                } else {
                    logErro(AL_CARACINVALIDO, 1, linha);
                }
                break;
            case 39: //ESTADODEACEITACAO OR
                printf("%s", caux);
                criarToken(SN, caux, OR);
                estado = 0;
                break;
            case 40:
                strcat(caux, &c);
                c = (char) getc(file);
                if (c != '+') {
                    ungetc(c, file);
                    estado = 41;
                } else {
                    logErro(AL_CARACINVALIDO, 1, linha);
                }
                break;
            case 41: //ESTADODEACEITACAO ADICAO
                printf("%s", caux);
                criarToken(SN, caux, ADICAO);
                estado = 0;
                break;
            case 42:
                strcat(caux, &c);
                c = (char) getc(file);
                if (c != '-') {
                    ungetc(c, file);
                    estado = 43;
                } else {
                    logErro(AL_CARACINVALIDO, 1, linha);
                }
                break;
            case 43: //ESTADODEACEITACAO SUBTRACAO
                printf("%s", caux);
                criarToken(SN, caux, SUBTRACAO);
                estado = 0;
                break;
            case 44:
                strcat(caux, &c);
                c = (char) getc(file);
                if (c != '*') {
                    ungetc(c, file);
                    estado = 45;
                } else {
                    logErro(AL_CARACINVALIDO, 1, linha);
                }
                break;
            case 45: //ESTADODEACEITACAO MULTIPLICACAO
                printf("%s", caux);
                criarToken(SN, caux, MULTIPLICACAO);
                estado = 0;
                break;
            case 46: //ESTADODEACEITACAO DIVISAO
                printf("/");
                criarToken(SN, caux, DIVISAO);
                estado = 0;
                break;
            case 47:
                strcat(caux, &c);
                c = (char) getc(file);
                if (c != ',') {
                    ungetc(c, file);
                    estado = 48;
                } else {
                    logErro(AL_CARACINVALIDO, 1, linha);
                }
                break;
            case 48: //ESTADODEACEITACAO VIRGULA
                printf("%s", caux);
                criarToken(SN, "/", VIRGULA);
                estado = 0;
                break;
            case 49: //ESTADODEACEITACAO PONTO E VIRGULA
                strcat(caux, &c);
                printf("%s", caux);
                criarToken(SN, caux, PONTOEVIRGULA);
                estado = 0;
                break;
            default: break;
        }
    }
    printf("\n\nTokens Gerados - Analisador Lexico:\n");
    for (tk_pos = 0; tk_pos < tkpos; tk_pos++) {
        imprimirToken(tk_pos);
    }
    return 1;
}

void analisePRouID(char valor[]) {
    int i = 0;
    int resultado = -1;
    char teste[SIZE];
    strcpy(teste, valor);
    for (i = 0; i < PR_TAM; i++) {
        if (strcmp(Pr[i], valor) == 0) {
            resultado = i;
        }
    }

    if (resultado != -1) {
        criarToken(PR, valor, resultado);
    } else {
        //Verificar se ID já foi adicionado a tabela
        resultado = -1;
        for (i = 0; i < SIZE; i++) {
            if (strcmp(ids[i], valor) == 0) {
                resultado = i;
            }
        }

        if (resultado == -1) {
            strcpy(ids[idpos], valor);
            criarToken(ID, ids[idpos], idpos);
            idpos++;
        } else {
            criarToken(ID, ids[resultado], resultado);
        }

    }
}

void criarToken(int categoria, char* valor, int codigo) {
    Token tk;
    tk.categoria = categoria;
    tk.linha = linha;
    switch (categoria) {
        case INTCON:
        case REALCON:
        case CT_C:
            strcpy(tk.lexema, valor);
            break;
        case ID:
            strcpy(tk.lexema, valor);
            break;
        case PR:
            tk.codigo = codigo;
            break;
        case SN:
            tk.codigo = codigo;
            break;
        case CT_L:
            strcpy(tk.lexema, valor);
            break;
        default:
            break;
    }
    armazenarToken(tk);
}

void armazenarToken(Token tk) {
    if (tkpos == 0) {
        tksize = 1;
        tokens = malloc(SIZE * sizeof(Token));
    } else if (tkpos >  (tksize * SIZE) - 2) {
        logErro(MAX_ALOCACAO, 0);
    }

    tokens[tkpos] = tk;
    tkpos++;
}

void imprimirToken(int tpos) {
    switch (tokens[tpos].categoria) {
        case INTCON:
            printf("<INTCON / %s>\n", tokens[tpos].lexema);
            break;
        case REALCON:
            printf("<REALCON / %s>\n", tokens[tpos].lexema);
            break;
        case ID:
            printf("<ID / %s>\n", tokens[tpos].lexema);
            break;
        case PR:
            printf("<PR / %s>\n", Pr[tokens[tpos].codigo]);
            break;
        case SN:
            printf("<SN / %s>\n", Sn[tokens[tpos].codigo]);
            break;
        case CT_C:
            printf("<CT_C / %s>\n", tokens[tpos].lexema);
            break;
        case CT_L:
            printf("<CT_L / %s>\n", tokens[tpos].lexema);
            break;
        default:
            printf("<%d / val = %s>\n", tokens[tpos].categoria, tokens[tpos].lexema);
            break;
    }
}