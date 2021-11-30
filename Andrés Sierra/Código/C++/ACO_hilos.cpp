/*
                    TRABAJO DE GRADUACION 
                UNIVERSIDAD DEL VALLE DE GUATEMALA

    ROBOTICA DE ENJAMBRE - ALGORITMO ANT COLONY OPTIMIZATION
                    
    ANDRES SIERRA
    CARNET 17025 

*/
// ====================== LIBRERIAS ==================================
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
// ====================================================================
// ====================== PARAMETROS DEL ACO ==========================
// ====================================================================
#define ITERACIONES (int)5
#define NUMEROHORMIGAS (int)5
#define NUMERONODOS (int)10

// Le da más peso a la feromona en la probabilidad
#define ALPHA (double)1
// Le da más peso al costo del link en la probabilidad
#define BETA (double)0.1
// Constante positiva que regula el depósito de feromona
#define Q (double)10
// Rate de evaporación de las feromonas (puede tomar valores entre 0 y 1)
#define RO (double)0.5
// Valor maximo de la feromona
#define TAUMAX (int)2

// Nodo inicial y final
#define NODOINICIAL (int)0
#define NODOFINAL (int)5
// ====================================================================
// ========= PUERTO Y LONGITUD DEL MENSAJE RECIBIDO POR UDP ===========
// ====================================================================

#define MSG_SIZE 60 // message size
// #define IP_LENGTH 15
#define PUERTO 2050

// ====================================================================
// =========== VARIABLES UTILIZADAS PARA LOS CONTROLADORES ============
// ====================================================================
// Controladores
//#define TUC_CONTROLLER 0
//#define PID_CONTROLLER 0
#define NKC_CONTROLLER_1 1
#define NKC_CONTROLLER_2 0
//#define NKC_CONTROLLER_3 0
//#define LQR_CONTROLLER 0
//#define LQI_CONTROLLER 0

// Filtro de Picos para TUC-PID
#define HARDSTOP_FILTER 0
// HARDSTOP Filter Parameters
#define MAX_CHANGE 1.00

// Variables globales
#define MAX_SPEED 2
#define ROBOT_RADIUS 35	  //Modificar valores en mm
#define WHEEL_RADIUS 20.5 //Modificar valores en mm

// Parametros NKC - (K_p > 0; K_b < 0; K_a > K_p) Kp = 0.01
#define K_DISTANCE 0.1
#define K_ALPHA 0.5
#define K_BETA 0.05

// Error de posicion entre robot-meta
static double e_x = 0; // Diferencial de posicion horizontal
static double e_y = 0; // Diferencia de posicion vertical
static double e_p = 0; // Distacia euclidiana entre robot y meta

// Nuevas posiciones del robot
static double new_position[] = {0, 0};
static double new_velocity[] = {0, 0};
static double theta_g = 0; // Orientacion nueva del robot

// Variables del controlador NKC
static double alpha = 0;
static double beta = 0;
static double rho_p = 0;
static double K_RHO = K_DISTANCE;
static double K_A = K_ALPHA;
static double K_B = -K_BETA;

// Posiciones actuales del robot
static double actual_position[] = {0, 0};
static double old_velocity[] = {0, 0};
static double theta_o = 0; // Orientacion actual del robot

// Variables de velocidades angulares
static double phi_r = 0; // Velocidad angular actual de motor derecho de robot
static double phi_l = 0; // Velocidad angular actual de motor izquierdo de robot

// Variables de velocidades angulares pasadas de motores para Filtro de Picos
static double PhiR_old = 0; // Velocidad angular anterior de motor derecho de robot
static double PhiL_old = 0; // Velocidad angular anterior de motor izquierdo de robot

// =====================================================================
// =========== VARIABLES UTILIZADAS PARA RECEPCION DE DATOS ============
// =====================================================================
// Variable de recibir
char buffer_recibir[MSG_SIZE]; // almacenar mensaje recibido
char *token;

// Variable para separar la informacion
double recepcion[2];

// Variables para guardar la pose del robot
double posicion_robot_X = 0.0;
double posicion_robot_Y = 0.0;
double rad = 0.0;

// Variables para guardar las coordenadas de cada nodo
int nodo = 0;
double nodo_cx = 0;
double nodo_cy = 0;

// Variable para guardar los nodos que se conectan
int nodo_1 = 1;
int nodo_2 = 2;

// Banderas para verificar si se reciben datos
int bandera_coord = 0;
int bandera_robot = 0;
int bandera_conexion = 0;

//------------------------------Comunicacion -------------------------------
void error(const char *msg)
{
	perror(msg);
	exit(0);
}
// ------------------------------------------------------------------------

void *receiving(void *ptr)
{
	int *sock, n, ret, ret2, ret3;
	int i = 0;
	sock = (int *)ptr;								  // identificador del socket
	unsigned int length = sizeof(struct sockaddr_in); // tamaño de la estructura
	struct sockaddr_in from;

	while (1)
	{
		memset(buffer_recibir, 0, MSG_SIZE); // "limpia" el buffer
		// recibir mensaje
		n = recvfrom(*sock, buffer_recibir, MSG_SIZE, 0, (struct sockaddr *)&from, &length);
		if (n < 0)
		{
			error("Error: recvfrom");
		}

		i = 0;
		ret = strncmp(buffer_recibir, "P", 1);
		ret2 = strncmp(buffer_recibir, "G", 1);
		ret3 = strncmp(buffer_recibir, "C", 1);
		//cout <<"ret: " <<ret << endl;
		//cout <<"ret2: " <<ret2 << endl;
		printf("Mensaje recibido: %s\n", buffer_recibir);

		// --------------- CONEXION DE NODOS ----------------
		if (ret2 == 0)
		{
			// descomponer buffer_recibir, strtok
			token = strtok(buffer_recibir, ",");
			recepcion[i] = atof(token);
			while ((token = strtok(NULL, ",")) != NULL)
			{
				i++;
				recepcion[i] = atof(token);
			}
			nodo_1 = recepcion[1];
			nodo_2 = recepcion[2];
			printf("Nodos a conectar: %d, %d. \n\n", nodo_1, nodo_2);
			bandera_conexion = 1;
		}
		// --------------- COORDENADAS DEL ROBOT -----------------
		else if (ret == 0)
		{
			// descomponer buffer_recibir, strtok
			token = strtok(buffer_recibir, ",");
			recepcion[i] = atof(token);
			while ((token = strtok(NULL, ",")) != NULL)
			{
				i++;
				recepcion[i] = atof(token);
			}
			//printf("recepcion %f, %f, %f.\n",recepcion[1],recepcion[2],recepcion[3]);
			posicion_robot_X = recepcion[1];
			posicion_robot_Y = recepcion[2];
			rad = recepcion[3];

			printf("Coordenadas recibidas del robot: %f, %f, %f. \n\n", posicion_robot_X, posicion_robot_Y, rad);
			bandera_robot = 1;
		}
		// -------------- COORDENADAS NODOS ------------------
		else if (ret3 == 0)
		{
			// descomponer buffer_recibir, strtok
			token = strtok(buffer_recibir, ",");
			recepcion[i] = atof(token);
			while ((token = strtok(NULL, ",")) != NULL)
			{
				i++;
				recepcion[i] = atof(token);
			}
			//printf("recepcion %f, %f, %f.\n",recepcion[1],recepcion[2],recepcion[3]);
			nodo = recepcion[1];
			nodo_cx = recepcion[2];
			nodo_cy = recepcion[3];
			printf("Coordenadas recibidas del nodo %d: coordenada X: %f, coordenada Y: %f. \n\n", nodo, nodo_cx, nodo_cy);
			bandera_coord = 1;
		}
		else
		{
			cout << "No fue posible identificar el mensaje" << endl;
			cout << " " << endl;
		}

		// Se envia un mensaje de recibido luego de cada mensaje.
		n = sendto(*sock, "Got your message\n", MSG_SIZE, 0, (struct sockaddr *)&from, length);
		if (n < 0)
		{
			error("sendto");
		}
	}
	pthread_exit(0);
}

// Main function
int main(int argc, char *argv[])
{
	ACO *AS = new ACO(NUMEROHORMIGAS, NUMERONODOS, ALPHA, BETA, Q, RO, TAUMAX, NODOINICIAL, NODOFINAL);
	AS->inicializar();

	int sock, n;
	unsigned int length = sizeof(struct sockaddr_in); // tamaño de la estructura
	char buffer_enviar1[MSG_SIZE];					  // almacenar mensaje
	struct sockaddr_in anybody;						  // configuracion del socket
	int boolval = 1;
	pthread_t thread_rec; // variables para multihilos

	// char IP_broadcast[IP_LENGTH]; // para la dirección de broadcast
	FILE *file;

	anybody.sin_family = AF_INET;				 // simbolo constante para internet
	anybody.sin_port = htons(PUERTO);			 // puerto seleccionado
	anybody.sin_addr.s_addr = htonl(INADDR_ANY); // para recibir de cualquiera

	sock = socket(AF_INET, SOCK_DGRAM, 0); // creacion del socket
	if (sock < 0)
		error("Error: socket");

	if (bind(sock, (struct sockaddr *)&anybody, sizeof(struct sockaddr_in)) < 0)
	{
		printf("Error binding socket.\n");
		exit(-1);
	}

	// Se creen los hilo de recepción.
	pthread_create(&thread_rec, NULL, receiving, (void *)&sock);

	printf("PLANIFICADOR DE TRAYECTORIAS - ANT SYSTEM.\n");

	while (1)
	{
		if (bandera_conexion == 1)
		{
			cout << "Conectando nodos ..." << endl;
			AS->conectarNODOS(nodo_1, nodo_2);
			cout << "Nodos conectados !." << endl;

			bandera_conexion = 0;
		}
		if (bandera_coord == 1)
		{
			cout << "Definiendo coordenadas del mapa ..." << endl;
			AS->setCOORDENADAS(nodo, nodo_cx, nodo_cy);
			cout << "Coordenadas establecidas !." << endl;
			bandera_coord = 0;
		}
		if (bandera_robot == 1)
		{
			cout << "Mapa recibido: " << endl;
			// ------------------------- Desplegar el grafo a trabajar ---------------------------------
			AS->imprimirGRAFO();
			// ----------------- Correr el algoritmo y calcular la mejor ruta --------------------------
			AS->optimizar(ITERACIONES);

			// ---------------------------- VARIABLES DE CONTROLADORES ---------------------------------
			// ----------- Posicion del robot -----------
			actual_position[0] = posicion_robot_X;
			actual_position[1] = posicion_robot_Y;

			// Inicializacion de velocidad lineal y angular
			double v = 0; // Velocidad lineal de robot
			double w = 0; // Velocidad angular de robot

			// Calculo de error de distancias
			e_x = new_position[0] - actual_position[0]; // Error en posicion X
			e_y = new_position[1] - actual_position[1]; // Error en posicion Y
			e_p = sqrt(pow(e_y, 2) + pow(e_x, 2));		// Magnitud de error vectorial

			// Dimensiones de E-Puck
			double l = ROBOT_RADIUS / 1000; // Distancia de centro a llantas en metros
			double r = WHEEL_RADIUS / 1000; // Radio de llantas en metros
			double a = ROBOT_RADIUS / 1000; // Distancia entre centro y punto de disomorfismo (E-puck front)

			// ===========================================================================================
			// =========================== CONTROLES NO LINEALES DE ROBOT ================================
			// ===========================================================================================

			// Limitar angulo entre eje frontal de robot (-Z_R) y vector hacia meta dentro de rango [-pi pi]
			alpha = -(theta_o * M_PI / 180) + atan2(e_y, e_x);
			if (alpha < -M_PI)
			{
				alpha = alpha + (2 * M_PI);
			}
			else if (alpha > M_PI)
			{
				alpha = alpha - (2 * M_PI);
			}
			// Limitar angulo de orientacion de meta entre [-pi pi]
			beta = -(theta_o * M_PI / 180) - alpha;
			if (beta < -M_PI)
			{
				beta = beta + (2 * M_PI);
			}
			else if (beta > M_PI)
			{
				beta = beta - (2 * M_PI);
			}
			// Distancia entre centro de robot y punto de meta
			rho_p = e_p;

			// Controlador simple de pose de robot
			if (NKC_CONTROLLER_1)
			{
				v = K_RHO * rho_p;
				w = K_A * alpha + K_B * beta;
				if ((alpha <= -M_PI / 2) || (alpha > M_PI / 2))
				{
					v = -v;
				}
			}

			// Controlador Lyapunov de pose de robot
			if (NKC_CONTROLLER_2)
			{
				v = K_RHO * rho_p * cos(alpha);
				w = K_RHO * sin(alpha) * cos(alpha) + K_A * alpha;
				if ((alpha <= -M_PI / 2) || (alpha > M_PI / 2))
				{
					v = -v;
				}
			}

			// Terminacion del movimiento cerca de la meta
			if (fabs(e_p) < 0.005)
			{
				v = 0;
				w = 0;
			}
			// ---------------- Transformacion de velocidades con modelo diferencial -------------------------------
			phi_r = (v + w * l) / r;
			phi_l = (v - w * l) / r;

			if (phi_r > 0)
			{
				if (phi_r > MAX_SPEED)
				{
					phi_r = MAX_SPEED;
				}
			}
			else
			{
				if (phi_r < -MAX_SPEED)
				{
					phi_r = -MAX_SPEED;
				}
			}

			// Truncar velocidades de rotacion de motor izquierdo a [-6.28, 6.28]
			if (phi_l > 0)
			{
				if (phi_l > MAX_SPEED)
				{
					phi_l = MAX_SPEED;
				}
			}
			else
			{
				if (phi_l < -MAX_SPEED)
				{
					phi_l = -MAX_SPEED;
				}
			}

			/*
            // Limpieza de Picos
            if (HARDSTOP_FILTER)
			{
                if (sqrt(pow(phi_r - PhiR_old, 2)) > MAX_CHANGE)
				{
                    phi_r = (phi_r + 2 * PhiR_old) / 3;
                }
                PhiR_old = phi_r;
                if (sqrt(pow(phi_l - PhiL_old, 2)) > MAX_CHANGE)
				{
                    phi_l = (phi_l + 2 * PhiL_old) / 3;
                }
                PhiL_old = phi_l;
            }
			*/
			cout << "Velocidad del motor izquierdo: " << phi_l << endl;
			cout << "Velocidad del motor derecho: " << phi_r << endl;

			bandera_robot = 0;
		}
	}

	close(sock); // cerrar el socket.

	return 0;
}