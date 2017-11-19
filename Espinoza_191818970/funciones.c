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

void colocarPalabra(Cache* cache ,int via,int bloque,int dato)
{
    int i;

    for (i = 0; i < cache->palabrasXBloque; ++i)
    {
        cache->vias[via].bloques[bloque].palabras[i] = dato - (dato%cache->palabrasXBloque) + i ; 
    }
}

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
    mostrarCache(cache);
    fclose(archivo);
}

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

void escribirPorcentajesMissHitt(Cache* cache,char nombre[])
{
    FILE *archivo;

    nombre =strcat(nombre,".out");
    archivo = fopen(nombre, "w"); // Abrimos le archivo en modo escritura y que se cree si no exite el archivo nombrado

    float tasaHit = ( ( (float) cache->hit) / (cache->miss+cache->hit) ) * 100;
    float tasaMiss = ( ( (float) cache->miss) / (cache->miss+cache->hit) ) * 100;

    fprintf(archivo,"Las estadisticas del cache realizado: \n");

    fprintf(archivo,"Cantidad de Hit obtenidos: %d \n", cache->hit );
    fprintf(archivo,"Cantidad de Miss obtenidos: %d \n", cache->miss);
    fprintf(archivo,"Tasa de Hit: %f %\n", tasaHit);
    fprintf(archivo,"Tasa de Miss: %f %\n",tasaMiss);    

    fclose(archivo);
}

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

int esPotenciaDeDos(int valor)
{
    if (valor  == 1)
    {
        return 1;
    }
    else
    {
        int i=1;

        while (1)
        {
            i *= 2;
            if (i == valor)
            {
                return 1;
            }
            if (i > valor)
            {
                return 0;
            }
        }
    }

    return 0;
}

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

int esPotilica(char* cadena)
{
    if (!strcmp(cadena,"MRU"))
    {
        return 1;
    }
    else if (!strcmp(cadena,"mru"))
    {
        return 1;
    }
    else if (!strcmp(cadena,"LRU"))
    {
        return 1;
    }
    else if (!strcmp(cadena,"lru"))
    {
        return 1;
    }
    else if (!strcmp(cadena,"FIFO"))
    {
        return 1;
    }
    else if (!strcmp(cadena,"fifo"))
    {
        return 1;
    }
    return 0;
}