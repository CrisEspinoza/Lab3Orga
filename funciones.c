#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include "struct.h"

// Funciones que leen los archivos

/*
- Entrada: Recibe como entrada el nombre del archivo.
- Salida: Entrega la cantidad de lineas del archivo.
- Procedimiento: Se preocupa de contar las lineas del archivo.
 */
int contarLineas(char nombre[])
{
    FILE *archivo;
    archivo = fopen(nombre, "r");
    char *string = (char*)malloc(sizeof(char)*100);
    int contador = 0;

    if(archivo == NULL)
    {
        printf("Error al abrir archivo \n");
        return 0;
    }
    
    while(!feof(archivo)) // Que lea el archivo siempre y cuando no sea un salto de linea
    {
        string = fgets(string, 100, archivo);
        contador++;
    }

    fclose(archivo);
    //printf("el numero de lineas es: %d\n", contador);
    return contador;
}

/*
- Entrada: Recibe como entrada el nombre del archivo y una arreglo de enteros.
- Salida: Entrega el arreglo de enteros llenado con los datos que se van a buscar en el cache.
- Procedimiento: Se preocupa de leer el archivo y guardar dichos numeros en un arreglo de enteros.
 */
int* leerNumeros(char nombre[], int* numeros)
{
    FILE *archivo;
    archivo = fopen(nombre, "r");
    int contador = 0;

    if(archivo == NULL)
    {
        printf("Error al abrir archivo \n");
        return 0;
    }
    
    while(!feof(archivo)) // Que lea el archivo siempre y cuando no sea un salto de linea
    {
        fscanf(archivo,"%d \n",&numeros[contador]);
        contador++;
    }

    fclose(archivo);

    return numeros;
}

/*
- Entrada: Recibe como entrada la politica a utilizar, la cantidad de vias, palabras y bloques del cache.
- Salida: Entrega el cache conformado.
- Procedimiento: Se encarga de crear el cache con los datos que se entregan como parametros.
 */

Cache* iniciarCache(char* politica, int vias, int palabras, int bloques)
{
    int c,r,i;
    Cache* cache = (Cache*)malloc(sizeof(Cache));
    cache->palabrasXBloque = palabras;
    cache->bloquesXVias = vias;
    cache->numeroDeVias = bloques/vias;
    cache->numeroDeBloques = bloques;
    cache->hit = 0;
    cache->miss = 0;
    cache->politica = (char*)malloc(sizeof(char)*20);
    cache->politica = politica;

    cache->vias = (Vias*)malloc(sizeof(Vias)*cache->numeroDeVias);
    for (c = 0 ; c < cache->numeroDeVias;c++)
    {
        cache->vias[c].bloques = (Bloque*)malloc(sizeof(Bloque)*cache->bloquesXVias);
        cache->vias[c].contador = 0;
        cache->vias[c].MRU = 0;

        for(r = 0 ; r < cache->bloquesXVias; r++)
        {
            cache->vias[c].bloques[r].palabras = (int*)malloc(sizeof(int)*cache->palabrasXBloque);
            cache->vias[c].bloques[r].LRU = 0;

            for (i = 0; i < cache->palabrasXBloque ; i++)
            {
                cache->vias[c].bloques[r].palabras[i] = -1;
            }
        }
    }

    return cache;

}

void mostrarCache(Cache* cache)
{
    int c,r,i;

    for ( c = 0; c < cache->numeroDeVias; c++)
    {   
        printf("vias[%d]:\n",c );
        for ( r = 0; r < cache->bloquesXVias; r++)
        {   
            printf("  bloques[%d]:\n",r );
            for ( i = 0; i < cache->palabrasXBloque; i++)
            {
                printf("    palabra[%d]: %d\n",i,cache->vias[c].bloques[r].palabras[i] );
            }
        }
    }
}