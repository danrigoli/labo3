#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TXT ".txt"

int main(int argc, char *argv[]) {
    FILE *fp;
    char filename[100], product[100];
    int number;

    printf("Nombre del producto (1 para terminar consumo): ");
    scanf("%s", product);

    while (atoi(product) != 1) {
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
        printf("Nombre del producto (1 para terminar consumo): ");
        scanf("%s", product);
    }
}