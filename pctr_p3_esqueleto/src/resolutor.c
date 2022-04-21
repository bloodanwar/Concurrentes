#include "globaltp.h"

void resolutor();

int main(int argc, char *argv[])
{
	resolutor();

	return 0;
}

void resolutor()
{
	pid_t pid = getpid(); //Para sacar mensaje informativo

	//Variables necesarias para la recepcion del mensaje de la cola operaciones
	mqd_t qHandler_operaciones;
	int rc;
	char operador;
	unsigned prioridad;

	//apertura en modo solo lectura
	qHandler_operaciones = mq_open(NOMBRE_COLA_OPERACIONES, O_RDONLY);

	if (qHandler_operaciones == -1)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	while (1)
	{

		//LOS PROCESOS RESOLUTORES QUEDAN BLOQUEADOS

		rc = mq_receive(qHandler_operaciones, (char *)&operador, sizeof(char), &prioridad);

		if (rc == -1)
			fprintf(stderr, "%s\n", strerror(errno));

		else
			printf("[%d]: Cliente Recibiendo mensaje: %c\n", pid, operador);

		/* INSERTAR CODIGO AQUI: UNA VEZ RECIBIDO EL OPERADOR HABRA QUE GESTIONAR LA RECEPCION DE LOS OPERANDOS, LA RESOLUCION DE LA OPERACION Y EL ENVIO DEL RESULTADO AL PROCESO SUMATORIO */
	}
}
