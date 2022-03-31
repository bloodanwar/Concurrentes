#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>


#include <semaforoI.h>
#include <memoriaI.h>


char barbero [1024];
char pago [1024];
char fin [1024];

int Caja;
int propina;
int dinero;
int pago_base;

void ctrlc(int senial);

int main(int argc, char *argv[]){//idBarbero,VelocidadCorte,PagoBase

    // Uno de los argumentos a recibir al crear el barbero tiene que ser su velocidad.
    // el barbero es el que tiene que hacer el deposito en la caja antes de dormirse.

    int velocidad_Base = atoi(argv[2]);
    int mi_velocidad=(atoi((argv[1]))+1)*velocidad_Base;

    sprintf(barbero,"Barbero_[%d]",atoi(argv[1]));
    sprintf(pago,"Pago[%d]",atoi(argv[1]));
    sprintf(fin,"Fin[%d]",atoi(argv[1]));
    

    while(1){
    wait_sem(get_sem(barbero));

        printf("Soy el barbero %s y empiezo a cortar el pelo.\n",barbero);
        sleep(mi_velocidad); //Esto es "Cortar el pelo"
        printf("Soy el barbero %s y he terminado de cortar el pelo. He tardado %d segundos\n",barbero,mi_velocidad);

        signal_sem(get_sem(fin));

            wait_sem(get_sem(pago));

                consultar_var(obtener_var(pago), &dinero);
                
                propina=dinero-pago_base;


                wait_sem(get_sem("Mutex_Caja"));
                    consultar_var(obtener_var("Caja"),&Caja);
                    modificar_var(obtener_var("Caja"),Caja+propina);
                signal_sem(get_sem("Mutex_Caja"));

}
    return EXIT_SUCCESS;
}
void ctrlc(int senial){
    printf("[Barbero %d] Finalizado(SIGINT)\n", getpid());
    exit(EXIT_SUCCESS);
}