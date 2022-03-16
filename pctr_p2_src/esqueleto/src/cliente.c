#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char *argv[]){
    
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


    printf("Soy el cliente %d",getpid());

    return EXIT_SUCCESS;
}