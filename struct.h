// Se importan las cosas que ayudaran al funcionamiento del codigo 
#include <stdio.h>
#include <string.h>
#include <locale.h>

// Definimos el arreglo constante que tendres, que se encarga de representar a los registros.

const char *registroMars[32] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
"$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};

// Definimos las estructuras a ocupar dentro de la aplicacion


// Esta estructura esta encargada de representar la linea de control de cada una de las isntrucciones del programa

typedef struct LineaDeControl
{
	char RegDst;
	char Jump;
	char Branch;
	char MemRead;
	char MemToReg;
	char* ALUOp;
	char MemWrite;
	char ALUSrc;
	char RegWrite;

}LineaDeControl;


 /*
 Esta estructura se encarga de guardar la informacion de cada una de las intrucciones y los respectivos registros
 que van a ocupar dependiendo la instrucciones que se este manejando
 */

typedef struct Instruccion
{
	char *instruccion;
	char *rt;
	char *rs;
	char *rd;
	int inmediato;
	int PC;
	int HC;
	LineaDeControl* lineaDeControl;

}Instruccion;

/*
Esta estrucutra se encarga de ir guardando la informacion de cada una de las intrucciones a medida que van pasadndo
por distintos buffer
 */

typedef struct Buffer
{
	int estado;
	int estado1;
	int estado2;
	Instruccion* instruccion;
	LineaDeControl* lineaDeControl;
	char* muxRegDs;
	char* register1;
	char* register2;
	int readData1Id;
	int readData2Id;
	int signoExtendido;
	int ALU;
	char* address;
	char* writeDataMem;
	int readData1Mem;
	char* writeRegister;
	int writeDataWb;
	int zero;
	int aluResult;
	int addPc;
	char* rs;
	char* rt;
	char* rd;
	int posRegistro;
}Buffer;


// Esta estrucutra se encarga de guardar la posicion de la etiqueta y el nombre correspodiente

typedef struct Label
{
	char *label;
	int PC;

}Label;



// Estructura que se encarga de armar una lista con tamaño variable que contendra una cantidad de lineas de control

typedef struct Lista
{
	int largo;
	LineaDeControl* lineaTraza;

}Lista;

/*
Estructura que se encarga de guardar a cada una de las estructuras mencionadas anteriormente, ademas de guardar informacion
revelando para el desarrollo del programa, como los valores de los registros que se tienen
 */

typedef struct Informacion
{
	Instruccion* instrucciones;
	int registros[32];
	int cantidadDeInstrucciones;
	Label etiqueta[100];
	int memoria[1000];
	Buffer* buffer;
	char* hazarDatoEx;
	char* hazarDatoMem;
	char* hazarControl;

}Informacion;


// FUNCIONES

Informacion* leerRegistros(Informacion* info,char nombre[]);
void inicializarMemoriaParaInstrucciones(Informacion *informacion, int cantidadLineas);
Informacion* leerInstrucciones(char nombre[], int numeroDeLineas);
int obtenerPosicionReg(char* registro);
int contarLineas(char nombre[]);
int obtenerPosicionReg(char* registro);
void add(Informacion *informacion, Instruccion *instruccion, int PC,int etapa);
void sub(Informacion *informacion, Instruccion *instruccion, int PC,int etapa);
void subi(Informacion *informacion, Instruccion *instruccion, int PC,int etapa);
void addi(Informacion *informacion, Instruccion *instruccion, int PC,int etapa);
void mul(Informacion *informacion, Instruccion *instruccion, int PC,int etapa);
void division(Informacion *informacion, Instruccion *instruccion, int PC,int etapa);
LineaDeControl* asignarLineasDeControl (char* instruccion);
void jump(Informacion *informacion, Instruccion *instruccion, int PC, int etapa);
int buscarPosicionEtiqueta(char* etiqueta, Informacion *info);
void sw(Informacion *informacion, Instruccion *instruccion, int PC,int etapa);
void lw(Informacion *informacion, Instruccion *instruccion, int PC,int etapa);
void beq(Informacion *informacion, Instruccion *instruccion, int PC, int etapa);
void mostrarLineasDeControl(Lista* lista);
void mostrar(Instruccion* lista, int largo);
void realizarSalida(Lista* lista,LineaDeControl* lineasControlDadas);
LineaDeControl* resetearLineasControl(LineaDeControl* lineaDeContol);
void iniciarBuffer(Buffer* buffer);
void pipeLine(Informacion *informacion, char nombreSalida1[], char nombreSalida2[]);
void etapaIF(Informacion *info, Instruccion *instruccion, int PC);
Instruccion* inicializarIntrucciones(Instruccion* instruccion);
void mostrarLineasControl(LineaDeControl* aux);
Instruccion* resetearInstruccion(Instruccion* instruccion);
void resetearBuffer(Informacion* informacion, int i);
void etapaID(Informacion *info, int PC);
void etapaEX(Informacion *info,int PC);
void etapaMEM(Informacion *info, int PC);
void etapaWB(Informacion *info, int PC);
int chequearBuffer(Informacion* informacion);
void vacio(Informacion *informacion, Instruccion *instruccion, int PC,int etapa);
void fordwarding(Informacion *informacion);
void escribirArchivoTraza(Informacion* informacion, int ciclo,char archivo[]);
void escribirArchivoHazar(Informacion* informacion, int ciclo, char archivo[]);
void hazarDatoMEM( Informacion* informacion );
void hazarDatoEx( Informacion* informacion );