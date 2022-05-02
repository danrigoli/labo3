#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "memoria_compartida.h"
#include "semaforo.h"

#define DESDE 1
#define HASTA 99
#define CLAVE_BASE 33
#define ESPERA 100


    // declaro variables
	dato *memoria = 0;
	int id_memoria, id_semaforo;
	int i, cantidad_cables, cantidad_artificieros, cable_desactiva, cantidad_intentos = 0;

int main(int argc, char *argv[])
{
    if (argv[1] == 0) { // si el primer argumento es nulo, termina el programa 
        printf("Debe ingresar la cantidad de cables\n");
        exit(0);
    }
    if (argv[2] == 0) { // si el segundo argumento es nulo, termina el programa
        printf("Debe ingresar la cantidad de artificieros\n");
        exit(0);
    }
	srand(time(0)); // se cambia la semilla para que sean numeros no repetitivos utilizando el time


    // tomo la cantidad de cables y de artificieros por argumentos y los convierto a enteros con atoi()
    cantidad_cables = atoi(argv[1]);
    cantidad_artificieros = atoi(argv[2]);

    // genero el cable que va a desarmar y la cantidad de intentos disponibles aleatoriamente
    cable_desactiva = rand() % (cantidad_cables - 1) + 1;
    cantidad_intentos = rand() % (cantidad_cables - 1) + 1;
	
    id_semaforo = creo_semaforo(); // se crea el semaforo

	inicia_semaforo(id_semaforo, 0); // empieza con el semaforo en 0 para que el primer artificiero no empiece a desarmar sin que esten todos

	memoria = (dato*)creo_memoria(sizeof(dato) * cantidad_artificieros, &id_memoria, CLAVE_BASE); // se crea la memoria compartida
	

    while (memoria[cantidad_artificieros-1].numero_artificiero == 0) { // espera a que se conecte el ultimo artificiero 
        
        printf("No se han conectado todos los artificieros \n");

        sleep(5);
    }

    levanta_semaforo(id_semaforo); // una vez que se conectan todos los artificieros, se levanta el semaforo
    printf("Cantidad de intentos: %d\n", cantidad_intentos); // se imprime la cantidad de intentos disponibles (DEBUG)
    sleep(1); // se duerme un segundo para que artificieros dejen sus cablesSeleccionados en memoria compartida
    
	while(memoria[0].estado_bomba == 0 || memoria[0].estado_bomba == 2) // mientras que el estado_bomba sea 0 o 2 (intentos disponibles) seguira intentando desarmar la bomba
	{
        // pongo en espera el semaforo
		espera_semaforo(id_semaforo);

        for (i = 0; i < cantidad_artificieros; i++) // recorro todos los artificieros
        {
            if (memoria[i].cableSeleccionado != 0) { // si el cableSeleccionado es distinto de 0 (no seleccionado)
                
                printf("Artificiero %d esta intentando cortar el cable %d...\n", memoria[i].numero_artificiero, memoria[i].cableSeleccionado); // se imprime que el artificiero esta intentando cortar el cable 
                sleep(2); // sleep para hacerlo dramatico
                if (memoria[i].cableSeleccionado == cable_desactiva) { // si el artificiero corto el cable que desactiva la bomba, el estado_bomba pasa a 1 (desactivada)
                    memoria[0].estado_bomba = 1;
                    printf("Artificiero %d desactivo la bomba\n", memoria[i].numero_artificiero);
                    printf("Cantidad de intentos restantes: %d\n", cantidad_intentos - 1);
                    printf("\n");
                    break; // break para que los otros artificieros no sigan cortando cables
                
                } else { // si no adivino el cable correcto
                    cantidad_intentos--; // resta intento
                    if (cantidad_intentos == 0) {  // si se quedo sin intentos, la bomba pasa a estado_bomba 3 (explotada)
                        memoria[0].estado_bomba = 3;
                        printf("Boom! Artificiero %d exploto la bomba\n", memoria[i].numero_artificiero);
                        printf("\n");
                        break;
                    } else { // si aun quedan intentos, se cambia a estado_bomba 2 y pueden seguir intentando cortar cables
                        memoria[0].estado_bomba = 2;
                        memoria[i].cableSeleccionado = 0;
                        printf("Artificiero %d corto el cable incorrecto, %d intentos restantes\n", memoria[i].numero_artificiero, cantidad_intentos);
                        printf("\n");
                    }
                }
            }
        }
		// levanto el semaforo
		levanta_semaforo(id_semaforo);
        usleep(ESPERA * 1000);
	}
    // termino de usar memoria y la cierro
    printf("Cierro memoria compartida\n");
	cierro_memoria(memoria, id_memoria);
	return 0;
}
