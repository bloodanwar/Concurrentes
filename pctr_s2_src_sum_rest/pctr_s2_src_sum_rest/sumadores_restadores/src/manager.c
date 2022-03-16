#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define ARCHIVO "./data/test.txt"
#define VALOR_INICIAL 7
#define P_SUMADORES 3
#define P_RESTADORES 3
#define NUM_OPERACIONES 10 /* Maximo 999 */

void esperar(int senhal);
void controlador(int senhal);

int main(int argc, char *argv[])
{
  FILE *fp;
  int pid_hijo;
  int aux, i;
  char *pNumOp;

  /* Apertura del archivo */
  if ((fp = fopen(ARCHIVO, "w")) == NULL)
  {
    fprintf(stderr, "Error de apertura del archivo %s\n", ARCHIVO);
    exit(EXIT_FAILURE);
  }

  /* Escritura del valor inicial en el archivo... */
  fprintf(fp, "%d", VALOR_INICIAL);
  fclose(fp);

  printf("Valor inicial: %d\n", VALOR_INICIAL);
  aux = VALOR_INICIAL + (P_SUMADORES - P_RESTADORES) * NUM_OPERACIONES;
  printf("Valor esperado: %d\n", aux);

  /* Conversión de NUM_OPERACIONES a char*... */
  pNumOp = malloc(sizeof(int));
  sprintf(pNumOp, "%d", NUM_OPERACIONES);

  /* Se espera la terminación de los procesos... */
  if (signal(SIGCHLD, esperar) == SIG_ERR)
  {
    fprintf(stderr, "Error al instalar manejador.\n");
    exit(EXIT_FAILURE);
  }

  /* Manejo de Ctrol+C */
  if (signal(SIGINT, controlador) == SIG_ERR)
  {
    fprintf(stderr, "Error al instalar manejador.\n");
    exit(EXIT_FAILURE);
  }

  /* Se lanzan los sumadores... */
  for (i = 0; i < P_SUMADORES; i++)
    switch (pid_hijo = fork())
    {
    case 0:
      /* Cada sumador recibe el nombre del archivo
   y el número de incrementos a realizar */
      if (execl("./exec/sumador", "sumador", ARCHIVO, pNumOp, NULL) == -1)
      {
        fprintf(stderr, "Error al usar execl().\n");
        exit(EXIT_FAILURE);
      }
      break;
    }

  /* Se lanzan los restadores... */
  for (i = 0; i < P_RESTADORES; i++)
    switch (pid_hijo = fork())
    {
    case 0:
      /* Cada restador recibe el nombre del archivo
   y el número de decrementos a realizar */
      if (execl("./exec/restador", "restador", ARCHIVO, pNumOp, NULL))
      {
        fprintf(stderr, "Error al usar execl().\n");
        exit(EXIT_FAILURE);
      }
      break;
    }

  /* A la espera de señales... */
  while (1)
    ;

  return 0;
}

void esperar(int senhal)
{
  pid_t pid = wait(NULL);
  printf("[MANAGER]:\tEl hijo [%d] ha acabado \n", pid);
}

void controlador(int senhal)
{
  printf("\nCtrl+c capturada.\n");
  printf("Finalizando...\n\n");

  /* Liberación de recursos... */

  /* Se muestra el contenido real del archivo... */
  int valor;
  FILE *fp = fopen(ARCHIVO, "r");
  fscanf(fp, "%d", &valor);
  fclose(fp);
  printf("Valor real: %d\n", valor);

  sleep(1);
  printf("<OK>\n");
  /* Salida del programa */
  exit(EXIT_SUCCESS);
}
