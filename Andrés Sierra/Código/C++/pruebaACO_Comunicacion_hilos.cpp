
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

#include <iostream>
#include <cstdlib>

#include <cmath>
#include <limits>
#include <climits>

#include "ACO.h"

using namespace std;

#define ITERACIONES (int)75
#define NUMEROHORMIGAS (int)40
#define NUMERONODOS (int)100

// Le da más peso a la feromona en la probabilidad -
#define ALPHA (double)1
// Le da más peso al costo del link en la probabilidad -
#define BETA (double)0.1
// Constante positiva que regula el depósito de feromona -
#define Q (double)10
// Rate de evaporación de las feromonas (puede tomar valores entre 0 y 1)
#define RO (double)0.5
// Valor maximo de la feromona
#define TAUMAX (int)2
/*
// -------------- VALORES DE PRUEBA ----------------
// ------------- MEJORES RESULTADOS ----------------
# ALPHA (double)1
# BETA (double)0.1
# Q (double)10
# RO (double)0.5
# TAUMAX (int)2
*/

// Nodo inicial y final
#define NODOINICIAL (int)0
#define NODOFINAL (int)72


#define MSG_SIZE 60			// message size
#define IP_LENGTH 15
#define PUERTO 2050

// Variable de recibir
char buffer_recibir[MSG_SIZE];         // almacenar mensaje recibido
char *token;


// Recepcion de datos de otros robots
double recepcion[2];

int num_agente = 0;
double posicion_robot_X = 10.0;
double posicion_robot_Y = 10.0;
double rad = 10.0;

//------------------------------Comunicacion -------------------------------
void error(const char *msg){
	perror(msg);
	exit(0);
}
// ------------------------------------------------------------------------

void *receiving(void *ptr){
	int *sock, n, ret, ret2;
	int i = 0;
	sock = (int *)ptr;								  	// identificador del socket
	unsigned int length = sizeof(struct sockaddr_in); 					// tamaño de la estructura
	struct sockaddr_in from;
		
	while (1){
		memset(buffer_recibir, 0, MSG_SIZE); 						// "limpia" el buffer
		// recibir mensaje
		n = recvfrom(*sock, buffer_recibir, MSG_SIZE, 0, (struct sockaddr *)&from, &length);
		if (n < 0){
			error("Error: recvfrom");
		}
		
		i = 0;
		ret=strncmp(buffer_recibir,"G",2);
		ret=strncmp(buffer_recibir,"P",2);

		if(ret == 0){
			// descomponer buffer_recibir, strtok
			token = strtok(buffer_recibir, ",");
			recepcion[i] = atof(token);
			while ((token = strtok(NULL, ",")) != NULL){
				i++;
				recepcion[i] = atof(token);
			}
			printf("recepcion %f,%f, %f.\n",recepcion[1],recepcion[2],recepcion[3]);
			
		}
		else if (ret2==0){
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
		else {
			cout << "No entro" << endl;
		}	
	}
	pthread_exit(0);
}


// Main function
int main(int argc, char *argv[]){
	int sock, n;
	unsigned int length = sizeof(struct sockaddr_in); 		// tamaño de la estructura
	char buffer_enviar1[MSG_SIZE];							// almacenar mensaje
	struct sockaddr_in anybody;								// configuracion del socket
	int boolval = 1;					
	pthread_t thread_rec;				 					// variables para multihilos
	char IP_broadcast[IP_LENGTH];					 		// para la dirección de broadcast
	FILE *file;
	
	anybody.sin_family = AF_INET;				 			// simbolo constante para internet
	anybody.sin_port = htons(PUERTO);			 			// puerto seleccionado
	anybody.sin_addr.s_addr = htonl(INADDR_ANY); 		 	// para recibir de cualquiera

	sock = socket(AF_INET, SOCK_DGRAM, 0);			 		// creacion del socket
	if (sock < 0)
		error("Error: socket");

	if (bind(sock, (struct sockaddr *)&anybody, sizeof(struct sockaddr_in)) < 0){
		printf("Error binding socket.\n");
		exit(-1);
	}


	// Se creen los hilo de recepción.
	pthread_create(&thread_rec, NULL, receiving, (void *)&sock); 		
   		

	printf("Este programa despliega lo que sea que reciba.\n");
	printf("Agente %d conectado\n", num_agente);
	
	while (1)
	{
		/* PARA QUE NO SE CIERRE EL SOCKET*/
	}
	

	close(sock); 										// cerrar el socket.
    //------------------------------Comunicacion -------------------------------

	//ANTS->ImprimirGRAFO();
    return 0;  
}   