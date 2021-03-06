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

char barberoAsignado[1024];
char fin [1024];
char pago [1024];
char barbero [1024];
char Sillones [1024];
char Sillas [1024];

int propina;
char transaccion [1024];
int transaccion1;
int pago_final;
int corte_base;

int main(int argc, char *argv[]){//idBarbero,CosteBase,AforoMaximo

    sprintf(barberoAsignado, "Barbero_[%d]", atoi(argv[1]));
    sprintf(pago, "Pago_[%d]", atoi(argv[1]));
    sprintf(fin, "Fin_[%d]", atoi(argv[1]));
    sprintf(transaccion, "Transaccion_[%d]", atoi(argv[1]));

    corte_base=atoi(argv[2]);
    Aforo_Max = atoi(argv[3]);

    srand(((int)getpid()));


    wait_sem(get_sem("mutexPuerta"));
    consultar_var(obtener_var("Aforo_Actual"), &Aforo_Actual);

    if (Aforo_Actual >= Aforo_Max){ //No hay hueco en la peluqueria
        printf("Aforo actual está lleno, el cliente %d se va de la peluqueria.\n", getpid());
        signal_sem(get_sem("mutexPuerta"));
        return EXIT_SUCCESS;
    }
    else{ //Hay hueco en la peluqueria
        Aforo_Actual++;
        modificar_var(obtener_var("Aforo_Actual"), Aforo_Actual);
        signal_sem(get_sem("mutexPuerta"));
        signal_sem(get_sem(barberoAsignado));

        wait_sem(get_sem("Sillas"));//Se sienta una persona que estaba de pie en una silla
            wait_sem(get_sem("Sillones"));//Se pasa una persona de la silla al sillon
                signal_sem(get_sem("Sillas"));//Se queda un hueco para sentarse.
                //Una vez se sienta en el sillon, el cliente espera a que termine su barbero y prepara el pago
                
                wait_sem(get_sem(fin));

                propina=rand()%6;
                pago_final=corte_base+propina;


                modificar_var(obtener_var(transaccion), pago_final);    
                signal_sem(get_sem(pago));
                signal_sem(get_sem("Sillones"));

                wait_sem(get_sem("mutexPuerta"));
                    consultar_var(obtener_var("Aforo_Actual"), &Aforo_Actual);
                    modificar_var(obtener_var("Aforo_Actual"), Aforo_Actual--);
                signal_sem(get_sem("mutexPuerta"));

    }
    return EXIT_SUCCESS;
}