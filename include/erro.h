//
// Created by mateus on 11/10/2017.
//

#ifndef COMPILADOR_ERRO_H
#define COMPILADOR_ERRO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

enum Erro {ABRIR_ARQUIVO,
    ERRO_INTERNO,
    MAX_ALOCACAO,
    AL_CARACINVALIDO,
    AL_COMENTARIO,
    AS_ERRO,
    AS_SE, AS_ENQUANTO, AS_PARA,
    AS_COMANDO,
    AS_FALTAPARENTESE,
    AS_FALTACHAVES,
    AS_FALTAID,
    AS_FALTAPONTOEVIRGULA,
    AS_FALTAEXPREOUNUM,
    ASEM_ATRIBUICAO,
    ASEM_RETORNARFUNCAO,
    ASEM_DECLVARIGUAL,
    ASEM_DECLFUNIGUAL,
    ASEM_DECLTIPODIFERENTE,
    ASEM_DECLTIPORETDIFERENTE,
    ASEM_CHAMADAFUNCAODECL,
    ASEM_CHAMADAFUNCAOCOMRET,
    ASEM_CHAMADAFUNCAOSEMRET,
    ASEM_PARAMETROS,
    ASEM_NOTBOOLEANO,
    ASEM_FALTAPRINCIPAL};

void logErro(int erro, int num, ...);

#endif //COMPILADOR_ERRO_H
