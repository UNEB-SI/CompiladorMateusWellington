#include "../include/gerenciador.h"
void gerenciador(int operacao, int tipo, char lexema[], int zombie, int classe, int linha){
	if (topo == 0) topo = -1;
	if(operacao == IGNORE) return;
	else if(operacao == INSERIR){
		if(topo == -1){
			memset(tabela, 0, DIM);
			topo = 0;
			inserir(tipo, lexema, zombie, classe, linha);
			return;
		} else inserir(tipo, lexema, zombie, classe, linha);
	} else if(operacao == EXCLUIR){
		excluir();
	} else if(operacao == CONSULTAR){
		consultar("x", 1);
	}

}

void inserir(int tipo, char lexema[], int zombie, int escopo, int linha) {
    strcpy(tabela[topo].lexema, lexema);
    tabela[topo].tipo = tipo;
	tabela[topo].escopo = escopo;
    tabela[topo].zombie = zombie;
    tabela[topo].linha = linha;
	topo++;
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

void mostrarTabela() {
	int i;
	printf("\n########## TABELA ##########");
	for (i = 0; i < topo; i++) {
		printf("\nLexema: %s", tabela[i].lexema);
		printf("\nTipo: %d", tabela[i].tipo);
		printf("\nEscopo: %d", tabela[i].escopo);
		printf("\nLinha: %d", tabela[i].linha);
		if (tabela[i].zombie != 0) {
			printf("\nZombie: %d", tabela[i].zombie);
		}
		printf("\n----------------------------");
	}
	printf("\n######## FIM TABELA ########\n");
}