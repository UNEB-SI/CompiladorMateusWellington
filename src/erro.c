//
// Created by mateus and wellington on 11/10/2017.
//

#include "../include/erro.h"

const char * Erro[] = {"\nErro ao abrir o arquivo\n",
                       "\nHouve algum erro interno\n",
                       "\nQuantidade de Alocacao Excedido %d\n",
                       "\nErro Lexico: Caracter invalido na linha %d\n",
                       "\nErro Lexico: Nao foi encontrado o fechamento do comentario na linha %d",
                       "\nErro Sintatico: Erro interno na linha %d\n",
                       "\nErro Sintatico: Erro no comando Se na linha %d.\n",
                       "\nErro Sintatico: Erro no comando Enquanto na linha %d.\n",
                       "\nErro Sintatico: Erro no comando Para na linha %d.\n",
                       "\nErro Sintatico: Era esperado um comando valido na linha %d.\n",
                       "\nErro Sintatico: Era esperado parenteses na linha %d.\n",
                       "\nErro Sintatico: Era esperado chaves na linha %d.\n",
                       "\nErro Sintatico: Era esperado um indentificador na linha %d.\n",
                       "\nErro Sintatico: Era esperado um ponto e virgula na linha %d.\n",
                       "\nErro Sintatico: Operando invalido! Era esperado uma expressao ou um numero na linha %d.\n",
                       "\nErro Semantico: Variavel nao instanciada ou conflito de tipo na linha %d.\n",
                       "\nErro Semantico: Funcao sem retorno, ou retorno nao condizente ao seu tipo na linha %d.\n",
                       "\nErro Semantico: Nao e possivel declarar variaveis com nomes iguais. Linha %d.\n",
                       "\nErro Semantico: Nao e possivel criar mais de uma assinaturas ou declarar funcoes com nomes iguais. Linha %d.\n",
                       "\nErro Semantico: Os parametros da assinatura da funcao nao coincide com a declarada na linha %d.\n",
                       "\nErro Semantico: Tipo do parametro de retorno nao coincide com a assinatura na linha %d.\n",
                       "\nErro Semantico: Chamada de funcao invalida. A funcao deve ser declarada ou instanciada na linha %d.\n",
                       "\nErro Semantico: Chamada de funcao invalida. A funcao deve ter retorno na linha %d.\n",
                       "\nErro Semantico: Chamada de funcao invalida. A funcao nao deve ter retorno na linha %d.\n",
                       "\nErro Semantico: Parametros invalidos na linha %d.\n"};

void logErro(int erro, int num, ...) {
    va_list list;
    va_start(list, num);
    printf(Erro[erro], (va_arg(list, int) + 1));
    va_end(list);
    exit(0);
}