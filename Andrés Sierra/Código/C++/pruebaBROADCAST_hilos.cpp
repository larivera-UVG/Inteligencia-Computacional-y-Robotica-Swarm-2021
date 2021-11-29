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



#define MSG_SIZE 40			// message size
#define IP_LENGTH 15
#define PUERTO 2000

// Variable de recibir 
char buffer_recibir[MSG_SIZE];	// to store received messages or messages to be sent.
char *token;


// Coordenadas a enviar 
double coords[] = {2.0,2.0};

// Recepcion de datos de otros robots
double recepcion[2];

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

// Receiving thr: constantly waits for messages. Whatever is received is displayed.
void *receiving(void *ptr)
{
	int *sock, n;
    int ret, ret2;
    int i =0;
	sock = (int *)ptr;		// socket identifier
	unsigned int length = sizeof(struct sockaddr_in);		// size of structure
	struct sockaddr_in from;

	while(1)
	{
		memset(buffer_recibir, 0, MSG_SIZE);	// "limpia" el buffer
		// receive message
		n = recvfrom(*sock, buffer_recibir, MSG_SIZE, 0, (struct sockaddr *)&from, &length);
		if(n < 0){
			error("Error: recvfrom");
        }
        i=0;
        printf("Agente 1 recibe: %s \n ", buffer_recibir);
        // descomponer buffer_recibir, strtok
        token=strtok(buffer_recibir, ",");
        recepcion[i] = atof(token);

        while((token = strtok(NULL, ",")) !=NULL){
            i++;
            recepcion[i] = atof(token);
        }
        ret=strncmp(buffer_recibir,"1",1);
		ret2=strncmp(buffer_recibir,"2",1);
        if(ret == 0)
        {
            printf("Coordendas del agente 1\nCoordenada X: %f, Coordenada Y: %f \n\n ", recepcion[1], recepcion[2]);
        }
        if(ret2 == 0)
        {
            printf("Coordendas del agente 2\nCoordenada X: %f, Coordenada Y: %f \n\n ", recepcion[1], recepcion[2]);
        }
	}
	pthread_exit(0);
}

// Main function
int main(int argc, char *argv[]){
    
    int sock, n;
	unsigned int length = sizeof(struct sockaddr_in);	// size of structure
	char buffer_enviar[MSG_SIZE];		                // to store received messages or messages to be sent.
	struct sockaddr_in anybody;	                        // for the socket configuration
	int boolval = 1;			                        // for a socket option
	pthread_t thread_rec;		                        // thread variable
	char IP_broadcast[IP_LENGTH];                       // para la dirección de broadcast
    FILE *file;
    //strcpy(IP_broadcast, "10.0.0.255");	        // Puede que se deba cambiar. Revisar ifconfig
    strcpy(IP_broadcast, "192.168.0.255");	        // Puede que se deba cambiar. Revisar ifconfig

    printf("La dirección de broadcast es: %s\n\n", IP_broadcast);

	anybody.sin_family = AF_INET;		            // symbol constant for Internet domain
	anybody.sin_port = htons(PUERTO);		        // port field
	anybody.sin_addr.s_addr = htonl(INADDR_ANY);	// para recibir de cualquiera

	sock = socket(AF_INET, SOCK_DGRAM, 0);          // Creates socket. Connectionless.
	if(sock < 0)
		error("Error: socket");

	// Sin el bind, no se reciben los mensajes
	if(bind(sock, (struct sockaddr *)&anybody, sizeof(struct sockaddr_in)) < 0)
	{
		printf("Error binding socket.\n");
		exit(-1);
	}

	// change socket permissions to allow broadcast
	if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &boolval, sizeof(boolval)) < 0)
	{
		printf("Error setting socket options.\n");
		exit(-1);
	}

	anybody.sin_addr.s_addr = inet_addr(IP_broadcast);	                // broadcast address

	// Hilo de recepción 
	pthread_create(&thread_rec, NULL, receiving, (void *)&sock);	    // for receiving

	//printf("Este programa despliega lo que sea que reciba.\n");
	//printf("También transmite lo que el usuario ingrese, max. 40 caracteres. (! para salir):\n");
	do
	{
		memset(buffer_enviar, 0, MSG_SIZE);	                            // "limpia" el buffer
		//fgets(buffer_enviar,MSG_SIZE-1,stdin); // MSG_SIZE-1 'cause a null character is added
        sleep(5);
        sprintf(buffer_enviar, "1, %f,%f",coords[0],coords[1]);
		if(buffer_enviar[0] != '!')
		{
			// send message to anyone there...
			n = sendto(sock, buffer_enviar, strlen(buffer_enviar), 0,
					(const struct sockaddr *)&anybody, length);
			if(n < 0)
				error("Error: sendto");
		}
	} while(buffer_enviar[0] != '!');
  
    close(sock);			// close socket.
    return 0;  
}   