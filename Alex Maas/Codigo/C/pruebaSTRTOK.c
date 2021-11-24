#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h>

static double recepcion[5];
static char buffer_enviar[40];

int main (){
    char contenido[] = "1.1,2.1,3.1,4.1,5.1";                  // datos a enviar 
    char *token;
    int i =0;

    token=strtok(contenido, ",");                               // se indica hacer una separacion cada que detecte el caracter  ","                                                        
    recepcion[i] = atof(token);                                 // se convierte cada caracter a su valor numérico
    printf("%f\n",recepcion[i]);
    
    // mientras aun hayan caracteres seguir separando
    while((token = strtok(NULL, ",")) !=NULL){
        i++;
        recepcion[i] = atof(token);                             // se convierte cada caracter a su valor numérico
        printf("%f\n",recepcion[i]);

    }
    sprintf(buffer_enviar, "%f,%f,%f",recepcion[0],recepcion[1],recepcion[2]);          // se agrupan en una nueva variable "buffer_enviar"
    printf("\n%s\n",buffer_enviar);
}



