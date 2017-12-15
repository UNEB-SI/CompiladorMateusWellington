//
// Created by mateus and wellington on 31/10/2017.
//

#include "../include/analisadorsintatico.h"
#include "../include/maqpilha.h"

int analisadorCodigo() {
    configurarPilha();
    tpos = 0; loop = 0; tipo = -1;
    Prog();
    excluirPilha();
    return 1;
}

/*Gramatica de Programa*/
void Prog() {
    int decl = 0, label = 0;
    funretorno = 0;
    CelulaTabela celulaTabela;
    INIP();
    while (faltaToken()) {
        if (reconhece(PR, PROTOTIPO)) {
            novoToken();
            if (Tipo() || reconhece(PR, SEMRETORNO)) {
                atualizaTipo();
                novoToken();
                if (!reconheceID()) erro(AS_FALTAID);
                celulaTabela = consultar(tokens[tpos].lexema);
                if (celulaTabela.tipo != -1) erro(ASEM_DECLFUNIGUAL);
                gerarLabel();
                inserirFuncao(tipo, tokens[tpos].lexema, ZOMBIE, GLOBAL, pegarLabel());
                novoToken();
                if (!reconhece(SN, ABREPARENTESES)) erro(AS_FALTAPARENTESE);
                novoToken();
                TipoParamOpc();
                if (!reconhece(SN, FECHAPARENTESE)) erro(AS_FALTAPARENTESE);
                novoToken();
                while (1) {
                    if (reconhece(SN, VIRGULA)) {
                        novoToken();
                        if (!reconheceID()) erro(AS_FALTAID);
                        celulaTabela = consultar(tokens[tpos].lexema);
                        if (celulaTabela.tipo != -1) erro(ASEM_DECLFUNIGUAL);
                        gerarLabel();
                        inserirFuncao(tipo, tokens[tpos].lexema, ZOMBIE, GLOBAL, pegarLabel());
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
            if (decl > 0) AMEM(decl);
            decl = 0;
            atualizaTipo();
            novoToken();
            if (!reconheceID()) erro(AS_FALTAID);
            tauxfun = tokens[tpos];
            celulaTabela = consultar(tauxfun.lexema);
            if (celulaTabela.tipo != -1 && celulaTabela.zombie != ZOMBIE) erro(ASEM_DECLFUNIGUAL);
            else if (celulaTabela.tipo != -1 && celulaTabela.tipo != tipo) erro(ASEM_DECLTIPORETDIFERENTE);
            gerarLabel();
            label = pegarLabel();
            GOTO(label);
            if (celulaTabela.tipo != -1) {
                LABEL(celulaTabela.label);
                inserirFuncao(tipo, tauxfun.lexema, !ZOMBIE, GLOBAL, celulaTabela.label);
            } else {
                LABEL(gerarLabel());
                inserirFuncao(tipo, tauxfun.lexema, !ZOMBIE, GLOBAL, pegarLabel());
            }
            INIPR(1);
            novoToken();
            if (!reconhece(SN, ABREPARENTESES)) erro(AS_FALTAPARENTESE);
            Funcao();
            LABEL(label);
        } else if (Tipo()) {
            atualizaTipo();
            novoToken();
            if (!reconheceID()) erro(AS_FALTAID);
            taux = tokens[tpos];
            novoToken();
            if (reconhece(SN, ABREPARENTESES)) {
                if (decl > 0) AMEM(decl);
                tauxfun = tokens[tpos-1];
                celulaTabela = consultar(tauxfun.lexema);
                if (celulaTabela.tipo != -1 && celulaTabela.zombie != ZOMBIE) erro(ASEM_DECLFUNIGUAL);
                else if (celulaTabela.tipo != -1 && celulaTabela.tipo != tipo) erro(ASEM_DECLTIPORETDIFERENTE);
                gerarLabel();
                label = pegarLabel();
                GOTO(label);
                if (celulaTabela.tipo != -1) {
                    LABEL(celulaTabela.label);
                    inserirFuncao(tipo, tauxfun.lexema, !ZOMBIE, GLOBAL, celulaTabela.label);
                } else {
                    LABEL(gerarLabel());
                    inserirFuncao(tipo, tauxfun.lexema, !ZOMBIE, GLOBAL, pegarLabel());
                }
                INIPR(1);
                Funcao();
                LABEL(label);
                //Função com tipo precisa de retorno
                if (funretorno != 1) erro(ASEM_RETORNARFUNCAO);
            } else if (reconhece(SN, VIRGULA)) {
                sizeVar++; decl++;
                celulaTabela = consultar(taux.lexema);
                if (celulaTabela.tipo != - 1) erro(ASEM_DECLVARIGUAL);
                inserirVar(tipo, taux.lexema, !ZOMBIE, GLOBAL, sizeVar);
                while (reconhece(SN, VIRGULA)) {
                    sizeVar++; decl++;
                    novoToken();
                    if (!reconheceID()) erro(AS_FALTAID);
                    celulaTabela = consultar(tokens[tpos].lexema);
                    if (celulaTabela.tipo != -1) erro(ASEM_DECLVARIGUAL);
                    inserirVar(tipo, tokens[tpos].lexema, !ZOMBIE, GLOBAL, sizeVar);
                    novoToken();
                }
                if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_FALTAPONTOEVIRGULA);
                novoToken();
            } else if (reconhece(SN, PONTOEVIRGULA)) {
                sizeVar++; decl++;
                celulaTabela = consultar(taux.lexema);
                if (celulaTabela.tipo != - 1) erro(ASEM_DECLVARIGUAL);
                inserirVar(tipo, taux.lexema, !ZOMBIE, GLOBAL, sizeVar);
                novoToken();
            } else {
                erro(AS_FALTAPONTOEVIRGULA);
            }
        } else {loop++;}
        if (loop > 10) { erro(ERRO_INTERNO); }
        setParametros(0);
    }
    celulaTabela = consultar("principal");
    if (celulaTabela.tipo == -1) {
        erro(ASEM_FALTAPRINCIPAL);
    } else {
        CALL(celulaTabela);
    }
    DMEM(sizeVar);
    HALT();
}

void Funcao() {
    int decl = 0, param;
    CelulaTabela celulaTabela;
    novoToken();
    param = TipoParam();
    setParametros(param);
    novoToken();
    if (!reconhece(SN, FECHAPARENTESE)) erro(AS_FALTAPARENTESE);
    novoToken();
    if (!reconhece(SN, ABRECHAVES)) erro(AS_FALTACHAVES);
    novoToken();
    //DECLARAÇÔES
    while(1) {
        if (Tipo()) {
            atualizaTipo();
            novoToken();
            if (!reconheceID()) erro(AS_FALTAID);
            celulaTabela = consultar(tokens[tpos].lexema);
            if (celulaTabela.tipo != -1 && celulaTabela.zombie != ZOMBIE) erro(ASEM_DECLVARIGUAL);
            inserirVar(tipo, tokens[tpos].lexema, 0, LOCAL, decl);
            novoToken();
            decl++;
            while (1) {
                if (reconhece(SN, VIRGULA)) {
                    novoToken();
                    if (!reconheceID()) erro(AS_FALTAID);
                    celulaTabela = consultar(tokens[tpos].lexema);
                    if (celulaTabela.tipo != -1 && celulaTabela.zombie != ZOMBIE) erro(ASEM_DECLVARIGUAL);
                    inserirVar(tipo, tokens[tpos].lexema, !ZOMBIE, LOCAL, decl);
                    novoToken();
                    decl++;
                } else break;
            }
            if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_FALTAPONTOEVIRGULA);
            novoToken();
        } else break;
    }
    AMEM(decl);
    //COMANDOS
    while (1) {
        if (!reconhece(SN, FECHACHAVES)) {
            if(!faltaToken()){
                break;
            }
            Cmd();
        } else break;
    }
    if (!reconhece(SN, FECHACHAVES)) erro(AS_FALTACHAVES);
    novoToken();
    excluirLocais(tauxfun.lexema, param);
    DMEM(decl);
    RET(param);
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
int TipoParam() {
    int posParam = 0;
    CelulaTabela celulaTabela;
    if (reconhece(PR, SEMPARAM)) {
        celulaTabela = consultarLocaisEGlobais(tauxfun.lexema);
        if (celulaTabela.tipo != -1 && celulaTabela.zombie == ZOMBIE) {
            celulaTabela = consultarTipoParametro(tauxfun.lexema, posParam);
            if (celulaTabela.tipo != -1 && celulaTabela.tipo != SEMPARAM) erro(ASEM_DECLTIPODIFERENTE);
        }
    } else if (Tipo()) {
        posParam++;
        atualizaTipo();
        celulaTabela = consultarLocaisEGlobais(tauxfun.lexema);
        if (celulaTabela.tipo != -1 && celulaTabela.zombie == ZOMBIE) {
            celulaTabela = consultarTipoParametro(tauxfun.lexema, posParam);
            if (celulaTabela.tipo != -1 && celulaTabela.tipo != tipo) erro(ASEM_DECLTIPODIFERENTE);
            else if (celulaTabela.tipo == -1) erro(ASEM_DECLTIPODIFERENTE);
            else if (celulaTabela.escopo != GLOBAL) erro(ASEM_DECLTIPODIFERENTE);
        }
        novoToken();
        if (!reconheceID()) erro(AS_FALTAID);
        celulaTabela = consultarLocaisEGlobais(tokens[tpos].lexema);
        if (celulaTabela.tipo != -1) erro(ASEM_DECLVARIGUAL);
        inserirVar(tipo, tokens[tpos].lexema, ZOMBIE, LOCAL, posParam);
        novoToken();
        while (1) {
            if (reconhece(SN, VIRGULA)) {
                novoToken();
                if (!Tipo()) erro(AS_ERRO);
                posParam++;
                atualizaTipo();
                celulaTabela = consultarLocaisEGlobais(tauxfun.lexema);
                if (celulaTabela.tipo != -1 && celulaTabela.zombie == ZOMBIE) {
                    celulaTabela = consultarTipoParametro(tauxfun.lexema, posParam);
                    if (celulaTabela.tipo != -1 && celulaTabela.tipo != tipo) erro(ASEM_DECLTIPODIFERENTE);
                    else if (celulaTabela.tipo == -1) erro(ASEM_DECLTIPODIFERENTE);
                    else if (celulaTabela.escopo != GLOBAL) erro(ASEM_DECLTIPODIFERENTE);
                }
                novoToken();
                if (!reconheceID()) erro(AS_ERRO);
                celulaTabela = consultar(tokens[tpos].lexema);
                if (celulaTabela.tipo != -1) erro(ASEM_DECLVARIGUAL);
                inserirVar(tipo, tokens[tpos].lexema, ZOMBIE, LOCAL, posParam);
                novoToken();
            } else {
                retornarToken();
                break;
            }
        }
        posParam++;
        celulaTabela = consultarTipoParametro(tauxfun.lexema, posParam);
        if (celulaTabela.tipo != -1 && strstr(celulaTabela.posicao, "L") == NULL) {
            if (celulaTabela.escopo == GLOBAL) erro(ASEM_PARAMETROS); //Faltam parâmetros que não foram preenchidos
        }
    } else erro(AS_ERRO);
    alterarParamVar(posParam);
    return posParam;
}

/*Gramatica de TipoParamOpc*/
void TipoParamOpc() {
    if (reconhece(PR, SEMPARAM)) {
        novoToken();
    } else if (Tipo()) {
        atualizaTipo();
        novoToken();
        inserirVar(tipo, "", ZOMBIE, GLOBAL, -1);
        if (reconheceID()) novoToken();
        while (1) {
            if (reconhece(SN, VIRGULA)) {
                novoToken();
                if (!Tipo()) erro(AS_ERRO);
                atualizaTipo();
                novoToken();
                inserirVar(tipo, "", ZOMBIE, GLOBAL, -1);
                if (reconheceID()) novoToken();
            } else break;
        }
    }
}

/*Gramatica de Comandos*/
void Cmd() {
    int tipo, pos = 0, result;
    CelulaTabela celulaTabela;
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
            tipo = consultarLocaisEGlobais(taux.lexema).tipo;
            if (tipo == INTCON && (result != INTCON && result != CT_C)) erro(ASEM_ATRIBUICAO);
            else if (tipo == CT_C && (result != INTCON && result != CT_C)) erro(ASEM_ATRIBUICAO);
            else if (tipo == REALCON && result != REALCON) erro(ASEM_ATRIBUICAO);
            else if (tipo == BOOLEANO && (result != BOOLEANO && result != INTCON)) erro(ASEM_ATRIBUICAO);
            else if (tipo == -1) erro(ASEM_ATRIBUICAO);
            if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_FALTAPONTOEVIRGULA);
            celulaTabela = consultar(taux.lexema);
            STOR(celulaTabela);
        } else {
            if (reconhece(SN, ABREPARENTESES)) {
                celulaTabela = consultar(taux.lexema);
                if (celulaTabela.tipo != -1 && celulaTabela.tipo != SEMRETORNO) erro(ASEM_CHAMADAFUNCAOSEMRET);
                else if (celulaTabela.tipo == -1) erro(ASEM_CHAMADAFUNCAODECL);
                novoToken();
                if (!reconhece(SN, FECHAPARENTESE)) {
                    pos++;
                    result = Expr();
                    celulaTabela = consultarTipoParametro(taux.lexema, pos);
                    if (celulaTabela.tipo != result || celulaTabela.escopo != GLOBAL) erro(ASEM_PARAMETROS);
                    while (reconhece(SN, VIRGULA)) {
                        novoToken();
                        pos++;
                        result = Expr();
                        celulaTabela = consultarTipoParametro(taux.lexema, pos);
                        if (celulaTabela.tipo != result || celulaTabela.escopo != GLOBAL) erro(ASEM_PARAMETROS);
                    }
                    pos++;
                    celulaTabela = consultarTipoParametro(taux.lexema, pos);
                    if (celulaTabela.tipo != -1) {
                        if (celulaTabela.escopo == GLOBAL) erro(ASEM_PARAMETROS);
                    }
                } else {
                    pos++;
                    celulaTabela = consultarTipoParametro(taux.lexema, pos);
                    if (celulaTabela.tipo != -1 || celulaTabela.escopo != GLOBAL) erro(ASEM_PARAMETROS);
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
    int result, lb;
    if (reconhece(SN, ABREPARENTESES)) {
        novoToken();
        result = Expr();
        if (label != 0) {
            LABEL(label);
            label = 0;
        }
        if (result != INTCON && result != BOOLEANO) erro(ASEM_NOTBOOLEANO);
        lb = pegarLabel();
        GOFALSE(lb);
        if (reconhece(SN, FECHAPARENTESE)) {
            novoToken();
            if (tokens[tpos].codigo != PONTOEVIRGULA) {
                Cmd();
                novoToken();
            }
        } else erro(AS_SE);
        lb = pegarLabel();
        GOTO(lb);
        lb = gerarLabel();
        LABEL(lb);
        Senao();
        lb = gerarLabel();
        LABEL(lb);
    } else erro(AS_SE);
}

void Senao() {
    if (reconhece(PR, SENAO)) {
        novoToken();
        Cmd();
        novoToken();
    } else ;
}

void Enquanto() {
    int result;
    if (reconhece(SN, ABREPARENTESES)) {
        LABEL(gerarLabel());
        novoToken();
        result = Expr();
        if (label != 0) {
            LABEL(label);
            label = 0;
        }
        if (result != INTCON && result != BOOLEANO) erro(ASEM_NOTBOOLEANO);
        GOFALSE(gerarLabel());
        if (reconhece(SN, FECHAPARENTESE)) {
            novoToken();
            if (tokens[tpos].codigo != PONTOEVIRGULA) {
                Cmd();
                novoToken();
            }
        } else erro(AS_ENQUANTO);
        GOTO(pegarLabel());
        LABEL(pegarLabel());
    } else erro(AS_ENQUANTO);
}

void Para() {
    int result;
    if (reconhece(SN, ABREPARENTESES)) {
        novoToken();
        if (!Atrib()) erro(AS_PARA);
        LABEL(gerarLabel());
        if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_PARA);
        novoToken();
        result = Expr();
        if (label != 0) {
            LABEL(label);
            label = 0;
        }
        if (result != INTCON && result != BOOLEANO) erro(ASEM_NOTBOOLEANO);
        GOFALSE(gerarLabel());
        GOTO(gerarLabel());
        LABEL(gerarLabel());
        if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_PARA);
        novoToken();
        if (!Atrib()) erro(AS_PARA);
        GOTO(pegarLabel());
        LABEL(pegarLabel() + 1);
        if (reconhece(SN, FECHAPARENTESE)) {
            novoToken();
            if (tokens[tpos].codigo != PONTOEVIRGULA) {
                Cmd();
                novoToken();
            }
        } else erro(AS_PARA);
        GOTO(pegarLabel() + 1);
        LABEL(pegarLabel() - 2);
    } else erro(AS_PARA);
}

void Retorne() {
    int result;
    char buffer[10], posicao[10];
    CelulaTabela function = consultar(tauxfun.lexema);
    if (function.tipo == SEMRETORNO && reconhece(SN, PONTOEVIRGULA)) ;
    else if (function.tipo != SEMRETORNO && !reconhece(SN, PONTOEVIRGULA)) {
        funretorno = 1;
        result = Expr();
        if (label != 0) {
            LABEL(label);
            label = 0;
        }
        if (function.tipo != result) erro(ASEM_RETORNARFUNCAO);
        if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_FALTAPONTOEVIRGULA);

        sprintf(buffer, "%d", 1);
        strcpy(posicao, buffer);
        strcpy(buffer, "");
        sprintf(buffer, "%d", -(getParametros() + 3));
        strcat(posicao, ",");
        strcat(posicao, buffer);
        STOR2(posicao);
    } else erro(ASEM_RETORNARFUNCAO);
}

/*Gramatica de Atribuição*/
int Atrib() {
    float result;
    if (reconheceID()) {
        taux = tokens[tpos];
        novoToken();
        if (!reconhece(SN, ATRIBUICAO)) erro(AS_PARA);
        novoToken();
        result = Expr();
        tipo = consultar(taux.lexema).tipo;
        if (tipo == INTCON && (result != INTCON && result != CT_C)) erro(ASEM_ATRIBUICAO);
        else if (tipo == CT_C && (result != INTCON && result != CT_C)) erro(ASEM_ATRIBUICAO);
        else if (tipo == REALCON && result != REALCON) erro(ASEM_ATRIBUICAO);
        else if (tipo == BOOLEANO && (result != BOOLEANO && result != INTCON)) erro(ASEM_ATRIBUICAO);
        else if (tipo == -1) erro(ASEM_ATRIBUICAO);
        CelulaTabela celulaTabela = consultar(taux.lexema);
        STOR(celulaTabela);
        return 1;
    }
    return 0;
}

/*Gramatica de Expressões*/
int Expr() {
    int result, segundoOp, operacao;
    result = ExprSimp();
    operacao = OpRel();
    if (operacao) {
        novoToken();
        segundoOp = Expr();
        //Semântico - Checar se o resultado é Booleano
        if ((result == INTCON || result == REALCON || result == CT_C || result == BOOLEANO) &&
                (segundoOp == INTCON || segundoOp == REALCON || segundoOp == CT_C || segundoOp == BOOLEANO)) ;
        else erro(ASEM_ATRIBUICAO);

        //MaqPilha - IGUAL, DIFERENTE, MAIORIGUAL, MENORIGUAL, MAIOR, MENOR
        if (result == REALCON || segundoOp == REALCON) SUBF();
        else SUB();
        if (operacao == IGUAL) {
            GOFALSE(pegarLabel());
            PUSH(0);
            GOTO(pegarLabel());
            LABEL(gerarLabel());
            PUSH(1);
            LABEL(gerarLabel());
        } else if (operacao == DIFERENTE) {
            GOFALSE(pegarLabel());
            PUSH(1);
            GOTO(pegarLabel());
            LABEL(gerarLabel());
            PUSH(0);
            LABEL(gerarLabel());
        } else if (operacao == MAIOR) {
            GOTRUE(pegarLabel());
            PUSH(0);
            GOTO(pegarLabel());
            LABEL(gerarLabel());
            PUSH(1);
            LABEL(gerarLabel());
        } else if (operacao == MAIORIGUAL) {
            COPY();
            GOFALSE(pegarLabel());
            GOTRUE(pegarLabel());
            PUSH(0);
            GOTO(pegarLabel());
            LABEL(gerarLabel());
            POP();
            LABEL(gerarLabel());
            PUSH(1);
            LABEL(gerarLabel());
        } else if (operacao == MENOR) {
            COPY();
            GOFALSE(pegarLabel());
            GOTRUE(pegarLabel());
            PUSH(1);
            GOTO(pegarLabel());
            LABEL(gerarLabel());
            POP();
            LABEL(gerarLabel());
            PUSH(0);
            LABEL(gerarLabel());
        } else if (operacao == MENORIGUAL) {
            COPY();
            GOFALSE(pegarLabel());
            GOTRUE(pegarLabel());
            POP();
            PUSH(0);
            GOTO(pegarLabel());
            LABEL(gerarLabel());
            LABEL(gerarLabel());
            PUSH(1);
            LABEL(gerarLabel());
        }

        result = BOOLEANO;
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
    int segunOp = 0, tmpResult = 0, result;
    result = primOp;
    while (reconhece(SN, ADICAO) || reconhece(SN, SUBTRACAO) || reconhece(SN, OR)) {
        //MaqPilha - OR, ADICAO, SUBTRACAO
        if (reconhece(SN, OR)) {
            COPY();
            if (label == 0) {
                gerarLabel();
                label = pegarLabel();
            }
            GOTRUE(label);
            POP();
            novoToken();
            segunOp = Termo();
            salva = tokens[tpos];
        } else if(reconhece(SN, ADICAO)) {
            novoToken();
            segunOp = Termo();
            if ((primOp == REALCON && segunOp == REALCON)) {
                ADDF();
            } else {
                ADD();
            }
        } else if(reconhece(SN, SUBTRACAO)){
            novoToken();
            segunOp = Termo();
            if ((primOp == REALCON && segunOp == REALCON)) {
                SUBF();
            } else {
                SUB();
            }
        }

        //Semântico - Tipos de expressões aceitos pelo Cmm
        if (salva.codigo == OR)                             tmpResult = BOOLEANO;
        else if (primOp == INTCON && segunOp == INTCON)     tmpResult = INTCON;
        else if (primOp == REALCON && segunOp == REALCON)   tmpResult = REALCON;
        else if (primOp == CT_C && segunOp == CT_C)         tmpResult = CT_C;
        else if (primOp == BOOLEANO && segunOp == BOOLEANO) tmpResult = BOOLEANO;
        else if ((primOp == INTCON && segunOp == CT_C) || (primOp == CT_C && segunOp == INTCON))         tmpResult = INTCON;
        else if ((primOp == BOOLEANO && segunOp == INTCON )|| (primOp == INTCON && segunOp == BOOLEANO)) tmpResult = BOOLEANO;
        else erro(ASEM_ATRIBUICAO);

        result = Resto(tmpResult);
    }
    return result;
}

int Sobra(int primOp) {
    Token salva;
    int segunOp = 0, tmpResult = 0, result;
    result = primOp;
    while (reconhece(SN, MULTIPLICACAO) || reconhece(SN, DIVISAO) || reconhece(SN, AND)) {
        //MaqPilha - AND, MULT, DIV
        if (reconhece(SN, AND)) {
            if (label == 0) {
                gerarLabel();
                label = pegarLabel();
            }
            GOFALSE(label);
            POP();
            novoToken();
            segunOp = Termo();
            salva = tokens[tpos];
        } else if(reconhece(SN, MULTIPLICACAO) ){
            novoToken();
            segunOp = Termo();
            if((primOp == REALCON && segunOp == REALCON)){
                MULF();
            }else{
                MUL();
            }
        } else if(reconhece(SN, DIVISAO)){
            novoToken();
            segunOp = Termo();
            if((primOp == REALCON && segunOp == REALCON)){
                DIVF();
            }else{
                DIV();
            }
        }

        //Semântico - Tipos de expressões aceitos pelo Cmm
        if (salva.codigo == OR)                             tmpResult = BOOLEANO;
        else if (primOp == INTCON && segunOp == INTCON)     tmpResult = INTCON;
        else if (primOp == REALCON && segunOp == REALCON)   tmpResult = REALCON;
        else if (primOp == CT_C && segunOp == CT_C)         tmpResult = CT_C;
        else if (primOp == BOOLEANO && segunOp == BOOLEANO) tmpResult = BOOLEANO;
        else if ((primOp == INTCON && segunOp == CT_C) || (primOp == CT_C && segunOp == INTCON))         tmpResult = INTCON;
        else if ((primOp == BOOLEANO && segunOp == INTCON )|| (primOp == INTCON && segunOp == BOOLEANO)) tmpResult = BOOLEANO;
        else erro(ASEM_ATRIBUICAO);
        result = Sobra(tmpResult);
    }
    return result;
}

int Fator() {
    Token salvar;
    CelulaTabela celulaTabela;
    int result = -1, pos = 0;
    if (tokens[tpos].categoria == INTCON ||
             tokens[tpos].categoria == REALCON ||
             tokens[tpos].categoria == CT_L ||
            (tokens[tpos].categoria == CT_C && !reconhece(SN, NOT)))
    {
        PUSH2(tokens[tpos]);
        result = tokens[tpos].categoria;
        novoToken();
    } else if (reconheceID()) {
        salvar = tokens[tpos];
        novoToken();
        if (reconhece(SN, ABREPARENTESES)) {
            celulaTabela = consultar(salvar.lexema);
            if (celulaTabela.tipo == SEMRETORNO) erro(ASEM_CHAMADAFUNCAOCOMRET);
            else if (celulaTabela.tipo == -1) erro(ASEM_CHAMADAFUNCAODECL);
            result = celulaTabela.tipo;
            novoToken();
            if (!reconhece(SN, FECHAPARENTESE)) {
                if (celulaTabela.tipo != SEMRETORNO) AMEM(1);
                pos++;
                result = Expr();
                celulaTabela = consultarTipoParametro(salvar.lexema, pos);
                if (celulaTabela.tipo != result || celulaTabela.escopo != GLOBAL) erro(ASEM_PARAMETROS); //Não achou a função / parâmetro
                while (reconhece(SN, VIRGULA)) {
                    novoToken();
                    pos++;
                    result = Expr();
                    celulaTabela = consultarTipoParametro(salvar.lexema, pos);
                    if (celulaTabela.tipo != result || celulaTabela.escopo != GLOBAL) erro(ASEM_PARAMETROS); //Não achou a função / parâmetro
                }
                pos++;
                celulaTabela = consultarTipoParametro(salvar.lexema, pos);
                if (celulaTabela.tipo != -1 && strstr(celulaTabela.posicao, "L") == NULL) {
                    if (celulaTabela.escopo == GLOBAL) erro(ASEM_PARAMETROS); //Faltam parâmetros que não foram preenchidos
                }
            } else {
                pos++;
                celulaTabela = consultarTipoParametro(salvar.lexema, pos);
                if (celulaTabela.tipo != -1 && celulaTabela.escopo == GLOBAL) erro(ASEM_PARAMETROS);
            }
            if (!reconhece(SN, FECHAPARENTESE)) erro(AS_FALTAPARENTESE);
            celulaTabela = consultar(salvar.lexema);
            CALL(celulaTabela);
            novoToken();
        } else {
            retornarToken();
            celulaTabela = consultar(tokens[tpos].lexema);
            LOAD(celulaTabela);
            novoToken();
            result = celulaTabela.tipo;
        }
    } else if (reconhece(SN, ABREPARENTESES)) {
        novoToken();
        result = Expr();
        if (!reconhece(SN, FECHAPARENTESE)) erro(AS_FALTAPARENTESE);
        novoToken();
    } else if (reconhece(SN, NOT)) {
        novoToken();
        result = Fator();
        //Semântico - Tipos de expressões aceitos pelo Cmm
        if (result == INTCON || result == REALCON || result == CT_C || result == BOOLEANO) result = BOOLEANO;
        else erro(ASEM_ATRIBUICAO);
        GOFALSE(pegarLabel());
        PUSH(0);
        GOTO(pegarLabel());
        LABEL(gerarLabel());
        PUSH(1);
        LABEL(gerarLabel());
    } else {
        erro(AS_FALTAEXPREOUNUM);
    }
    return result;

}

/*Gramatica de Operador Relacional*/
int OpRel() {
    if (reconhece(SN, IGUAL) || reconhece(SN, DIFERENTE) ||
            reconhece(SN, MENORIGUAL) || reconhece(SN, MENOR) ||
            reconhece(SN, MAIORIGUAL) || reconhece(SN, MAIOR))
    {
        return tokens[tpos].codigo;
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

void setParametros(int param) {
    funparam = param;
}

int getParametros() {
    return funparam;
}
