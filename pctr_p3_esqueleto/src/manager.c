#include "globaltp.h"

void Controlador(int senhal);
void LiberaRecursos(void);
void FinalizarProcesos(void);
int ParsearEnviar(char *argumento);
void CreacionColas(void);

mqd_t qHandler_operaciones;

pid_t vpids[MAX_RESOLUTORES + 1];

int main(int argc, char *argv[])
{
	int register i;

	int resolutores = atoi(argv[2]); // Numero de procesos para la realización de operaciones
	int numero_operaciones;			 //valor devuelto tras analizar el primer argumento de linea de comandos

	memset(vpids, 0, sizeof(pid_t) * MAX_RESOLUTORES + 1);

	if (resolutores > MAX_RESOLUTORES)
	{

		printf("El número de resolutores excede el máximo");
		exit(1);
	}

	// Manejo de Ctrol+C.
	if (signal(SIGINT, Controlador) == SIG_ERR)
	{
		fprintf(stderr, "Terminación incorrecta.\n");
		exit(1);
	}

	CreacionColas();

	for (i = 0; i < resolutores; i++)
		if ((vpids[i] = fork()) == 0)
		{

			execl("./exec/resolutor", "resolutor", NULL);
		}

	numero_operaciones = ParsearEnviar(argv[1]);

	/*AQUI HAY QUE LANZAR EL PROCESO SUMATORIO Y MANDARLE LOS ARGUMENTOS NECESARIOS EN SU CASO */

	/*EL PROCESO MANAGER QUEDA A LA ESPERA DE QUE EL PROCESO SUMATORIO LE INDIQUE QUE TODAS LAS OPERACIONES HAN SIDO REALIZADAS */

	/*
	//En espera de los procesos hijo	

	for (; i>=resolutores+1; i--) waitpid(vpids[i], 0, 0);
	FinalizarProcesos();
	LiberaRecursos();*/

	//ESTE WHILE SE SUBSTITUIRA POR EL CODIGO QUE ESTA COMENTADO ANTERIORMENTE RELATIVO AL CONTROL DE SEÑALES. ASI NO SE TERMINARA OBLIGATORIAMENTE CON CTRL+C
	while (1)
		pause();

	return 0;
}

void CreacionColas()
{
	struct mq_attr mqAttr_operaciones;
	mqAttr_operaciones.mq_maxmsg = MAX_BUFFER;
	mqAttr_operaciones.mq_msgsize = sizeof(char);

	qHandler_operaciones = mq_open(NOMBRE_COLA_OPERACIONES, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR, &mqAttr_operaciones);

	if (qHandler_operaciones == -1)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	//SE AÑADIRA EL CODIGO RELACIONADO CON LA CREACION DE TODAS LAS COLAS QUE SEAN NECESARIAS
}

int ParsearEnviar(char *argumento)
{
	//DEBERA AÑADIRSE TODO EL CODIGO ASOCIADO AL ENVIO DE LOS OPERANDOS Y OPERADORES A LAS DISTINTAS COLAS DE MENSAJES

	char *cad;			//auxiliar para extraer subcadenas de argumento
	int register j = 1; //indice para discriminar entre operando1, operador y operando2

	printf("operando 1: %i\n", atoi(strtok(argumento, ".")));

	while ((cad = strtok(NULL, ".")) != NULL)
	{

		if (j % 3 == 0)
		{
			printf("operando 1: %s\n", cad);
		}
		else if (j % 3 == 1)
		{

			printf("operador %c\n", cad[0]);
		}
		else if (j % 3 == 2)
		{
			printf("operando 2: %s\n", cad);
		}
		j++;
	} // while cad

	return j / 3; //devuelve el numero de operaciones
}

void LiberaRecursos(void)
{
	//HABRA QUE AÑADIR TODAS LAS COLAS UTILIZADAS

	mq_close(qHandler_operaciones);
	mq_unlink(NOMBRE_COLA_OPERACIONES);
}

void FinalizarProcesos(void)
{
	//DEBERA AÑADIRSE TODO EL CODIGO ASOCIADO A LA FINALIZACION DE LOS PROCESOS HIJOS
}

void Controlador(int senhal)
{
	FinalizarProcesos();
	LiberaRecursos();

	exit(0);
}
