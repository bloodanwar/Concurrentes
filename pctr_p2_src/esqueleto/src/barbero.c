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
char fin [1024];
char Mutex_Caja [1024];
char transaccion [1024];
char pago [1024];
char propina [1024];

int Caja;
int propina_actual;
int dinero;
int pago_base;
int pago_final;
int propina_acumulada;

int main(int argc, char *argv[]){//idBarbero,PagoBase,VelocidadCorte
    // Uno de los argumentos a recibir al crear el barbero tiene que ser su velocidad.
    // el barbero es el que tiene que hacer el deposito en la caja antes de dormirse.

    int velocidad_Base = atoi(argv[3]);
    int mi_velocidad=(atoi((argv[1]))+1)*velocidad_Base;
    pago_base=atoi(argv[2]);

    sprintf(barbero,"Barbero_[%d]",(atoi(argv[1])));
    sprintf(transaccion,"Transaccion_[%d]",(atoi(argv[1])));  
    sprintf(fin, "Fin_[%d]", atoi(argv[1])); 
    sprintf(pago, "Pago_[%d]", atoi(argv[1]));
    sprintf(propina,"Propina_[%d]",atoi(argv[1]));

    while(1){
    wait_sem(get_sem(barbero));
        printf("%s empieza a cortar el pelo.\n",barbero);
        sleep(mi_velocidad); //Esto es "Cortar el pelo"
        printf("%s termina de cortar el pelo. Ha tardado %d segundos\n",barbero,mi_velocidad);

        signal_sem(get_sem(fin));
            wait_sem(get_sem(pago));
                consultar_var(obtener_var(transaccion), &pago_final);
                pago_base=atoi(argv[2]);
                propina_actual=pago_final-pago_base;
                
                consultar_var(obtener_var(propina),&propina_acumulada);
                modificar_var(obtener_var(propina),propina_acumulada+propina_actual);

                wait_sem(get_sem("Mutex_Caja"));
                    consultar_var(obtener_var("Caja"), &Caja);
                    modificar_var(obtener_var("Caja"), pago_base+Caja);
                signal_sem(get_sem("Mutex_Caja"));

}
    return EXIT_SUCCESS;
}