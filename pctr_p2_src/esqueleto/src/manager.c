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

void liberarsemaforos(); 
void finalizarprocesos();
void creasemaforos();
void ctrlc(int);

int i;

int main(int argc, char *argv[]){

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
    printf ("\nFin del programa (Ctrol + C).\n\n Los barberos terminan su jornada y se van a casa.\n"); 
    exit(EXIT_SUCCESS);
}

void liberarsemaforos(){
  for(i= 0; i < NUM_BARBEROS; i++)
}

int creasemaforos(){

}