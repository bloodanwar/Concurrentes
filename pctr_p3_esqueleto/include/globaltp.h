#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_BUFFER 10
#define MAX_RESOLUTORES 5

#define NOMBRE_COLA_OPERACIONES "/OPERACIONES"
#define NOMBRE_COLA_SUMA "/SUMA"
#define NOMBRE_COLA_RESTA "/RESTA"
#define NOMBRE_COLA_PRODUCTO "/PRODUCTO"
#define NOMBRE_COLA_SUMATORIO "/SUMATORIO"

struct operandos{
int a;
int b;
};
