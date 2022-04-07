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

#define TIEMPO_CORTE_BASE "1" //Tiempo que se tarda en cortar el pelo. 
#define AFORO_MAX "25"


/*
  Hay N sillas (sala de espera) y M clientes de pie
  Los clientes que no quepan se van directamente.

  Hay 3 sillones para cortar el pelo y 3 barberos para cortar el pelo.
  Hay 3 velocidades de barbero.

  Solo se puede sentar en un sillon un cliente que este en una silla.
  -----------------------------------------------------------------------------------------
  cuando matas al proceso padre, gestionar la eliminacion de los procesos hijos
asignarle a un cliente, un barbero desde el principio
pasar por linea de argumentos (al barbero), la velocidad del barbero

*/





void liberaRecursos(); 
void finalizarprocesos();
void creaRecursos();
void ctrlc(int);

int barberoAsignado;
int i;
int j;
char idBarb[1024];
char aforoMaximo[1024]=AFORO_MAX;
char asignadoBarbero [1024];
pid_t pids_clientes[NUM_CLIENTES];
pid_t pids_barberos[NUM_BARBEROS];  

char barbero [1024];
char pago [1024];
char fin [1024];
char transaccion [1024];
char propina [1024];


//Cuerpo de la ejecucion
int main(int argc, char *argv[]){

  srand(((int)getpid()));

  creaRecursos();


  if (signal(SIGINT, ctrlc) == SIG_ERR) {
        fprintf(stderr, "Abrupt termination.\n"); 
        exit(EXIT_FAILURE);
    }
  
  printf("\n------------------------------------------------------------\n\n<<<<<<<<<<<<< SE ABRE LA BARBERIA >>>>>>>>>>>>>\n\n");

  for(j=0; j <NUM_CLIENTES; ++j){
    barberoAsignado=rand()%NUM_BARBEROS;
      switch (pids_clientes[j]=fork()){
         case -1:
        fprintf(stderr,"Error en la creacion del cliente.\n");
        return EXIT_FAILURE;
        break;

      case 0:
        sprintf(asignadoBarbero, "%d", barberoAsignado);
        sleep(rand()%30); //Simula el tiempo que tardan en llegar los clientes a la barberia.
        execl("./exec/cliente","./exec/cliente", asignadoBarbero, COSTE_CORTE, AFORO_MAX, NULL);  //virginia, para que funcione como lo haces tu, cambia el ./ecec/cliente a ./cliente
        fprintf(stderr,"No se esta ejecutando el execl del cliente. \n");
         return EXIT_FAILURE;
        break;
    
      default:
        break;
      }
    }

  //Ejecucion de todos los procesos barberos.
  for(i=0; i<NUM_BARBEROS; ++i){

    switch(pids_barberos[i]=fork()){
      case -1:
        fprintf(stderr,"Error en la creacion del barbero.\n");
        return EXIT_FAILURE;
      break;

      case 0: 

        sprintf(idBarb,"%d",i);
        execl("./exec/barbero", "./exec/barbero",idBarb, COSTE_CORTE,TIEMPO_CORTE_BASE, NULL); //virginia, para que funcione como lo haces tu, cambia el ./ecec/barbero a ./barbero
        fprintf(stderr,"No se esta ejecutando el execl del barbero. \n");
        return EXIT_FAILURE;
        break;

      default:
      break;
        //continue;
    }
  }
  
    for (i = 0; i < NUM_CLIENTES; i++) {
        waitpid(pids_clientes[i], 0, 0);
    }

    printf("\n------------------------------------------------------------\nTodos los clientes han sido atendidos.\n");

    
    liberaRecursos(); 

    return EXIT_SUCCESS;

}

//Signal handler para la interrupcion manual.
void ctrlc(int senial) {
    printf ("\nFin del programa (Ctrol+C).\n");
    liberaRecursos();
    finalizarprocesos(); 
    printf ("Todos los barberos han terminado su jornada. La barberia esta cerrada.\n"); 
    exit(EXIT_SUCCESS);
}

//Crear todos los semaforos y variables compartidas.
void creaRecursos(){
  printf ("------------------------------------------------------------\nInicializacion de recursos compartidos.");
  for(i=0; i < NUM_BARBEROS; i++){

    sprintf(barbero,"Barbero_[%d]",i);
    crear_sem(barbero, 0); 
    sprintf(pago,"Pago_[%d]",i);
    crear_sem(pago,0);    

    sprintf(fin,"Fin_[%d]",i);
    crear_sem(fin,0);

    sprintf(transaccion, "Transaccion_[%d]", i);
    crear_var(transaccion, 0);

    sprintf(propina,"Propina_[%d]",i);
    crear_var(propina, 0);

    
    // printf("creado semaforo: %s\n",barbero);
    // printf("creado semaforo: %s\n",pago);
    // printf ("creado semaforo: %s\n",fin);
    // printf ("creada variable: %s\n",transaccion);

  }

  crear_sem("Sillones",NUM_SILLONES);
  // printf("Se crea el semaforo Sillones con %d sillones disponibles\n",NUM_SILLONES);
  crear_sem("Sillas",NUM_SILLAS);
  // printf("Se crea el semaforo Sillas con %d sillas disponibles\n",NUM_SILLAS);


  crear_sem("Mutex_Caja", 1);
  // printf("Mutex_Caja creado \n");


  crear_sem("mutexPuerta", 1);
  //printf ("Mutex_Puerta creado\n");

  crear_var("Aforo_Actual",0);
  crear_var("Caja", 0);
  //printf ("Caja registradora abierta\n");


  printf ("<De forma exitosa>");
}

//Destruir semaforos.
void liberaRecursos(){
  int recaudacion;
  int propina_total;
  
  printf ("------------------------------------------------------------\n");

  for(i= 0; i < NUM_BARBEROS; i++){
    sprintf(barbero,"Barbero_[%d]",i);
    sprintf(pago,"Pago_[%d]",i);
    sprintf(fin,"Fin_[%d]",i);
    sprintf(transaccion,"Transaccion_[%d]",i);
    sprintf(propina,"Propina_[%d]",i);

    destruir_sem(barbero);
    destruir_sem(pago);
    destruir_sem(fin);

    consultar_var(obtener_var(propina),&propina_total);

    destruir_var(transaccion);
    destruir_var(propina);

    printf("El barbero %d se marcha a casa con una propina de %d Euros.\n",i,propina_total);
  }

  destruir_sem("Sillones");
  destruir_sem("Sillas");

  destruir_sem("Mutex_Caja");
  destruir_sem("mutexPuerta");


  consultar_var(obtener_var("Caja"),&recaudacion);
  printf ("\nLa barberia ha sacado un beneficio de %d Euros hoy. \n",recaudacion);
  destruir_var("Caja");

  destruir_var("Aforo_Actual");

  //printf ("\n\nSemaforos limpios\n");
}

//matar procesos clientes y matar barberos.
void finalizarprocesos () {
 int i;
 printf ("\n <<<<<<<<<<<<<< Terminando >>>>>>>>>>>>>>  \n");
 for (i = 0; i < NUM_CLIENTES; i++) {
  if (pids_clientes[i]) {
   //printf ("Finalizando proceso cliente [%d]... ", pids_clientes[i]);
    kill(pids_clientes[i], SIGINT);
   printf ("<Finalizado exitosamente>\n");
    }
 }
 for (i = 0; i < NUM_BARBEROS; i++) {
  if (pids_barberos[i]) {
   //printf ("Finalizando proceso barbero [%d]... ", pids_barberos[i]);
    kill(pids_barberos[i], SIGINT);
   printf ("<Finalizado exitosamente>\n");
    }
  }
 }