#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>






/* TODO: 


    El barbero esta dormido
    el barbero recibe una senial
    el barbero despierta
    el barbero corta el pelo
    El barbero agradece la propina (D es precio de corte, si recibimos D+P de pago, le restamos D y tenemos un P que imprimir por pantalla como propina.)
    el barbero se duerme
    back to the top
    while(1){
        wait(barbero);
        cortar();
        wait(caja);
        operaciones//variables;
        signal(caja);
        signal(fin);
    }

*/

char[1024] barbero;
char[1024] pago;
char[1024] fin;
void ctrlc(int senial);

int main(int argc, char *argv[]){

    srand((int)getpid());
    // Uno de los argumentos a recibir al crear el barbero tiene que ser su velocidad.
    // el barbero es el que tiene que hacer el deposito en la caja antes de dormirse.

    int velocidad_Base = atoi(argv[2]);
    int mi_velocidad=(idbarbero+1)*velocidad_Base;

    sprintf(barbero,"Barbero_[%d]",atoi(argv[1]));
    sprintf(pago,"Pago[%d]",atoi(argv[1]));
    sprintf(pago,"Fin[%d]",atoi(argv[1]));


    while(1){
        
    wait_sem(get_sem(barbero));

    printf("Soy el barbero %d y empiezo a cortar el pelo.\n",idbarbero);
    sleep(mi_velocidad); //Esto es "Cortar el pelo"
    printf("Soy el barbero %d y he terminado de cortar el pelo. He tardado %d segundos\n",idbarbero,mi_velocidad);

    signal_sem(get_sem(fin));

    wait_sem(get_sem(pago))

    wait_sem(get_sem("Mutex_Caja"));

    consultar_var(obtener_var("Aforo_Actual"));

}
    return EXIT_SUCCESS;
}
void ctrlc(int senial){
    printf("[Barbero %d] Finalizado(SIGINT)\n", getpid);
    exit(EXIT_SUCCESS);
}