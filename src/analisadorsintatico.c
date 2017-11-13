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
    if (tkpos < tpos) erro(ERRO_INTERNO);
    imprimirToken();
    tpos++;
}

void ungetTPos() {
    tpos--;
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
        if (!reconheceID()) erro(AS_FALTAID);
        updateTPos();
        ProgRestoSobra();
    } else {
        imprimirToken();
    }
}

void ProgRestoSobra() {
    if (reconhece(SN, ABREPARENTESES)) {
        ungetTPos(); ungetTPos();
        Funcao();
    } else if (reconhece(SN, VIRGULA)) {
        updateTPos();
        Decl();
        updateTPos();
        ProgRestoSobra();
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
    } else ;
}

void Prototipo() {
    if (Tipo()) {
        updateTPos();
        PrototipoDecl();
    } else if (reconhece(SN, SEMRETORNO)) {
        updateTPos();
        PrototipoDecl();
    } else {
        erro(AS_ERRO);
    }
}

void PrototipoDecl() {
    if (!reconheceID()) erro(AS_FALTAID);
    updateTPos();
    if (!reconhece(SN, ABREPARENTESES)) erro(AS_FALTAPARENTESE);
    updateTPos();
    TipoParamOpc();
    updateTPos();
    if (!reconhece(SN, FECHAPARENTESE)) erro(AS_FALTAPARENTESE);
    updateTPos();
    ProtitipoResto();
    updateTPos();
    if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_FALTAPONTOEVIRGULA);
}

void ProtitipoResto() {
    if (reconhece(SN, VIRGULA)) {
        updateTPos();
        if (!reconheceID()) erro(AS_FALTAID);
        updateTPos();
        if (!reconhece(SN, ABREPARENTESES)) erro(AS_FALTAPARENTESE);
        updateTPos();
        TipoParamOpc();
        updateTPos();
        if (!reconhece(SN, FECHAPARENTESE)) erro(AS_FALTAPARENTESE);
        updateTPos();
        ProtitipoResto();
    } else ;
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
    if (!reconheceID()) erro(AS_FALTAID);
    updateTPos();
    if (!reconhece(SN, ABREPARENTESES)) erro(AS_FALTAPARENTESE);
    updateTPos();
    TipoParam();
    updateTPos();
    if (!reconhece(SN, FECHAPARENTESE)) erro(AS_FALTAPARENTESE);
    updateTPos();
    if (!reconhece(SN, ABRECHAVES)) erro(AS_FALTACHAVES);
    updateTPos();
    NovaDecl();
    updateTPos();
    Cmd();
    if (!reconhece(SN, FECHACHAVES)) erro(AS_FALTACHAVES);
    updateTPos();
    printf("\n\n ------- \n\n");
}

void NovaDecl() {
    if (Tipo()) {
        updateTPos();
        if (!reconheceID()) erro(AS_FALTAID);
        Decl();
        updateTPos();
        NovaDecl();
        if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_FALTAPONTOEVIRGULA);
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
        if (!reconheceID()) erro(ERRO_INTERNO);
        TipoParamResto();
    } else {
        //TODO LOG ERRO
    }
}

void TipoParamResto() {
    if (reconhece(SN, VIRGULA)) {
        updateTPos();
        if (Tipo()) {
            updateTPos();
            if (!reconheceID()) erro(ERRO_INTERNO);
            updateTPos();
            TipoParamResto();
        }
    } else ;
}

/*Gramatica de TipoParamOpc*/
void TipoParamOpc() {
    if (reconhece(PR, SEMPARAM)) {
    } else if (Tipo()) {
        updateTPos();
        IdResto();
        updateTPos();
        TipoParamOpcResto();
        updateTPos();
        if (!reconheceID()) erro(AS_FALTAID);
        updateTPos();
        while (reconhece(SN, VIRGULA)) {
            updateTPos();
            if (Tipo()) {
                updateTPos();
                if (!reconheceID()) erro(AS_FALTAID);
            } else {
                erro(ERRO_INTERNO);
            }
        }
    } else {
        //TODO LOG ERRO
    }
}

void TipoParamOpcResto() {
    if (reconhece(SN, VIRGULA)) {
        updateTPos();
        if (Tipo()) {
            updateTPos();
            IdResto();
            updateTPos();
            TipoParamOpcResto();
        }
    } else ;
}

void IdResto() {
    if (reconheceID()) erro(AS_FALTAID);
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
        if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_COMANDO);
    } else if (reconheceID()) {
        updateTPos();
        NId();
    } else if (reconhece(SN, ABRECHAVES)) {
        NovoComando();
    } else if (reconhece(SN, PONTOEVIRGULA)) {
        updateTPos();
    } else {
        //ungetTPos();
        if (!reconhece(SN, FECHACHAVES)) erro(AS_COMANDO);
    }
}

void Se() {
    if (reconhece(SN, ABREPARENTESES)) {
        updateTPos();
        Expr();
        updateTPos();
        if (!reconhece(SN, FECHAPARENTESE)) erro(AS_SE);
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
    if (reconhece(PR, ABREPARENTESES)) {
        updateTPos();
        Expre();
        updateTPos();
        if (reconhece(PR, FECHAPARENTESE)) {
            updateTPos();
            Cmd();
        } else {
            erro(AS_ENQUANTO);
        }
    }
}

void Para() {
    if (reconhece(PR, ABREPARENTESES)) {
        updateTPos();
        if (Atrib()) {
            updateTPos();
            if (!reconhece(PR, PONTOEVIRGULA)) erro(AS_PARA);
            updateTPos();
            Expr();
            updateTPos();
            if (!reconhece(PR, PONTOEVIRGULA)) erro(AS_PARA);
            updateTPos();
            if (Atrib()) {
                updateTPos();
                if (reconhece(PR, FECHAPARENTESE)) {
                    updateTPos();
                    Cmd();
                } else erro(AS_PARA);
            }
        }
    }
}

void Retorne() {
    if (!reconhece(SN, PONTOEVIRGULA)) {
        updateTPos();
        Expr();
        updateTPos();
        if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_PARA);
    }
}

void NId() {
    if (reconhece(SN, ABREPARENTESES)) {
        updateTPos();
        Conteudo();
        updateTPos();
        if (!reconhece(SN, FECHAPARENTESE)) erro(AS_ERRO);
        updateTPos();
        if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_ERRO);
        updateTPos();
    } else {
        erro(AS_ERRO);
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
    if (reconhece(SN, ABRECHAVES)) {
        updateTPos();
        Cmd();
        updateTPos();
        if (!reconhece(SN, FECHACHAVES)) erro(AS_ERRO);
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
            logErro(AS_FALTAPARENTESE, 0);
        }
        updateTPos();
    }
    else if (tokens[tpos].categoria == INTCON ||
                tokens[tpos].categoria == REALCON) {
        updateTPos();
    }
    else {
        logErro(AS_FALTAEXPREOUNUM, 0);
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
