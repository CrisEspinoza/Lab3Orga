
// LABORATORIO 3     DE ORGA 

// Bibliotecas a utilizar
// #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "funciones.c"
#include <locale.h>
#include <time.h>

int main(int argc, char *argv[])
{  
    char archivoEntrada[50];
    char archivoSalida[50];
    int opcion,cantNumeros;
    int* numeros;
    Cache* cache = (Cache*)malloc(sizeof(Cache));

    do
    {
        printf( "\n   1. Ingrese el nombre del archivo de numeros de cache");
        printf( "\n   2. Ingrese el nombre del archivo de salida ");
        printf( "\n   3. Realizar la traza del programa");
        printf( "\n   4. Creditos");
        printf( "\n   5. Salir" );
        printf( "\n\n   Introduzca opcion (1-5): ");

        scanf( "%d", &opcion );

        /* Inicio del anidamiento */

        switch ( opcion )
        {
            case 1: printf("Ingrese el nombre de archivo de entrada: ");
                    scanf("%s",archivoEntrada);   
                    cantNumeros = contarLineas(archivoEntrada);
                    printf("%d\n", cantNumeros );
                    numeros = (int*)malloc(sizeof(int)*cantNumeros);
                    numeros = leerNumeros(archivoEntrada,numeros);

                    /*for (int i = 0; i < cantNumeros; i++)
                    {
                        printf("El numero %d : %d \n", i+1 , numeros[i]);
                    }*/

                    cache = iniciarCache( argv[1],atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
                    printf("%d\n", cache->numeroDeVias );
                    printf("%d\n", cache->bloquesXVias);
                    printf("%d\n", cache->palabrasXBloque);
                    printf("%d\n", cache->numeroDeBloques);
                    elCache(cache,archivoEntrada);
                    break;

            case 2: 
                    break;
                    
            case 3: system("cls");
                    break;

            case 4: system("cls");
                    printf(" * Autor: CRISTIAN EDUARDO ESPINOZA SILVA \n ");
                    printf(" * Universidad santiago de chile \n");
                    system("cls");
                    break;
        }

    }while(opcion!=5);

    return 0;
}