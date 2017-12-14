#include <stdio.h>
#include <stdlib.h>
#include "include/erro.h"
#include "include/analisadorlexico.h"
#include "include/analisadorsintatico.h"

int main()
{
    FILE *file = fopen("codigo.cmm", "r");
    if (file == NULL) {
        logErro(ABRIR_ARQUIVO, 0);
    }
    if (analisadorLexico(file)) {
        fclose(file);
        analisadorCodigo();
        printf("\nCodigo gerado no arquivo \"codigo.cmp\".\n");
        exit(1);
    }

    return 0;
}
