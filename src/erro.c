//
// Created by mateus on 11/10/2017.
//

#include "../include/erro.h"

const char * Erro[] = {"\nErro ao abrir o arquivo\n",
                       "\nHouve algum erro interno\n",
                       "\nQuantidade de Alocacao Excedido %d\n",
                       "\nErro Lexico: Caracter invalido na linha %d\n",
                       "\nErro Lexico: Nao foi encontrado o fechamento do comentario na linha %d",
                       "\nErro Sintatico: Erro interno\n",
                       "\nErro Sintatico: Erro no comando Se.\n",
                       "\nErro Sintatico: Erro no comando Enquanto.\n",
                       "\nErro Sintatico: Erro no comando Para.\n",
                       "\nErro Sintatico: Era esperado um comando valido.\n",
                       "\nErro Sintatico: Era esperado parenteses\n",
                       "\nErro Sintatico: Era esperado chaves\n",
                       "\nErro Sintatico: Era esperado um indentificador\n",
                       "\nErro Sintatico: Era esperado um ponto e virgula\n",
                       "\nErro Sintatico: Operando invalido! Era esperado uma expressao ou um numero"};

void logErro(int erro, int num, ...) {
    va_list list;
    va_start(list, num);
    printf(Erro[erro], (va_arg(list, int) + 1));
    va_end(list);
    exit(0);
}

void erro(int erro) {
    printf(Erro[erro]);
    exit(0);
}