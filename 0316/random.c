#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define FROM 0
#define TO 10

int array_contains(int *array, int size, int value);

int main(int argc, char *argv[]) {

    int from = FROM;
    int to = TO;
    int num, quantity;
    if(argc==3) {
        from = atoi(argv[1]);
        to = atoi(argv[2]);
    }

    if (argc == 2) {
        to = atoi(argv[1]);
    }

    quantity = to - from;
    int *array = malloc(quantity * sizeof(int));

    srand(time(NULL));
    
    for (int i = 0; i < quantity; i++) {
        num = rand() % (to - from + 1) + from;
        if (array_contains(array, quantity, num) == 1) {
            i--;
        } else {
            array[i] = num;
            printf("Number: %d \n", num);
        }
    }

    return 0;
 }

int array_contains(int *array, int size, int value) {
    for (int i = 0; i < size; i++) {
        if (array[i] == value) {
            return 1;
        }
    }
    return 0;
}