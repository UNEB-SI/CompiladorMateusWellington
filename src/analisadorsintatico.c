//
// Created by mateus and wellington on 31/10/2017.
//

#include "../include/analisadorsintatico.h"

int analisadorSintatico() {
    tpos = 0;
    printf("\n\nTokens Lidos com Sucesso:\n");
    Prog();
    return 1;
}

void imprimirToken() {
    switch (tokens[tpos].categoria) {
        case INTCON:
            printf("<INTCON / %s>\n", tokens[tpos].lexema);
            break;
        case REALCON:
            printf("<REALCON / %s>\n", tokens[tpos].lexema);
            break;
        case ID:
            printf("<ID / %s>\n", ids[tokens[tpos].codigo]);
            break;
        case PR:
            printf("<PR / %d>\n", tokens[tpos].codigo);
            break;
        case SN:
            printf("<SN / %d>\n", tokens[tpos].codigo);
            break;
        case CT_C:
            printf("<CT_C / %s>\n", tokens[tpos].lexema);
            break;
        case CT_L:
            printf("<CT_L / %s>\n", literal[tokens[tpos].codigo]);
            break;
        default:
            printf("<%d / val = %s>\n", tokens[tpos].categoria, tokens[tpos].lexema);
            break;
    }
}

int faltaToken() {
    return tkpos != tpos;
}

int updateTPos() {
    if (tkpos < tpos) logErro(ERRO_INTERNO, 1, tokens[tpos].linha);
    imprimirToken();
    return tpos++;
}

int ungetTPos() {
    return tpos--;
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
        ProgResto();
    }
}

void ProgResto() {
    if (reconhece(PR, PROTOTIPO)) {
        updateTPos();
        Prototipo();
    } else if (reconhece(PR, SEMRETORNO)) {
        Funcao();
    } else if (Tipo()) {
        updateTPos();
        if (!reconheceID()) {
            ungetTPos();
            logErro(AS_FALTAID, 1, tokens[tpos].linha);
        }
        updateTPos();
        ProgRestoSobra();
    } else {
        updateTPos();
        imprimirToken();
        /*if (!reconhece(SN, FECHACHAVES)) {
            imprimirToken();
            logErro(AS_ERRO, 0, tokens[tpos].linha);
        }*/
    }
}

void ProgRestoSobra() {
    if (reconhece(SN, ABREPARENTESES)) {
        ungetTPos(); ungetTPos();
        Funcao();
    } else if (reconhece(SN, VIRGULA)) {
        updateTPos();
        Decl();
    } else if (reconhece(SN, PONTOEVIRGULA)) {
        updateTPos();
    } else {
        ungetTPos();
        logErro(AS_FALTAPONTOEVIRGULA, 1, tokens[tpos].linha);
    }
}

void Decl() {
    updateTPos();
    if (reconhece(SN, VIRGULA)) {
        updateTPos();
        reconheceID();
        Decl();
    } else ;
    if (!reconhece(SN, PONTOEVIRGULA)) {
        ungetTPos();
        logErro(AS_FALTAPONTOEVIRGULA, 1, tokens[tpos].linha);
    }
}

void Prototipo() {
    if (Tipo()) {
        updateTPos();
        PrototipoDecl();
    } else if (reconhece(PR, SEMRETORNO)) {
        updateTPos();
        PrototipoDecl();
    } else {
        ungetTPos();
        logErro(AS_ERRO, 1, tokens[tpos].linha);
    }
}

void PrototipoDecl() {

    if (!reconheceID()) {
        ungetTPos();
        logErro(AS_FALTAID, 1, tokens[tpos].linha);
    }
    updateTPos();
    if (!reconhece(SN, ABREPARENTESES)) {
        ungetTPos();
        logErro(AS_FALTAPARENTESE, 1, tokens[tpos].linha);
    }
    updateTPos();
    TipoParamOpc();
    updateTPos();
    if (!reconhece(SN, FECHAPARENTESE)) {
        ungetTPos();
        logErro(AS_FALTAPARENTESE, 1, tokens[tpos].linha);
    }
    updateTPos();
    ProtitipoResto();
    updateTPos();
    if (!reconhece(SN, PONTOEVIRGULA)) {
        ungetTPos();
        logErro(AS_FALTAPONTOEVIRGULA, 1, tokens[tpos].linha);
    }
    updateTPos();
}

void ProtitipoResto() {
    if (reconhece(SN, VIRGULA)) {
        updateTPos();
        if (!reconheceID()) {
            ungetTPos();
            logErro(AS_FALTAID, 1, tokens[tpos].linha);
        }
        updateTPos();
        if (!reconhece(SN, ABREPARENTESES)) {
            ungetTPos();
            logErro(AS_FALTAPARENTESE, 1, tokens[tpos].linha);
        }
        updateTPos();
        TipoParamOpc();
        updateTPos();
        if (!reconhece(SN, FECHAPARENTESE)) {
            ungetTPos();
            logErro(AS_FALTAPARENTESE, 1, tokens[tpos].linha);
        }
        updateTPos();
        ProtitipoResto();
    } else ungetTPos();
}

void Funcao() {
    if (reconhece(PR, SEMRETORNO)) {
        updateTPos();
        FuncaoDecl();
    } else if (Tipo()){
        updateTPos();
        FuncaoDecl();
    }
}

void FuncaoDecl() {
    if (!reconheceID()) {
        ungetTPos();
        logErro(AS_FALTAID, 1, tokens[tpos].linha);
    }
    updateTPos();
    if (!reconhece(SN, ABREPARENTESES)) {
        ungetTPos();
        logErro(AS_FALTAPARENTESE, 1, tokens[tpos].linha);
    }
    updateTPos();
    TipoParam();
    updateTPos();
    if (!reconhece(SN, FECHAPARENTESE)) {
        ungetTPos();
        logErro(AS_FALTAPARENTESE, 1, tokens[tpos].linha);
    }
    updateTPos();
    if (!reconhece(SN, ABRECHAVES)) {
        ungetTPos();
        logErro(AS_FALTACHAVES, 1, tokens[tpos].linha);
    }
    updateTPos();
    NovaDecl();
    updateTPos();
    Cmd();
    updateTPos();
    if (!reconhece(SN, FECHACHAVES)) {
        ungetTPos();
        logErro(AS_FALTACHAVES, 1, tokens[tpos].linha);
    }
    updateTPos();
}

void NovaDecl() {
    if (Tipo()) {
        updateTPos();
        if (!reconheceID()) {
            ungetTPos();
            logErro(AS_FALTAID, 1, tokens[tpos].linha);
        }
        Decl();
        updateTPos();
        NovaDecl();
        if (!reconhece(SN, PONTOEVIRGULA)) {
            ungetTPos();
            logErro(AS_FALTAPONTOEVIRGULA, 1, tokens[tpos].linha);
        }
    } else ungetTPos();
}

/*Gramatica de Tipo*/
int Tipo() {
    if (tokens[tpos].categoria != PR ||
            (tokens[tpos].codigo != INTEIRO &&
                    tokens[tpos].codigo != REAL &&
                    tokens[tpos].codigo != CARACTER &&
                    tokens[tpos].codigo != CADEIA &&
                    tokens[tpos].codigo != BOOLEANO)) {
        return 0;
    }
    return 1;
}

/*Gramatica de TipoParam*/
void TipoParam() {
    if (reconhece(PR, SEMPARAM)) {
    } else if (Tipo()) {
        updateTPos();
        if (!reconheceID()) {
            ungetTPos();
            logErro(ERRO_INTERNO, 1, tokens[tpos].linha);
        }
        TipoParamResto();
    }
}

void TipoParamResto() {
    if (reconhece(SN, VIRGULA)) {
        updateTPos();
        if (Tipo()) {
            updateTPos();
            if (!reconheceID()) {
                ungetTPos();
                logErro(ERRO_INTERNO, 1, tokens[tpos].linha);
            }
            updateTPos();
            TipoParamResto();
        }
    } else ;
}

/*Gramatica de TipoParamOpc*/
void TipoParamOpc() {
    if (reconhece(PR, SEMPARAM)) {}
    else {
        TipoParamOpcResto();
    }
}

void TipoParamOpcResto() {
    if (!Tipo()) {
        ungetTPos();
        logErro(AS_ERRO, 1, tokens[tpos].linha);
    }
    updateTPos();
    IdResto();
    updateTPos();
    if (reconhece(SN, VIRGULA)) {
        updateTPos();
        TipoParamOpcResto();
    } else ungetTPos();
}

void IdResto() {
    if (!reconheceID()) {
        ungetTPos();
        logErro(AS_FALTAID, 1, tokens[tpos].linha);
    }
}

/*Gramatica de Comandos*/
void Cmd() {
    if (reconhece(PR, SE)) {
        updateTPos();
        Se();
    } else if (reconhece(PR, ENQUANTO)) {
        updateTPos();
        Enquanto();
    } else if (reconhece(PR, PARA)) {
        updateTPos();
        Para();
    } else if (reconhece(PR, RETORNE)) {
        updateTPos();
        Retorne();
    } else if (Atrib()) {
        updateTPos();
        if (!reconhece(SN, PONTOEVIRGULA)) {
            ungetTPos();
            logErro(AS_COMANDO, 1, tokens[tpos].linha);
        }
    } else if (reconheceID()) {
        updateTPos();
        NId();
    } else if (reconhece(SN, ABRECHAVES)) {
        NovoComando();
    } else if (reconhece(SN, PONTOEVIRGULA)) {
        updateTPos();
    } else {
        updateTPos();
        imprimirToken();
        if (!reconhece(SN, FECHACHAVES)) {
            ungetTPos();
            logErro(AS_COMANDO, 1, tokens[tpos].linha);
        }
    }
}

void Se() {
    if (reconhece(SN, ABREPARENTESES)) {
        updateTPos();
        Expr();
        updateTPos();
        if (!reconhece(SN, FECHAPARENTESE)) {
            ungetTPos();
            logErro(AS_SE, 1, tokens[tpos].linha);
        }
        updateTPos();
        Cmd();
    } else {
        Senao();
    }
}

void Senao() {
    if (reconhece(PR, SENAO)) {
        updateTPos();
        Cmd();
    } else ;
}

void Enquanto() {
    if (reconhece(SN, ABREPARENTESES)) {
        updateTPos();
        Expre();
        updateTPos();
        if (reconhece(SN, FECHAPARENTESE)) {
            updateTPos();
            Cmd();
        } else {
            logErro(AS_ENQUANTO, 1, tokens[tpos].linha);
        }
    }
}

void Para() {
    if (reconhece(SN, ABREPARENTESES)) {
        updateTPos();
        if (Atrib()) {
            if (!reconhece(SN, PONTOEVIRGULA)) {
                ungetTPos();
                logErro(AS_PARA, 1, tokens[tpos].linha);
            }
            updateTPos();
            Expr();
            updateTPos();
            if (!reconhece(SN, PONTOEVIRGULA)) {
                ungetTPos();
                logErro(AS_PARA, 1, tokens[tpos].linha);
            }
            updateTPos();
            if (Atrib()) {
                if (reconhece(SN, FECHAPARENTESE)) {
                    updateTPos();
                    Cmd();
                } else logErro(AS_PARA, 1, tokens[tpos].linha);
            }
        }
    }
}

void Retorne() {
    if (!reconhece(SN, PONTOEVIRGULA)) {
        updateTPos();
        Expr();
        updateTPos();
        if (!reconhece(SN, PONTOEVIRGULA)) {
            ungetTPos();
            logErro(AS_PARA, 1, tokens[tpos].linha);
        }
    }
}

void NId() {
    if (reconhece(SN, ABREPARENTESES)) {
        updateTPos();
        Conteudo();
        updateTPos();
        if (!reconhece(SN, FECHAPARENTESE)) {
            ungetTPos();
            logErro(AS_ERRO, 1, tokens[tpos].linha);
        }
        updateTPos();
        if (!reconhece(SN, PONTOEVIRGULA)) {
            ungetTPos();
            logErro(AS_ERRO, 1, tokens[tpos].linha);
        }
        updateTPos();
    } else {
        ungetTPos();
        logErro(AS_ERRO, 1, tokens[tpos].linha);
    }
}

void Conteudo() {
    if (!reconhece(SN, FECHAPARENTESE)) {
        updateTPos();
        Expr();
        updateTPos();
        ConteudoResto();
    } else ungetTPos();
}

void ConteudoResto() {
    if (reconhece(SN, VIRGULA)) {
        updateTPos();
        Expr();
        updateTPos();
        ConteudoResto();
    } else ;
}

void NovoComando() {
    //updateTPos();
    if (reconhece(SN, ABRECHAVES)) {
        updateTPos();
        if (!reconhece(SN, FECHACHAVES)) {
            Cmd();
        }
    } else ;
}

/*Gramatica de Atribuição*/
int Atrib() {
    if (tokens[tpos].categoria == ID) {
        updateTPos();
        if (tokens[tpos].categoria == SN &&
            tokens[tpos].codigo == ATRIBUICAO) {
            updateTPos();
            Expre();
            ungetTPos();
            return 1;
        } else ungetTPos();
    }
    return 0;
}

/*Gramatica de Expressões*/
void Expr() {
    Expre();
    updateTPos();
    TermoExpr();
}

void TermoExpr() {
    if (OpRel()) {
        updateTPos();
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
    if (tokens[tpos].categoria == SN &&
           tokens[tpos].codigo == ADICAO ||
           tokens[tpos].codigo == SUBTRACAO) {
        updateTPos();
        Termo();
        Resto();
    } else ;
}

void Sobra() {
    if (tokens[tpos].categoria == SN &&
            tokens[tpos].codigo == MULTIPLICACAO ||
            tokens[tpos].codigo == DIVISAO) {
        updateTPos();
        Termo();
        Resto();
    } else ;
}

void Fator() {
    if (tokens[tpos].categoria == SN && tokens[tpos].codigo == ABREPARENTESES) {
        updateTPos();
        Expre();
        if (tokens[tpos].categoria != SN || tokens[tpos].codigo != FECHAPARENTESE) {
            ungetTPos();
            logErro(AS_FALTAPARENTESE, 1, tokens[tpos].linha);
        }
        updateTPos();
    } else if (tokens[tpos].categoria == ID ||
                tokens[tpos].categoria == INTCON ||
                tokens[tpos].categoria == REALCON ||
                tokens[tpos].categoria == CT_C) {
        updateTPos();
    }
    else {
        logErro(AS_FALTAEXPREOUNUM, 1, tokens[tpos].linha);
    }

}

/*Gramatica de Operador Relacional*/
int OpRel() {
    if (tokens[tpos].categoria == SN &&
            (tokens[tpos].codigo == IGUAL || tokens[tpos].codigo == DIFERENTE ||
                    tokens[tpos].codigo == MENORIGUAL || tokens[tpos].codigo == MENORIGUAL ||
                    tokens[tpos].codigo == MAIORIGUAL || tokens[tpos].codigo == MAIOR))
    {
        updateTPos();
        return 1;
    }
    updateTPos();
    return 0;
}
