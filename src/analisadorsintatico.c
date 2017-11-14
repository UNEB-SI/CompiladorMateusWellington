//
// Created by mateus and wellington on 31/10/2017.
//

#include "../include/analisadorsintatico.h"

int analisadorSintatico() {
    tpos = 0;
    printf("\n\nTokens Lidos com Sucesso:\n");
    Prog();
    //Cmd();
    return 1;
}

int faltaToken() {
    return tkpos != tpos;
}

int updateTPos() {
    if (tkpos < tpos) erro(ERRO_INTERNO);
    imprimirToken(tpos);
    return tpos++;
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
            erro(AS_FALTAID);
        }
        updateTPos();
        ProgRestoSobra();
    } else {
        updateTPos();
        //imprimirToken();
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
        erro(AS_FALTAPONTOEVIRGULA);
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
        erro(AS_FALTAPONTOEVIRGULA);
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
        erro(AS_ERRO);
    }
}

void PrototipoDecl() {

    if (!reconheceID()) {
        erro(AS_FALTAID);
    }
    updateTPos();
    if (!reconhece(SN, ABREPARENTESES)) {
        erro(AS_FALTAPARENTESE);
    }
    updateTPos();
    TipoParamOpc();
    updateTPos();
    if (!reconhece(SN, FECHAPARENTESE)) {
        erro(AS_FALTAPARENTESE);
    }
    updateTPos();
    ProtitipoResto();
    updateTPos();
    if (!reconhece(SN, PONTOEVIRGULA)) {
        erro(AS_FALTAPONTOEVIRGULA);
    }
    updateTPos();
}

void ProtitipoResto() {
    if (reconhece(SN, VIRGULA)) {
        updateTPos();
        if (!reconheceID()) {
            erro(AS_FALTAID);
        }
        updateTPos();
        if (!reconhece(SN, ABREPARENTESES)) {
            erro(AS_FALTAPARENTESE);
        }
        updateTPos();
        TipoParamOpc();
        updateTPos();
        if (!reconhece(SN, FECHAPARENTESE)) {
            erro(AS_FALTAPARENTESE);
        }
        updateTPos();
        ProtitipoResto();
    } else ungetTPos();
}

void Funcao() {
    if (reconhece(PR, SEMRETORNO)) {
        printf("\n-INI FUN-\n");
        updateTPos();
        FuncaoDecl();
        printf("\n-FIM FUN-\n");
    } else if (Tipo()){
        printf("\n-INI FUN-\n");
        updateTPos();
        FuncaoDecl();
        printf("\n-FIM FUN-\n");
    }
}

void FuncaoDecl() {
    if (!reconheceID()) {
        erro(AS_FALTAID);
    }
    updateTPos();
    if (!reconhece(SN, ABREPARENTESES)) {
        erro(AS_FALTAPARENTESE);
    }
    updateTPos();
    TipoParam();
    updateTPos();
    if (!reconhece(SN, FECHAPARENTESE)) {
        erro(AS_FALTAPARENTESE);
    }
    updateTPos();
    if (!reconhece(SN, ABRECHAVES)) {
        erro(AS_FALTACHAVES);
    }
    updateTPos();
    NovaDecl();
    updateTPos();
    Cmd();
    //updateTPos();
    if (!reconhece(SN, FECHACHAVES)) {
        //Cmd();
        NovoComando();
        if (!faltaToken()) erro(AS_FALTACHAVES);
    }
    updateTPos();
}

void NovaDecl() {
    if (Tipo()) {
        updateTPos();
        if (!reconheceID()) {
            erro(AS_FALTAID);
        }
        Decl();
        updateTPos();
        NovaDecl();
        if (!reconhece(SN, PONTOEVIRGULA)) {
            ungetTPos();
            erro(AS_FALTAPONTOEVIRGULA);
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
            erro(ERRO_INTERNO);
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
                erro(ERRO_INTERNO);
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
        erro(AS_ERRO);
    }
    updateTPos();
    IdResto();
    if (reconhece(SN, VIRGULA)) {
        updateTPos();
        TipoParamOpcResto();
    } else ungetTPos();
}

void IdResto() {
    if (reconheceID()) {}
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
        if (!reconhece(SN, PONTOEVIRGULA)) {
            erro(AS_COMANDO);
        }
    } else if (reconheceID()) {
        updateTPos();
        NId();
    } else if (reconhece(SN, ABRECHAVES)) {
        NovoComando();
    } else if (reconhece(SN, PONTOEVIRGULA)) {
        updateTPos();
    } else {
        /*updateTPos();
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
        updateTPos();
        Expr();
        updateTPos();
        if (!reconhece(SN, FECHAPARENTESE)) {
            erro(AS_SE);
        }
        updateTPos();
        //while (!reconhece(SN, FECHACHAVES)) {
            Cmd();
        //}
        updateTPos();
        printf("\n-FIM  SE-\n");
        Senao();
        updateTPos();
    } else {
        Senao();
    }
}

void Senao() {
    imprimirToken(tpos);
    if (reconhece(PR, SENAO)) {
        printf("\n-INI SEN-\n");
        updateTPos();
        Cmd();
        printf("\n-FIM SEN-\n");
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
            erro(AS_ENQUANTO);
        }
    }
}

void Para() {
    if (reconhece(SN, ABREPARENTESES)) {
        printf("\n-INI PAR-\n");
        updateTPos();
        if (!Atrib()) erro(AS_PARA);
        if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_PARA);
        updateTPos();
        imprimirToken(tpos);
        Expr();
        updateTPos();
        if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_PARA);
        updateTPos();
        if (!Atrib()) erro(AS_PARA);
        if (reconhece(SN, FECHAPARENTESE)) {
            updateTPos();
            Cmd();
            printf("\n-FIM PAR-\n");
        } else erro(AS_PARA);
    }
}

void Retorne() {
    if (!reconhece(SN, PONTOEVIRGULA)) {
        printf("\n-INI RET-\n");
        //updateTPos();
        imprimirToken(tpos);
        Expr();
        updateTPos();
        if (!reconhece(SN, PONTOEVIRGULA)) {
            erro(AS_FALTAPONTOEVIRGULA);
        }
        printf("\n-FIM RET-\n");
    }
}

void NId() {
    if (reconhece(SN, ABREPARENTESES)) {
        printf("\n-INI NID-\n");
        updateTPos();
        Conteudo();
        updateTPos();
        if (!reconhece(SN, FECHAPARENTESE)) {
            erro(AS_ERRO);
        }
        updateTPos();
        if (!reconhece(SN, PONTOEVIRGULA)) {
            erro(AS_ERRO);
        }
        updateTPos();
        printf("\n-FIM NID-\n");
    } else {
        erro(AS_ERRO);
    }
}

void Conteudo() {
    if (!reconhece(SN, FECHAPARENTESE)) {
        updateTPos();
        Expr();
        ConteudoResto();
    } else ungetTPos();
}

void ConteudoResto() {
    if (reconhece(SN, VIRGULA)) {
        updateTPos();
        Expr();
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
        printf("\n-INI ATR\n");
        updateTPos();
        if (tokens[tpos].categoria == SN &&
            tokens[tpos].codigo == ATRIBUICAO) {
            updateTPos();
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
    //updateTPos(); /*TODO AINDA PODE GERAR ERRO EM PARENTESES*/
    TermoExpr();
    printf("\n-FIM EXP-\n");
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
    if (reconhece(SN, ADICAO) || reconhece(SN, SUBTRACAO) || reconhece(SN, OR)) {
        updateTPos();
        Termo();
        Resto();
    } else ;
}

void Sobra() {
    if (reconhece(SN, MULTIPLICACAO) || reconhece(SN, DIVISAO) || reconhece(SN, AND)) {
        updateTPos();
        Termo();
        Resto();
    } else ;
}

void Fator() {
    if (reconhece(SN, ABREPARENTESES)) {
        updateTPos();
        Expr();
        updateTPos();
        if (!reconhece(SN, FECHAPARENTESE)) {
            erro(AS_FALTAPARENTESE);
        }
        updateTPos();
    } else if (tokens[tpos].categoria == ID ||
                tokens[tpos].categoria == INTCON ||
                tokens[tpos].categoria == REALCON ||
                tokens[tpos].categoria == CT_C) {
        updateTPos();
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
        //updateTPos();
        return 1;
    }
    //updateTPos();
    return 0;
}
