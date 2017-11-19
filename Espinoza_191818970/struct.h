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
void colocarPalabra(Cache* cache ,int via,int bloque,int dato);
void FIFO(Cache* cache, int dato);
int estaEnCache(Cache* cache,int dato);
void elCache(Cache* cache, char nombre[]);
int indiceBloque(Cache* cache, int via, int dato);
void MRU(Cache* cache, int dato);
int estaCompleto(Cache* cache, int via);
int indiceBloqueLRU(Cache* cache, int via);
void interacionBloquePolLRU(Cache* cache, int via,int bloque);
void LRU(Cache* cache, int dato);
void escribirArchivoCache(Cache* cache, char nombre[]);
void escribirPorcentajesMissHitt(Cache* cache,char nombre[]);
int validarDatos(int argc, char** argv);
int esPotenciaDeDos(int valor);
int esNumero(char* cadena);
int esPotilica(char* cadena);