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
    mostrarTabela();
    fclose(codigo);
    return 1;
}

/*Gramatica de Programa*/
void Prog() {
    size = 0;
    funretorno = 0;
    CelulaTabela celulaTabela;
    while (faltaToken()) {
        if (reconhece(PR, PROTOTIPO)) {
            novoToken();
            if (Tipo() || reconhece(PR, SEMRETORNO)) {
                atualizaTipo();
                novoToken();
                if (!reconheceID()) erro(AS_FALTAID);
                celulaTabela = consultar(tokens[tpos].lexema);
                if (celulaTabela.tipo != -1) erro(ASEM_DECLFUNIGUAL);
                inserir(tipo, tokens[tpos].lexema, ZOMBIE, GLOBAL, tokens[tpos].linha + 1, -1);
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
                        inserir(tipo, tokens[tpos].lexema, ZOMBIE, GLOBAL, tokens[tpos].linha + 1, -1);
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
            if (size > 0) AMEM(size);
            size = 0;
            atualizaTipo();
            novoToken();
            if (!reconheceID()) erro(AS_FALTAID);
            tauxfun = tokens[tpos];
            celulaTabela = consultar(tauxfun.lexema);
            if (celulaTabela.tipo != -1 && celulaTabela.zombie != ZOMBIE) erro(ASEM_DECLFUNIGUAL);
            else if (celulaTabela.tipo != -1 && celulaTabela.tipo != tipo) erro(ASEM_DECLTIPORETDIFERENTE);
            inserir(tipo, tauxfun.lexema, !ZOMBIE, GLOBAL, tauxfun.linha + 1, -1);
            novoToken();
            if (!reconhece(SN, ABREPARENTESES)) erro(AS_FALTAPARENTESE);
            Funcao();
            excluirLocais();
        } else if (Tipo()) {
            atualizaTipo();
            novoToken();
            if (!reconheceID()) erro(AS_FALTAID);
            taux = tokens[tpos];
            novoToken();
            if (reconhece(SN, ABREPARENTESES)) {
                if (size > 0) AMEM(size);
                tauxfun = tokens[tpos-1];
                celulaTabela = consultar(tauxfun.lexema);
                if (celulaTabela.tipo != -1 && celulaTabela.zombie != ZOMBIE) erro(ASEM_DECLFUNIGUAL);
                else if (celulaTabela.tipo != -1 && celulaTabela.tipo != tipo) erro(ASEM_DECLTIPORETDIFERENTE);
                inserir(tipo, tauxfun.lexema, !ZOMBIE, GLOBAL, tauxfun.linha + 1, -1);
                Funcao();
                //Função com tipo precisa de retorno
                if (funretorno != 1) erro(ASEM_RETORNARFUNCAO);
                excluirLocais();
            } else if (reconhece(SN, VIRGULA)) {
                size++;
                celulaTabela = consultar(taux.lexema);
                if (celulaTabela.tipo != - 1) erro(ASEM_DECLVARIGUAL);
                inserir(tipo, taux.lexema, !ZOMBIE, GLOBAL, taux.linha + 1, -1);
                while (reconhece(SN, VIRGULA)) {
                    size++;
                    novoToken();
                    if (!reconheceID()) erro(AS_FALTAID);
                    celulaTabela = consultar(tokens[tpos].lexema);
                    if (celulaTabela.tipo != -1) erro(ASEM_DECLVARIGUAL);
                    inserir(tipo, tokens[tpos].lexema, !ZOMBIE, GLOBAL, tokens[tpos].linha + 1, -1);
                    novoToken();
                }
                if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_FALTAPONTOEVIRGULA);
                novoToken();
            } else if (reconhece(SN, PONTOEVIRGULA)) {
                size++;
                celulaTabela = consultar(taux.lexema);
                if (celulaTabela.tipo != - 1) erro(ASEM_DECLVARIGUAL);
                inserir(tipo, taux.lexema, !ZOMBIE, GLOBAL, taux.linha + 1, -1);
                novoToken();
            } else {
                erro(AS_FALTAPONTOEVIRGULA);
            }
        } else {loop++;}
        if (loop > 10) { erro(ERRO_INTERNO);    }
    }
    HALT();
}

void Funcao() {
    int decl = 0;
    CelulaTabela celulaTabela;
    novoToken();
    TipoParam();
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
            inserir(tipo, tokens[tpos].lexema, 0, LOCAL, tokens[tpos].linha + 1, decl);
            novoToken();
            decl++;
            while (1) {
                if (reconhece(SN, VIRGULA)) {
                    novoToken();
                    if (!reconheceID()) erro(AS_FALTAID);
                    celulaTabela = consultar(tokens[tpos].lexema);
                    if (celulaTabela.tipo != -1 && celulaTabela.zombie != ZOMBIE) erro(ASEM_DECLVARIGUAL);
                    inserir(tipo, tokens[tpos].lexema, !ZOMBIE, LOCAL, tokens[tpos].linha + 1, decl);
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
    int posParam = 0;
    CelulaTabela celulaTabela;
    if (reconhece(PR, SEMPARAM)) {
        celulaTabela = consultar2(tauxfun.lexema);
        if (celulaTabela.tipo != -1 && celulaTabela.zombie == ZOMBIE) {
            //celulaTabela = consultarTipoParametro(tauxfun.lexema, posParam);
            if (celulaTabela.tipo != -1 && celulaTabela.tipo != SEMPARAM) erro(ASEM_DECLTIPODIFERENTE);
            else if (celulaTabela.tipo == -1) erro(ASEM_DECLTIPODIFERENTE);
        }
    } else if (Tipo()) {
        posParam++;
        atualizaTipo();
        celulaTabela = consultar2(tauxfun.lexema);
        if (celulaTabela.tipo != -1 && celulaTabela.zombie == ZOMBIE) {
            celulaTabela = consultarTipoParametro(tauxfun.lexema, posParam);
            if (celulaTabela.tipo != -1 && celulaTabela.tipo != tipo) erro(ASEM_DECLTIPODIFERENTE);
            else if (celulaTabela.tipo == -1) erro(ASEM_DECLTIPODIFERENTE);
            else if (celulaTabela.escopo != LOCAL) erro(ASEM_DECLTIPODIFERENTE);
        }
        novoToken();
        if (!reconheceID()) erro(AS_FALTAID);
        inserir(tipo, tokens[tpos].lexema, ZOMBIE, LOCAL, tokens[tpos].linha + 1, -1);
        novoToken();
        while (1) {
            celulaTabela = consultar(tokens[tpos].lexema);
            LOAD(celulaTabela);
            if (reconhece(SN, VIRGULA)) {
                novoToken();
                if (!Tipo()) erro(AS_ERRO);
                posParam++;
                atualizaTipo();
                celulaTabela = consultar2(tauxfun.lexema);
                if (celulaTabela.tipo != -1 && celulaTabela.zombie == ZOMBIE) {
                    celulaTabela = consultarTipoParametro(tauxfun.lexema, posParam);
                    if (celulaTabela.tipo != -1 && celulaTabela.tipo != tipo) erro(ASEM_DECLTIPODIFERENTE);
                    else if (celulaTabela.tipo == -1) erro(ASEM_DECLTIPODIFERENTE);
                    else if (celulaTabela.escopo != LOCAL) erro(ASEM_DECLTIPODIFERENTE);
                }
                novoToken();
                if (!reconheceID()) erro(AS_ERRO);
                celulaTabela = consultar(tokens[tpos].lexema);
                if (celulaTabela.tipo != -1 && celulaTabela.zombie) erro(ASEM_DECLVARIGUAL);
                inserir(tipo, tokens[tpos].lexema, ZOMBIE, LOCAL, tokens[tpos].linha + 1, -1);
                novoToken();
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
        novoToken();
    } else if (Tipo()) {
        atualizaTipo();
        novoToken();
        inserir(tipo, "", ZOMBIE, LOCAL, tokens[tpos].linha + 1, -1);
        if (reconheceID()) novoToken();
        while (1) {
            if (reconhece(SN, VIRGULA)) {
                novoToken();
                if (!Tipo()) erro(AS_ERRO);
                atualizaTipo();
                novoToken();
                inserir(tipo, "", ZOMBIE, LOCAL, tokens[tpos].linha + 1, -1);
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
            tipo = consultar(taux.lexema).tipo;
            if (tipo == INTCON && (result != INTCON && result != CT_C)) erro(ASEM_ATRIBUICAO);
            else if (tipo == CT_C && (result != INTCON && result != CT_C)) erro(ASEM_ATRIBUICAO);
            else if (tipo == REALCON && result != REALCON) erro(ASEM_ATRIBUICAO);
            else if (tipo == BOOLEANO && (result != BOOLEANO && result != INTCON)) erro(ASEM_ATRIBUICAO);
            else if (tipo == -1) erro(ASEM_ATRIBUICAO);
            if (!reconhece(SN, PONTOEVIRGULA)) erro(AS_FALTAPONTOEVIRGULA);
            CelulaTabela celulaTabela = consultar(taux.lexema);
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
                    if (celulaTabela.tipo != result || celulaTabela.escopo != LOCAL) erro(ASEM_PARAMETROS);
                    while (reconhece(SN, VIRGULA)) {
                        novoToken();
                        pos++;
                        result = Expr();
                        celulaTabela = consultarTipoParametro(taux.lexema, pos);
                        if (celulaTabela.tipo != result || celulaTabela.escopo != LOCAL) erro(ASEM_PARAMETROS);
                    }
                    pos++;
                    celulaTabela = consultarTipoParametro(taux.lexema, pos);
                    if (celulaTabela.tipo != -1) {
                        if (celulaTabela.escopo == LOCAL) erro(ASEM_PARAMETROS);
                    }
                } else {
                    pos++;
                    celulaTabela = consultarTipoParametro(taux.lexema, pos);
                    if (celulaTabela.tipo != -1 || celulaTabela.escopo != LOCAL) erro(ASEM_PARAMETROS);
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
        //novoToken();
    } else if (reconhece(SN, PONTOEVIRGULA)) {novoToken();}
}

void Se() {
    int result, label;
    if (reconhece(SN, ABREPARENTESES)) {
        novoToken();
        result = Expr();
        if (result != INTCON && result != BOOLEANO) erro(ASEM_NOTBOOLEANO);
        label = pegarLabel();
        GOFALSE(label);
        if (!reconhece(SN, FECHAPARENTESE)) erro(AS_SE);
        novoToken();
        Cmd();
        label = pegarLabel();
        GOTO(label);
        label = gerarLabel();
        LABEL(label);
        novoToken();
        Senao();
        label = gerarLabel();
        LABEL(label);
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
        if (result != INTCON && result != BOOLEANO) erro(ASEM_NOTBOOLEANO);
        GOFALSE(gerarLabel());
        if (reconhece(SN, FECHAPARENTESE)) {
            novoToken();
            Cmd();
            novoToken();
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
            Cmd();
            novoToken();
        } else erro(AS_PARA);
        GOTO(pegarLabel() + 1);
        LABEL(pegarLabel() - 2);
    } else erro(AS_PARA);
}

void Retorne() {
    int result;
    CelulaTabela function = consultar(tauxfun.lexema);
    if (function.tipo == SEMRETORNO && reconhece(SN, PONTOEVIRGULA)) ;
    else if (function.tipo != SEMRETORNO && !reconhece(SN, PONTOEVIRGULA)) {
        funretorno = 1;
        result = Expr();
        if (function.tipo != result) erro(ASEM_RETORNARFUNCAO);
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
            tipo = consultar(taux.lexema).tipo;
            if (tipo == INTCON) {
                if (!isInteger(result)) erro(ASEM_ATRIBUICAO);
            } else if (tipo == REALCON) {
            } else erro(ASEM_ATRIBUICAO);
            CelulaTabela celulaTabela = consultar(taux.lexema);
            STOR(celulaTabela);
            return 1;
        } else {
            retornarToken();
        }
    }
    return 0;
}

/*Gramatica de Expressões*/
int Expr() {
    Token pushtk;
    int result, segundoOp, operacao;
    result = ExprSimp();
    operacao = OpRel();
    if (operacao != 0) {
        novoToken();
        segundoOp = Expr();
        if ((result == INTCON || result == REALCON || result == CT_C || result == BOOLEANO) &&
                (segundoOp == INTCON || segundoOp == REALCON || segundoOp == CT_C || segundoOp == BOOLEANO))
            result = BOOLEANO;
        else erro(ASEM_ATRIBUICAO);
        pushtk = tokens[tpos];
        if (operacao == IGUAL) {
            SUB();
            GOFALSE(pegarLabel());
            strcpy(pushtk.lexema, "0");
            PUSH(pushtk);
            GOTO(pegarLabel());
            LABEL(gerarLabel());
            strcpy(pushtk.lexema, "1");
            PUSH(pushtk);
            LABEL(gerarLabel());
        } else if (operacao == DIFERENTE) {

        } else if (operacao == MAIOR) {
            SUB();
            GOTRUE(pegarLabel());
            strcpy(pushtk.lexema, "0");
            PUSH(pushtk);
            GOTO(pegarLabel());
            LABEL(gerarLabel());
            strcpy(pushtk.lexema, "1");
            PUSH(pushtk);
            LABEL(gerarLabel());
        } else if (operacao == MAIORIGUAL) {

        } else if (operacao == MENOR) {
            SUB();
            COPY();
            GOFALSE(pegarLabel());
            GOTRUE(pegarLabel());
            strcpy(pushtk.lexema, "1");
            PUSH(pushtk);
            GOTO(pegarLabel());
            LABEL(gerarLabel());
            POP();
            LABEL(gerarLabel());
            strcpy(pushtk.lexema, "0");
            PUSH(pushtk);
            LABEL(gerarLabel());
        } else if (operacao == MENORIGUAL) {

        }
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
        if (reconhece(SN, OR)) salva = tokens[tpos];

        if(reconhece(SN, ADICAO) ){
            novoToken();
            segunOp = Termo();
            if((primOp == REALCON && segunOp == REALCON)){
                ADDF();
            }else{
                ADD();
            }

        }
        if(reconhece(SN, SUBTRACAO)){
            novoToken();
            segunOp = Termo();
            if((primOp == REALCON && segunOp == REALCON)){
                SUBF();
            }else{
                SUB();
            }
        }

        if (salva.codigo == OR) tmpResult = BOOLEANO;
        else if (primOp == INTCON && segunOp == INTCON) tmpResult = INTCON;
        else if (primOp == REALCON && segunOp == REALCON) tmpResult = REALCON;
        else if (primOp == CT_C && segunOp == CT_C) tmpResult = CT_C;
        else if ((primOp == INTCON && segunOp == CT_C) || (primOp == CT_C && segunOp == INTCON)) tmpResult = INTCON;
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
        if (reconhece(SN, OR)) salva = tokens[tpos];
       if(reconhece(SN, MULTIPLICACAO) ){
            novoToken();
            segunOp = Termo();
            if((primOp == REALCON && segunOp == REALCON)){
                MULF();
            }else{
                MUL();
            }

        }
        if(reconhece(SN, DIVISAO)){
            novoToken();
            segunOp = Termo();
            if((primOp == REALCON && segunOp == REALCON)){
                DIVF();
            }else{
                DIV();
            }
        }
        if (salva.codigo == OR) tmpResult = BOOLEANO;
        else if (primOp == INTCON && segunOp == INTCON) tmpResult = INTCON;
        else if (primOp == REALCON && segunOp == REALCON) tmpResult = REALCON;
        else if (primOp == CT_C && segunOp == CT_C) tmpResult = CT_C;
        else if ((primOp == INTCON && segunOp == CT_C) || (primOp == CT_C && segunOp == INTCON)) tmpResult = INTCON;
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
            (tokens[tpos].categoria == CT_C && tokens[tpos].lexema != "!")||
        tokens[tpos].categoria == CT_L)
    {
        PUSH(tokens[tpos]);
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
                pos++;
                result = Expr();
                celulaTabela = consultarTipoParametro(salvar.lexema, pos);
                if (celulaTabela.tipo != result || celulaTabela.escopo != LOCAL) erro(ASEM_PARAMETROS);
                while (reconhece(SN, VIRGULA)) {
                    novoToken();
                    pos++;
                    result = Expr();
                    celulaTabela = consultarTipoParametro(salvar.lexema, pos);
                    if (celulaTabela.tipo != result || celulaTabela.escopo != LOCAL) erro(ASEM_PARAMETROS);
                }
                pos++;
                celulaTabela = consultarTipoParametro(salvar.lexema, pos);
                if (celulaTabela.tipo != -1) {
                    if (celulaTabela.escopo == LOCAL) erro(ASEM_PARAMETROS);
                }
            } else {
                pos++;
                celulaTabela = consultarTipoParametro(salvar.lexema, pos);
                if (celulaTabela.tipo != -1 && celulaTabela.escopo != GLOBAL) erro(ASEM_PARAMETROS);
            }
            if (!reconhece(SN, FECHAPARENTESE)) erro(AS_FALTAPARENTESE);
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
        if (result == INTCON || result == REALCON || result == CT_C) result = BOOLEANO;
        else erro(ASEM_ATRIBUICAO);
        salvar = tokens[tpos];
        GOFALSE(pegarLabel());
        strcpy(salvar.lexema, "0");
        PUSH(salvar);
        GOTO(pegarLabel());
        LABEL(gerarLabel());
        strcpy(salvar.lexema, "1");
        PUSH(salvar);
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

int isInteger(float val) {
    int truncated = (int)val;
    return (val == truncated);
}
