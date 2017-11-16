// Se importan las cosas que ayudaran al funcionamiento del codigo 
#include <stdio.h>
#include <string.h>
#include <locale.h>

// Definimos las estructuras a ocupar dentro de la aplicacion

typedef	struct Bloque
{
	int* palabras;
	int LRU;

}Bloque;

typedef struct Vias
{
	Bloque* bloques;
	int contador;
	int MRU;

}Vias;


typedef struct Cache
{
	Vias* vias;
	int palabrasXBloque;
	int bloquesXVias;
	int numeroDeVias;
	int numeroDeBloques;
	int hit;
	int miss;
	char* politica;
}Cache;

//Funciones 

void mostrarCache(Cache* cache);
Cache* iniciarCache(char* politica, int vias, int palabras, int bloques);
int contarLineas(char nombre[]);
int* leerNumeros(char nombre[], int* numeros);