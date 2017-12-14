//
// Created by mateus and wellington on 31/10/2017.
//

#ifndef COMPILADOR_ANALISADORSINTATICO_H
#define COMPILADOR_ANALISADORSINTATICO_H

#include "analisadorlexico.h"
#include "gerenciador.h"

#include "erro.h"

extern Token *tokens;
extern int tkpos;
int tpos, tipo, loop, funparam,
        funretorno, sizeVar, label;
Token taux, tauxfun;

int analisadorCodigo();

/*Gramatica de Programa*/
/*
 * Prog → {
 *      Tipo Id { VIRGULA Id } PONTOEVIRGULA |
 *      Prototipo Tipo       Id ABREPARENTESES TipoParamOpc FECHAPARENTESES { VIRGULA Id ABRECHAVES TipoParamOpc FECHAPARENTESES } PONTOEVIRGULA |
 *      Prototipo SEMRETORNO Id ABREPARENTESES TipoParamOpc FECHAPARENTESES { VIRGULA Id ABRECHAVES TipoParamOpc FECHAPARENTESES } PONTOEVIRGULA |
 *      Tipo Id ABREPARENTESES Funcao |
 *      SEMRETORNO Id ABREPARENTESES Funcao
 *      }
 * Funcao → TipoParam FECHAPARENTESES ABRECHAVES { Tipo Id { VIRGULA Id } PONTOEVIRGULA } { Cmd } FECHACHAVES
 */

void Prog();
void Funcao();

/*Gramatica de Tipo*/
/*
 * Tipo → caracter | inteiro | real | booleano
 */

int Tipo();

/*Gramatica de TipoParam*/
/*
 * TipoParam → semparam | Tipo Id {VIRGULA Tipo Id}
 */

int TipoParam();

/*Gramatica de TipoParamOpc*/
/*
 * TipoParamOpc → semparam | Tipo [Id] {',' Tipo [Id]}
 */

void TipoParamOpc();

/*Gramatica de Comandos*/
/*
 * Cmd → Se | Enquanto | Para | Retorne |
 *       Atrib PONTOEVIRGULA | NId | NovoComando | PONTOEVIRGULA
 * Se → ABREPARENTESES Expr FECHAPARENTESES Cmd | Senao
 *   Senao → Cmd
 * Enquanto → ABREPARENTESES Expr FECHAPARENTESES Cmd
 * Para → ABREPARENTESES Atrib PONTOEVIRGULA Expr PONTOEVIRGULA Atrib FECHAPARENTESES Cmd
 * Retorne → Expr PONTOEVIRGULA | PONTOEVIRGULA
 * Id → ABREPARENTESES [Expr {VIRGULA Expr}]  FECHAPARENTESES PONTOEVIRGULA
 * NovoComando → ABRECHAVES Cmd FECHACHAVES | ε
 */

void Cmd();
void Se();
void Senao();
void Enquanto();
void Para();
void Retorne();

/*Gramatica de Atribuição*/
/*
 * Atrib → Id ATRIBUICAO Expr
 */

int Atrib();

/*Gramatica de Expressões*/
/*
 * Expr → ExprSimp [ OpRel Expr ]
 */

int Expr();

/*Gramatica de Expressões Simples*/
/*
 * ExprSimp → = [ + | – ] Termo Resto
 * Termo → Fator Sobra
 * Resto → [ [ SOMA | SUBTRACAO | OR ]  Termo ]
 * Sobra → [ [ MULTIPLICACAO | DIVISAO | AND ]  Sobra ]
 * Fator → Id | intcon | realcon | caraccon | cadeiacon |
 *          Id ABREPARENTESES Expr { VIRGULA Expr } ] FECHAPARENTESES | ABREPARENTESES Expr FECHAPARENTESES | DIFERENTE Fator
 */
int ExprSimp();
int Termo();
int Resto(int primOp);
int Sobra(int primOp);
int Fator();

/*Gramatica de Operador Relacional*/
/*
 * OpRel → IGUAL | DIFERENTE | MENORIGUAL | MENOR | MAIORIGUAL | MAIOR
 */

int OpRel();

int faltaToken();
int novoToken();
int retornarToken();
void erro(int erro);
void atualizaTipo();
int reconheceID();
int reconhece(int categoria, int codigo);
void setParametros(int param);
int getParametros();

#endif //COMPILADOR_ANALISADORSINTATICO_H
