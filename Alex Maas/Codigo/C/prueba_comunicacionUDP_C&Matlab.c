#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MSG_SIZE 40			// tamaño del mensaje
#define PUERTO 2010			// Puerto seleccionado		

void error(const char *msg){
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
	int sockfd, length, n;
	socklen_t fromlen;
	struct sockaddr_in server;
	struct sockaddr_in from;
	char buffer[MSG_SIZE];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 				// Creacion del socket
	if(sockfd < 0)
		error("Opening socket");

	length = sizeof(server);						// largo de la estructura
	memset((char *)&server, 0, length); 					// establece todos los valores en cero
	server.sin_family = AF_INET;						// simbolo constante para internet
	server.sin_addr.s_addr = htonl(INADDR_ANY);				// para recibir de cualquier interfaz de red
	server.sin_port = htons(PUERTO);					// Seleccion del puerto

	if(bind(sockfd, (struct sockaddr *)&server, length) < 0)
       error("binding");

	fromlen = sizeof(struct sockaddr_in);					// tamaño de la estructura

	while(1){
		memset(buffer, 0, MSG_SIZE);					// limpia el buffer
		//  recepción  de informacion 
		n = recvfrom(sockfd, buffer, MSG_SIZE, 0, (struct sockaddr *)&from, &fromlen);
		if(n < 0)
	 		error("recvfrom");

		printf("Mensaje recibido: %s\n", buffer);
		 
		// Se envia un mensaje de recibido luego de cada mensaje. 
		n = sendto(sockfd, "Got your message\n", MSG_SIZE, 0, (struct sockaddr *)&from, fromlen);
		if(n < 0)
	 		error("sendto");
	}

	return 0;
 }

