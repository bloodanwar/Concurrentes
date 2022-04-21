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
