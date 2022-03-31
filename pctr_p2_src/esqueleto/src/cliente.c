#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include <semaforoI.h>
#include <memoriaI.h>

int Aforo_Actual;
int Aforo_Max;
int coste_corte;

char barberoAsignado[1024];
char fin [1024];
char pago [1024];

void ctrlc(int senial);
int main(int argc, char *argv[]){//idBarbero,CosteBase,AforoMaximo

    sprintf(barberoAsignado, "Barbero_[%d]", atoi(argv[1]));
    sprintf(fin, "Fin_[%d]", atoi(argv[1]));
    sprintf(pago,"Pago[%d]", atoi(argv[1]));

    Aforo_Max = atoi(argv[2]);
    coste_corte = atoi(argv[3]);

    srand((int)getpid());


    wait_sem(get_sem("Mutex_Puerta"));
    consultar_var(obtener_var("Aforo_Actual"), &Aforo_Actual);

    if (Aforo_Actual >= Aforo_Max){ //No hay hueco en la peluqueria
        printf("Aforo actual está lleno, el cliente %d se va de la peluqueria.", getpid());
        signal_sem(get_sem("Mutex_Puerta"));
        return EXIT_SUCCESS;
    }
    else{ //Hay hueco en la peluqueria
        Aforo_Actual++;
        modificar_var(obtener_var("Aforo_Actual"), Aforo_Actual);
        signal_sem(get_sem("Mutex_Puerta"));

        wait_sem(get_sem("Contador_Sillas"));//Se sienta una persona que estaba de pie en una silla
            wait_sem(get_sem("Sillones"));//Se pasa una persona de la silla al sillon
                signal_sem(get_sem("Contador_Sillas"));//Se queda un hueco para sentarse.
                //Una vez se sienta en el sillon, el cliente espera a que termine su barbero y prepara el pago
                
                /*
                    Pagobarbero[i] = coste_corte+rand()%coste_corte // esto establece el pago que se le va a hacer al barbero con la propina incluido.                
                */

                wait_sem(get_sem(fin));

                signal_sem(get_sem(pago));
                signal_sem(get_sem("Sillones"));

                wait_sem(get_sem("Mutex_Puerta"));
                    consultar_var(obtener_var("Aforo_Actual"), &Aforo_Actual);
                    modificar_var(obtener_var("Aforo_Actual",Aforo_Actual));
                signal_sem(get_sem("Mutex_Puerta"));

    }
    return EXIT_SUCCESS;
}
void ctrlc(int senial){
    printf("[Cliente %d] Finalizado(SIGINT)\n", getpid());
    exit(EXIT_SUCCESS);
}