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

#include <iostream>
#include <cstdlib>

#include <cmath>
#include <limits>
#include <climits>

#define MSG_SIZE 60			// message size
#define IP_LENGTH 15
#define PUERTO 2050

using namespace std;

char *token;

// Recepcion de datos de otros robots
double recepcion[2];

// Guardar las coordenadas del robot y del nodo
double posicion_robot_X = 0.0;
double posicion_robot_Y = 0.0;
double rad = 0.0;

int nodo = 0;
double nodo_cx = 0;
double nodo_cy = 0;

int nodo1 = 20;
int nodo2 = 20;


void error(const char *msg){
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, length, n;
    int i =0;
    int ret, ret2, ret3;

	socklen_t fromlen;
	struct sockaddr_in server;
	struct sockaddr_in from;
	char buffer_recibir[MSG_SIZE];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 				// Creacion del socket
	if(sockfd < 0)
		error("Opening socket");

	length = sizeof(server);						        // largo de la estructura
	memset((char *)&server, 0, length); 					// establece todos los valores en cero
	server.sin_family = AF_INET;						    // simbolo constante para internet
	server.sin_addr.s_addr = htonl(INADDR_ANY);				// para recibir de cualquier interfaz de red
	server.sin_port = htons(PUERTO);					    // Seleccion del puerto

	if(bind(sockfd, (struct sockaddr *)&server, length) < 0)
       error("binding");

	fromlen = sizeof(struct sockaddr_in);					// tamaño de la estructura

	while(1)
	{	
		memset(buffer_recibir, 0, MSG_SIZE);					    // limpia el buffer
		//  recepción  de informacion 
		n = recvfrom(sockfd, buffer_recibir, MSG_SIZE, 0, (struct sockaddr *)&from, &fromlen);
		if(n < 0)
	 		error("recvfrom");
        i = 0;
		ret=strncmp(buffer_recibir,"P",1);
		ret2=strncmp(buffer_recibir,"G",1);
		ret3=strncmp(buffer_recibir,"C",1);
        //cout <<"ret: " <<ret << endl;
        //cout <<"ret2: " <<ret2 << endl;
		printf("Mensaje recibido: %s\n", buffer_recibir);

		if(ret2 == 0){
			// descomponer buffer_recibir, strtok
			token = strtok(buffer_recibir, ",");
			recepcion[i] = atof(token);
			while ((token = strtok(NULL, ",")) != NULL){
				i++;
				recepcion[i] = atof(token);
			}
			nodo1 = recepcion[1];
			nodo2 = recepcion[2];
			printf("Nodos a conectar: %d, %d. \n\n",nodo1,nodo2);
			
		}
		else if (ret==0){
			// descomponer buffer_recibir, strtok
			token = strtok(buffer_recibir, ",");
			recepcion[i] = atof(token);
			while ((token = strtok(NULL, ",")) != NULL){
				i++;
				recepcion[i] = atof(token);
			}
			//printf("recepcion %f, %f, %f.\n",recepcion[1],recepcion[2],recepcion[3]);
			posicion_robot_X = recepcion[1];
            posicion_robot_Y = recepcion[2];
            rad = recepcion[3];

            printf("Coordenadas recibidas del robot: %f, %f, %f. \n\n", posicion_robot_X, posicion_robot_Y, rad);
		}
		else if (ret3==0){
			// descomponer buffer_recibir, strtok
			token = strtok(buffer_recibir, ",");
			recepcion[i] = atof(token);
			while ((token = strtok(NULL, ",")) != NULL){
				i++;
				recepcion[i] = atof(token);
			}
			//printf("recepcion %f, %f, %f.\n",recepcion[1],recepcion[2],recepcion[3]);
			nodo= recepcion[1];
            nodo_cx = recepcion[2];
            nodo_cy = recepcion[3];
            printf("Coordenadas recibidas del nodo %d: coordenada X: %f, coordenada Y: %f. \n\n", nodo, nodo_cx, nodo_cy);
		}
		else {
			cout << "No fue posible identificar el mensaje" << endl;
		}	

		// Se envia un mensaje de recibido luego de cada mensaje. 
		n = sendto(sockfd, "Got your message\n", MSG_SIZE, 0, (struct sockaddr *)&from, fromlen);
		if(n < 0)
        {
            error("sendto");
        }
	 		
		
	}
	return 0;
}
 
