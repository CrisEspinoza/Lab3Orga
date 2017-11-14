
// LABORATORIO 1 DE ORGA 

// Bibliotecas a utilizar
#include <stdio.h>
#include "funciones.c"
#include <string.h>
#include <locale.h>
#include <time.h>

int main()
{    
    int opcion,contador,instrucciones;
    int bandera = 0;
    char archivoEntrada[50];
    char archivoSalida1[50];
    char archivoSalida2[50];
    char archivoRegistro[50];
    Informacion* info = (Informacion*)malloc(sizeof(Informacion));

    do
    {
        printf( "\n   1. Ingrese el nombre del archivo de instrucciones de mips");
        printf( "\n   2. Ingrese el nombre del archivo de los valores de los registros");
        printf( "\n   3. Realizar la traza del programa");
        printf( "\n   4. Creditos");
        printf( "\n   5. Salir" );
        printf( "\n\n   Introduzca opcion (1-5): ");

        scanf( "%d", &opcion );

        /* Inicio del anidamiento */

        switch ( opcion )
        {
            case 1: system("cls");
                    printf("Ingrese nombre del archivo a leer\n");
                    scanf("%s",archivoEntrada);            
                    instrucciones = contarLineas(archivoEntrada);
                    info = leerInstrucciones(archivoEntrada,instrucciones);
                    bandera++;
                    break;

            case 2: if (bandera == 1)
                    {
                        system("cls");
                        printf("Ingrese nombre del archivo de registros\n");
                        scanf("%s",archivoRegistro); 
                        info = leerRegistros(info,archivoRegistro);
                        bandera++;
                        break;
                    }
                    else
                    {
                        printf("Primero tiene que realizar la primera opcion del menu.\n");
                    }

            case 3: system("cls");
                    if (bandera >= 2)
                    {
                        printf("Ingrese el nombre de archivo de salida del primer archivo (Sin extension):\n");
                        scanf("%s",archivoSalida1); 
                        printf("Ingrese el nombre de archivo de salida del primer archivo (Sin extension):\n");
                        scanf("%s",archivoSalida2); 
                        strcat(archivoSalida1,".csv");
                        strcat(archivoSalida2,".csv");
                        pipeLine(info,archivoSalida1,archivoSalida2);
                    }
                    else
                    {
                        printf("Primero tiene que cargar la opcion numero 1 y 2, para poder realizar esta opcion.\n");
                    }
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