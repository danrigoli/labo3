#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define FROM 0
#define TO 10

int *randomize(int from, int to, int quantity) {
    int *array = malloc(quantity * sizeof(int));
    for (int i = 0; i < quantity; i++) {
        int num = rand() % (to - from + 1) + from;
        if (array_contains(array, quantity, num) == 1) {
            i--;
        } else {
            array[i] = num;
        }
    }
    return array;
}

int array_contains(int *array, int size, int value) {
    for (int i = 0; i < size; i++) {
        if (array[i] == value) {
            return 1;
        }
    }
    return 0;
}