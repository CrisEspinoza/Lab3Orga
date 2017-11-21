#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include "struct.h"

// Funciones que leen los archivos

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

/*
- Entrada: Recibe como entrada el cache que estamos utilizando
- Salida: -
- Procedimiento: Se encarga de mostrar cada uno de los datos del cache.
 */

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

/*
- Entrada: Recibe como parametro el cache que estamos utilizando, ademas de un valor entero.
- Salida: Entre un valor 1 si el valor se encuentra en el cache, entrega un valor 0 si el dato no esta en cache.
- Procedimiento: Se encarga de verificar si el dato se encuentra en cache.
 */

int estaEnCache(Cache* cache,int dato)
{
    int via = (dato / cache->palabrasXBloque) % cache->numeroDeVias;
    int i,j;
    for (i = 0; i < cache->bloquesXVias; ++i)
    {
        for (j = 0; j < cache->palabrasXBloque; ++j)
        {
            if (cache->vias[via].bloques[i].palabras[j] == dato)
            {
                return 1;
            }
        }
    }
    return 0;
}

/*
- Entrada: Recibe como parametro el cache que estamos utilizando, ademas de un valor entero.
- Salida: -
- Procedimiento: Se encarga de ingresar el nuevo valor al cache, con respecto a la politica FIFO.
 */

void FIFO(Cache* cache, int dato)
{
    int via = dato / cache->palabrasXBloque % cache->numeroDeVias;
    int bloque = cache->vias[via].contador % cache->bloquesXVias;

    if ( estaEnCache(cache,dato) )
    {
        cache->hit++;
    }

    else
    {
        cache->miss++;
        colocarPalabra(cache,via,bloque, dato);
        cache->vias[via].contador++;
    }
}

/*
- Entrada: Recibe como parametro el cache que estamos utilizando, ademas de un valor entero.
- Salida: -
- Procedimiento: Se encarga de ingresar el nuevo valor al cache, con respecto a la politica MRU.
 */


void MRU(Cache* cache, int dato)
{
    int via;  
    via = dato / cache->palabrasXBloque % cache->numeroDeVias;
    
    if ( !estaCompleto(cache,via) )
    {
        FIFO(cache,dato);
    }

    else
    {   
        if(cache->vias[via].contador == cache->bloquesXVias )
        {
            cache->vias[via].MRU = cache->vias[via].contador - 1;
            cache->vias[via].contador++;
        }

        if (estaEnCache(cache,dato))
        {
            cache->hit++;
            cache->vias[via].MRU = indiceBloque(cache,via,dato);
        }

        else
        {
            cache->miss++;
            colocarPalabra(cache,via,cache->vias[via].MRU, dato);
        }
    }
}

/*
- Entrada: Recibe como parametro el cache que estamos utilizando, ademas de un valor entero.
- Salida: -
- Procedimiento: Se encarga de ingresar el nuevo valor al cache, con respecto a la politica LRU.
 */

void LRU(Cache* cache, int dato)
{
    int via;
    int bloque;  
    via = dato / cache->palabrasXBloque % cache->numeroDeVias;
    
    if ( !estaCompleto(cache,via) )
    {
        if ( estaEnCache(cache,dato))
        {
            cache->hit++;
            bloque = indiceBloque(cache,via,dato);
            interacionBloquePolLRU(cache,via,bloque);
        }
        else
        {
            bloque = cache->vias[via].contador % cache->bloquesXVias;
            cache->miss++;
            colocarPalabra(cache,via,bloque, dato);
            interacionBloquePolLRU(cache,via,bloque);
            cache->vias[via].contador++;
        }
    }
    else
    {   
        if (estaEnCache(cache,dato))
        {
            cache->hit++;
            bloque = indiceBloque(cache,via,dato);
            interacionBloquePolLRU(cache,via,bloque);
        }
        else
        {
            cache->miss++;
            bloque = indiceBloqueLRU(cache,via);
            colocarPalabra(cache,via,bloque, dato);
            interacionBloquePolLRU(cache,via,bloque);
        }
    }
}

/*
- Entrada: Recibe como parametro el cache que estamos utilizando, ademas de 3 valores enteros, que son los valores de donde tiene que ir ubicada la palabra en el cache
- Salida: -
- Procedimiento: Se encarga de ingresar el nuevo valor al cache, con respecto a los valores ingresados como parametros
 */

void colocarPalabra(Cache* cache ,int via,int bloque,int dato)
{
    int i;

    for (i = 0; i < cache->palabrasXBloque; ++i)
    {
        cache->vias[via].bloques[bloque].palabras[i] = dato - (dato%cache->palabrasXBloque) + i ; 
    }
}

/*
- Entrada: Recibe como parametro el cache que estamos utilizando, ademas de dos valores enteros, uno el dato y el otro la via en la que tiene que estar.
- Salida: -
- Procedimiento: Se encarga de buscar el boloque en el cual se encuentra el dato que se paso como parametro
 */

int indiceBloque(Cache* cache, int via, int dato)
{
    int i,j;

    for (i = 0; i < cache->bloquesXVias ; i++)
    {
        for (j = 0 ; j < cache->palabrasXBloque ; j++) 
        {
            if( cache->vias[via].bloques[i].palabras[j] == dato)
            {
                return i;
            }
        }
    }

    return -1;
}

/*
- Entrada: Recibe como parametro el cache que estamos utilizando, ademas de dos valores enteros, la via y el bloque del dato.
- Salida: -
- Procedimiento: Se encarga de aumentar la variable LRU de la estructura bloques, por cada dato que es distinto de -1 (cache vacio)
 */

void interacionBloquePolLRU(Cache* cache, int via,int bloque)
{
    int i;

    for (i = 0; i < cache->bloquesXVias; ++i)
    {
        if (cache->vias[via].bloques[i].LRU != -1)
        {
            cache->vias[via].bloques[i].LRU++;
        }
    }
    cache->vias[via].bloques[bloque].LRU = 0;
}

/*
- Entrada: Recibe como parametro el cache que estamos utilizando, ademas de un valor entero.
- Salida: Devuelve el indice en el cual se encuentra el bloque 
- Procedimiento: Se encarga de buscar el maximo valor del dato, esto se ocupa para buscar el dato en la politica de reemplazo LRU
 */

int indiceBloqueLRU(Cache* cache, int via)
{
    int maximo = cache->vias[via].bloques[0].LRU;
    int indice = 0;
    int i;
    for ( i = 1; i < cache->bloquesXVias; ++i)
    {
        if(maximo < cache->vias[via].bloques[i].LRU)
        {
            maximo = cache->vias[via].bloques[i].LRU;

            indice = i;
        }
    }
    return indice;
}

/*
- Entrada: Recibe como parametro el cache que estamos utilizando, ademas de un valor entero.
- Salida: Entrega un 1 si el cache esta completo, en caso contrario entrega un 0
- Procedimiento: Se encarga de verfirificar si el cache esta completo y no tiene espacio para colocar mas datos, esto lo ocupamos para saber cuando ocupar las 
    politicas de reeemplazo.
 */

int estaCompleto(Cache* cache, int via)
{
    if ( (cache->vias[via].contador) >= (cache->bloquesXVias))
    {
        return 1;
    }

    else
    {
        return 0;
    }
}

/*
- Entrada: Recibe como parametro el cache que estamos utilizando, ademas del nombre del archivo
- Salida: -
- Procedimiento: Se encarga de realizar el llamado a la politica de reemplazo que el usuario ingreso, para comenzar a realizar la traza de llenado de cache
 */

void elCache(Cache* cache, char nombre[])
{
    FILE* archivo = fopen(nombre,"r");

    int numero;
    while (!feof(archivo))
    {
        fscanf(archivo,"%d\n",&numero);
        if(!strcmp(cache->politica, "FIFO") || !strcmp(cache->politica, "fifo"))
        {
            FIFO(cache,numero);
        }
        
        else if(!strcmp(cache->politica, "MRU") || !strcmp(cache->politica, "mru"))
        {
            MRU(cache,numero);
        }
        
        else if(!strcmp(cache->politica, "LRU") || !strcmp(cache->politica, "lru")){
            LRU(cache,numero);
        }        
    }    
    fclose(archivo);
}

/*
- Entrada: Recibe como parametro el cache que estamos utilizando, ademas del nombre del archivo de salida1
- Salida: -
- Procedimiento: Se encarga de escribir en el archivo, los ultimos valores que obtuvo el cache
 */

void escribirArchivoCache(Cache* cache, char nombre[])
{
    FILE *archivo;

    nombre =strcat(nombre,".out");
    archivo = fopen(nombre, "w"); // Abrimos le archivo en modo escritura y que se cree si no exite el archivo nombrado

    int c,r,i;

    fprintf(archivo,"La solución del cache : \n");
    fprintf(archivo,"\n");

    for ( c = 0; c < cache->numeroDeVias; c++)
    {   
        fprintf(archivo,"vias[%d]:\n",c);
        for ( r = 0; r < cache->bloquesXVias; r++)
        {   
            fprintf(archivo,"  bloques[%d]:\n",r );
            for ( i = 0; i < cache->palabrasXBloque; i++)
            {
                fprintf(archivo,"    palabra[%d]: %d\n",i,cache->vias[c].bloques[r].palabras[i] );
            }
        }
    }

    fclose(archivo);
}

/*
- Entrada: Recibe como parametro el cache que estamos utilizando, ademas del archivo de salida2
- Salida: -
- Procedimiento: Se encarga de escribir en el archivo los porcentajes de tasa de mis y hit, ademas de la cantidad de cada uno de ellos que se obtuvo.
 */

void escribirPorcentajesMissHitt(Cache* cache,char nombre[])
{
    FILE *archivo;
    float tasaHit,tasaMiss;

    nombre =strcat(nombre,".out");
    archivo = fopen(nombre, "w"); // Abrimos le archivo en modo escritura y que se cree si no exite el archivo nombrado

    tasaHit = ( ( (float) cache->hit) / (cache->miss+cache->hit) ) * 100;
    tasaMiss = ( ( (float) cache->miss) / (cache->miss+cache->hit) ) * 100;

    fprintf(archivo,"Las estadisticas del cache realizado: \n");

    fprintf(archivo,"Cantidad de Hit obtenidos: %d \n", cache->hit );
    fprintf(archivo,"Cantidad de Miss obtenidos: %d \n", cache->miss);
    fprintf(archivo,"Tasa de Hit: %f %\n", tasaHit);
    fprintf(archivo,"Tasa de Miss: %f %\n",tasaMiss);    

    fclose(archivo);
}

/*
- Entrada: Recibe los parametros que son incluidos al momento de ejecutar el programa.
- Salida: Retorna 1 si todo los valores son validos, en caso contrario retorna un 0
- Procedimiento: Se encarga de realizar el llamado a funciones que se encargar de verificar si los datos ingresados son validos para la ejecucion del cache
 */

int validarDatos(int argc, char** argv)
{
    if (argc < 5)
    {
        return 0;
    }

    //verificar que lo entregado es correcto
    if (!esNumero(argv[2]), !esNumero(argv[3]) ,!esNumero(argv[4]))
    {
        return 0;
    }
    if (!esPotenciaDeDos(atoi(argv[2])) || !esPotenciaDeDos(atoi(argv[3])) || !esPotenciaDeDos(atoi(argv[4])))
    {
        return 0;
    }
    if (!esPotilica(argv[1]))
    {
        return 0;
    }
    if (atoi(argv[2]) > atoi(argv[4]))
    {
        return 0;
    }

    return 1;
}

/*
- Entrada: Recibe como parametro un numero
- Salida: Entrega como salida un 1 si el numero es potencia de dos, en caso contrario entrega un 0
- Procedimiento: Se encarga de verificar si el numero ingresado es potencia de dos
 */

int esPotenciaDeDos(int numero)
{
	if (numero == 1)
	{
		return 1; //ES POTENCIA DE 2
	}
	if (numero % 2 == 1)
	{
		return 0;
	}
	return esPotenciaDeDos(numero/2);
}

/*
- Entrada: Recibe como parametro una cadena de caracteres, para verificar si el dato es numerico
- Salida: Entrega un 1 si es dato es numerico, en caso contrario entrega un 0
- Procedimiento: Se encarga de verificar si el dato es numerico, ocupando el codigo ASCII
 */

int esNumero(char* cadena)
{
    int i;

    for ( i = 0 ; i < (int)strlen(cadena) ; i++)
    {
        if (!(cadena[i] >= 48 && cadena[i] <= 57 ) ) 
        {
            return 0;
        }
    }

    return 1;
}

/*
- Entrada: Recibe como parametro una cadena de caracteres, para verificar si el dato es una politica de reemplazo
- Salida: Entrega un 1 si es dato es una politica de reemplazo, en caso contrario entrega un 0
- Procedimiento: Se encarga de verificar si el dato ingresado pertenece a una politica de reemplazo
 */

int esPotilica(char* cadena)
{
    if (!strcmp(cadena,"MRU") || !strcmp(cadena,"mru"))
    {
        return 1;
    }

    else if (!strcmp(cadena,"LRU") || !strcmp(cadena,"lru"))
    {
        return 1;
    }

    else if (!strcmp(cadena,"FIFO") || !strcmp(cadena,"fifo"))
    {
        return 1;
    }

    return 0;
}