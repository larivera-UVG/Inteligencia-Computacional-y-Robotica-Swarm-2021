#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <ctype.h>


#define MSG_SIZE 40 			// tamaño del mensaje
#define IP_LENGTH 15
#define PUERTO1 2001			// puerto para enviar informacion entre agentes (UDP broadcast)
#define PUERTO2 2010			// puerto para enviar la pose de los agentes (UDP)


// Variable de recibir
char buffer_recibir[MSG_SIZE]; 		// almacenar mensaje recibido
char *token;
char *token2;

// Posicion Local de prueba MODIFICAR
static double best_local[] = {1.0, 1.0};
static double fitness_local = 10.0;

// Recepcion de datos de otros robots
static double recepcion[2];
static double recepcion2[2];

// Posicion Global
static double best_global[] = {1.0, 1.0};
static double fitness_global = 10.0;
int num_agente = 0;
static double posicion_robot_X = 10.0;
static double posicion_robot_Y = 10.0;
static double rad = 10.0;

void error(const char *msg){
	perror(msg);
	exit(0);
}

//----------  Recepcion Broadcast (comunicacion agentes) -------------------------
void *receiving(void *ptr){
	int *sock, n, ret;
	int i = 0;
	sock = (int *)ptr;								  // identificador del socket
	unsigned int length = sizeof(struct sockaddr_in); 				  // tamaño de la estructura
	struct sockaddr_in from;

	while (1){
		memset(buffer_recibir, 0, MSG_SIZE); 					// "limpia" el buffer
		// recibir mensaje
		n = recvfrom(*sock, buffer_recibir, MSG_SIZE, 0, (struct sockaddr *)&from, &length);
		if (n < 0){
			error("Error: recvfrom");
		}
		i = 0;

		// descomponer buffer_recibir, strtok
		token = strtok(buffer_recibir, ",");
		recepcion[i] = atof(token);
		while ((token = strtok(NULL, ",")) != NULL){
			i++;
			recepcion[i] = atof(token);
		}
		// Actualizar global best
		if (recepcion[3] <= fitness_global){ 			//Local< Global, asigna el valor local al global
		    best_global[0] = recepcion[1];
		    best_global[1] = recepcion[2];
		    fitness_global = recepcion[3];
		    fitness_local = recepcion[3];
			printf("Agente %d actualiza su Best Global a %f.\n", num_agente, fitness_global);
		}else{
			printf("Agente %d no actualiza su Best Global %f.\n", num_agente, fitness_global);
		}
	}
	pthread_exit(0);
}
//---------------------------------------------------------------------------

//------------ Recepcion Matlab (Vision por computadora) ------------------
void *receiving2(void *ptr){
    int sockfd, length, n;
    int j = 0;
	socklen_t fromlen;
	struct sockaddr_in server;
	struct sockaddr_in from;
	char buffer[MSG_SIZE];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 		// Creacion del socket
	if(sockfd < 0)
		error("Opening socket");

	length = sizeof(server);				// length of structure
	memset((char *)&server, 0, length); 			// establece todos los valores en cero
	server.sin_family = AF_INET;				// simbolo constante para internet
	server.sin_addr.s_addr = htonl(INADDR_ANY);		// para recibir de cualquier interfaz de red
	server.sin_port = htons(PUERTO2);			// Seleccion del puerto

	if(bind(sockfd, (struct sockaddr *)&server, length) < 0)
       error("binding");

	fromlen = sizeof(struct sockaddr_in);			// tamaño de la estructura

	while(1){
		memset(buffer, 0, MSG_SIZE);			// limpia el buffer
		//  recepción  de informacion
		n = recvfrom(sockfd, buffer, MSG_SIZE, 0, (struct sockaddr *)&from, &fromlen);
		if(n < 0)
            		error("recvfrom");
       		 j = 0;
		// descomponer buffer_recibir, strtok
		token2 = strtok(buffer, ",");
        	recepcion2[j] = atof(token2);
		while ((token2 = strtok(NULL, ",")) != NULL){
			j++;
			recepcion2[j] = atof(token2);
		}
        	posicion_robot_X = recepcion2[1];
        	posicion_robot_Y = recepcion2[2];
        	rad = recepcion2[3];
        	printf("Coordenadas de Vision por Computadora del agente %d son: %f, %f, %f.\n", num_agente, posicion_robot_X, posicion_robot_Y, rad);
    }
    pthread_exit(0);
}

// ----------------------------- MAIN -----------------------------------------
// Main function
int main(int argc, char *argv[]){
	//------------------------------Comunicacion -------------------------------
	int sock, n;
	unsigned int length = sizeof(struct sockaddr_in); 		// tamaño de la estructura
	char buffer_enviar1[MSG_SIZE];					// almacenar mensaje
	struct sockaddr_in anybody;					// configuracion del socket
	int boolval = 1;					
	pthread_t thread_rec, thread_rec2;				 // variables para multihilos
	char IP_broadcast[IP_LENGTH];					 // para la dirección de broadcast
	FILE *file;
	strcpy(IP_broadcast, "10.0.0.255"); 				// Puede que se deba cambiar. Revisar ifconfig
	//strcpy(IP_broadcast, "192.168.1.255");

	printf("La dirección de broadcast es: %s\n\n", IP_broadcast);

	if (argc == 4){
		fitness_local = atof(argv[1]);
		fitness_global = atof(argv[2]);
		num_agente = atoi(argv[3]);
	}
	anybody.sin_family = AF_INET;				 // simbolo constante para internet
	anybody.sin_port = htons(PUERTO1);			 // puerto seleccionado
	anybody.sin_addr.s_addr = htonl(INADDR_ANY); 		 // para recibir de cualquiera

	sock = socket(AF_INET, SOCK_DGRAM, 0);			 // creacion del socket
	if (sock < 0)
		error("Error: socket");

	if (bind(sock, (struct sockaddr *)&anybody, sizeof(struct sockaddr_in)) < 0){
		printf("Error binding socket.\n");
		exit(-1);
	}

	// configuracion para permitir broadcast
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &boolval, sizeof(boolval)) < 0){
		printf("Error setting socket options.\n");
		exit(-1);
	}

	anybody.sin_addr.s_addr = inet_addr(IP_broadcast); 		// direccion del broadcast 
	
	// Se creen los hilo de recepción.
	pthread_create(&thread_rec, NULL, receiving, (void *)&sock); 		
   	pthread_create(&thread_rec2, NULL, receiving2, (void *)&sock); 		

	printf("Este programa despliega lo que sea que reciba.\n");
	printf("Agente %d conectado\n", num_agente);
	do
	{
		memset(buffer_enviar1, 0, MSG_SIZE); 			// "limpia" el buffer
										
		sleep(5);

		// Valores a enviar
		sprintf(buffer_enviar1, "T1,%f,%f,%f", best_local[0], best_local[1], fitness_local);
		if (buffer_enviar1[0] != '!'){
			// enviar mensaje
			n = sendto(sock, buffer_enviar1, strlen(buffer_enviar1), 0,
					   (const struct sockaddr *)&anybody, length);
			if (n < 0)
				error("Error: sendto");
		}
	} while (buffer_enviar1[0]  != '!');

	close(sock); 			// cerrar el socket.
	return 0;
}
