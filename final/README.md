
# Como ejecutar:
cd final && make -f Makefile
./bolera
./jugadores

## Consideraciones:
Si corren ./jugadores antes que ./bolera, va a avisar que necesita que bolera le pase x memoria la cantidad de jugadores y terminara.
Si paran el programa ./bolera en ejecucion, no va a saltar la alerta porque no se limpio la memoria.