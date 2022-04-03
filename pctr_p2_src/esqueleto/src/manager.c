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
  char Mutex_Puerta [1024];


int main(int argc, char *argv[]){

  srand(((int)getpid()));

  creaRecursos();


  if (signal(SIGINT, ctrlc) == SIG_ERR) {
        fprintf(stderr, "Abrupt termination.\n"); 
        exit(EXIT_FAILURE);
    }
  
  for(j=0; j <NUM_CLIENTES; ++j){
    barberoAsignado=rand()%NUM_BARBEROS;
      switch (pids_clientes[j]=fork()){
         case -1:
        fprintf(stderr,"Error en la creacion del cliente.\n");
        return EXIT_FAILURE;
        break;

      case 0:
        sprintf(asignadoBarbero, "%d", barberoAsignado);

        execl("./exec/cliente","./exec/cliente", asignadoBarbero, COSTE_CORTE, AFORO_MAX, MUTEX_PUERTA, NULL);  //virginia, para que funcione como lo haces tu, cambia el ./ecec/cliente a ./cliente
        fprintf(stderr,"No se esta ejecutando el execl del cliente. \n");
         return EXIT_FAILURE;
        break;
    
      default:
        break;
      }
    }

  //Ejecucion de todos los procesos barberos.
  printf("porque aqui si\n");
  for(i=0; i<NUM_BARBEROS; ++i){

    printf("aqui %d\n", i);

    switch(pids_barberos[i]=fork()){
      case -1:
        fprintf(stderr,"Error en la creacion del barbero.\n");
        return EXIT_FAILURE;
      break;

      case 0: 

        sprintf(idBarb,"%d",i);
        execl("./exec/barbero", "./exec/barbero",idBarb, COSTE_CORTE, NULL); //virginia, para que funcione como lo haces tu, cambia el ./ecec/barbero a ./barbero
        fprintf(stderr,"No se esta ejecutando el execl del barbero. \n");
        return EXIT_FAILURE;

      default:
      break;
        //continue;
    }
  }

  
    for (i = 0; i < NUM_BARBEROS; i++) 
        waitpid(pids_barberos[i], 0, 0);
    for (i = 0; i < NUM_CLIENTES; i++) 
        waitpid(pids_clientes[i], 0, 0);
    
    liberaRecursos(); 

    return EXIT_SUCCESS;
}

void ctrlc(int senial) {
    finalizarprocesos(); 
    liberaRecursos();
    printf ("\nFin del programa (Ctrol + C).\n\n Todos los barberos han terminado su jornada. La barberia esta cerrada.\n"); 
    exit(EXIT_SUCCESS);
}


  //Crear todos los semaforos y variables compartidas
void creaRecursos(){

  for(i=0; i < NUM_BARBEROS; i++){

    sprintf(barbero,"Barbero_[%d]",i);
    crear_sem(barbero, 0); //HE CAMBIADO ESTO A 1 Y PASA EL PRIMER BARBERO. NO SE PORQUE EL RESTO NO
    //!!!!!!!!!!!!!!!! EL RESTO DE BARBEROS NO PASAN PORQUE EL PRIMER BARBERO DEJA NEL SEMÁFORO EN 0 Y EL SIGNAL ESTÁ EN CLIENTES
    //COMO EL SIGNAL ESTÁ EN CLIENTES Y POR AHÍ NO ENTRA, SOLO ENTRA EL PRIMER BARBERO. Además, lo de la propina solo me sale la primera vez que lo ejecuto
    //como no entra en cliente, el cliente no paga y por eso la propina sale negativa y que los beneficios son 0
    //he vuelto a dejar el semáforo de barbero en 0 porque es así como tiene que esta

    sprintf(pago,"Pago_[%d]",i);
    crear_sem(pago,1);    

    sprintf(fin,"Fin_[%d]",i);
    crear_sem(fin,0);

    sprintf(transaccion, "Transaccion_[%d]", i);
    crear_var(transaccion, 0);

    
    // printf("creado semaforo: %s\n",barbero);
    // printf("creado semaforo: %s\n",pago);
    // printf ("creado semaforo: %s\n",fin);
    // printf ("creada variable: %s\n",transaccion);

  }

  crear_sem("Sillones",NUM_SILLONES);
  crear_sem("Sillas",NUM_SILLAS);


  crear_sem("Mutex_Caja", 1);

  sprintf(Mutex_Puerta, MUTEX_PUERTA);
  crear_sem(Mutex_Puerta, 1);

  crear_var("Aforo_Actual",0);
  crear_var("Caja", 0);

}


//Destruir semaforos
void liberaRecursos(){

  //TODO: HACER QUE ESTO FUNCIONE, CREO QUE NO ESTA FUNCIONANDO
  char barbero [1024];
  char pago [1024];
  char fin [1024];
  for(i= 0; i < NUM_BARBEROS; i++){
    sprintf(barbero,"Barbero_[%d]",i);
    sprintf(pago,"Pago[%d]",i);
    sprintf(fin,"Fin[%d]",i);
    

    destruir_sem(barbero);
    destruir_sem(pago);
    destruir_sem(fin);

    printf("El barbero %d se marcha a casa.",i);
  }


  destruir_sem("Mutex_Caja");
  destruir_sem("GenteDentro");

  //destruir_var();
 
}



//matar clientes y matar barberos

void finalizarprocesos () {
 int i;
 printf ("-------------- Terminando ------------- \n");
 for (i = 0; i < NUM_CLIENTES; i++) {
  if (pids_clientes[i]) {
   printf ("Finalizando proceso cliente [%d]... ", pids_clientes[i]);
    kill(pids_clientes[i], SIGINT);
   printf ("<Finalizado exitosamente>\n");
    }
 }
 for (i = 0; i < NUM_BARBEROS; i++) {
  if (pids_barberos[i]) {
   printf ("Finalizando proceso barbero [%d]... ", pids_barberos[i]);
    kill(pids_barberos[i], SIGINT);
   printf ("<Finalizado exitosamente>\n");
    }
  }
 }