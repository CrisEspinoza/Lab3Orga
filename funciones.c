#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include "struct.h"

/* Se declaran variables globales, con el fin de ir guardando banderas al momento de obtener un Hazar de control y 
el PC de la instrucción en la cual se va.
*/

int INTRUCCIONESEJECUTADAS = 0;
int BANDERA = 0 ;
int BANDERA1 = 0 ;
int BANDERA2 = 0 ;

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
- Entrada: Recibe como entrada el nombre del archivo.
- Salida: Entrega la cantidad de lineas del archivo.
- Procedimiento: Se preocupa de leer los valores de los registros
 */
Informacion* leerRegistros(Informacion* info,char nombre[])
{
    FILE *archivo;
    archivo = fopen(nombre, "r");
    char *instruccion = (char*)malloc(sizeof(char)*100);
    int valor;
    int contador = 0;

    if(archivo == NULL)
    {
        printf("Error al abrir archivo \n");
        return 0;
    }
    
    while(!feof(archivo)) // Que lea el archivo siempre y cuando no sea un salto de linea
    {
        fscanf(archivo,"%s %d", instruccion, &valor);
        //printf("%d\n",valor );
        info->registros[contador] = valor;
        contador++;
    }

    fclose(archivo);
    return info;
}

/*
- Entrada: Recibe como parametro el nombre y la cantidad de lineas del archivo.
- Salida: Entrega las instrucciones leidas del archivo.
- Procedimiento: Se encarga de almacenar cada una de las instrucciones que son leidas del archivo uno en una
estructura llamada informacion que contiene a toda las instrucciones que estan contenidas en el archivo uno.
 */

Informacion* leerInstrucciones(char nombre[], int numeroDeLineas)
{
    FILE *archivo; // Declaramos un puntero File para poder leer el archivo de entrada.

    archivo = fopen(nombre, "r"); // Estamos abriendo el archivo en modo lectura

    Informacion* info = (Informacion*)malloc(sizeof(Informacion)); // Declaramos un variable de tipo de la structura tablero.

    //Verificamos si el archivo se encuentra nulo
    if( archivo == NULL)
    {
        printf("Error al abrir archivo \n");
        return 0;
    }

    inicializarMemoriaParaInstrucciones(info,numeroDeLineas);
    
    int posicionArreglo = 0;

    while(!feof(archivo)) // Que lea el archivo siempre y cuando no sea un salto de linea
    {
        char *instruccion = (char*)malloc(sizeof(char)*6);
        fscanf(archivo,"%s ", instruccion);
        //printf("%d\n", posicionArreglo );

        if(strcmp(instruccion, "add") == 0 || strcmp(instruccion, "sub") == 0 
            || strcmp(instruccion, "mul") == 0 || strcmp(instruccion, "div") == 0 )
        {
            char *rt = (char*)malloc(sizeof(char)*10);
            char *rs = (char*)malloc(sizeof(char)*10);
            char *rd = (char*)malloc(sizeof(char)*10);
            fscanf(archivo,"%[^','], %[^','], %s", rd, rs, rt);
            
            info->instrucciones[posicionArreglo].instruccion = instruccion;
            info->instrucciones[posicionArreglo].rd = rd;
            info->instrucciones[posicionArreglo].rs = rs;
            info->instrucciones[posicionArreglo].rt = rt;
            info->instrucciones[posicionArreglo].PC = posicionArreglo;
            posicionArreglo++;
            info->cantidadDeInstrucciones = posicionArreglo;
        }

        else if(strcmp(instruccion, "addi") == 0 || strcmp(instruccion, "subi") == 0 )
        {
            char *rt = (char*)malloc(sizeof(char)*10);
            char *rs = (char*)malloc(sizeof(char)*10);
            int inmediato;
            fscanf(archivo,"%[^','], %[^','], %d", rt, rs, &inmediato);
            
            info->instrucciones[posicionArreglo].instruccion = instruccion;
            info->instrucciones[posicionArreglo].inmediato = inmediato;
            info->instrucciones[posicionArreglo].rs = rs;
            info->instrucciones[posicionArreglo].rt = rt;
            info->instrucciones[posicionArreglo].PC = posicionArreglo;
            posicionArreglo++;
            info->cantidadDeInstrucciones = posicionArreglo;
        }

        else if(strcmp(instruccion, "beq") == 0)
        {
            char *rt = (char*)malloc(sizeof(char)*10);
            char *rs = (char*)malloc(sizeof(char)*10);
            char *label = (char*)malloc(sizeof(char)*10);
            fscanf(archivo,"%[^','], %[^','], %s", rs, rt, label);
            
            info->instrucciones[posicionArreglo].instruccion = instruccion;
            info->instrucciones[posicionArreglo].rd = label;
            info->instrucciones[posicionArreglo].rs = rs;
            info->instrucciones[posicionArreglo].rt = rt;
            info->instrucciones[posicionArreglo].PC = posicionArreglo;
            posicionArreglo++;
            info->cantidadDeInstrucciones = posicionArreglo;
        }

        else if(strcmp(instruccion, "j") == 0)
        {
            char *label = (char*)malloc(sizeof(char)*10);
            fscanf(archivo,"%s",label);            
            info->instrucciones[posicionArreglo].instruccion = instruccion;
            info->instrucciones[posicionArreglo].rd = label;
            info->instrucciones[posicionArreglo].PC = posicionArreglo;
            posicionArreglo++;
            info->cantidadDeInstrucciones = posicionArreglo;
        }

        else if(strcmp(instruccion, "sw") == 0 || strcmp(instruccion, "lw") == 0)
        {
            char *rt = (char*)malloc(sizeof(char)*10);
            char *rs = (char*)malloc(sizeof(char)*10);
            int inmediato;
            fscanf(archivo,"%[^','], %d(%[^')'])", rt, &inmediato, rs);
            
            info->instrucciones[posicionArreglo].instruccion = instruccion;
            info->instrucciones[posicionArreglo].inmediato = inmediato;
            info->instrucciones[posicionArreglo].rs = rs;
            info->instrucciones[posicionArreglo].rt = rt;
            info->instrucciones[posicionArreglo].PC = posicionArreglo;
            posicionArreglo++;
            info->cantidadDeInstrucciones = posicionArreglo;
        }

        else if(instruccion[strlen(instruccion)-1] == ':')
        { 
            info->etiqueta[posicionArreglo].label = instruccion;
            info->etiqueta[posicionArreglo].PC = posicionArreglo;
        }

    }

    fclose(archivo); // Cerramos el archivo
 
    return info; //Retornamo la variable matriz, con el tablero contenido, el cual se leyó desde el archivo ingresado
}

/*
- Entrada: Recibe como parametro una estructura informacion y la cantidad de lineas del archivo uno.
- Salida: Entrega como salida la misma estructura recibida como parametro.
- Procedimiento: Se encarga de inicializar cada una de los atributos que contiene la estructura informacion
que es recibida como parametro.
 */

void inicializarMemoriaParaInstrucciones(Informacion *informacion, int cantidadLineas)
{   
    int i ;

    informacion->instrucciones = (Instruccion*)malloc(sizeof(Instruccion)*cantidadLineas);
    informacion->buffer = (Buffer*)malloc(sizeof(Buffer)*5);
    informacion->hazarDatoEx = (char*)malloc(sizeof(char)*100);
    informacion->hazarDatoMem = (char*)malloc(sizeof(char)*100);
    informacion->hazarControl = (char*)malloc(sizeof(char)*100);
    informacion->hazarDatoEx = "" ;
    informacion->hazarDatoMem = "" ;
    informacion->hazarControl = "" ; 

    for (i = 0; i < cantidadLineas; i++)
    {
        informacion->instrucciones[i].instruccion = (char*)malloc(sizeof(char)*100);
        informacion->instrucciones[i].lineaDeControl = (LineaDeControl*)malloc(sizeof(LineaDeControl));
        informacion->instrucciones[i].lineaDeControl->ALUOp = (char*)malloc(sizeof(char)*100);
        informacion->instrucciones[i].rt = (char*)malloc(sizeof(char)*10);
        informacion->instrucciones[i].rs = (char*)malloc(sizeof(char)*10);
        informacion->instrucciones[i].rd = (char*)malloc(sizeof(char)*10);
    }

    for (i = 0; i < cantidadLineas; i++)
    {
        informacion->etiqueta[i].label = (char*)malloc(sizeof(char)*50);
    }

    for (i = 0; i < 5; i++)
    {
        informacion->buffer[i].instruccion = (Instruccion*)malloc(sizeof(Instruccion)); 
        informacion->buffer[i].instruccion->instruccion = (char*)malloc(sizeof(char)*100);
        informacion->buffer[i].instruccion->rt = (char*)malloc(sizeof(char)*100);
        informacion->buffer[i].instruccion->rd = (char*)malloc(sizeof(char)*100);
        informacion->buffer[i].instruccion->rs = (char*)malloc(sizeof(char)*100);   
        informacion->buffer[i].muxRegDs = (char*)malloc(sizeof(char)*100);      
        informacion->buffer[i].lineaDeControl = (LineaDeControl*)malloc(sizeof(LineaDeControl));
        informacion->buffer[i].lineaDeControl->ALUOp = (char*)malloc(sizeof(char)*10);
        informacion->buffer[i].register1 = (char*)malloc(sizeof(char)*100);        
        informacion->buffer[i].register2 = (char*)malloc(sizeof(char)*100);
        informacion->buffer[i].address = (char*)malloc(sizeof(char)*100);
        informacion->buffer[i].writeDataMem = (char*)malloc(sizeof(char)*100);
        informacion->buffer[i].writeRegister = (char*)malloc(sizeof(char)*100);
        informacion->buffer[i].rt = (char*)malloc(sizeof(char)*100);
        informacion->buffer[i].rd = (char*)malloc(sizeof(char)*100);
        informacion->buffer[i].rs = (char*)malloc(sizeof(char)*100);
        iniciarBuffer(&informacion->buffer[i]);
    }
}

/*
- Entrada: Recibe como parametro una estructura linea de control de una instrucción.
- Salida: Entrega una linea de control.
- Procedimiento: Se encarga de resetear toda las lineas de control en 0.
 */
LineaDeControl* resetearLineasControl(LineaDeControl* lineaDeControl)
{
    lineaDeControl->RegDst = '0';
    lineaDeControl->Jump = '0';
    lineaDeControl->Branch = '0';
    lineaDeControl->MemRead = '0';
    lineaDeControl->MemToReg = '0';
    strcpy(lineaDeControl->ALUOp , "00");
    lineaDeControl->MemWrite = '0';
    lineaDeControl->ALUSrc = '0';
    lineaDeControl->RegWrite = '0';

    return lineaDeControl;
}

/*
- Entrada: Recibe como parametro una estructura instrucción.
- Salida: Entrega como salida una estructura instrucción.
- Procedimiento: Se encarga de resetear la instrucción y dejarla como vacio.
 */

Instruccion* inicializarIntrucciones(Instruccion* instruccion)
{
    instruccion->instruccion = "vacio";
    instruccion->rs = "";
    instruccion->rt = "";
    instruccion->rd = "";
    instruccion->HC = 0;

    return instruccion;
}
/*
- Entrada: Recibe como parametro una estructura buffer.
- Salida: -
- Procedimiento: Se encarga de inicializar todo sus valores en 0 y vacio en caso de string.
 */

void iniciarBuffer(Buffer* buffer)
{
    buffer->instruccion = inicializarIntrucciones(buffer->instruccion);
    buffer->lineaDeControl = resetearLineasControl(buffer->lineaDeControl);
    buffer->register1 = "";
    buffer->register2 = "";
    buffer->writeRegister = "";
    buffer->rd = "";
    buffer->rt = "";
    buffer->rs = "";
    buffer->muxRegDs = "";
    buffer->readData1Id = 0;
    buffer->readData2Id = 0;
    buffer->signoExtendido = 0; 
    buffer->posRegistro = 0;   
    buffer->ALU = 0;
    buffer->address = "";
    buffer->writeDataMem = "";
    buffer->readData1Mem = 0;
    buffer->writeDataWb = 0;
    buffer->zero = 0;
    buffer->aluResult = 0;
    buffer->addPc = 0;
    buffer->estado = 0; 
    buffer->estado1 = 0; 
    buffer->estado2 = 0;    
}

/*
- Entrada: Recibe como parametro una estructura buffer.
- Salida: -
- Procedimiento: Se encarga de mostrar cada información que contiene el buffer.
 */

void mostrarBuffer(Buffer* buffer)
{
    int j;
    for (j = 0; j < 4; j++)
    {
        printf("%s\n",buffer[j].instruccion->instruccion);
        printf("Los registros que estan en instruccion");
        printf("%s\n",buffer[j].instruccion->rs);
        printf("%s\n",buffer[j].instruccion->rt);
        printf("%s\n",buffer[j].instruccion->rd);
        printf("%s\n",buffer[j].muxRegDs);
        printf("Se acabo lo anterior\n");
        printf("%s\n",buffer[j].writeRegister);
        printf("%s\n",buffer[j].register1);
        printf("%s\n",buffer[j].rd);
        printf("%s\n",buffer[j].rt);
        printf("%s\n",buffer[j].rs);
        printf("%d\n",buffer[j].posRegistro);
        printf("%s\n",buffer[j].register2);
        printf("%d\n",buffer[j].readData1Id);
        printf("%d\n",buffer[j].readData2Id);
        printf("%d\n",buffer[j].signoExtendido);   
        printf("%d\n",buffer[j].ALU);
        printf("%s\n",buffer[j].address);
        printf("%s\n",buffer[j].writeDataMem);
        printf("%d\n",buffer[j].readData1Mem);
        printf("%d\n",buffer[j].estado);
        printf("%d\n",buffer[j].writeDataWb);
        printf("%d\n",buffer[j].zero);
        printf("%d\n",buffer[j].aluResult);
        printf("%d\n",buffer[j].addPc); 
        printf("Linas de control \n");
        printf("\n");
        printf("RegDst : %c ",buffer[j].lineaDeControl->RegDst);
        printf("Jump : %c ",buffer[j].lineaDeControl->Jump);
        printf("Branch : %c ",buffer[j].lineaDeControl->Branch);
        printf("MemRead : %c ",buffer[j].lineaDeControl->MemRead);
        printf("MemToReg : %c ",buffer[j].lineaDeControl->MemToReg);
        printf("ALUOp : %s ",buffer[j].lineaDeControl->ALUOp);
        printf("MemWrite : %c ",buffer[j].lineaDeControl->MemWrite);
        printf("ALUSrc : %c ",buffer[j].lineaDeControl->ALUSrc);
        printf("RegWrite : %c \n",buffer[j].lineaDeControl->RegWrite);
    }
}

// Funciones que hacen los calculos de los registros obtenidos  

/*
- Entrada: Funcion que recibe en nombre del registro.
- Salida: Entrega la posicion del registro que se recibe como parametro.
- Procesimiento: Se encarga de buscar la ubicacion que tiene el registro dentro de los registros.
 */

int obtenerPosicionReg(char* registro)
{
    int i;

    for(i = 0; i < 32 ; i++)
    {
        if(strcmp(registroMars[i],registro) == 0)
        {
            return i;
        }
    }

    return -1;
} 

/*
- Entrada: Funcion que recibe como parametro el nombre de la etiqueta y la estructura informacion.
- Salida: Entrega la posicion de la etiqueta.
- Procedimiento: Se encarga de buscar la etiquera dentro de las instrucciones que fueron leidas desde el archivo 
numer uno, para luego entregar dicho numero.
 */

int buscarPosicionEtiqueta(char* etiqueta, Informacion *info)
{
    int i;

    for(i = 0; i < (info->cantidadDeInstrucciones + 1 ); i++)
    {   
        if(strcmp(etiqueta,info->etiqueta[i].label) == 0)
        {   
            return i;
        }
    }
    return -1;
}

/*
- Entrada: Recibe como parametro una estructura linea de control.
- Salida: -
- Procedimiento: Se encarga de mostrar cada uno de los valores que contiene la estrucutra linea de control.
 */

void mostrarLineasControl(LineaDeControl* aux)
{ 
    printf("Lineas de control \n");
    printf("RegDst : %c ",aux->RegDst);
    printf("Jump : %c ",aux->Jump);
    printf("Branch : %c ",aux->Branch);
    printf("MemRead : %c ",aux->MemRead);
    printf("MemToReg : %c ",aux->MemToReg);
    printf("ALUOp : %s ",aux->ALUOp);
    printf("MemWrite : %c ",aux->MemWrite);
    printf("ALUSrc : %c ",aux->ALUSrc);
    printf("RegWrite : %c \n",aux->RegWrite);
}

/*
- Entrada: Funcion que recibe como parametro un puntero a la estructura informacion , la instruccion que se desea trabajar, el PC de la intrucción y la etapa en la que se
esta desarollando.
- Salida: -
- Procedimiento: Modifica el puntero a la estructura informacion, tomando los registros rs y rt y realizando la suma de ellos 
y asi guardando el resultado en rd.
 */

void add(Informacion *informacion, Instruccion *instruccion, int PC,int etapa) // listo 
{           
    int posrt;
    int posrs;
    int posrd;

    posrt = obtenerPosicionReg(instruccion->rt);
    posrs = obtenerPosicionReg(instruccion->rs);
    posrd = obtenerPosicionReg(instruccion->rd);

    
    if(etapa == 1) // ETAPA IF buffers[0] = IF/ID
    {   
        informacion->buffer[0].estado = 1;
        informacion->buffer[0].instruccion = instruccion;
        informacion->buffer[0].addPc = PC;
        informacion->buffer[0].estado2 = 0;
    }

    else if(etapa == 2) //ETAPA ID buffers[1] = ID/EX
    {
        informacion->buffer[1] = informacion->buffer[0];
        // Etapa nueva 
        informacion->buffer[0].estado = 0;
        informacion->buffer[1].muxRegDs = informacion->buffer[1].instruccion->rd;
        informacion->buffer[1].rs = informacion->buffer[1].instruccion->rs;
        informacion->buffer[1].rt = informacion->buffer[1].instruccion->rt;
        informacion->buffer[1].rd = informacion->buffer[1].instruccion->rd;
        informacion->buffer[1].lineaDeControl = asignarLineasDeControl(informacion->buffer[1].instruccion->instruccion);
        informacion->buffer[1].writeRegister = informacion->buffer[0].instruccion->rd;
        informacion->buffer[1].readData1Id = informacion->registros[posrs];
        informacion->buffer[1].readData2Id = informacion->registros[posrt];
    }
 
    else if(etapa == 3) //ETAPA EX buffers[2] = EX/MEM
    {   
        informacion->buffer[2] = informacion->buffer[1];
        informacion->buffer[1].estado = 0;
        // Etapa nueva 
        informacion->buffer[2].aluResult = informacion->buffer[2].readData1Id + informacion->buffer[2].readData2Id;
    }

    else if(etapa == 4) //ETAPA MEM buffers[3] = MEM/WB
    {
        informacion->buffer[3] = informacion->buffer[2];
        informacion->buffer[2].estado = 0;
        //Etapa nueva   
        if (BANDERA1 == 0 && informacion->buffer[3].estado2 == 0)
        {      
            informacion->buffer[3].writeDataWb = informacion->buffer[3].aluResult;
        }
        else
        {
            BANDERA1--;
        }
    }

    else if(etapa == 5) //ETAPA WB
    {
        informacion->buffer[4] = informacion->buffer[3];
        informacion->buffer[3].estado = 0;
        // Etapa nueva
        if (BANDERA1 == 0 && informacion->buffer[4].estado2 == 0)
        {
            informacion->registros[posrd] = informacion->buffer[3].writeDataWb;
        }
        else
        {
            BANDERA1--;
        }
    }   
}

/*
- Entrada: Funcion que recibe como parametro un puntero a la estructura informacion , la instruccion que se desea trabajar, el PC de la intrucción y la etapa en la que se
esta desarollando.
- Salida: -
- Procedimiento: Modifica el puntero a la estructura informacion, tomando los registros rs y rt y realizando la resta de ellos 
y asi guardando el resultado en rd.
 */

void sub(Informacion *informacion, Instruccion *instruccion, int PC,int etapa) // listo 
{           
    int posrt;
    int posrs;
    int posrd;

    posrt = obtenerPosicionReg(instruccion->rt);
    posrs = obtenerPosicionReg(instruccion->rs);
    posrd = obtenerPosicionReg(instruccion->rd);

    
    if(etapa == 1) // ETAPA IF buffers[0] = IF/ID
    {   
        informacion->buffer[0].estado = 1;
        informacion->buffer[0].instruccion = instruccion;
        informacion->buffer[0].addPc = PC;
        informacion->buffer[0].estado2 = 0;
    }

    else if(etapa == 2) //ETAPA ID buffers[1] = ID/EX
    {
        informacion->buffer[1] = informacion->buffer[0];
        informacion->buffer[0].estado = 0;
        // Etapa nueva 
        informacion->buffer[1].muxRegDs = informacion->buffer[1].instruccion->rd;
        informacion->buffer[1].rs = informacion->buffer[1].instruccion->rs;
        informacion->buffer[1].rt = informacion->buffer[1].instruccion->rt;
        informacion->buffer[1].rd = informacion->buffer[1].instruccion->rd;
        informacion->buffer[1].lineaDeControl = asignarLineasDeControl(informacion->buffer[1].instruccion->instruccion);
        informacion->buffer[1].writeRegister = informacion->buffer[0].instruccion->rd;
        informacion->buffer[1].readData1Id = informacion->registros[posrs];
        informacion->buffer[1].readData2Id = informacion->registros[posrt];
    }
 
    else if(etapa == 3) //ETAPA EX buffers[2] = EX/MEM
    {   
        informacion->buffer[2] = informacion->buffer[1];
        informacion->buffer[1].estado = 0;
        // Etapa nueva 
        informacion->buffer[2].aluResult = informacion->buffer[2].readData1Id - informacion->buffer[2].readData2Id;
    }

    else if(etapa == 4) //ETAPA MEM buffers[3] = MEM/WB
    {
        informacion->buffer[3] = informacion->buffer[2];
        informacion->buffer[2].estado = 0;
        //Etapa nueva    
        if (BANDERA1 == 0 && informacion->buffer[3].estado2 == 0)
        {     
            informacion->buffer[3].writeDataWb = informacion->buffer[3].aluResult;
        }
        else
        {
            BANDERA1--;
        }
    }

    else if(etapa == 5) //ETAPA WB
    {
        informacion->buffer[4] = informacion->buffer[3];
        informacion->buffer[3].estado = 0;
        // Etapa nueva
        if (BANDERA1 == 0 && informacion->buffer[4].estado2 == 0)
        {
            informacion->registros[posrd] = informacion->buffer[3].writeDataWb;
        }
        else
        {
            BANDERA1--;
        }
    }   
}

/*
- Entrada: Funcion que recibe como parametro un puntero a la estructura informacion , la instruccion que se desea trabajar, el PC de la intrucción y la etapa en la que se
esta desarollando.
- Salida: -
- Procedimiento: Modifica el puntero a la estructura informacion, tomando los registros rs y rt y realizando la multiplicacíon de ellos 
y asi guardando el resultado en rd.
 */

void mul(Informacion *informacion, Instruccion *instruccion, int PC,int etapa)
{           
    int posrt;
    int posrs;
    int posrd;

    posrt = obtenerPosicionReg(instruccion->rt);
    posrs = obtenerPosicionReg(instruccion->rs);
    posrd = obtenerPosicionReg(instruccion->rd);

    
    if(etapa == 1) // ETAPA IF buffers[0] = IF/ID
    {   
        informacion->buffer[0].instruccion = instruccion;
        informacion->buffer[0].addPc = PC;
        informacion->buffer[0].estado = 1;
        informacion->buffer[0].estado2 = 0;
    }

    else if(etapa == 2) //ETAPA ID buffers[1] = ID/EX
    {
        informacion->buffer[1] = informacion->buffer[0];
        informacion->buffer[0].estado = 0;
        // Etapa nueva 
        informacion->buffer[1].muxRegDs = informacion->buffer[1].instruccion->rd;
        informacion->buffer[1].rs = informacion->buffer[1].instruccion->rs;
        informacion->buffer[1].rt = informacion->buffer[1].instruccion->rt;
        informacion->buffer[1].rd = informacion->buffer[1].instruccion->rd;
        informacion->buffer[1].lineaDeControl = asignarLineasDeControl(informacion->buffer[1].instruccion->instruccion);
        informacion->buffer[1].writeRegister = informacion->buffer[0].instruccion->rd;
        informacion->buffer[1].readData1Id = informacion->registros[posrs];
        informacion->buffer[1].readData2Id = informacion->registros[posrt];

    }
 
    else if(etapa == 3) //ETAPA EX buffers[2] = EX/MEM
    {   
        informacion->buffer[2] = informacion->buffer[1];
        informacion->buffer[1].estado = 0;
        // Etapa nueva 
        informacion->buffer[2].aluResult = informacion->buffer[2].readData1Id * informacion->buffer[2].readData2Id;
    }

    else if(etapa == 4) //ETAPA MEM buffers[3] = MEM/WB
    {
        informacion->buffer[3] = informacion->buffer[2];
        informacion->buffer[2].estado = 0;
        //Etapa nueva    
        if (BANDERA1 == 0 && informacion->buffer[3].estado2 == 0)
        {     
            informacion->buffer[3].writeDataWb = informacion->buffer[3].aluResult;
        }
        else
        {
            BANDERA1--;
        }
    }

    else if(etapa == 5) //ETAPA WB
    {
        informacion->buffer[4] = informacion->buffer[3];
        informacion->buffer[3].estado = 0;
        // Etapa nueva
        if (BANDERA1 == 0 && informacion->buffer[4].estado2 == 0)
        {
            informacion->registros[posrd] = informacion->buffer[3].writeDataWb;
        }
        else
        {
            BANDERA1--;
        }
    }   
}

/*
- Entrada: Funcion que recibe como parametro un puntero a la estructura informacion , la instruccion que se desea trabajar, el PC de la intrucción y la etapa en la que se
esta desarollando.
- Salida: -
- Procedimiento: Modifica el puntero a la estructura informacion, agregando una instrucción vacia, lo cua representa un noob.
 */

void vacio(Informacion *informacion, Instruccion *instruccion, int PC,int etapa)
{           
    if(etapa == 1) // ETAPA IF buffers[0] = IF/ID
    {   
        //resetearBuffer(&informacion->buffer[0]);
        informacion->buffer[0].instruccion = instruccion;
        informacion->buffer[0].addPc = PC;
        informacion->buffer[0].estado = 1;
        informacion->buffer[0].estado2 = 0;
    }

    else if(etapa == 2) //ETAPA ID buffers[1] = ID/EX
    {
        informacion->buffer[1] = informacion->buffer[0];
        informacion->buffer[0].estado = 0;
        // Etapa nueva 
        informacion->buffer[1].muxRegDs = "";
        informacion->buffer[1].rs = "";
        informacion->buffer[1].rt = "";
        informacion->buffer[1].rd = "";
        informacion->buffer[1].lineaDeControl = asignarLineasDeControl(informacion->buffer[1].instruccion->instruccion);
        informacion->buffer[1].writeRegister = "";
        informacion->buffer[1].readData1Id = 0;
        informacion->buffer[1].readData2Id = 0;

    }
 
    else if(etapa == 3) //ETAPA EX buffers[2] = EX/MEM
    {   
        informacion->buffer[2] = informacion->buffer[1];
        informacion->buffer[1].estado = 0;
        // Etapa nueva 
        informacion->buffer[2].aluResult = 0;
    }

    else if(etapa == 4) //ETAPA MEM buffers[3] = MEM/WB
    {
        informacion->buffer[3] = informacion->buffer[2];
        informacion->buffer[2].estado = 0;
        //Etapa nueva         
        if (BANDERA1 == 0 && informacion->buffer[3].estado2 == 0)
        {
            informacion->buffer[3].writeDataWb = 0;
        }
        else
        {
            BANDERA1--;
        }
    }

    else if(etapa == 5) //ETAPA WB
    {
        informacion->buffer[4] = informacion->buffer[3];
        informacion->buffer[3].estado = 0;
        if (BANDERA1 != 0)
        {
            BANDERA1--;
        }
    }   
}

/*
- Entrada: Funcion que recibe como parametro un puntero a la estructura informacion , la instruccion que se desea trabajar, el PC de la intrucción y la etapa en la que se
esta desarollando.
- Salida: -
- Procedimiento: Modifica el puntero a la estructura informacion, tomando los registros rs y rt y realizando la division de ellos 
y asi guardando el resultado en rd.
 */

void division(Informacion *informacion, Instruccion *instruccion, int PC,int etapa)
{           
    int posrt;
    int posrs;
    int posrd;

    posrt = obtenerPosicionReg(instruccion->rt);
    posrs = obtenerPosicionReg(instruccion->rs);
    posrd = obtenerPosicionReg(instruccion->rd);

    
    if(etapa == 1) // ETAPA IF buffers[0] = IF/ID
    {   
        informacion->buffer[0].instruccion = instruccion;
        informacion->buffer[0].addPc = PC;
        informacion->buffer[0].estado = 1;
        informacion->buffer[0].estado2 = 0;
    }

    else if(etapa == 2) //ETAPA ID buffers[1] = ID/EX
    {
        informacion->buffer[1] = informacion->buffer[0];
        informacion->buffer[0].estado = 0;
        // Etapa nueva 
        informacion->buffer[1].muxRegDs = informacion->buffer[1].instruccion->rd;
        informacion->buffer[1].rs = informacion->buffer[1].instruccion->rs;
        informacion->buffer[1].rt = informacion->buffer[1].instruccion->rt;
        informacion->buffer[1].rd = informacion->buffer[1].instruccion->rd;
        informacion->buffer[1].lineaDeControl = asignarLineasDeControl(informacion->buffer[1].instruccion->instruccion);
        informacion->buffer[1].writeRegister = informacion->buffer[0].instruccion->rd;
        informacion->buffer[1].readData1Id = informacion->registros[posrs];
        informacion->buffer[1].readData2Id = informacion->registros[posrt];
    }
 
    else if(etapa == 3) //ETAPA EX buffers[2] = EX/MEM
    {   
        informacion->buffer[2] = informacion->buffer[1];
        informacion->buffer[1].estado = 0;
        // Etapa nueva 
        informacion->buffer[2].aluResult = informacion->buffer[2].readData1Id / informacion->buffer[2].readData2Id;
    }

    else if(etapa == 4) //ETAPA MEM buffers[3] = MEM/WB
    {
        informacion->buffer[3] = informacion->buffer[2];
        informacion->buffer[2].estado = 0;
        //Etapa nueva       
        if (BANDERA1 == 0 && informacion->buffer[3].estado2 == 0)
        {  
            informacion->buffer[3].writeDataWb = informacion->buffer[3].aluResult;
        }
        else
        {
            BANDERA1--;
        }
    }

    else if(etapa == 5) //ETAPA WB
    {
        informacion->buffer[4] = informacion->buffer[3];
        informacion->buffer[3].estado = 0;
        // Etapa nueva
        if (BANDERA1 == 0 && informacion->buffer[4].estado2 == 0)
        {
            informacion->registros[posrd] = informacion->buffer[3].writeDataWb;
        }
        else
        {
            BANDERA1--;
        }
    }   
}

/*
- Entrada: Funcion que recibe como parametro un puntero a la estructura informacion , la instruccion que se desea trabajar, el PC de la intrucción y la etapa en la que se
esta desarollando.
- Salida: -
- Procedimiento: Modifica el puntero a la estructura informacion, tomando los registros rs y inmediato y realizando la resta de ellos 
y asi guardando el resultado en rd.
 */


void subi(Informacion *informacion, Instruccion *instruccion, int PC,int etapa)
{           
    int posrt;
    int posrs;

    posrt = obtenerPosicionReg(instruccion->rt);
    posrs = obtenerPosicionReg(instruccion->rs);

    
    if(etapa == 1) // ETAPA IF buffers[0] = IF/ID
    {   
        informacion->buffer[0].instruccion = instruccion;
        informacion->buffer[0].addPc = PC;
        informacion->buffer[0].estado = 1;
        informacion->buffer[0].estado2 = 0;
    }

    else if(etapa == 2) //ETAPA ID buffers[1] = ID/EX
    {
        informacion->buffer[1] = informacion->buffer[0];
        informacion->buffer[0].estado = 0;
        // Etapa nueva 
        informacion->buffer[1].muxRegDs = informacion->buffer[1].instruccion->rd;
        informacion->buffer[1].rs = informacion->buffer[1].instruccion->rs;
        informacion->buffer[1].rt = informacion->buffer[1].instruccion->rt;        
        informacion->buffer[1].rd = informacion->buffer[1].instruccion->rt;
        informacion->buffer[1].signoExtendido = informacion->buffer[1].instruccion->inmediato;
        informacion->buffer[1].lineaDeControl = asignarLineasDeControl(informacion->buffer[1].instruccion->instruccion);
        informacion->buffer[1].writeRegister = informacion->buffer[0].instruccion->rt;
        informacion->buffer[1].readData1Id = informacion->registros[posrs];
    }
 
    else if(etapa == 3) //ETAPA EX buffers[2] = EX/MEM
    {   
        informacion->buffer[2] = informacion->buffer[1];
        informacion->buffer[1].estado = 0;
        // Etapa nueva 
        informacion->buffer[2].aluResult = informacion->buffer[2].readData1Id - informacion->buffer[2].signoExtendido;
    }

    else if(etapa == 4) //ETAPA MEM buffers[3] = MEM/WB
    {
        informacion->buffer[3] = informacion->buffer[2];
        informacion->buffer[2].estado = 0;
        //Etapa nueva 
        if (BANDERA1 == 0 && informacion->buffer[3].estado2 == 0)
        {        
            informacion->buffer[3].writeDataWb = informacion->buffer[3].aluResult;
        }
        else
        {
            BANDERA1--;
        }
    }

    else if(etapa == 5) //ETAPA WB
    {
        informacion->buffer[4] = informacion->buffer[3];
        informacion->buffer[3].estado = 0;       
         // Etapa nueva
        if (BANDERA1 == 0 && informacion->buffer[4].estado2 == 0)
        {
            informacion->registros[posrt] = informacion->buffer[3].writeDataWb;
        }
        else
        {
            BANDERA1--;
        }
    }   
}

/*
- Entrada: Funcion que recibe como parametro un puntero a la estructura informacion , la instruccion que se desea trabajar, el PC de la intrucción y la etapa en la que se
esta desarollando.
- Salida: -
- Procedimiento: Modifica el puntero a la estructura informacion, tomando los registros rs y inmediato y realizando la suma de ellos 
y asi guardando el resultado en rd.
 */

void addi(Informacion *informacion, Instruccion *instruccion, int PC,int etapa)
{           
    int posrt;
    int posrs;

    posrt = obtenerPosicionReg(instruccion->rt);
    posrs = obtenerPosicionReg(instruccion->rs);

    
    if(etapa == 1) // ETAPA IF buffers[0] = IF/ID
    {   
        informacion->buffer[0].instruccion = instruccion;
        informacion->buffer[0].addPc = PC;
        informacion->buffer[0].estado = 1;
        informacion->buffer[0].estado2 = 0;
    }

    else if(etapa == 2) //ETAPA ID buffers[1] = ID/EX
    {
        informacion->buffer[1] = informacion->buffer[0];
        informacion->buffer[0].estado = 0;
        // Etapa nueva 
        informacion->buffer[1].muxRegDs = informacion->buffer[1].instruccion->rd;
        informacion->buffer[1].rs = informacion->buffer[1].instruccion->rs;
        informacion->buffer[1].rt = informacion->buffer[1].instruccion->rt;        
        informacion->buffer[1].rd = informacion->buffer[1].instruccion->rt;
        informacion->buffer[1].signoExtendido = informacion->buffer[1].instruccion->inmediato;
        informacion->buffer[1].lineaDeControl = asignarLineasDeControl(informacion->buffer[1].instruccion->instruccion);
        informacion->buffer[1].writeRegister = informacion->buffer[0].instruccion->rt;
        informacion->buffer[1].readData1Id = informacion->registros[posrs];
    }
 
    else if(etapa == 3) //ETAPA EX buffers[2] = EX/MEM
    {   
        informacion->buffer[2] = informacion->buffer[1];
        informacion->buffer[1].estado = 0;
        // Etapa nueva 
        informacion->buffer[2].aluResult = informacion->buffer[2].readData1Id + informacion->buffer[2].signoExtendido;
    }

    else if(etapa == 4) //ETAPA MEM buffers[3] = MEM/WB
    {
        informacion->buffer[3] = informacion->buffer[2];
        informacion->buffer[2].estado = 0;
        //Etapa nueva   
        if (BANDERA1 == 0 && informacion->buffer[3].estado2 == 0)
        {      
            informacion->buffer[3].writeDataWb = informacion->buffer[3].aluResult;
        }
        else
        {
            BANDERA1--;
        }
    }

    else if(etapa == 5) //ETAPA WB
    {
        informacion->buffer[4] = informacion->buffer[3];
        informacion->buffer[3].estado = 0;
        // Etapa nueva
        if (BANDERA1 == 0 && informacion->buffer[4].estado2 == 0)
        {
            informacion->registros[posrt] = informacion->buffer[3].writeDataWb;
        }
        else
        {
            BANDERA1--;
        }
    }   
}

/*
- Entrada: Funcion que recibe como parametro un puntero a la estructura informacion , la instruccion que se desea trabajar, el PC de la intrucción y la etapa en la que se
esta desarollando.
- Salida: Entrega el valor de donde debe realizar el salto la funcion.
- Procedimiento: Se encarga de concatenar el nombre del label con un ':', para que asi luego ocupe el llamado a la funcion 
buscarPosicionEtiqueta y entregue el numero de donde se encuentra
 */

void jump(Informacion *informacion, Instruccion *instruccion, int PC, int etapa)
{   

    if(etapa == 1) // ETAPA IF buffers[0] = IF/ID
    {   
        informacion->buffer[0].instruccion = instruccion;
        informacion->buffer[0].addPc = PC;
        informacion->buffer[0].estado = 1;
        informacion->buffer[0].estado2 = 0;
    }

    else if(etapa == 2) //ETAPA ID buffers[1] = ID/EX
    {
        informacion->buffer[1] = informacion->buffer[0];
        informacion->buffer[0].estado = 0;
        // Etapa nueva 
        informacion->buffer[1].rs = informacion->buffer[1].instruccion->rs;
        informacion->buffer[1].rt = informacion->buffer[1].instruccion->rt;        
        informacion->buffer[1].rd = informacion->buffer[1].instruccion->rt;
        informacion->hazarControl = "H.C" ;
    }
 
    else if(etapa == 3) //ETAPA EX buffers[2] = EX/MEM
    {   
        informacion->buffer[2] = informacion->buffer[1];
        informacion->buffer[1].estado = 0;
        BANDERA2 = 1;
    }

    else if(etapa == 4) //ETAPA MEM buffers[3] = MEM/WB
    {
        informacion->buffer[3] = informacion->buffer[2];
        informacion->buffer[2].estado = 0;
        if (BANDERA1 != 0)
        {
            BANDERA1--;
        }
    }

    else if(etapa == 5) //ETAPA WB
    {
        informacion->buffer[4] = informacion->buffer[3];
        informacion->buffer[3].estado = 0;
        if (BANDERA1 != 0)
        {
            BANDERA1--;
        }
    } 
}

/*
- Entrada: Funcion que recibe como parametro un puntero a la estructura informacion , la instruccion que se desea trabajar, el PC de la intrucción y la etapa en la que se
esta desarollando.
- Salida: Entrega un 1 si se cumple la condicion y un 0 si esta no se cumple.
- Procedimiento: Se encarga de verificar si la condición de los registro se cumple o no, para asi tomar la decision
del valor a que retornara.
 */

void beq(Informacion *informacion, Instruccion *instruccion, int PC, int etapa)
{            
    int posrt;
    int posrs;
    int posrd;

    
    if(etapa == 1) // ETAPA IF buffers[0] = IF/ID
    {   
        informacion->buffer[0].instruccion = instruccion;
        informacion->buffer[0].addPc = PC;
        informacion->buffer[0].estado = 1;
        informacion->buffer[0].estado2 = 0;
    }

    else if(etapa == 2) //ETAPA ID buffers[1] = ID/EX
    {
        informacion->buffer[1] = informacion->buffer[0];
        informacion->buffer[0].estado = 0;        
        // Etapa nueva 
        //printf("%d %d \n", posrt,posrs );
        informacion->buffer[1].rs = informacion->buffer[1].instruccion->rs;
        informacion->buffer[1].rt = informacion->buffer[1].instruccion->rt;        
        informacion->buffer[1].rd = informacion->buffer[1].instruccion->rd;
        posrt = obtenerPosicionReg(informacion->buffer[1].rt);
        posrs = obtenerPosicionReg(informacion->buffer[1].rs); 
        informacion->buffer[1].lineaDeControl = asignarLineasDeControl(informacion->buffer[1].instruccion->instruccion);
        informacion->buffer[1].readData1Id = informacion->registros[posrs];
        informacion->buffer[1].readData2Id = informacion->registros[posrt];
    }
 
    else if(etapa == 3) //ETAPA EX buffers[2] = EX/MEM
    {   
        informacion->buffer[2] = informacion->buffer[1];
        informacion->buffer[1].estado = 0;
        // Etapa nueva 
        informacion->buffer[2].zero = informacion->buffer[2].readData1Id - informacion->buffer[2].readData2Id;
    }

    else if(etapa == 4) //ETAPA MEM buffers[3] = MEM/WB
    {
        informacion->buffer[3] = informacion->buffer[2];
        informacion->buffer[2].estado = 0;
        //Etapa nueva   
        if (BANDERA1 == 0 && informacion->buffer[3].estado2 == 0) 
        {      
            if (informacion->buffer[3].zero == 0 )
            {
                char *etiqueta = (char*)malloc(sizeof(char)*100);
                strcpy(etiqueta,informacion->buffer[3].instruccion->rd);
                strcat(etiqueta,":");
                posrd = buscarPosicionEtiqueta(etiqueta, informacion);
                informacion->instrucciones[informacion->buffer[1].addPc].HC = 1;            
                informacion->instrucciones[informacion->buffer[2].addPc].HC = 1;
                INTRUCCIONESEJECUTADAS = posrd;
                informacion->buffer[1].rs = "" ;
                informacion->buffer[1].rt = "" ;
                informacion->buffer[1].rd = "" ;
                informacion->buffer[2].rs = "" ;
                informacion->buffer[2].rt = "" ;
                informacion->buffer[2].rd = "" ;
                informacion->hazarControl = "H.C";
                BANDERA1 = 4;
                BANDERA = 1;
            }
        }
        else
        {
            BANDERA1--;
        }
    }

    else if(etapa == 5) //ETAPA WB
    {
        informacion->buffer[4] = informacion->buffer[3];
        informacion->buffer[3].estado = 0;
        if (BANDERA1 != 0)
        {
            BANDERA1--;
        }
    }  
}

/*
- Entrada: Funcion que recibe como parametro un puntero a la estructura informacion , la instruccion que se desea trabajar, el PC de la intrucción y la etapa en la que se
esta desarollando.
- Salida: -
- Procedimiento: Modifica el puntero a la estructura informacion, tomando los registros inmediato y la direccion, para asi luego buscar
el valor del registro con dicho valor y asi guardarlo en el registro correspondiente.
 */

void lw(Informacion *informacion, Instruccion *instruccion, int PC,int etapa)
{           
    int posrt;
    int posrs;

    posrt = obtenerPosicionReg(instruccion->rt);
    posrs = obtenerPosicionReg(instruccion->rs);

    
    if(etapa == 1) // ETAPA IF buffers[0] = IF/ID
    {   
        informacion->buffer[0].instruccion = instruccion;
        informacion->buffer[0].addPc = PC;
        informacion->buffer[0].estado = 1;
        informacion->buffer[0].estado2 = 0;
    }

    else if(etapa == 2) //ETAPA ID buffers[1] = ID/EX
    {
        informacion->buffer[1] = informacion->buffer[0];
        informacion->buffer[0].estado = 0;
        // Etapa nueva 
        informacion->buffer[1].muxRegDs = informacion->buffer[1].instruccion->rt;
        informacion->buffer[1].rs = informacion->buffer[1].instruccion->rs;
        informacion->buffer[1].rt = informacion->buffer[1].instruccion->rt;        
        informacion->buffer[1].rd = informacion->buffer[1].instruccion->rt;
        informacion->buffer[1].signoExtendido = informacion->buffer[1].instruccion->inmediato;
        informacion->buffer[1].lineaDeControl = asignarLineasDeControl(informacion->buffer[1].instruccion->instruccion);
        informacion->buffer[1].writeRegister = informacion->buffer[1].instruccion->rt;
        informacion->buffer[1].readData1Id = informacion->registros[posrs];
    }
 
    else if(etapa == 3) //ETAPA EX buffers[2] = EX/MEM
    {   
        informacion->buffer[2] = informacion->buffer[1];
        informacion->buffer[1].estado = 0;
        // Etapa nueva 
        informacion->buffer[2].aluResult = informacion->buffer[2].readData1Id + (informacion->buffer[2].signoExtendido/4);
    }

    else if(etapa == 4) //ETAPA MEM buffers[3] = MEM/WB
    {
        informacion->buffer[3] = informacion->buffer[2];
        informacion->buffer[2].estado = 0;
        //Etapa nueva      
        if (BANDERA1 == 0 && informacion->buffer[3].estado2 == 0)
        {   
            informacion->buffer[3].writeDataWb = informacion->memoria[informacion->buffer[3].aluResult];
        }
        else
        {
            BANDERA1--;
        }
    }

    else if(etapa == 5) //ETAPA WB
    {
        informacion->buffer[4] = informacion->buffer[3];
        informacion->buffer[3].estado = 0;
        // Etapa nueva
        if (BANDERA1 == 0 && informacion->buffer[4].estado2 == 0)
        {
            informacion->registros[posrt] = informacion->buffer[3].writeDataWb;
        }
        else
        {
            BANDERA1--;
        }
    }   
}

/*
- Entrada: Funcion que recibe como parametro un puntero a la estructura informacion , la instruccion que se desea trabajar, el PC de la intrucción y la etapa en la que se
esta desarollando.
- Salida: -
- Procedimiento: Modifica el puntero a la estructura informacion, tomando los registros inmediato y la direccion, para asi luego buscar
el valor del registro con dicho valor y asi guardarlo en la memoria correspondiente.
 */

void sw(Informacion *informacion, Instruccion *instruccion, int PC,int etapa)
{           
    int posRegistro;
    int posrs;

    posrs = obtenerPosicionReg(instruccion->rs);

    if(etapa == 1) // ETAPA IF buffers[0] = IF/ID
    {   
        informacion->buffer[0].instruccion = instruccion;
        informacion->buffer[0].addPc = PC;
        informacion->buffer[0].estado = 1;
        informacion->buffer[0].estado2 = 0;
    }

    else if(etapa == 2) //ETAPA ID buffers[1] = ID/EX
    {
        informacion->buffer[1] = informacion->buffer[0];
        informacion->buffer[0].estado = 0;
        // Etapa nueva 
        informacion->buffer[1].muxRegDs = informacion->buffer[1].instruccion->rt;
        informacion->buffer[1].rs = informacion->buffer[1].instruccion->rs;
        informacion->buffer[1].rt = informacion->buffer[1].instruccion->rt;        
        informacion->buffer[1].rd = informacion->buffer[1].instruccion->rt;
        informacion->buffer[1].signoExtendido = informacion->buffer[1].instruccion->inmediato;
        informacion->buffer[1].lineaDeControl = asignarLineasDeControl(informacion->buffer[1].instruccion->instruccion);
        informacion->buffer[1].writeRegister = informacion->buffer[0].instruccion->rt;
        informacion->buffer[1].readData1Id = informacion->registros[posrs];
    }
 
    else if(etapa == 3) //ETAPA EX buffers[2] = EX/MEM
    {   
        informacion->buffer[2] = informacion->buffer[1];
        informacion->buffer[1].estado = 0;
        // Etapa nueva 
        informacion->buffer[2].aluResult = informacion->buffer[2].readData1Id + (informacion->buffer[2].signoExtendido/4);
        informacion->buffer[2].posRegistro = obtenerPosicionReg(informacion->buffer[2].instruccion->rt);
    }

    else if(etapa == 4) //ETAPA MEM buffers[3] = MEM/WB
    {
        informacion->buffer[3] = informacion->buffer[2];
        informacion->buffer[2].estado = 0;
        //Etapa nueva   
        if (BANDERA1 == 0 && informacion->buffer[3].estado2 == 0)
        {      
            informacion->memoria[informacion->buffer[3].aluResult] = informacion->registros[informacion->buffer[3].posRegistro];
        }
        else
        {
            BANDERA1--;
        }
    }

    else if(etapa == 5) //ETAPA WB
    {
        informacion->buffer[4] = informacion->buffer[3];
        informacion->buffer[3].estado = 0;
        //No ocupe esta etapa
        if(BANDERA1 != 0)
        {
            BANDERA1--;
        }
    }   
}

/*
- Entrada: Funcion que recibe como parametro una lista y el largo de ella.
- Salida: -
- Procedimiento: Se encarga de mostrar las lineas de control que contiene la lista.
 */

void mostrar(Instruccion* lista, int largo)
{
    int j;
    for (j = 0; j < largo; j++)
    {
        printf("RegDst : %c ",lista[j].lineaDeControl->RegDst);
        printf("Jump : %c ",lista[j].lineaDeControl->Jump);
        printf("Branch : %c ",lista[j].lineaDeControl->Branch);
        printf("MemRead : %c ",lista[j].lineaDeControl->MemRead);
        printf("MemToReg : %c ",lista[j].lineaDeControl->MemToReg);
        printf("ALUOp : %s ",lista[j].lineaDeControl->ALUOp);
        printf("MemWrite : %c ",lista[j].lineaDeControl->MemWrite);
        printf("ALUSrc : %c ",lista[j].lineaDeControl->ALUSrc);
        printf("RegWrite : %c \n",lista[j].lineaDeControl->RegWrite);
    }
}

// Lineas de control para las funciones

/*
- Entrada: Funcion que recibe como parametro el nombre de la instruccion y el PC.
- Salida: Se encarga de entregar la linea de control correspondiente al nombre de la instruccion dada.
- Procedimiento: Se encarga de verificar el nombre de la instruccion y otorgarle las lienas de control que le corresponden.
 */

LineaDeControl* asignarLineasDeControl (char* instruccion)
{
    LineaDeControl* aux = (LineaDeControl*)malloc(sizeof(LineaDeControl));
    aux->ALUOp = (char*)malloc(sizeof(char)*10);

    if (
        !strcmp(instruccion,"add") ||
        !strcmp(instruccion,"sub") ||
        !strcmp(instruccion,"mul") ||
        !strcmp(instruccion,"div")
        ){
            aux->RegDst = '1';
            aux->Jump = '0';
            aux->Branch = '0';
            aux->MemRead = '0';
            aux->MemToReg = '0';
            strcpy(aux->ALUOp , "10");
            aux->MemWrite = '0';
            aux->ALUSrc = '0';
            aux->RegWrite = '1';
    }

    else if (!strcmp(instruccion,"addi"))
    {
            aux->RegDst = '0';
            aux->Jump = '0';
            aux->Branch = '0';
            aux->MemRead = '0';
            aux->MemToReg = '0';
            strcpy(aux->ALUOp , "00");
            aux->MemWrite = '0';
            aux->ALUSrc = '1';
            aux->RegWrite = '1';
    }

    else if (!strcmp(instruccion,"vacio"))
    {
            aux->RegDst = '0';
            aux->Jump = '0';
            aux->Branch = '0';
            aux->MemRead = '0';
            aux->MemToReg = '0';
            strcpy(aux->ALUOp , "00");
            aux->MemWrite = '0';
            aux->ALUSrc = '0';
            aux->RegWrite = '0';
    }

    else if (!strcmp(instruccion,"subi"))
    {
            aux->RegDst = '0';
            aux->Jump = '0';
            aux->Branch = '0';
            aux->MemRead = '0';
            aux->MemToReg = '0';
            strcpy(aux->ALUOp , "01");
            aux->MemWrite = '0';
            aux->ALUSrc = '1';
            aux->RegWrite = '1';
    }

    else if (
        !strcmp(instruccion,"beq")
        ){
            aux->RegDst = 'x';
            aux->Jump = '0';
            aux->Branch = '1';
            aux->MemRead = '0';
            aux->MemToReg = 'x';
            strcpy(aux->ALUOp , "01");
            aux->MemWrite = '0';
            aux->ALUSrc = '0';
            aux->RegWrite = '0';
    }

    else if (
        !strcmp(instruccion,"j")
        ){
            aux->RegDst = 'x';
            aux->Jump = '1';
            aux->Branch = 'x';
            aux->MemRead = '0';
            aux->MemToReg = 'x';
            strcpy(aux->ALUOp , "xx");
            aux->MemWrite = '0';
            aux->ALUSrc = 'x';
            aux->RegWrite = '0';
    }

    else if (
        !strcmp(instruccion,"lw")
        ){
            aux->RegDst = '0';
            aux->Jump = '0';
            aux->Branch = '0';
            aux->MemRead = '1';
            aux->MemToReg = '1';
            strcpy(aux->ALUOp , "00");
            aux->MemWrite = '0';
            aux->ALUSrc = '1';
            aux->RegWrite = '1';
    }

    else if (
        !strcmp(instruccion,"sw")
        ){
            aux->RegDst = 'x';
            aux->Jump = '0';
            aux->Branch = '0';
            aux->MemRead = '0';
            aux->MemToReg = 'x';
            strcpy(aux->ALUOp , "00");
            aux->MemWrite = '1';
            aux->ALUSrc = '1';
            aux->RegWrite = '0';
    }

    return aux;
}

/*
- Entrada: Funcion que recibe como parametro una estructura informacion.
- Salida: Entrega como salida un 1 si se cumple la condición, o en caso contratio un 0.
- Procedimiento: Se encarga de verificar si existe la necesidad de colocar un nop.
 */

int is_nop(Informacion* informacion)
{
    if( (informacion->buffer[1].lineaDeControl->MemRead == '1' ) && (strcmp(informacion->buffer[1].instruccion->instruccion , informacion->instrucciones[INTRUCCIONESEJECUTADAS].instruccion) != 0) &&
        ( strcmp(informacion->buffer[1].rt , informacion->instrucciones[INTRUCCIONESEJECUTADAS].rs ) == 0 || 
        strcmp( informacion->buffer[1].rt  , informacion->instrucciones[INTRUCCIONESEJECUTADAS].rt ) == 0 ) && 
        strcmp(informacion->buffer[1].instruccion->instruccion , "lw") == 0 )  
    {
        return 1;
    }

    else
    {
        return 0;
    }
}

/*
- Entrada: Funcion que recibe como parametro una estructura informacion.
- Salida: -.
- Procedimiento: Se encarga de verificar si existe un hazar de dato en la etapa EX.
 */

void hazarDatoEx( Informacion* informacion ) 
{

    if ( informacion->buffer[2].lineaDeControl->RegWrite == '1' && strcmp(informacion->buffer[2].rd , informacion->buffer[1].rs) == 0  &&
        strcmp(informacion->buffer[2].instruccion->instruccion, "beq") != 0 )
    {
        informacion->hazarDatoEx = informacion->buffer[2].rd; 
        informacion->buffer[1].readData1Id = informacion->buffer[2].aluResult;
    }

    if ( informacion->buffer[2].lineaDeControl->RegWrite == '1' && strcmp(informacion->buffer[2].rd , informacion->buffer[1].rt) == 0 &&
        strcmp(informacion->buffer[2].instruccion->instruccion, "beq") != 0 )
    {
        informacion->hazarDatoEx = informacion->buffer[2].rd; 
        informacion->buffer[1].readData2Id = informacion->buffer[2].aluResult;
    }
}

/*
- Entrada: Funcion que recibe como parametro una estructura informacion.
- Salida: -.
- Procedimiento: Se encarga de verificar si existe un hazar de dato en la etapa MEN.
 */

void hazarDatoMEM( Informacion* informacion ) 
{
    if (informacion->buffer[3].lineaDeControl->RegWrite == '1' && strcmp(informacion->buffer[3].rd , informacion->buffer[1].rs ) == 0 &&
        strcmp(informacion->buffer[2].instruccion->instruccion, "beq") != 0 )
    {
        informacion->hazarDatoMem = informacion->buffer[3].rd; 

        if(informacion->buffer[3].lineaDeControl->MemRead == '1')
        {
            informacion->buffer[1].readData1Id = informacion->buffer[3].readData1Mem;
        }

        else
        {
            informacion->buffer[1].readData1Id = informacion->buffer[3].aluResult;
        }

    }

    if (informacion->buffer[3].lineaDeControl->RegWrite == '1' && strcmp(informacion->buffer[3].rd , informacion->buffer[1].rt ) == 0 &&
    strcmp(informacion->buffer[2].instruccion->instruccion, "beq") != 0  )
    {
        informacion->hazarDatoMem = informacion->buffer[3].rd; 

        if(informacion->buffer[3].lineaDeControl->MemRead == '1' )
        {
            informacion->buffer[1].readData2Id = informacion->buffer[3].readData1Mem;
        }

        else
        {
            informacion->buffer[1].readData2Id = informacion->buffer[3].aluResult;
        }
    }

}

/*
- Entrada: Funcion que recibe como parametro una estructura instruccion.
- Salida: Entrega un puntero a instruccion que se encarga de resetarla.
- Procedimiento: Se encarga de resetear la instruccioón y dejarla como un nop.
 */

Instruccion* resetearInstruccion(Instruccion* instruccion)
{
    instruccion->instruccion = "vacio";
    instruccion->rs = "" ;
    instruccion->rt = "" ;
    instruccion->rd = "" ;
}

/*
- Entrada: Funcion que recibe como parametro una estructura informacion y un numer entero ue indica la etapa en la que esta.
- Salida: -.
- Procedimiento: Se encarga de resetear el buffer.
 */

void resetearBuffer(Informacion* informacion, int i)
{
    informacion->buffer[i].instruccion = resetearInstruccion(informacion->buffer[i].instruccion);
    informacion->buffer[i].lineaDeControl = resetearLineasControl(informacion->buffer[i].lineaDeControl);
    informacion->buffer[i].addPc = 0;
    informacion->buffer[i].posRegistro = 0;
    informacion->buffer[i].readData1Id = 0;
    informacion->buffer[i].readData2Id = 0;
    informacion->buffer[i].signoExtendido= 0;
    informacion->buffer[i].zero = 0;
    informacion->buffer[i].aluResult = 0;
    informacion->buffer[i].readData1Mem = 0;
    informacion->buffer[i].ALU = 0;
    informacion->buffer[i].writeDataWb = 0;
    informacion->buffer[i].rs = "" ;
    informacion->buffer[i].rt = "";
    informacion->buffer[i].register1 = "";
    informacion->buffer[i].writeRegister = "";
    informacion->buffer[i].writeDataMem = "";
    informacion->buffer[i].address = "" ;
    informacion->buffer[i].register2 = "";
    informacion->buffer[i].rd = "";
    informacion->buffer[i].muxRegDs = "";
}

/*
- Entrada: Funcion que recibe como parametro una estructura informacion.
- Salida: Entrega un 1 si los buffer estan vacios, en caso contrario un 0.
- Procedimiento: Verifica cada uno de los buffer, para ver si se encuentran vacios.
 */

int chequearBuffer(Informacion* informacion)
{
    int i;

    for ( i = 0 ; i < 4 ; i++) 
    {
        if ( strcmp(informacion->buffer[i].instruccion->instruccion,"vacio") != 0 )
        {
            return 1;
        }
    }
    return 0;
}

/*
- Entrada: Funcion que recibe como parametro una estructura informacion.
- Salida: -.
- Procedimiento: Se encarga de hacer e procedimiento pipelene, modelando que las instrucciones pueden llegar a estar en 5 etapas distintas.
 */

void pipeLine(Informacion *informacion, char nombreSalida1[], char nombreSalida2[])
{
    int i = 0;
    int ciclo = 1;
    int nubes = 0;

    Instruccion* nob = (Instruccion*)malloc(sizeof(Informacion));
    nob->instruccion = (char*)malloc(sizeof(char)*100);
    nob->rd = (char*)malloc(sizeof(char)*100);
    nob->rs = (char*)malloc(sizeof(char)*100);
    nob->rt = (char*)malloc(sizeof(char)*100);
    nob->lineaDeControl = (LineaDeControl*)malloc(sizeof(LineaDeControl));
    nob = inicializarIntrucciones(nob);
    do
    {
        if (informacion->buffer[3].estado == 1)
        {
            etapaWB(informacion,INTRUCCIONESEJECUTADAS);
        }

        if (informacion->buffer[2].estado == 1)
        {
            hazarDatoMEM(informacion);  
            etapaMEM(informacion,INTRUCCIONESEJECUTADAS);        
        }

        if (informacion->buffer[1].estado == 1)
        {
            hazarDatoEx(informacion);
            etapaEX(informacion,INTRUCCIONESEJECUTADAS);            
        }

        if (informacion->buffer[0].estado == 1)
        {            
            etapaID(informacion,INTRUCCIONESEJECUTADAS);
        }

        if (informacion->cantidadDeInstrucciones > INTRUCCIONESEJECUTADAS)
        {
            if (is_nop(informacion) == 1 || BANDERA == 1 )
            {
                etapaIF(informacion,nob,INTRUCCIONESEJECUTADAS);
            }

            else
            {
                etapaIF(informacion,&informacion->instrucciones[INTRUCCIONESEJECUTADAS],INTRUCCIONESEJECUTADAS);
                INTRUCCIONESEJECUTADAS++;
            }
        }

        else if (informacion->cantidadDeInstrucciones <= INTRUCCIONESEJECUTADAS)
        {
            etapaIF(informacion,nob,INTRUCCIONESEJECUTADAS);
            INTRUCCIONESEJECUTADAS++;
        }

        if (BANDERA == 1)
        {
            informacion->buffer[1].estado1 = 1;
            informacion->buffer[2].estado1 = 1;
            informacion->buffer[1].rs = "" ;
            informacion->buffer[1].rt = "" ;
            informacion->buffer[1].rd = "" ;
            informacion->buffer[2].rs = "" ;
            informacion->buffer[2].rt = "" ;
            informacion->buffer[2].rd = "" ;
            BANDERA = 0;
        }

        if (BANDERA2 == 1)
        {
            char *etiqueta = (char*)malloc(sizeof(char)*100);
            strcpy(etiqueta,informacion->buffer[2].instruccion->rd);
            strcat(etiqueta,":");
            INTRUCCIONESEJECUTADAS = buscarPosicionEtiqueta(etiqueta, informacion);
            informacion->buffer[1].rs = "" ;
            informacion->buffer[1].rt = "" ;
            informacion->buffer[1].rd = "" ;
            informacion->buffer[1].readData1Id = 0;
            informacion->buffer[1].readData2Id = 0;
            informacion->buffer[1].writeDataWb = 0;
            informacion->buffer[1].estado2 = 1;
            BANDERA2--;
        }
        
        escribirArchivoTraza(informacion ,ciclo,nombreSalida1);
        escribirArchivoHazar(informacion,ciclo,nombreSalida2);
        ciclo++;
    }while( chequearBuffer(informacion) != 0 );
}

/*
- Entrada: Funcion que recibe como parametro una estructura informacion, una estructura instruccion y un entero que es el PC.
- Salida: -.
- Procedimiento: Se encarga de llamar a la funcion, en su respectiva etapa que se encuentra.
 */

void etapaIF(Informacion *info, Instruccion *instruccion , int PC)
{   
    if(!strcmp(instruccion->instruccion,"add"))
    {
        add(info,instruccion,PC, 1);
    }

    else if(!strcmp(instruccion->instruccion,"sub"))
    {
        sub(info,instruccion,PC, 1);
    }

    else if(!strcmp(instruccion->instruccion,"mul"))
    {
        mul(info,instruccion,PC, 1);
    }

    else if(!strcmp(instruccion->instruccion,"div"))
    {
        division(info,instruccion,PC, 1);
    }

    else if(!strcmp(instruccion->instruccion,"subi"))
    {
        subi(info,instruccion,PC, 1);
    }

    else if(!strcmp(instruccion->instruccion,"addi"))
    {
        addi(info,instruccion,PC, 1);
    }

    else if(!strcmp(instruccion->instruccion,"j"))
    {
        jump(info,instruccion,PC,1);
    }

    else if(!strcmp(instruccion->instruccion,"beq"))
    {
        beq(info,instruccion,PC,1);
    }

    else if(!strcmp(instruccion->instruccion,"sw"))
    {
        sw(info,instruccion,PC, 1);
    }

    else if(!strcmp(instruccion->instruccion,"lw"))
    {
        lw(info,instruccion,PC, 1);
    }

    else if(!strcmp(instruccion->instruccion,"vacio"))
    {
        vacio(info,instruccion,PC, 1);
    }
}

/*
- Entrada: Funcion que recibe como parametro una estructura informacion, una estructura instruccion y un entero que es el PC.
- Salida: -.
- Procedimiento: Se encarga de llamar a la funcion, en su respectiva etapa que se encuentra.
 */

void etapaID(Informacion *info, int PC)
{   
    if(!strcmp(info->buffer[0].instruccion->instruccion,"add"))
    {
        add(info,info->buffer[0].instruccion,PC, 2);
    }

    else if(!strcmp(info->buffer[0].instruccion->instruccion,"mul"))
    {
        mul(info,info->buffer[0].instruccion,PC,2);
    }

    else if(!strcmp(info->buffer[0].instruccion->instruccion,"sub"))
    {
        sub(info,info->buffer[0].instruccion,PC,2);
    }

    else if(!strcmp(info->buffer[0].instruccion->instruccion,"div"))
    {
        division(info,info->buffer[0].instruccion,PC,2);
    }

    else if(!strcmp(info->buffer[0].instruccion->instruccion,"j"))
    {
        jump(info,info->buffer[0].instruccion,PC,2);
    }

    else if(!strcmp(info->buffer[0].instruccion->instruccion,"beq"))
    {
        beq(info,info->buffer[0].instruccion,PC,2);
    }

    else if(!strcmp(info->buffer[0].instruccion->instruccion,"sw"))
    {
        sw(info,info->buffer[0].instruccion,PC,2);
    }

    else if(!strcmp(info->buffer[0].instruccion->instruccion,"lw"))
    {
        lw(info,info->buffer[0].instruccion,PC,2);
    }

    else if(!strcmp(info->buffer[0].instruccion->instruccion,"addi"))
    {
        addi(info,info->buffer[0].instruccion,PC, 2);
    }

    else if(!strcmp(info->buffer[0].instruccion->instruccion,"subi"))
    {
        subi(info,info->buffer[0].instruccion,PC,2);
    }

    else if(!strcmp(info->buffer[0].instruccion->instruccion,"vacio"))
    {
        vacio(info,info->buffer[0].instruccion,PC, 2);
    }
}

/*
- Entrada: Funcion que recibe como parametro una estructura informacion, una estructura instruccion y un entero que es el PC.
- Salida: -.
- Procedimiento: Se encarga de llamar a la funcion, en su respectiva etapa que se encuentra.
 */

void etapaEX(Informacion *info, int PC)
{
    if(!strcmp(info->buffer[1].instruccion->instruccion,"add"))
    {
        add(info,info->buffer[1].instruccion,PC, 3);
    }

    else if(!strcmp(info->buffer[1].instruccion->instruccion,"mul"))
    {
        mul(info,info->buffer[1].instruccion,PC,3);
    }

    else if(!strcmp(info->buffer[1].instruccion->instruccion,"j"))
    {
        jump(info,info->buffer[1].instruccion,PC,3);
    }

    else if(!strcmp(info->buffer[1].instruccion->instruccion,"beq"))
    {
        beq(info,info->buffer[1].instruccion,PC,3);
    }

    else if(!strcmp(info->buffer[1].instruccion->instruccion,"div"))    
    {
        division(info,info->buffer[1].instruccion,PC,3);
    }

    else if(!strcmp(info->buffer[1].instruccion->instruccion,"sub"))
    {
        sub(info,info->buffer[1].instruccion,PC,3);
    }

    else if(!strcmp(info->buffer[1].instruccion->instruccion,"sw"))
    {
        sw(info,info->buffer[1].instruccion,PC,3);
    }

    else if(!strcmp(info->buffer[1].instruccion->instruccion,"lw"))
    {
        lw(info,info->buffer[1].instruccion,PC,3);
    }

    else if(!strcmp(info->buffer[1].instruccion->instruccion,"addi"))
    {
        addi(info,info->buffer[1].instruccion,PC, 3);
    }

    else if(!strcmp(info->buffer[1].instruccion->instruccion,"subi"))
    {
        subi(info,info->buffer[1].instruccion,PC,3);
    }

    else if(!strcmp(info->buffer[1].instruccion->instruccion,"vacio"))
    {
        vacio(info,info->buffer[1].instruccion,PC, 3);
    }
}

/*
- Entrada: Funcion que recibe como parametro una estructura informacion, una estructura instruccion y un entero que es el PC.
- Salida: -.
- Procedimiento: Se encarga de llamar a la funcion, en su respectiva etapa que se encuentra.
 */

void etapaMEM(Informacion *info, int PC)
{
    if(!strcmp(info->buffer[2].instruccion->instruccion,"add"))
    {
        add(info,info->buffer[2].instruccion,PC, 4);
    }

    else if(!strcmp(info->buffer[2].instruccion->instruccion,"mul"))
    {
        mul(info,info->buffer[2].instruccion,PC,4);
    }

    else if(!strcmp(info->buffer[2].instruccion->instruccion,"j"))
    {
        jump(info,info->buffer[2].instruccion,PC,4);
    }

    else if(!strcmp(info->buffer[2].instruccion->instruccion,"beq"))
    {
        beq(info,info->buffer[2].instruccion,PC,4);
    }

    else if(!strcmp(info->buffer[2].instruccion->instruccion,"sub"))
    {
        sub(info,info->buffer[2].instruccion,PC,4);
    }

    else if(!strcmp(info->buffer[2].instruccion->instruccion,"div"))
    {
        division(info,info->buffer[2].instruccion,PC,4);
    }

    else if(!strcmp(info->buffer[2].instruccion->instruccion,"sw"))
    {
        sw(info,info->buffer[2].instruccion,PC,4);
    }

    else if(!strcmp(info->buffer[2].instruccion->instruccion,"lw"))
    {
        lw(info,info->buffer[2].instruccion,PC,4);
    }

    else if(!strcmp(info->buffer[2].instruccion->instruccion,"addi"))
    {
        addi(info,info->buffer[2].instruccion,PC, 4);
    }

    else if(!strcmp(info->buffer[2].instruccion->instruccion,"subi"))
    {
        subi(info,info->buffer[2].instruccion,PC,4);
    }

    else if(!strcmp(info->buffer[2].instruccion->instruccion,"vacio"))
    {
        vacio(info,info->buffer[2].instruccion,PC, 4);
    }
}

/*
- Entrada: Funcion que recibe como parametro una estructura informacion, una estructura instruccion y un entero que es el PC.
- Salida: -.
- Procedimiento: Se encarga de llamar a la funcion, en su respectiva etapa que se encuentra.
 */

void etapaWB(Informacion *info, int PC)
{
    if(!strcmp(info->buffer[3].instruccion->instruccion,"add"))
    {
        add(info,info->buffer[3].instruccion,PC, 5);
    }

    else if(!strcmp(info->buffer[3].instruccion->instruccion,"mul"))
    {
        mul(info,info->buffer[3].instruccion,PC,5);
    }

    else if(!strcmp(info->buffer[3].instruccion->instruccion,"j"))
    {
        jump(info,info->buffer[3].instruccion,PC,5);
    }

    else if(!strcmp(info->buffer[3].instruccion->instruccion,"beq"))
    {
        beq(info,info->buffer[3].instruccion,PC,5);
    }

    else if(!strcmp(info->buffer[3].instruccion->instruccion,"sub"))
    {
        sub(info,info->buffer[3].instruccion,PC,5);
    }

    else if(!strcmp(info->buffer[3].instruccion->instruccion,"div"))
    {
        division(info,info->buffer[3].instruccion,PC,5);
    }

    else if(!strcmp(info->buffer[3].instruccion->instruccion,"sw"))
    {
        sw(info,info->buffer[3].instruccion,PC,5);
    }

    else if(!strcmp(info->buffer[3].instruccion->instruccion,"lw"))
    {
        lw(info,info->buffer[3].instruccion,PC,5);
    }

    else if(!strcmp(info->buffer[3].instruccion->instruccion,"addi"))
    {
        addi(info,info->buffer[3].instruccion,PC, 5);
    }

    else if(!strcmp(info->buffer[3].instruccion->instruccion,"subi"))
    {
        subi(info,info->buffer[3].instruccion,PC,5);
    }

    else if(!strcmp(info->buffer[3].instruccion->instruccion,"vacio"))
    {
        vacio(info,info->buffer[3].instruccion,PC, 5);
    }
}

/*
- Entrada: Funcion que recibe como parametro una estructura informacion y un entero que representa los ciclos.
- Salida: -.
- Procedimiento: Se encarga de crear el archivo que muestra los hazar de datos y control que fueron encontrados.
 */

void escribirArchivoHazar(Informacion* informacion, int ciclo, char nombreArchivo[])
{
    FILE* archivo = fopen(nombreArchivo , "a");
    int i;

    if (ciclo == 1)
    {
        fprintf (archivo, "%s ; %s ; %s \n", "Ciclo","Hazar de datos (Ex - Mem)","Hazar de control");
    }

    fprintf(archivo, " %d ; %s - %s ; %s ", ciclo, informacion->hazarDatoEx,informacion->hazarDatoMem, informacion->hazarControl );

    fprintf(archivo, "\n");

    informacion->hazarDatoEx = "";
    informacion->hazarDatoMem = "";
    informacion->hazarControl = "";

    fclose(archivo);
}

/*
- Entrada: Funcion que recibe como parametro una estructura informacion, una estructura instruccion y un entero que es el PC.
- Salida: -.
- Procedimiento: Se encarga de ir escribiendo la traza del programa en una arhivo, representando lo que sucede en cada ciclo.
 */

void escribirArchivoTraza(Informacion* informacion, int ciclo,char nombreArchivo[]) 
{ 
    FILE *archivo = fopen(nombreArchivo, "a"); 

    int i ;
    if (ciclo == 1)
    {
        fprintf (archivo, "%s ; %s ;%s ;%s ;%s ;%s \n", "Ciclo","IF","ID","EX","MEM","WB");
    }

    fprintf(archivo, " %d ;", ciclo );

    for (i = 0; i < 5; i++)
    {
        if (informacion->buffer[i].estado1 == 0 && informacion->buffer[i].estado2 == 0)
        {
            if  (
            !strcmp(informacion->buffer[i].instruccion->instruccion,"add") ||
            !strcmp(informacion->buffer[i].instruccion->instruccion,"sub") ||
            !strcmp(informacion->buffer[i].instruccion->instruccion,"mul") ||
            !strcmp(informacion->buffer[i].instruccion->instruccion,"div")
            )
            {
                fprintf (archivo, "%s %s %s %s ;",informacion->buffer[i].instruccion->instruccion,informacion->buffer[i].instruccion->rd, informacion->buffer[i].instruccion->rs ,
                informacion->buffer[i].instruccion->rt);
            }

            else if (
            !strcmp(informacion->buffer[i].instruccion->instruccion,"addi") ||
            !strcmp(informacion->buffer[i].instruccion->instruccion,"subi")
            )
            {
                fprintf (archivo, "%s %s %s %d ;",informacion->buffer[i].instruccion->instruccion,informacion->buffer[i].instruccion->rt, informacion->buffer[i].instruccion->rs ,
                informacion->buffer[i].instruccion->inmediato);
            }

            else if (!strcmp(informacion->buffer[i].instruccion->instruccion,"lw") || !strcmp(informacion->buffer[i].instruccion->instruccion,"sw") ) 
            {
                fprintf (archivo, "%s %s %d(%s) ;",informacion->buffer[i].instruccion->instruccion,informacion->buffer[i].instruccion->rt, informacion->buffer[i].instruccion->inmediato ,
                informacion->buffer[i].instruccion->rs);
            }

            else if (!strcmp(informacion->buffer[i].instruccion->instruccion,"j") ) 
            {
                fprintf (archivo, "%s %s ;",informacion->buffer[i].instruccion->instruccion,informacion->buffer[i].instruccion->rd);
            }

            else if (!strcmp(informacion->buffer[i].instruccion->instruccion,"beq") ) 
            {
                fprintf (archivo, "%s %s %s %s;",informacion->buffer[i].instruccion->instruccion,informacion->buffer[i].instruccion->rs, informacion->buffer[i].instruccion->rt, 
                    informacion->buffer[i].instruccion->rd);
            }

            else if (
            !strcmp(informacion->buffer[i].instruccion->instruccion,"vacio") 
            )
            {
                fprintf (archivo, " ;");
            }       
        }

        else
        {
            fprintf (archivo, " ;");
        }

    }

    fprintf(archivo, "\n");

    fclose(archivo); 
} 