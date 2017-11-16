//
// Created by mateus and wellington on 31/10/2017.
//

#include "../include/analisadorsintatico.h"

int i;

int analisadorSintatico() {
    tpos = 0; loop = 0; tipo = -1;
    printf("\n\nTokens Com Sucesso - Analisador Sintatico:\n");
    imprimirToken(tpos);
    Prog();
    mostrarTabela();
    return 1;
}

/*Gramatica de Programa*/
void Prog() {
    while (faltaToken()) {
        if (reconhece(PR, PROTOTIPO)) {
            novoToken();
            if (Tipo() || reconhece(PR, SEMRETORNO)) {
                novoToken();
                if (!reconheceID()) erro(AS_FALTAID);
                novoToken();
                if (!reconhece(SN, ABREPARENTESES)) erro(AS_FALTAPARENTESE);
                novoToken();
                TipoParamOpc();
                if (!reconhece(SN, FECHAPARENTESE)) erro(AS_FALTAPARENTESE);
                novoToken();
                while (1) {
                    if (reconhece(SN, VIRGULA)) {
                        novoToken();
                        if (!reconhece(SN, ABREPARENTESES)) erro(AS_FALTAPARENTESE);
                        novoToken();
                        TipoParamOpc();
                        if (!reconhece(SN, FECHAPARENTESE)) erro(AS_FALTAPARENTESE);
                        novoToken();
                    } else break;
                }
                if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_FALTAPONTOEVIRGULA);
                novoToken();
            } else erro(AS_ERRO);
        } else if (reconhece(PR, SEMRETORNO)) {
            atualizaTipo();
            novoToken();
            if (!reconheceID()) erro(AS_FALTAID);
            tauxfun = tokens[tpos];
            inserir(tipo, tauxfun.lexema, !ZOMBIE, GLOBAL, tauxfun.linha + 1);
            alterar(tauxfun.lexema, LOCAL);
            novoToken();
            if (!reconhece(SN, ABREPARENTESES)) erro(AS_FALTAPARENTESE);
            Funcao();
            alterar(tauxfun.lexema, GLOBAL);
            excluirLocais();
        } else if (Tipo()) {
            atualizaTipo();
            novoToken();
            if (!reconheceID()) erro(AS_FALTAID);
            taux = tokens[tpos];
            novoToken();
            if (reconhece(SN, ABREPARENTESES)) {
                tauxfun = tokens[tpos-1];
                inserir(tipo, tauxfun.lexema, !ZOMBIE, GLOBAL, tauxfun.linha + 1);
                alterar(tauxfun.lexema, LOCAL);
                Funcao();
                alterar(tauxfun.lexema, GLOBAL);
                excluirLocais();
            } else if (reconhece(SN, VIRGULA)) {
                inserir(tipo, taux.lexema, !ZOMBIE, GLOBAL, taux.linha + 1);
                while (reconhece(SN, VIRGULA)) {
                    novoToken();
                    if (!reconheceID()) erro(AS_FALTAID);
                    inserir(tipo, tokens[tpos].lexema, !ZOMBIE, GLOBAL, tokens[tpos].linha + 1);
                    novoToken();
                }
                if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_FALTAPONTOEVIRGULA);
                novoToken();
            } else if (reconhece(SN, PONTOEVIRGULA)) {
                inserir(tipo, taux.lexema, !ZOMBIE, GLOBAL, taux.linha + 1);
                novoToken();
            } else {
                erro(AS_FALTAPONTOEVIRGULA);
            }
        } else {loop++;}
        if (loop > 10) erro(ERRO_INTERNO);
    }
}

void Funcao() {
    printf("\n-INI FUN-\n");
    novoToken();
    TipoParam();
    novoToken();
    if (!reconhece(SN, FECHAPARENTESE)) erro(AS_FALTAPARENTESE);
    novoToken();
    if (!reconhece(SN, ABRECHAVES)) erro(AS_FALTACHAVES);
    novoToken();
    while(1) {
        if (Tipo()) {
            atualizaTipo();
            novoToken();
            if (!reconheceID()) erro(AS_FALTAID);
            inserir(tipo, tokens[tpos].lexema, 0, LOCAL, tokens[tpos].linha + 1);
            novoToken();
            while (1) {
                if (reconhece(SN, VIRGULA)) {
                    novoToken();
                    if (!reconheceID()) erro(AS_FALTAID);
                    inserir(tipo, tokens[tpos].lexema, !ZOMBIE, LOCAL, tokens[tpos].linha + 1);
                    novoToken();
                } else break;
            }
            if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_FALTAPONTOEVIRGULA);
            novoToken();
        } else break;
    }

    while (1) {
        if (!reconhece(SN, FECHACHAVES)) {
            Cmd();
            if (!reconhece(SN, FECHACHAVES)) novoToken();
        } else break;
    }
    if (!reconhece(SN, FECHACHAVES)) erro(AS_FALTACHAVES);
    novoToken();
    printf("\n-FIM FUN-\n");
}

/*Gramatica de Tipo*/
int Tipo() {
    if (tokens[tpos].categoria != PR ||
            (tokens[tpos].codigo != INTEIRO &&
                    tokens[tpos].codigo != REAL &&
                    tokens[tpos].codigo != CARACTER &&
                    tokens[tpos].codigo != BOOLEANO))
    {
        return 0;
    }
    return 1;
}

/*Gramatica de TipoParam*/
void TipoParam() {
    if (reconhece(PR, SEMPARAM)) {
    } else if (Tipo()) {
        atualizaTipo();
        novoToken();
        if (!reconheceID()) erro(AS_FALTAID);
        inserir(tipo, tokens[tpos].lexema, ZOMBIE, LOCAL, tokens[tpos].linha + 1);
        novoToken();
        while (1) {
            if (reconhece(SN, VIRGULA)) {
                novoToken();
                if (!Tipo()) erro(AS_ERRO);
                atualizaTipo();
                novoToken();
                if (!reconheceID()) erro(AS_ERRO);
                inserir(tipo, tokens[tpos].lexema, ZOMBIE, LOCAL, tokens[tpos].linha + 1);
            } else {
                retornarToken();
                break;
            }
        }
    } else {
        erro(AS_ERRO);
    }
}

/*Gramatica de TipoParamOpc*/
void TipoParamOpc() {
    if (reconhece(PR, SEMPARAM)) {
    } else if (Tipo()) {
        novoToken();
        if (reconheceID()) novoToken();
        while (1) {
            if (reconhece(SN, VIRGULA)) {
                novoToken();
                if (!Tipo()) erro(AS_ERRO);
                novoToken();
                if (reconheceID()) novoToken();
            } else break;
        }
    }
}

/*Gramatica de Comandos*/
void Cmd() {
    if (reconhece(PR, SE)) {
        novoToken();
        Se();
    } else if (reconhece(PR, ENQUANTO)) {
        novoToken();
        Enquanto();
    } else if (reconhece(PR, PARA)) {
        novoToken();
        Para();
    } else if (reconhece(PR, RETORNE)) {
        novoToken();
        Retorne();
    } else if (reconheceID()) {
        novoToken();
        if (reconhece(SN, ATRIBUICAO)) {
            printf("\n-INI ATR-\n");
            novoToken();
            ExprSimp();
            if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_FALTAPONTOEVIRGULA);
            printf("-FIM ATR-\n\n");
        } else {
            if (reconhece(SN, ABREPARENTESES)) {
                printf("\n-INI FID-\n");
                novoToken();
                if (!reconhece(SN, FECHAPARENTESE)) {
                    Expr();
                    while (reconhece(SN, VIRGULA)) {
                        novoToken();
                        Expr();
                    }
                }
                if (!reconhece(SN, FECHAPARENTESE)) erro(AS_FALTAPARENTESE);
                novoToken();
                if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_FALTAPONTOEVIRGULA);
                printf("-FIM FID-\n\n");
            } else {
                erro(AS_ERRO);
            }
        }
    } else if (reconhece(SN, ABRECHAVES)) {
        novoToken();
        while (!reconhece(SN, FECHACHAVES)) {
            Cmd();
            novoToken();
        }
    } else if (reconhece(SN, PONTOEVIRGULA)) {}
}

void Se() {
    if (reconhece(SN, ABREPARENTESES)) {
        printf("\n-INI  SE-\n");
        novoToken();
        Expr();
        if (!reconhece(SN, FECHAPARENTESE)) erro(AS_SE);
        novoToken();
        Cmd();
        novoToken();
        printf("\n-FIM  SE-\n");
        Senao();
    } else {
        Senao();
    }
}

void Senao() {
    if (reconhece(PR, SENAO)) {
        printf("\n-INI SEN-\n");
        novoToken();
        Cmd();
        novoToken();
        printf("\n-FIM SEN-\n");
    } else ;
}

void Enquanto() {
    if (reconhece(SN, ABREPARENTESES)) {
        novoToken();
        Expr();
        if (reconhece(SN, FECHAPARENTESE)) {
            novoToken();
            Cmd();
            novoToken();
        } else erro(AS_ENQUANTO);
    }
}

void Para() {
    if (reconhece(SN, ABREPARENTESES)) {
        printf("\n-INI PAR-\n");
        novoToken();
        if (!Atrib()) erro(AS_PARA);
        if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_PARA);
        novoToken();
        Expr();
        if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_PARA);
        novoToken();
        if (!Atrib()) erro(AS_PARA);
        if (reconhece(SN, FECHAPARENTESE)) {
            novoToken();
            Cmd();
            novoToken();
            printf("\n-FIM PAR-\n");
        } else erro(AS_PARA);
    }
}

void Retorne() {
    if (!reconhece(SN, PONTOEVIRGULA)) {
        printf("\n-INI RET-\n");
        Expr();
        if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_FALTAPONTOEVIRGULA);
        printf("\n-FIM RET-\n");
    }
}

/*Gramatica de Atribuição*/
int Atrib() {
    if (tokens[tpos].categoria == ID) {
        printf("\n-INI ATR\n");
        novoToken();
        if (tokens[tpos].categoria == SN &&
            tokens[tpos].codigo == ATRIBUICAO) {
            novoToken();
            ExprSimp();
            printf("\n-FIM ATR\n");
            return 1;
        } else {
            retornarToken();
            printf("\n-FIM ATR\n");
        }
    }
    return 0;
}

/*Gramatica de Expressões*/
void Expr() {
    i++;
    int j;
    for (j = 0; j < i; j++) printf("-");
    printf("INI EXP-\n");
    ExprSimp();
    //novoToken();
    if (OpRel()) {
        printf("\n-INI REL-\n");
        novoToken();
        ExprSimp();
        printf("\n-FIM REL-\n");
    } else ;
    i--;
    printf("\n");
    for (j = 0; j < i; j++) printf("-");
    printf("-FIM EXP-\n");
}

/*Gramatica de Expressões Simples*/
void ExprSimp() {
    if (reconhece(SN, ADICAO) || reconhece(SN, SUBTRACAO)) novoToken();
    Termo();
    while (reconhece(SN, ADICAO) || reconhece(SN, SUBTRACAO) || reconhece(SN, OR)) {
        imprimirToken(tpos);
        novoToken();
        Termo();
    }
}

void Termo() {
    Fator();
    while (reconhece(SN, MULTIPLICACAO) || reconhece(SN, DIVISAO) || reconhece(SN, AND)) {
        imprimirToken(tpos);
        novoToken();
        imprimirToken(tpos);
        Fator();
        //retornarToken();
    }
}

void Fator() {
    if (tokens[tpos].categoria == INTCON ||
        tokens[tpos].categoria == REALCON ||
            (tokens[tpos].categoria == CT_C && tokens[tpos].lexema != "!")||
        tokens[tpos].categoria == CT_L)
    {
        imprimirToken(tpos);
        novoToken();
    } else if (reconheceID()) {
        imprimirToken(tpos);
        novoToken();
        if (reconhece(SN, ABREPARENTESES)) {
            imprimirToken(tpos);
            novoToken();
            if (!reconhece(SN, FECHAPARENTESE)) {
                imprimirToken(tpos);
                Expr();
                while (reconhece(SN, VIRGULA)) {
                    novoToken();
                    Expr();
                }
            }
            if (!reconhece(SN, FECHAPARENTESE)) erro(AS_FALTAPARENTESE);
            imprimirToken(tpos);
            novoToken();
        }
    } else if (reconhece(SN, ABREPARENTESES)) {
        imprimirToken(tpos);
        novoToken();
        imprimirToken(tpos);
        Expr();
        imprimirToken(tpos);
        if (!reconhece(SN, FECHAPARENTESE)) erro(AS_FALTAPARENTESE);
        novoToken();
    } else if (reconhece(SN, NOT)) {
        novoToken();
        Fator();
    } else {
        erro(AS_FALTAEXPREOUNUM);
    }

}

/*Gramatica de Operador Relacional*/
int OpRel() {
    if (tokens[tpos].categoria == SN &&
            (tokens[tpos].codigo == IGUAL || tokens[tpos].codigo == DIFERENTE ||
                    tokens[tpos].codigo == MENORIGUAL || tokens[tpos].codigo == MENOR ||
                    tokens[tpos].codigo == MAIORIGUAL || tokens[tpos].codigo == MAIOR))
    {
        return 1;
    }
    return 0;
}

int faltaToken() {
    return tkpos != tpos;
}

int novoToken() {
    if (tkpos < tpos) erro(ERRO_INTERNO);
    tpos++;
    imprimirToken(tpos);
    return tpos;
}

int retornarToken() {
    return tpos--;
}

void erro(int erro) {
    tpos--;
    logErro(erro, 1, tokens[tpos].linha);
}

void atualizaTipo() {
    tipo = tokens[tpos].codigo;
}

int reconheceID() {
    return tokens[tpos].categoria == ID;
}

int reconhece(int categoria, int codigo) {
    return tokens[tpos].categoria == categoria &&
           tokens[tpos].codigo == codigo;
}