#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include <semaforoI.h>
#include <memoriaI.h>

#define NUM_CLIENTES 20
#define NUM_BARBEROS 3
#define NUM_SILLONES 3
#define NUM_SILLAS 10
#define COSTE_CORTE "10"

#define TIEMPO_CORTE_BASE "3" //Tiempo que se tarda en cortar el pelo. 
#define AFORO_MAX "20"
#define MUTEX_PUERTA "Mutex_Puerta"

int Aforo_Actual;
int Aforo_Max;

char barberoAsignado[1024];
char fin [1024];
char pago [1024];
char barbero [1024];
char Sillones [1024];
char Sillas [1024];
char Mutex_Puerta [1024];

int propina;
char transaccion [1024];
int transaccion1;
int pago_final;