#include <stdlib.h>
#include <stdio.h>

#define FROM 0
#define TO 1000

int randomize(void);
int randomizeReader(FILE *in);
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

    randomize();
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


int randomize(void)
{
    if (!randomizeReader(fopen("/dev/random", "r")))
        return 0;

    return -1;
}

int randomizeReader(FILE *in)
{
    unsigned int  seed;

    if (!in)
        return -1;

    if (fread(&seed, sizeof seed, 1, in) == 1) {
        fclose(in);
        srand(seed);
        return 0;
    }

    fclose(in);
    return -1;
}

int array_contains(int *array, int size, int value) {
    for (int i = 0; i < size; i++) {
        if (array[i] == value) {
            return 1;
        }
    }
    return 0;
}
