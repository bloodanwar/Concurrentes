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

int main(int argc, char *argv[]){

    // Uno de los argumentos a recibir al crear el barbero tiene que ser su velocidad.
    // el barbero es el que tiene que hacer el deposito en la caja antes de dormirse.

    int idbarbero = atoi(argv[1]);
    int velocidad_Base = atoi(argv[2]);
    int mi_velocidad=(idbarbero+1)*velocidad_Base;

    sprintf(barbero,"barbero_[%d]",idbarbero);

    wait_sem(get_sem(barbero));

    printf("Soy el barbero %d y empiezo a cortar el pelo.\n",idbarbero);
    sleep(mi_velocidad); //Esto es "Cortar el pelo"
    printf("Soy el barbero %d y he terminado de cortar el pelo. He tardado %d segundos\n",idbarbero,mi_velocidad);

    /*
    
    signal (fin corte pelo)
    
    */


    return EXIT_SUCCESS;
}