//
// Created by mateus and wellington on 31/10/2017.
//

#ifndef COMPILADOR_ANALISADORSINTATICO_H
#define COMPILADOR_ANALISADORSINTATICO_H

#include "analisadorlexico.h"
#include "erro.h"

extern Token t;
extern Token *tokens;
extern int tkpos;
int tpos;

int analisadorSintatico();

int updateTPos();
int ungetTPos();
int erro(int erro);

/*Gramatica de Programa*/
/*
 * Prog → ProgResto | ε
 * ProgResto → Prototipo | Tipo Id ProgRestoSobra
 * ProgRestoSobra → VIRGULA Decl | ABREPARENTESES Funcao | PONTOEVIRGULA
 * Decl → VIRGULA Id | ε
 * Prototipo → Tipo PrototipoDecl | SEMRETORNO PrototipoDecl
 *   PrototipoDecl → Id ABREPARENTESES TipoParamOpc FECHAPARENTESES ProtitipoResto PONTOEVIRGULA
 *   ProtitipoResto → VIRGULA Id ABREPARENTESES TipoParamOpc FECHAPARENTESES ProtitipoResto | ε
 * Funcao → Tipo FuncaoDeclTipo | SEMRETORNO FuncaoDecl
 *   FuncaoDecl → Id ABREPARENTESES TipoParam FECHAPARENTESES
 *                  ABRECHAVES NovaDecl NovoCmd FECHACHAVES
 *   NovaDecl → Decl | ε
 *   NovoCmd → Cmd | ε
 */

void Prog();
void ProgResto();
void ProgRestoSobra();
void Decl();
void Prototipo();
void PrototipoDecl();
void ProtitipoResto();
void Funcao();
void FuncaoDecl();
void NovaDecl();
/*Gramatica de Tipo*/
/*
 * Tipo → caracter | cadeia | inteiro | real | booleano
 */

int Tipo();

/*Gramatica de TipoParam*/
/*
 * TipoParam → semparam | Tipo Id TipoParamResto
 * TipoParamResto → ',' Tipo Id TipoParamResto | ε
 */

void TipoParam();
void TipoParamResto();

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

#endif //COMPILADOR_ANALISADORSINTATICO_H
