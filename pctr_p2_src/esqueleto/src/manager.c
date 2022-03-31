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
#define NUM_SILLONES 3
#define NUM_SILLAS 10
#define COSTE_CORTE 10

#define TIEMPO_CORTE_BASE "3" //Tiempo que se tarda en cortar el pelo. 
#define AFORO_MAX "20"




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
int creaRecursos();
void ctrlc(int);

int barberoAsignado;
int i;
char idBarb[1024];
char aforoMaximo[1024]=AFORO_MAX;
char asignadoBarbero [1024];
pid_t pids_clientes[NUM_CLIENTES];
pid_t pids_barberos[NUM_BARBEROS];  

int main(int argc, char *argv[]){

  srand(((int)getpid()));

  //Ejecucion de todos los procesos barberos.

  for(i=0; i<NUM_BARBEROS; ++i){
    switch(pids_barberos[i]=fork()){
      case 0: 
        sprintf(idBarb,"%d",i);
        execl("./barbero", "./barbero",idBarb,TIEMPO_CORTE_BASE, NULL);
        fprintf(stderr,"No se esta ejecutando el execl del barbero. \n");
        return EXIT_FAILURE;

      case -1:
        fprintf(stderr,"Error en la creacion del barbero.\n");
        return EXIT_FAILURE;
      
      default:
        continue;
    }

    for(i=0; i <NUM_CLIENTES; i++){
      switch (pids_clientes[i]=fork()){
      case 0:
        barberoAsignado=rand()%NUM_BARBEROS;
        sprintf(asignadoBarbero, "%d", barberoAsignado);
        execl("./cliente","./cliente", asignadoBarbero, AFORO_MAX,COSTE_CORTE,NULL);
        break;
      
      default:
        break;
      }
    }
  }
  

  if (creaRecursos()!=0){
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
    liberaRecursos();
    printf ("\nFin del programa (Ctrol + C).\n\n Todos los barberos han terminado su jornada. La barberia esta cerrada.\n"); 
    exit(EXIT_SUCCESS);
}


int creaRecursos(){
  //Crear todos los semaforos 
  char barbero [1024];
  char pago [1024];
  char fin [1024];

  for(i=0; i < NUM_BARBEROS; i++){
    sprintf(barbero,"Barbero_[%d]",i);
    sprintf(pago,"Pago[%d]",i);
    sprintf(fin,"Fin[%d]",i);
  
    crear_sem(barbero, 0);
    crear_sem(pago,1);
    crear_sem(fin,0);

    crear_var(pago,0);

  }
  crear_sem("Sillones",NUM_SILLONES);
  crear_sem("Sillas",NUM_SILLAS);
  
  crear_sem("Mutex_Caja",1);
  crear_sem("Mutex_Puerta",1);
  

  //Crear las variables

  crear_var("Aforo_Actual",0);
  return 0;

}

//Destruir semaforos
void liberaRecursos(){
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

  //matar clientes y matar barberos
}
void finalizarprocesos () {
 int i;
 printf ("-------------- Terminando ------------- \n");
 for (i = 0; i < NUM_CLIENTES; i++) {
  if (pids_clientes[i]) {
   printf ("Finalizando proceso [ %d]... ", pids_clientes[i]);
    kill(pids_clientes[i], SIGINT);
   printf ("<Ok>\n");
    }
 }
 for (i = 0; i < NUM_BARBEROS; i++) {
  if (pids_barberos[i]) {
   printf ("Finalizando proceso [ %d]... ", pids_barberos[i]);
    kill(pids_barberos[i], SIGINT);
   printf ("<Ok>\n");
    }
  }
 }