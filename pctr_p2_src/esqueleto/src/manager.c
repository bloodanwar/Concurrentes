#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include <semaforoI.h>
#include <memoriaI.h>


#define NUM_CLIENTES 100
#define NUM_BARBEROS 3
#define TIEMPO_CORTE_BASE 2 //Tiempo que se tarda en cortar el pelo. 




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





void liberarsemaforos(); 
void finalizarprocesos();
void creasemaforos();
void ctrlc(int);

  int i;

pid_t pids_clientes[NUM_CLIENTES];
pid_t pids_barberos[NUM_BARBEROS];  

int main(int argc, char *argv[]){ 
  srand(((int)getpid()));
   srand(((int)getpid()));
  for(i=0; i<NUM_BARBEROS; i++){
    switch(pids_barbero[i]=fork()){
      case 0: 
      execl("./exe/barbero", "barbero", i, TIEMPO_CORTE_BASE);
      break;
      for(i=0; i<NUM_BARBERO; )
    }
  }
  
1
  if (creasemaforos()!=0){
    fprintf(stderr,"Error en la creacion de los semaforos.");
    return EXIT_FAILURE;
  }

  if (signal(SIGINT, ctrlc) == SIG_ERR) {
        fprintf(stderr, "Abrupt termination.\n"); 
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

void ctrlc(int senial) {
    finalizarprocesos(); 
    liberarsemaforos();
    printf ("\nFin del programa (Ctrol + C).\n\n Todos los barberos han terminado su jornada. La barberia esta cerrada.\n"); 
    exit(EXIT_SUCCESS);
}

void liberarsemaforos(){
  for(i= 0; i < NUM_BARBEROS; i++){
    destruir_sem(semBarberos[i]);
    printf("El barbero %d se marcha a casa.",i);
  }

  destruir_var()
}

int creasemaforos(){
  char[1024] barbero;
  for(i=0; i < NUM_BARBEROS; i++){
    fprintf(barbero,"barbero_[%d]",i);
    crear_sem(barbero, 1);

  }
}