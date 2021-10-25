#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h>

#define MSG_SIZE 40 // message size
#define IP_LENGTH 15
#define PUERTO 2001

// Variable de recibir
char buffer_recibir[MSG_SIZE]; // to store received messages or messages to be sent.
char *token;

// Posicion Local de prueba MODIFICAR
static double best_local[] = {1.0, 1.0};
static double fitness_local = 10.0;

// Recepcion de datos de otros robots
static double recepcion[2];

// Posicion Global
static double best_global[] = {1.0, 1.0};
static double fitness_global = 10.0;
int num_agente = 0;
static double posicion_robot_X = 10.0;
static double posicion_robot_Y = 10.0;
static double rad = 10.0;
//------------------------------Comunicacion -------------------------------
void error(const char *msg)
{
	perror(msg);
	exit(0);
}

// Receiving thr: constantly waits for messages. Whatever is received is displayed.
void *receiving(void *ptr)
{
	int *sock, n, ret;
	int i = 0;
	int j = 0;
	sock = (int *)ptr;								  // socket identifier
	unsigned int length = sizeof(struct sockaddr_in); // size of structure
	struct sockaddr_in from;

	while (1)
	{
		memset(buffer_recibir, 0, MSG_SIZE); // "limpia" el buffer
		// receive message
		n = recvfrom(*sock, buffer_recibir, MSG_SIZE, 0, (struct sockaddr *)&from, &length);
		if (n < 0){
			error("Error: recvfrom");
		}
		i = 0;
		j = 0;
		ret=strncmp(buffer_recibir, "T2" ,2);

		if(ret != 0){
			// descomponer buffer_recibir, strtok
			token = strtok(buffer_recibir, ",");
			recepcion[i] = atof(token);
			while ((token = strtok(NULL, ",")) != NULL){
				i++;
				recepcion[i] = atof(token);
			}
			printf("recepcion %f,%f, %f.\n",recepcion[1],recepcion[2],recepcion[3]);
			// Actualizar global best
			if (recepcion[3] <= fitness_global){ //Local< Global, asigna el valor local al global
				best_global[0] = recepcion[1];
				best_global[1] = recepcion[2];
				fitness_global = recepcion[3];
				fitness_local = recepcion[3];
				printf("Agente %d actualiza su Best Global a %f.\n", num_agente, fitness_global);
			}else{
				printf("Agente %d no actualiza su Best Global %f.\n", num_agente, fitness_global);
			}

		}else{
			// descomponer buffer_recibir, strtok
			token = strtok(buffer_recibir, ",");
			recepcion[i] = atof(token);
			while ((token = strtok(NULL, ",")) != NULL){
				i++;
				recepcion[i] = atof(token);
			}
			printf("recepcion %f, %f, %f.\n",recepcion[1],recepcion[2],recepcion[3]);
			posicion_robot_X = recepcion[1];
            posicion_robot_Y = recepcion[2];
            rad = recepcion[3];
            printf("Coordenadas recibidas del agente %d son: %f, %f, %f.\n", num_agente, posicion_robot_X, posicion_robot_Y, rad);
		}	
	}
	pthread_exit(0);
}
//------------------------------------------------------------------------------------------

// Main function
int main(int argc, char *argv[])
{
	//------------------------------Comunicacion -------------------------------

	int sock, n;
	unsigned int length = sizeof(struct sockaddr_in); // size of structure
	char buffer_enviar1[MSG_SIZE], buffer_enviar2[MSG_SIZE];					  // to store received messages or messages to be sent.
	struct sockaddr_in anybody;						  // for the socket configuration
	int boolval = 1;								  // for a socket option
	pthread_t thread_rec;							  // thread variable
	char IP_broadcast[IP_LENGTH];					  // para la dirección de broadcast
	FILE *file;
	//strcpy(IP_broadcast, "10.0.0.255"); 		// Puede que se deba cambiar. Revisar ifconfig
	strcpy(IP_broadcast, "192.168.0.255");

	printf("La dirección de broadcast es: %s\n\n", IP_broadcast);

	if (argc == 7){
		fitness_local = atof(argv[1]);
		fitness_global = atof(argv[2]);
		num_agente = atoi(argv[3]);
		posicion_robot_X = atof(argv[4]);
		posicion_robot_Y = atof(argv[5]);
		rad = atof(argv[6]);
	}
	anybody.sin_family = AF_INET;				 // symbol constant for Internet domain
	anybody.sin_port = htons(PUERTO);			 // port field
	anybody.sin_addr.s_addr = htonl(INADDR_ANY); // para recibir de cualquiera

	sock = socket(AF_INET, SOCK_DGRAM, 0); // Creates socket. Connectionless.
	if (sock < 0)
		error("Error: socket");

	// Sin el bind, no se reciben los mensajes
	if (bind(sock, (struct sockaddr *)&anybody, sizeof(struct sockaddr_in)) < 0){
		printf("Error binding socket.\n");
		exit(-1);
	}

	// change socket permissions to allow broadcast
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &boolval, sizeof(boolval)) < 0){
		printf("Error setting socket options.\n");
		exit(-1);
	}

	anybody.sin_addr.s_addr = inet_addr(IP_broadcast); // broadcast address

	// Se creae el hilo de recepción.
	pthread_create(&thread_rec, NULL, receiving, (void *)&sock); // for receiving

	printf("Este programa despliega lo que sea que reciba.\n");
	printf("Agente %d conectado\n", num_agente);
	do
	{
		memset(buffer_enviar1, 0, MSG_SIZE); // "limpia" el buffer
		memset(buffer_enviar2, 0, MSG_SIZE); // "limpia" el buffer
											//fgets(buffer_enviar,MSG_SIZE-1,stdin); // MSG_SIZE-1 'cause a null character is added
		sleep(5);

		// Valores a enviar
		sprintf(buffer_enviar1, "T1,%f,%f,%f", best_local[0], best_local[1], fitness_local);
		sprintf(buffer_enviar2, "T2,%f,%f,%f", posicion_robot_X, posicion_robot_Y, rad);

		if (buffer_enviar1[0] != '!')
		{
			// send message to anyone there...
			n = sendto(sock, buffer_enviar1, strlen(buffer_enviar1), 0,
					   (const struct sockaddr *)&anybody, length);
			if (n < 0)
				error("Error: sendto");
		}
		if (buffer_enviar2[0] != '!')
		{
			// send message to anyone there...
			n = sendto(sock, buffer_enviar2, strlen(buffer_enviar2), 0,
					   (const struct sockaddr *)&anybody, length);
			if (n < 0)
				error("Error: sendto");
		}
	} while ((buffer_enviar1[0] || buffer_enviar2[0]) != '!');

	close(sock); // close socket.
	return 0;
}
