/*  Implementacion del  PSO para su uso en Raspberry 
Alex Maas #17146  */

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


#define MSG_SIZE 40 			// tamaño del mensaje
#define IP_LENGTH 15
#define PUERTO1 2001			// puerto para enviar informacion entre agentes (UDP broadcast)
#define PUERTO2 2010			// puerto para enviar la pose de los agentes (UDP)

// Controladores
#define TUC_CONTROLLER 0
#define PID_CONTROLLER 1
#define NKC_CONTROLLER_1 0
#define NKC_CONTROLLER_2 0
#define NKC_CONTROLLER_3 0
#define LQR_CONTROLLER 0
#define LQI_CONTROLLER 0

// Filtro de Picos para TUC-PID
#define HARDSTOP_FILTER 1

// Filtro PID para velocidad angular de TUC-LQR
#define LQR_PID_COMBO 0

// Parametro de orientacion
#define USE_BEARING 0

// Variables globales
#define TIME_STEP 32
#define MAX_SPEED 2
#define ROBOT_RADIUS 35           	//Modificar valores en mm
#define WHEEL_RADIUS 20.5           	//Modificar valores en mm

// Fitness Funcion   			(0-Sphere, 1-Rosenbrock, 2- Booth, 3-Himmelblau)
#define funcion_costo 0   
#define TIME_DELTA 0.032

// Parametro actualizar PSO
#define PSO_STEP 1
#define USE_STANDART_PSO 1

// Parametro de Inercia  		(0-cte, 1-linear, 2-chaotic, 3-random, 4-exponential)
#define INERTIA_TYPE 4

// Parametros PSO
#define CONSTRICTION_FACTOR 0.8
#define COGNITIVE_WEIGTH 2
#define SOCIAL_WEIGTH 10

// Parametros PID
#define K_PROPORTIONAL 0.5
#define K_INTEGRAL 0.1
#define K_DERIVATIVE 0.001

// Parametros NKC
#define K_DISTANCE 0.1
#define K_ALPHA 0.5
#define K_BETA 0.05

// HARDSTOP Filter Parameters
#define MAX_CHANGE 1.00

// Variables de pose del robot 
static double posicion_robot_X = 0;
static double posicion_robot_Y = 0;
static double rad = 0;

// Nuevas posiciones del robot
static double new_position[] = {0, 0};
static double new_velocity[] = {0, 0};
static double theta_g = 0; 			// Orientacion nueva del robot

// Posiciones actuales del robot
static double actual_position[] = {0, 0};
static double old_velocity[] = {0, 0};
static double theta_o = 0; 			// Orientacion actual del robot
static double fitness_actual = 0;

// Posicion Local
static double best_local[] = {0, 0};
static double fitness_local = 100000000;

// Posicion Global
static double best_global[] = {0, 0};
static double fitness_global = 100000000;

// Parametros PSO
static double epsilon = CONSTRICTION_FACTOR;
static double C1 = COGNITIVE_WEIGTH;
static double C2 = SOCIAL_WEIGTH;
static double w = 0;
static double rho1 = 0;
static double rho2 = 0;
static double V_scaler = TIME_DELTA;

// Calculo de Inercia PSO
static double w_min = 0.5;
static double w_max = 1;
static int Maxiter = 10000;
static int iter = 0;

// Variables del controlador PID
static double KP = K_PROPORTIONAL;
static double KI = K_INTEGRAL;
static double KD = K_DERIVATIVE;
static double e_old = 0;
static double E_old = 0;
static double e_o = 0;
static double E_o = 0;
static double e_D = 0;

// Variables del controlador NKC
static double alpha = 0;
static double beta = 0;
static double rho_p = 0;
static double K_RHO = K_DISTANCE;
static double K_A = K_ALPHA;
static double K_B = -K_BETA;

// Variables del conntrolador LQI
static double delta_t = TIME_DELTA;
static double XI_X = 0; 			// Error integrativo de X
static double XI_Y = 0; 			// Error integrativo de Y

// Error de posicion entre robot-meta
static double e_x = 0; 				// Diferencial de posicion horizontal
static double e_y = 0; 				// Diferencia de posicion vertical
static double e_p = 0; 				// Distacia euclidiana entre robot y meta

// Variables de velocidades angulares pasadas de motores para Filtro de Picos
static double phi_r = 0;                          // Velocidad angular actual de motor derecho de robot
static double phi_l = 0;                          // Velocidad angular actual de motor izquierdo de robot
static double PhiR_old = 0;                       // Velocidad angular anterior de motor derecho de robot
static double PhiL_old = 0;                       // Velocidad angular anterior de motor izquierdo de robot

// Numero de iteracion 
static int numero_iter = 0;

//Inicializacion
int estado = 0;
int num_agente = 0;				// Numero del agente (se asigna al ejecutar)

// Recepcion de datos de otros robots
static double recepcion[2];
static double recepcion2[2];

// Variable de recibir
char buffer_recibir[MSG_SIZE];  			// almacenar mensaje recibido
char *token;
char *token2;
int bandera = 0;					// indicador si se recibio una nueva pose (sirve para comunicacion con mesa de pruebas)



// Numeros aleatorios
double printRandoms(int lower, int upper, int count){
    int i;
    double num = 0;
    for (i = 0; i < count; i++){
        num = (rand() % (upper - lower + 1)) + lower;
    }
    return num;
}

// Numeros random
double randfrac(void){
    double res = (rand() % RAND_MAX) / (double)RAND_MAX;
    return res;
}

// Funciones costo
double funcion(double x, double y){
    double f = 0;
    if (funcion_costo == 0){
        f = pow(x, 2) + pow(y, 2); 				// Sphere
    }
    else if (funcion_costo == 1){
        f = pow(1 - x, 2) + 100 * pow(y - pow(x, 2), 2); 	// Rosenbrock
    }
    else if (funcion_costo == 2){
        f = pow(x + 2 * y - 7, 2) + pow(2 * x + y - 5, 2); 	// Booth
    }
    else if (funcion_costo == 3){
        f = pow(x * x + y - 11, 2) + pow(x + y * y - 7, 2); 	// Himmelblau
    }
    return f;
}
//------------------------------Error de Comunicacion -------------------------------
void error(const char *msg){
    perror(msg);
    exit(0);
}
//----------------------------------------------------------------------------------

//------------------------Recepcion Broadcast (comunicacion agentes)--------------------------------------
void *receiving(void *ptr){
    int *sock, n, ret;
    int i = 0;
    sock = (int *)ptr;                                			// identificador del socket
    unsigned int length = sizeof(struct sockaddr_in); 			// tamaño de la estructura
    struct sockaddr_in from;

    while (1){
        memset(buffer_recibir, 0, MSG_SIZE); 				// "limpia" el buffer
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
	//printf("recepcion %f,%f, %f y %f.\n",recepcion[0],recepcion[1],recepcion[2],fitness_global);
	// Actualizar global best
	if (recepcion[2] < fitness_global){ 				//Local< Global, asigna el valor local al global
		best_global[0] = recepcion[0];
		best_global[1] = recepcion[1];
		fitness_global = recepcion[2];
		//printf("Agente %d actualiza su Best Global a %f.\n", num_agente, fitness_global);
	}else{
		//printf("Agente %d no actualiza su Best Global %f.\n", num_agente, fitness_global);
	}	
    }
    pthread_exit(0);
}
//----------------------------------------------------------------------------------------

//------------ Recepcion Matlab (Vision por computadora) ------------------
void *receiving2(void *ptr){
    int sockfd, length, n;
    int j = 0;
	socklen_t fromlen;
	struct sockaddr_in server;
	struct sockaddr_in from;
	char buffer[MSG_SIZE];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);		// Creacion del socket
	if(sockfd < 0)
		error("Opening socket");

	length = sizeof(server);				// largo de la estructura
	memset((char *)&server, 0, length); 			// establece todos los valores en cero
	server.sin_family = AF_INET;				// simbolo constante para internet
	server.sin_addr.s_addr = htonl(INADDR_ANY);		// para recibir de cualquier interfaz de red
	server.sin_port = htons(PUERTO2);			// Seleccion del puerto

	if(bind(sockfd, (struct sockaddr *)&server, length) < 0)
       error("binding");

	fromlen = sizeof(struct sockaddr_in);			// tamaño de la estructura

	while(1){
		memset(buffer, 0, MSG_SIZE);			// limpia el buffer.
		//  recepción  de informacion
		n = recvfrom(sockfd, buffer, MSG_SIZE, 0, (struct sockaddr *)&from, &fromlen);
		if(n < 0)
            		error("recvfrom");

		//printf("Mensaje recibido: %s\n", buffer);
        	bandera = 1;
        	j = 0;
		// descomponer buffer_recibir, strtok
		token2 = strtok(buffer, ",");
       		recepcion2[j] = atof(token2);
		while ((token2 = strtok(NULL, ",")) != NULL){
			j++;
			recepcion2[j] = atof(token2);   
		}
        	posicion_robot_X = recepcion2[0];
        	posicion_robot_Y = recepcion2[1];
        	theta_o = recepcion2[2];
        	printf("Coordenadas agente %d son: %f, %f, %f.\n", num_agente, posicion_robot_X, posicion_robot_Y, theta_o);
    }
    pthread_exit(0);
}

//------------------------------------------------------------------------------------

//-------------------------------- MAIN ----------------------------------------------------------
// Main function
int main(int argc, char *argv[]){
    //------------------------------Proceso de Comunicacion -------------------------------
    int sock, n;
    unsigned int length = sizeof(struct sockaddr_in);           // tamaño de la estructura
    char buffer_enviar1[MSG_SIZE];                              // almacenar mensaje
    struct sockaddr_in anybody;                                 // configuracion del socket
    int boolval = 1;                                           
    pthread_t thread_rec, thread_rec2; 				// variables para multihilos
    char IP_broadcast[IP_LENGTH];                               // para la dirección de broadcast
    FILE *file;
    strcpy(IP_broadcast, "10.0.0.255"); 			// Puede que se deba cambiar. Revisar ifconfig
    //strcpy(IP_broadcast, "192.168.0.255");
	
    printf("La dirección de broadcast Es: %s\n\n", IP_broadcast);

    if (argc == 2){
	    num_agente = atoi(argv[1]);				// numero colocado al ejecutar
    }

    anybody.sin_family = AF_INET;                		// simbolo constante para internet
    anybody.sin_port = htons(PUERTO1);           	 	// puerto seleccionado
    anybody.sin_addr.s_addr = htonl(INADDR_ANY); 		// para recibir de cualquiera

    sock = socket(AF_INET, SOCK_DGRAM, 0); 			// creacion del socket
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
 
    // Se crean los hilo de recepción.
    pthread_create(&thread_rec, NULL, receiving, (void *)&sock); 
    pthread_create(&thread_rec2, NULL, receiving2, (void *)&sock); 

    printf("Tesis Alex.\n");
    printf("Agente %d conectado\n", num_agente);

    while (1){
        if (bandera == 1){
            memset(buffer_enviar1, 0, MSG_SIZE); 		// "limpia" el buffer
											
            sleep(5);
            // Valores a enviar
            sprintf(buffer_enviar1, "%f,%f,%f", best_local[0], best_local[1], fitness_local);
	    if (buffer_enviar1[0] != '!'){
		    // enviar mensaje
                    n = sendto(sock, buffer_enviar1, strlen(buffer_enviar1), 0,
                        		(const struct sockaddr *)&anybody, length);
                    if (n < 0)
                    	error("Error: sendto");
            }
            //-------------------------- Algoritmo  PSO ---------------------------------------
            numero_iter++;
            printf("Comienza PSO No. de iteracion %d\n",numero_iter);
		
            // --------------------Configuracion de valores iniciales del PSO-------------------
            if (estado == 0)
            {
                actual_position[0] = posicion_robot_X; 			// coordenada X
                actual_position[1] = posicion_robot_Y; 			// coordenada Y
                best_local[0] = actual_position[0];
                best_local[1] = actual_position[1];
                best_global[0] = actual_position[0];
                best_global[1] = actual_position[1];

                // Calcular  Fitness valor
                fitness_local = funcion(actual_position[0], actual_position[1]);
                fitness_global = funcion(actual_position[0], actual_position[1]);
                //printf("Valor Fitness Local Inicial: %f y Valor Fitnes Global Inicial: %f.\n", fitness_local, fitness_global);
                estado = 1;
            }
            //-------------------Obtener posicion y orientacion del robot-------------------------------
            //posicion_robot = 0; GPS
            //const double north = [0,0];   //GPS angulo
            //double rad = atan2(north[0],north[2]);

            // Orientacion como angulo de brujula
            /*
            double bearing = ((rad) / M_PI) * 180.0;
            theta_o = bearing + 180;

            // Orientacion como angulo normal
            double bearing2 = ((rad) / M_PI) * 180.0;

            // Convertir numero negativo entre 0 y 360
            if (bearing2 < 0){
                bearing2 = 360 + bearing2; // asegurar angulp entre 0 y 2pi
            }
            double theta_o2 = 270 - bearing2;
            if (theta_o2 < 0){
                theta_o2 = theta_o2 + 360; // asegurar angulp entre 0 y 2pi
            }
            if (USE_BEARING == 0){
                theta_o = theta_o2;
            }*/
            //--------------------------- Actualizacion de global y local best-------------------------------

            // Calcular fitness valor de posicion actual
            actual_position[0] = posicion_robot_X;
            actual_position[1] = posicion_robot_Y;
            fitness_actual = funcion(actual_position[0], actual_position[1]);
            //printf("Valor Fitness Actual: %f.\n", fitness_actual);

            // Actualizar local best si posicion actual  posee una mejor fitness valor
            if (fitness_actual < fitness_local)
            {
                best_local[0] = actual_position[0];
                best_local[1] = actual_position[1];
                fitness_local = fitness_actual;
            }

            // Actualizar global best (propio no el recibido-enviado)
            if (fitness_local < fitness_global)
            {
                best_global[0] = best_local[0];
                best_global[1] = best_local[1];
                fitness_global = fitness_local;
            }
            // ----------------------- MPSO Algorithm --------------

            // Parametros de uniformidad
            rho1 = randfrac();
            rho2 = randfrac();

            // Calculo de Inercia invariante en el tiempo
            if (INERTIA_TYPE == 0){
                w = 0.8;
            }else if (INERTIA_TYPE == 1){
                w = w_max - (w_max - w_min) * iter / Maxiter;
            }else if (INERTIA_TYPE == 2){
                double zi = 0.2;
                double zii = 4 * zi * (1 - zi);
                w = (w_max - w_min) * ((Maxiter - iter) / Maxiter) * w_max * zii;
                iter = iter + 1;
            }else if (INERTIA_TYPE == 3){
                w = 0.5 + printRandoms(0, 1, 1) / 2;
            }else if (INERTIA_TYPE == 4){
                w = w_min + (w_max - w_min) * exp((-1 * iter) / (Maxiter / 10));
            }

            // Configuracion estandar del PSO
            if (USE_STANDART_PSO == 1){
                C1 = 2.05;
                C2 = 2.05;

                // Parametro de Constriccion
                double phi_T = C1 + C2;
                epsilon = 2.0 / fabs(2 - phi_T - sqrt(pow(phi_T, 2) - 4 * phi_T));

                // Configuracion de PSO Velocity Scaler
                V_scaler = 0.25;
                if (TUC_CONTROLLER == 1){
                    V_scaler = 0.625;
                }
                if (PID_CONTROLLER == 1){
                    V_scaler = 7.8125;
                }
            }

            // Ecuaciones de velocidad PSO para calcular una nueva velocidad
            old_velocity[0] = new_velocity[0];
            old_velocity[1] = new_velocity[1];
            new_velocity[0] = epsilon * (w * old_velocity[0] + C1 * rho1 * (best_local[0] - actual_position[0]) + C2 * rho2 * (best_global[0] - actual_position[0]));
            new_velocity[1] = epsilon * (w * old_velocity[1] + C2 * rho2 * (best_local[1] - actual_position[1]) + C2 * rho2 * (best_global[1] - actual_position[1]));

            // Ecuacion de posicion PSO para calcular nueva posicion
            if (numero_iter % PSO_STEP == 0 || numero_iter == 1)
            {
                new_position[0] = actual_position[0] + new_velocity[0] * V_scaler;
                new_position[1] = actual_position[1] + new_velocity[1] * V_scaler;
            }
            printf("Nuevas Posiciones del agente %d son: %f, %f.\n", num_agente, new_position[0], new_position[1]);
            // ------------------ Variables de Controladores ----------------------

            // Inicializacion  de velocidad angular y lineal
            double v = 0;
            double w = 0;

            // Calculo de error de distancias
            e_x = new_position[0] - actual_position[0];
            e_y = new_position[1] - actual_position[1];
            e_p = sqrt(pow(e_y, 2) + pow(e_x, 2));

            // Dimensiones para el Robot (actualmente dimensiones del E-Puck)
            double l = ROBOT_RADIUS / 1000; 			// Distancia de centro a llantas en metros
            double r = WHEEL_RADIUS / 1000; 			// Radio de llantas en metros
            double a = ROBOT_RADIUS / 1000; 			// Distancia entre centro y punto de disomorfismo (E-puck front)

            // -------------------- Control de ecuaciones cinematicas ---------------

            // Constante de ponderacion
            double K = 3.12 * (1 - exp(-2 * pow(e_p, 1))) / e_p;

            // Si Bearing angle desactivado (desde vertical +X), utilizar orientacion normal (desde horizontal +Z)
            if (USE_BEARING == 0){
                // Velocidad Lineal (aplicada con matriz de disomorfismo y 2*tanh(x) para acotar velocidades a MAX_SPEED de E-Puck (6.28 rad/s)
                v = (2 * tanh((K / MAX_SPEED) * e_x)) * cos((theta_o)*M_PI / 180) +
                    (2 * tanh((K / MAX_SPEED) * e_y)) * sin((theta_o)*M_PI / 180);

                // Velocidad Angular (aplicada con matriz de disomorfismo y 2*tanh(x) para acotar velocidades a MAX_SPEED de E-Puck (6.28 rad/s)
                w = (2 * tanh((K / MAX_SPEED) * e_x)) * (-sin((theta_o)*M_PI / 180) / a) +
                    (2 * tanh((K / MAX_SPEED) * e_y)) * (cos((theta_o)*M_PI / 180) / a);
            }
            else{
                // Velocidad Lineal (aplicada con matriz de disomorfismo y 2*tanh(x) para acotar velocidades a MAX_SPEED de E-Puck (6.28 rad/s)
                v = (2 * tanh((K / MAX_SPEED) * e_x)) * cos((90 - theta_o) * M_PI / 180) +
                    (2 * tanh((K / MAX_SPEED) * e_y)) * sin((90 - theta_o) * M_PI / 180);

                // Velocidad Angular (aplicada con matriz de disomorfismo y 2*tanh(x) para acotar velocidades a MAX_SPEED de E-Puck (6.28 rad/s)
                w = (2 * tanh((K / MAX_SPEED) * e_x)) * (-sin((90 - theta_o) * M_PI / 180) / a) +
                    (2 * tanh((K / MAX_SPEED) * e_y)) * (cos((90 - theta_o) * M_PI / 180) / a);
            }

            // -------------------- Control PID de velocidad angular ---------------
            if (USE_BEARING == 0){
                // Angulo de meta calculado en orientacion normal desde la horizontal
                theta_g = atan2(new_position[1] - actual_position[1], new_position[0] - actual_position[0]);
            }
            else{
                // Angulo de meta calculado en orientacion de brujula desde la vertical
                theta_g = atan2(new_position[0] - actual_position[0], new_position[1] - actual_position[1]);
            }

            // Mnatener angulo de orinetacion entre -pi y pi
            double orientation_angle = 0;
            if (theta_o > 180){
                orientation_angle = theta_o - 360;
            }
            else{
                orientation_angle = theta_o;
            }

            // error de distancia angular
            e_o = atan2(sin(theta_g - (orientation_angle * M_PI / 180)), cos(theta_g - (orientation_angle * M_PI / 180)));

            // PID velocidad angular
            e_D = e_o - e_old;
            E_o = E_old + e_o;
            if (PID_CONTROLLER){
                w = KP * e_o + KI * E_o + KD * e_D;
            }
            // Si no se esta utilizando PID de LQR, ejecutar actualizacion de errores en este bloque
            if (LQR_PID_COMBO == 0){
                e_old = e_o;
                E_old = E_o;
            }
            // ----------- Controles no lineales de robot --------------------------------
            rho_p = e_p;

            // LImitar angulo entre eje frontal del robot
            alpha = -(theta_o * M_PI / 180) + atan2(e_y, e_x);
            if (alpha < -M_PI){
                alpha = alpha + (2 * M_PI);
            }
            else if (alpha > M_PI){
                alpha = alpha - (2 * M_PI);
            }

            // Limitar angulo de orientacion de meta entre -pi y pi
            beta = -(theta_o * M_PI / 180) - alpha;
            if (beta < -M_PI){
                beta = beta + (2 * M_PI);
            }
            else if (beta > M_PI){
                beta = beta - (2 * M_PI);
            }

            // Controlador simple de pose de robot
            if (NKC_CONTROLLER_1){
                v = K_RHO * rho_p;
                w = K_A * alpha + K_B * beta;
                if ((alpha <= -M_PI / 2) || (alpha > M_PI / 2)){
                    v = -v;
                }
            }

            // Controlador Lyapunov de pose de robot
            if (NKC_CONTROLLER_2){
                v = K_RHO * rho_p * cos(alpha);
                w = K_RHO * sin(alpha) * cos(alpha) + K_A * alpha;
                if ((alpha <= -M_PI / 2) || (alpha > M_PI / 2)){
                    v = -v;
                }
            }

            // Controlador Closed-loop steering
            if (NKC_CONTROLLER_3){
                double k_1 = 1;
                double k_2 = 10;
                w = -(2.0 / 5) * (v / rho_p) * (k_2 * (-alpha - atan(-k_1 * beta)) + (1 + k_1 / (1 + pow(k_1 * beta, 2))) * sin(-alpha));
            }

            // Controlador LQR
            if (LQR_CONTROLLER){
                double K_x = 0.1;
                double K_y = 0.1;
                double u_1 = K_x * e_x;
                double u_2 = K_y * e_y;

                u_1 = -K_x * (actual_position[0] - new_position[0]);
                u_2 = -K_y * (actual_position[1] - new_position[1]);

                v = u_1 * cos((theta_o)*M_PI / 180) + u_2 * sin((theta_o)*M_PI / 180);
                w = (-u_1 * sin((theta_o)*M_PI / 180) + u_2 * cos((theta_o)*M_PI / 180)) / l;

                if (LQR_PID_COMBO){
                    double Ti = 3; // Constante de reduccion de oscilaciones para PID+LQR
                    w = KP * e_o + (KI / Ti) * E_o + KD * e_D;
                    e_old = e_o;
                    E_old = E_o;
                }
            }

            // Controlador LQI
            if (LQI_CONTROLLER){
                double Klqr_x = 0.2127;
                double Klqr_y = 0.2127;
                double Klqi_x = -0.0224;
                double Klqi_y = -0.0224;

                // Dampers control LQI
                double bv_p = 0.95;
                double bv_i = 0.01;

                // Controlador LQI
                double u_1 = -Klqr_x * (1 - bv_p) * (actual_position[0] - new_position[0]) - Klqi_x * XI_X;
                double u_2 = -Klqr_y * (1 - bv_p) * (actual_position[1] - new_position[1]) - Klqi_y * XI_Y;

                // Integracion numerica de error entre posicion actual y Global Best de enjambre
                XI_X = XI_X + (best_global[0] - actual_position[0]) * delta_t;
                XI_Y = XI_Y + (best_global[1] - actual_position[1]) * delta_t;

                // Frenado de integrador para evitar oscilaciones en posiciones de robots
                XI_X = (1 - bv_i) * XI_X;
                XI_Y = (1 - bv_i) * XI_Y;

                // mapeo de velocidades LQI a velocidades de robot diferencial
                v = u_1 * cos((theta_o)*M_PI / 180) + u_2 * sin((theta_o)*M_PI / 180);
                w = (-u_1 * sin((theta_o)*M_PI / 180) + u_2 * cos((theta_o)*M_PI / 180)) / l;
            }

            // Terminacion del movimiento cerca de la meta
            if (fabs(e_p) < 0.005){
                v = 0;
                w = 0;
            }
            // ---------------- Transformacion de velocidades con modelo diferencial -------------------------------
            phi_r = (v + w * l) / r;
            phi_l = (v - w * l) / r;

            if (phi_r > 0){
                if (phi_r > MAX_SPEED){
                    phi_r = MAX_SPEED;
                }
            }
            else{
                if (phi_r < -MAX_SPEED){
                    phi_r = -MAX_SPEED;
                }
            }

            // Truncar velocidades de rotacion de motor izquierdo a [-6.28, 6.28]
            if (phi_l > 0){
                if (phi_l > MAX_SPEED){
                    phi_l = MAX_SPEED;
                }
            }
            else{
                if (phi_l < -MAX_SPEED){
                    phi_l = -MAX_SPEED;
                }
            }

            // Limpieza de Picos
            if (HARDSTOP_FILTER){
                if (sqrt(pow(phi_r - PhiR_old, 2)) > MAX_CHANGE){
                    phi_r = (phi_r + 2 * PhiR_old) / 3;
                }
                PhiR_old = phi_r;
                if (sqrt(pow(phi_l - PhiL_old, 2)) > MAX_CHANGE){
                    phi_l = (phi_l + 2 * PhiL_old) / 3;
                }
                PhiL_old = phi_l;
            }
            printf("Velocidad del agente %d son: %f, %f.\n\n\n", num_agente, phi_l, phi_r);
            // Envio de velocidads a motores
            bandera = 0;
            
        }
        usleep(1000);
    }
    close(sock); 			// cerrar socket.
    return 0;
}
