#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LOTE 10
#define TXT ".txt"

int main(int argc, char *argv[]) {
    FILE *fp;
    char filename[100], product[100];
    int i, number;
    if (argc == 2) {
        if (strcmp(argv[1], "consume") != 0 && strcmp(argv[1], "generate") != 0) {
            printf("Error en los argumentos - Debe ser 'consume' o 'generate' \n");
            exit(1);
        }
    } else {
        printf("Error en los argumentos - Debe ser 'consume' o 'generate' \n");
        exit(1);
    }
    printf("Nombre del producto (q para terminar): ");
    scanf("%s", product);
    if (strcmp(argv[1], "generate") == 0) {
        while (strcmp(product, "q") != 0) {
            strcat(strcpy(filename, product), TXT);
            if ((fp=fopen(filename,"w")) == NULL) {
                printf("Error al abrir el archivo\n");
                exit(1);
            } else {    
                for (i = 1; i <= LOTE; i++) {
                fprintf(fp, "%s - %d\n", product, i);
                }
                fclose(fp);
            }
            printf("Nombre del producto (q para terminar): ");
            scanf("%s", product);
        }
    } else {
        while (strcmp(product, "q") != 0) {
            strcat(strcpy(filename, product), TXT);
            if ((fp = fopen(filename, "r")) == NULL) {
                printf("No existe el producto correspondiente\n");
            } else {    
                while(!feof(fp)) {
                    fscanf(fp,"%s - %d\n", product, &number);
                    printf("%s - %d\n", product, number);
                }
                fclose(fp);
            }
            printf("Nombre del producto (q para terminar): ");
            scanf("%s", product);
        }
    }    
}