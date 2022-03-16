#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void restador(const char *pFileName, const char *pNumOP);

int main(int argc, char *argv[])
{
  restador(argv[1], argv[2]);

  return 0;
}

void restador(const char *pFileName, const char *pNumOP)
{
  FILE *fp;
  int i, valor, operaciones;

  printf("[%d]:\t decrementando...\n", getpid());
  operaciones = atoi(pNumOP);

  for (i = 0; i < operaciones; i++)
  {
    /* Lectura */
    if ((fp = fopen(pFileName, "r")) == NULL)
    {
      fprintf(stderr, "Error de apertura del archivo %s\n", pFileName);
      exit(EXIT_FAILURE);
    }
    fscanf(fp, "%d", &valor);
    fclose(fp);

    /* Actualización */
    valor--;

    /* Escritura */
    if ((fp = fopen(pFileName, "w")) == NULL)
    {
      fprintf(stderr, "Error de apertura del archivo %s\n", pFileName);
      exit(EXIT_FAILURE);
    }
    fprintf(fp, "%d", valor);
    fclose(fp);

    printf("[%d]:\t %d\n", getpid(), valor);
  }
}
