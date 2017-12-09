//
// Created by mateus and wellington on 31/10/2017.
//

#include "../include/analisadorsintatico.h"
#include "../include/maqpilha.h"

int analisadorCodigo() {
    codigo = fopen("codigo.cmp", "w+");
    configurarPilha(codigo);
    tpos = 0; loop = 0; tipo = -1;
    Prog();
    //mostrarTabela();
    fclose(codigo);
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
            //alterar(tauxfun.lexema, LOCAL);
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
                //alterar(tauxfun.lexema, LOCAL);
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
            //CHECAR SE PODE REALMENTE TIRAR
            //if (!reconhece(SN, FECHACHAVES)) novoToken();
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
    int tipo;
    int result;

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
        taux = tokens[tpos];
        novoToken();
        if (reconhece(SN, ATRIBUICAO)) {
            novoToken();
            result = Expr();
            if (result != -99999) {
                tipo = consultar(taux.lexema).tipo;
                if (tipo == INTCON && (result != INTCON && result != CT_C)) erro(ASEM_ATRIBUICAO);
                else if (tipo == CT_C && (result != INTCON && result != CT_C)) erro(ASEM_ATRIBUICAO);
                else if (tipo == REALCON && result != REALCON) erro(ASEM_ATRIBUICAO);
                else if (tipo == BOOLEANO && (result != BOOLEANO && result != INTCON)) erro(ASEM_ATRIBUICAO);
                else if (tipo == -1) erro(ASEM_ATRIBUICAO);
            } else {
//                tipo = consultar(taux.lexema).tipo;
//                if (tipo == CT_C && taux2.categoria == CT_C) ;
//                else if (tipo == INTCON && taux2.categoria == CT_C) ;
//                else if (tipo == CT_C && taux2.categoria == INTCON) ;
//                else if (tipo == CT_L && taux2.categoria == CT_L) ;
//                else erro(ASEM_ATRIBUICAO);
            }
            if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_FALTAPONTOEVIRGULA);
        } else {
            if (reconhece(SN, ABREPARENTESES)) {
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
    } else if (reconhece(SN, PONTOEVIRGULA)) {novoToken();}
}

void Se() {
    if (reconhece(SN, ABREPARENTESES)) {
        novoToken();
        Expr();
        if (!reconhece(SN, FECHAPARENTESE)) erro(AS_SE);
        novoToken();
        Cmd();
        novoToken();
        Senao();
    } else {
        Senao();
    }
}

void Senao() {
    if (reconhece(PR, SENAO)) {
        novoToken();
        Cmd();
        novoToken();
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
        } else erro(AS_PARA);
    }
}

void Retorne() {
    CelulaTabela function = consultarUltimoGlobal();
    if (function.tipo == SEMRETORNO && reconhece(SN, PONTOEVIRGULA)) ;
    else if (function.tipo != SEMRETORNO && !reconhece(SN, PONTOEVIRGULA)) {
        Expr();
        if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_FALTAPONTOEVIRGULA);
    } else erro(ASEM_RETORNARFUNCAO);
}

/*Gramatica de Atribuição*/
int Atrib() {
    float result;
    if (tokens[tpos].categoria == ID) {
        taux = tokens[tpos];
        novoToken();
        if (tokens[tpos].categoria == SN &&
            tokens[tpos].codigo == ATRIBUICAO) {
            novoToken();
            result = Expr();
            if (result != -99999) {
                tipo = consultar(taux.lexema).tipo;
                if (tipo == INTCON) {
                    if (!isInteger(result)) erro(ASEM_ATRIBUICAO);
                } else if (tipo == REALCON) {
                } else erro(ASEM_ATRIBUICAO);
            } else erro(ASEM_ATRIBUICAO);
            return 1;
        } else {
            retornarToken();
        }
    }
    return 0;
}

/*Gramatica de Expressões*/
int Expr() {
    int result, segundoOp;
    result = ExprSimp();
    printf("\n\n\n\n RESULTADO = %d \n\n\n\n", result);
    if (OpRel()) {
        novoToken();
        segundoOp = Expr();
        if ((result == INTCON || result == REALCON || result == CT_C) && (segundoOp == INTCON || segundoOp == REALCON || segundoOp == CT_C)) result = BOOLEANO;
        else erro(ASEM_ATRIBUICAO);
        printf("\n\n\n\n RESULTADO = %d \n\n\n\n", result);
    } else ;
    return result;
}

/*Gramatica de Expressões Simples*/
int ExprSimp() {
    int primOp, result;
    if (reconhece(SN, ADICAO) || reconhece(SN, SUBTRACAO)) novoToken();
    primOp = Termo();
    result = Resto(primOp);
    return  result;
}

int Termo() {
    int primOp, result;
    primOp = Fator();
    result = Sobra(primOp);
    return result;
}

int Resto(int primOp) {
    Token salva;
    int segunOp, tmpResult = 0, result;
    result = primOp;
    while (reconhece(SN, ADICAO) || reconhece(SN, SUBTRACAO) || reconhece(SN, OR)) {
        salva = tokens[tpos];
        novoToken();
        segunOp = Termo();
        if (primOp == INTCON && segunOp == INTCON) tmpResult = INTCON;
        else if (primOp == REALCON || segunOp == REALCON) tmpResult = REALCON;
        else if (primOp == CT_C && segunOp == CT_C) tmpResult = CT_C;
        else if ((primOp == INTCON && segunOp == CT_C) || (primOp == CT_C && segunOp == INTCON)) tmpResult = CT_C;
        else erro(ASEM_ATRIBUICAO);
        result = Resto(tmpResult);
    }
    return result;
}

int Sobra(int primOp) {
    Token salva;
    int segunOp, tmpResult = 0, result;
    result = primOp;
    while (reconhece(SN, MULTIPLICACAO) || reconhece(SN, DIVISAO) || reconhece(SN, AND)) {
        salva = tokens[tpos];
        novoToken();
        segunOp = Fator();
        if (primOp == INTCON && segunOp == INTCON) tmpResult = INTCON;
        else if (primOp == REALCON || segunOp == REALCON) tmpResult = REALCON;
        else if (primOp == CT_C && segunOp == CT_C) tmpResult = CT_C;
        else if ((primOp == INTCON && segunOp == CT_C) || (primOp == CT_C && segunOp == INTCON)) tmpResult = CT_C;
        else erro(ASEM_ATRIBUICAO);
        result = Sobra(tmpResult);
    }
    return result;
}

int Fator() {
    int result = -99999;
    if (tokens[tpos].categoria == INTCON ||
        tokens[tpos].categoria == REALCON ||
            (tokens[tpos].categoria == CT_C && tokens[tpos].lexema != "!")||
        tokens[tpos].categoria == CT_L)
    {
        result = tokens[tpos].categoria;
        taux2 = tokens[tpos];
        novoToken();
    } else if (reconheceID()) {
        novoToken();
        if (reconhece(SN, ABREPARENTESES)) {
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
        } else {
            retornarToken();
            CelulaTabela celulaTabela = consultar(tokens[tpos].lexema);
            novoToken();
            return celulaTabela.tipo;
        }
    } else if (reconhece(SN, ABREPARENTESES)) {
        novoToken();
        result = Expr();
        if (!reconhece(SN, FECHAPARENTESE)) erro(AS_FALTAPARENTESE);
        novoToken();
    } else if (reconhece(SN, NOT)) {
        novoToken();
        Fator();
    } else {
        erro(AS_FALTAEXPREOUNUM);
    }
    return result;

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

int isInteger(float val) {
    int truncated = (int)val;
    return (val == truncated);
}