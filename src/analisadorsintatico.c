//
// Created by mateus and wellington on 31/10/2017.
//

#include "../include/analisadorsintatico.h"

int analisadorSintatico() {
    tpos = 0;
    printf("\n\nTokens Lidos com Sucesso:\n");
    imprimirToken(tpos);
    Prog();
    mostrarTabela();
    return 1;
}

int faltaToken() {
    //printf("\n%d %d\n", tkpos, tpos);
    return tkpos != tpos;
}

int novoToken() {
    if (tkpos < tpos) erro(ERRO_INTERNO);
    tpos++;
    imprimirToken(tpos);
    return tpos;
}

int ungetTPos() {
    return tpos--;
}

int erro(int erro) {
    tpos--;
    logErro(erro, 1, tokens[tpos].linha);
}

int reconheceID() {
    if (tokens[tpos].categoria == ID) {
        return 1;
    }
    return 0;
}

int reconhece(int categoria, int codigo) {
    if (tokens[tpos].categoria == categoria &&
        tokens[tpos].codigo == codigo) {
        return 1;
    }
    return 0;
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
                printf("\n\n\n");
                imprimirToken(tpos);
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
            novoToken();
            if (!reconheceID()) erro(AS_FALTAID);
            novoToken();
            if (!reconhece(SN, ABREPARENTESES)) erro(AS_FALTAPARENTESE);
            Funcao();
        } else if (Tipo()) {
            tipo = tokens[tpos].codigo;
            novoToken();
            if (!reconheceID()) erro(AS_FALTAID);
            taux = tokens[tpos];
            novoToken();
            if (reconhece(SN, ABREPARENTESES)) {
                Funcao();
            } else if (reconhece(SN, VIRGULA)) {
                gerenciador(INSERIR, tipo, taux.lexema, 0, GLOBAL, taux.linha + 1);
                while (reconhece(SN, VIRGULA)) {
                    novoToken();
                    if (!reconheceID()) erro(AS_FALTAID);
                    gerenciador(INSERIR, tipo, tokens[tpos].lexema, 0, GLOBAL, tokens[tpos].linha + 1);
                    novoToken();
                }
                if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_FALTAPONTOEVIRGULA);
                novoToken();
            } else if (reconhece(SN, PONTOEVIRGULA)) {
                novoToken();
            } else {
                erro(AS_FALTAPONTOEVIRGULA);
            }
        }
    }
}

void Funcao() {
    novoToken();
    TipoParam();
    novoToken();
    if (!reconhece(SN, FECHAPARENTESE)) erro(AS_FALTAPARENTESE);
    novoToken();
    if (!reconhece(SN, ABRECHAVES)) erro(AS_FALTACHAVES);
    novoToken();
    while(1) {
        if (Tipo()) {
            novoToken();
            if (!reconheceID()) erro(AS_FALTAID);
            novoToken();
            while (1) {
                if (reconhece(SN, VIRGULA)) {
                    novoToken();
                    if (!reconheceID()) erro(AS_FALTAID);
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
            novoToken();
        } else break;
    }
    if (!reconhece(SN, FECHACHAVES)) erro(AS_FALTACHAVES);
    novoToken();
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
        novoToken();
        if (!reconheceID()) erro(ERRO_INTERNO);
        novoToken();
        while (1) {
            if (reconhece(SN, VIRGULA)) {
                novoToken();
                if (!Tipo()) erro(ERRO_INTERNO);
                novoToken();
                if (!reconheceID()) erro(ERRO_INTERNO);
            } else {
                ungetTPos();
                break;
            }
        }
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
                if (!Tipo()) erro(ERRO_INTERNO);
                novoToken();
                if (reconheceID()) novoToken();
            } else break;
        }
    }
}

void TipoParamOpcResto() {
    if (!Tipo()) {
        erro(AS_ERRO);
    }
    novoToken();
    IdResto();
    if (reconhece(SN, VIRGULA)) {
        novoToken();
        TipoParamOpcResto();
    } else ungetTPos();
}

void IdResto() {
    if (reconheceID()) {}
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
    } else if (Atrib()) {
        if (!reconhece(SN, PONTOEVIRGULA)) {
            erro(AS_COMANDO);
        }
    } else if (reconheceID()) {
        novoToken();
        NId();
    } else if (reconhece(SN, ABRECHAVES)) {
        NovoComando();
    } else if (reconhece(SN, PONTOEVIRGULA)) {
        novoToken();
    } else {
        /*novoToken();
        imprimirToken();
        if (!reconhece(SN, FECHACHAVES)) {
            ungetTPos();
            logErro(AS_COMANDO, 1, tokens[tpos].linha);
        }*/
    }
}

void Se() {
    if (reconhece(SN, ABREPARENTESES)) {
        printf("\n-INI  SE-\n");
        novoToken();
        Expr();
        novoToken();
        if (!reconhece(SN, FECHAPARENTESE)) {
            erro(AS_SE);
        }
        novoToken();
        //while (!reconhece(SN, FECHACHAVES)) {
            Cmd();
        //}
        novoToken();
        printf("\n-FIM  SE-\n");
        Senao();
        novoToken();
    } else {
        Senao();
    }
}

void Senao() {
    imprimirToken(tpos);
    if (reconhece(PR, SENAO)) {
        printf("\n-INI SEN-\n");
        novoToken();
        Cmd();
        printf("\n-FIM SEN-\n");
    } else ;
}

void Enquanto() {
    if (reconhece(SN, ABREPARENTESES)) {
        novoToken();
        Expre();
        novoToken();
        if (reconhece(SN, FECHAPARENTESE)) {
            novoToken();
            Cmd();
        } else {
            erro(AS_ENQUANTO);
        }
    }
}

void Para() {
    if (reconhece(SN, ABREPARENTESES)) {
        printf("\n-INI PAR-\n");
        novoToken();
        if (!Atrib()) erro(AS_PARA);
        if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_PARA);
        novoToken();
        imprimirToken(tpos);
        Expr();
        novoToken();
        if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_PARA);
        novoToken();
        if (!Atrib()) erro(AS_PARA);
        if (reconhece(SN, FECHAPARENTESE)) {
            novoToken();
            Cmd();
            printf("\n-FIM PAR-\n");
        } else erro(AS_PARA);
    }
}

void Retorne() {
    if (!reconhece(SN, PONTOEVIRGULA)) {
        printf("\n-INI RET-\n");
        //novoToken();
        imprimirToken(tpos);
        Expr();
        novoToken();
        if (!reconhece(SN, PONTOEVIRGULA)) {
            erro(AS_FALTAPONTOEVIRGULA);
        }
        printf("\n-FIM RET-\n");
    }
}

void NId() {
    if (reconhece(SN, ABREPARENTESES)) {
        printf("\n-INI NID-\n");
        novoToken();
        Conteudo();
        novoToken();
        if (!reconhece(SN, FECHAPARENTESE)) {
            erro(AS_ERRO);
        }
        novoToken();
        if (!reconhece(SN, PONTOEVIRGULA)) {
            erro(AS_ERRO);
        }
        printf("\n-FIM NID-\n");
    } else {
        erro(AS_ERRO);
    }
}

void Conteudo() {
    if (!reconhece(SN, FECHAPARENTESE)) {
        novoToken();
        Expr();
        ConteudoResto();
    } else ungetTPos();
}

void ConteudoResto() {
    if (reconhece(SN, VIRGULA)) {
        novoToken();
        Expr();
        ConteudoResto();
    } else ;
}

void NovoComando() {
    //novoToken();
    if (reconhece(SN, ABRECHAVES)) {
        novoToken();
        while (!reconhece(SN, FECHACHAVES)) {
            Cmd();
            novoToken();
        }
    } else Cmd();
}

/*Gramatica de Atribuição*/
int Atrib() {
    if (tokens[tpos].categoria == ID) {
        printf("\n-INI ATR\n");
        novoToken();
        if (tokens[tpos].categoria == SN &&
            tokens[tpos].codigo == ATRIBUICAO) {
            novoToken();
            Expre();
            printf("\n-FIM ATR\n");
            return 1;
        } else {
            ungetTPos();
            printf("\n-FIM ATR\n");
        }
    }
    return 0;
}

/*Gramatica de Expressões*/
void Expr() {
    printf("\n-INI EXP-\n");
    Expre();
    //novoToken(); /*TODO AINDA PODE GERAR ERRO EM PARENTESES*/
    TermoExpr();
    printf("\n-FIM EXP-\n");
}

void TermoExpr() {
    if (OpRel()) {
        novoToken();
        RestoExpr();
    }
    ungetTPos();
}

void RestoExpr() {
    Expre();
}

/*Gramatica de Expressões Simples*/
void Expre() {
    Termo();
    Resto();
}

void Termo() {
    Fator();
    Sobra();
}

void Resto() {
    if (reconhece(SN, ADICAO) || reconhece(SN, SUBTRACAO) || reconhece(SN, OR)) {
        novoToken();
        Termo();
        Resto();
    } else ;
}

void Sobra() {
    if (reconhece(SN, MULTIPLICACAO) || reconhece(SN, DIVISAO) || reconhece(SN, AND)) {
        novoToken();
        Termo();
        Resto();
    } else ;
}

void Fator() {
    if (reconhece(SN, ABREPARENTESES)) {
        novoToken();
        Expr();
        novoToken();
        if (!reconhece(SN, FECHAPARENTESE)) {
            erro(AS_FALTAPARENTESE);
        }
        novoToken();
    } else if (tokens[tpos].categoria == ID ||
                tokens[tpos].categoria == INTCON ||
                tokens[tpos].categoria == REALCON ||
                tokens[tpos].categoria == CT_C) {
        novoToken();
    }
    else {
        //imprimirToken();
        //erro(AS_FALTAEXPREOUNUM);
    }

}

/*Gramatica de Operador Relacional*/
int OpRel() {
    if (tokens[tpos].categoria == SN &&
            (tokens[tpos].codigo == IGUAL || tokens[tpos].codigo == DIFERENTE ||
                    tokens[tpos].codigo == MENORIGUAL || tokens[tpos].codigo == MENOR ||
                    tokens[tpos].codigo == MAIORIGUAL || tokens[tpos].codigo == MAIOR))
    {
        //novoToken();
        return 1;
    }
    //novoToken();
    return 0;
}