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

char barberoAsignado;
char fin;
void ctrlc(int senial);
int main(int argc, char *argv[]){
   srand((int)getpid());
    sprintf(barberoAsignado, "Barbero_[%d]", atoi(argv[1]));
    sprintf(fin, "Fin_[%d]", atoi(argv[1]));

    Aforo_Max = atoi(argv[2]);
    coste_corte = atoi(argv[3]);


    /*

    wait(mutex);
        if num_clientes == N then
            signal(mutex);
            return;
        num_clientes++;
    signal(mutex)

    wait(sillón);
        wait(mutex);
            num_clientes--;
        signal(mutex);
        signal(barbero);


        wait(fin);
    signal(sillón);


    */

    wait_sem(get_sem("Mutex_Puerta"));
    consultar_var(obtener_var("Aforo_Actual"));

    if (Aforo_Actual >= Aforo_Max){ //No hay hueco en la peluqueria
        printf("Aforo actual está lleno, el cliente %d se va de la peluqueria.", getpid());
        signal_sem(get_sem("Mutex_Puerta"));
        return EXIT_SUCCESS;
    }
    else{ //Hay hueco en la peluqueria
        Aforo_Actual++;
        modificar_var(obtener_var("Aforo_Actual"), Aforo_Actual);

        wait_sem(get_sem("Contador_Sillas"));
            //Se sienta una persona que estaba de pie en una silla

            wait(get_sem("Sillones"));

            //Se pasa una persona de la silla al sillon
            signal_sem(get_sem("Contador_Sillas"));

                //Se queda un hueco para estar de pie
                wait_sem(get_sem("Mutex_Puerta"));
                consultar_var(obtener_var("Aforo_Actual"));
                modificar_var(obtener_var("Aforo_Actual"), Aforo_Actual--);
                signal_sem(get_sem("Mutex_Puerta"));

                //Una vez se sienta en el sillon, el cliente espera a que termine su barbero
                
                wait_sem(get_sem(fin))
                
                /*
                
                    wait(sillones)
                    for i in sillones{
                        if  barb= false;
                            signal barbero_[i]
                            wait (fin_corte pelo)
                            eros[i] == true {
                            barberos [i] 
                                            
                                    (modificar variable (pagar_barb_[i], pago)       
                                    pagar.exe)
                                barberos [i] = true;
                            }
                    }
                
                */

    }

    printf("Soy el cliente %d", getpid());

    return EXIT_SUCCESS;
}
void ctrlc(int senial){
    printf("[Cliente %d] Finalizado(SIGINT)\n", getpid());
    exit(EXIT_SUCCESS);
}