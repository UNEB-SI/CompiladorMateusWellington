#include "gerenciador.h"
topo = -1;
void gerenciador(int operacao, int tipo, int zombie, int classe){
	if(operacao == IGNORE){
		return;
	}
	else if(operacao == INSERIR){
		if(topo == -1){
			memset(tabela, 0, DIM);
			inserir(tipo, zombie, classe);
			return;
		}
		else
			inserir(tipo, zombie, classe);
	}
	else if(operacao == EXCLUIR){
		excluir();
	}
	else if(operacao == CONSULTAR){
		consultar("x", 1); //teste
	}

}

void inserir(int tipo, int zombie, int classe){

    topo ++;

    strcpy( tabela[topo].lexema, token.lexema);
    tabela[topo].tipo = tipo;
    tabela[topo].escopo = escopo;
    tabela[topo].zombie = zombie;
    tabela[topo].linha = linha;
}

int consultar(char lexema[DIM], int esc){

	int auxTopo = topo;
	while(auxTopo > -1){
		if((strcmp(tabela[auxTopo].lexema, lexema) == 0) && (tabela[auxTopo].escopo == esc))
			return 1;
		else
			return 0;

		auxTopo--;
	}

	return 0;
}

void excluir(){

	while(1){
		if( tabela[topo].zombie == 0 && tabela[topo].escopo == 1 )
			topo--;
		else
			break;
	}
}
