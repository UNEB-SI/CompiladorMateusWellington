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
int tpos;
int tipo;
Token taux;

int analisadorSintatico();

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

void TipoParam();

/*Gramatica de TipoParamOpc*/
/*
 * TipoParamOpc → semparam | TipoParamOpcResto
 * TipoParamOpcResto → Tipo IdResto ',' TipoParamOpcResto | ε
 * IdResto → Id | ε
 */

void TipoParamOpc();
void TipoParamOpcResto();
void IdResto();

/*Gramatica de Comandos*/
/*
 * Cmd → Se | Enquanto | Para | Retorne |
 *       Atrib PONTOEVIRGULA | NId | NovoComando | PONTOEVIRGULA
 * Se → ABREPARENTESES Expr FECHAPARENTESES Cmd | Senao
 *   Senao → Cmd
 * Enquanto → ABREPARENTESES Expr FECHAPARENTESES Cmd
 * Para → ABREPARENTESES Atrib PONTOEVIRGULA Expr PONTOEVIRGULA Atrib FECHAPARENTESES Cmd
 * Retorne → Expr ; | ;
 * Id → ABREPARENTESES Conteudo FECHAPARENTESES PONTOEVIRGULA
 *   Conteudo → Expr ConteudoResto | ε
 *   ConteudoResto → VIRGULA Expr ConteudoResto | ε
 * NovoComando → ABRECHAVES Cmd FECHACHAVES | ε
 */

void Cmd();
void Se();
void Senao();
void Enquanto();
void Para();
void Retorne();
void NId();
void Conteudo();
void ConteudoResto();
void NovoComando();

/*Gramatica de Atribuição*/
/*
 * Atrib → Id ATRIBUICAO Expre
 */

int Atrib();

/*Gramatica de Expressões*/
/*
 * Expr → Expre | TermoExpr
 * TermoExpr → OpRel RestoExpr | ε
 * RestoExpr → Expre
 */

void Expr();
void TermoExpr();
void RestoExpr();

/*Gramatica de Expressões Simples*/
/*
 * Expre → Termo Resto
 * Termo → Fator Sobra
 * Resto → ‘ +‘ Termo Resto | ‘ –‘ Termo Resto | ‘ OR‘ Termo Resto | ε
 * RestoTerm →
 * Sobra → ‘ *‘ Fator Sobra | ‘ /‘ Fator Sobra | ‘ AND‘ Fator Sobra | ε
 * Fator → Id | Id ‘(‘ Expr ‘)‘ | ‘(‘ Expr ‘)‘ | num |  intcon | realcon | caraccon | '!' Fator
 */
void Expre(); //Simples
void Termo();
void Resto();
void Sobra();
void Fator();

/*Gramatica de Operador Relacional*/
/*
 * OpRel → '==' | '!=' | '<=' | '<' | '>=' | '>'
 */

int OpRel();

int novoToken();
int ungetTPos();
int erro(int erro);

#endif //COMPILADOR_ANALISADORSINTATICO_H
