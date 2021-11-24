//Librerias
#include <webots/robot.h>
#include <webots/distance_sensor.h>
#include <webots/emitter.h>
#include <webots/gps.h>
#include <webots/compass.h>
#include <webots/motor.h>
#include <webots/receiver.h>
#include <math.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <string.h>

// Variables globales
#define TIME_STEP 32
#define MAX_SPEED 2
#define COMMUNICATION_CHANNEL 1
#define ROBOT_RADIUS 35.0
#define WHEEL_RADIUS 20.5

// Filtro de Picos para TUC-PID
#define HARDSTOP_FILTER   1

#define TIME_DELTA 0.032

#define PSO_STEP 1

// PSO Parameters (Default: Constriction = 0.8, Local Weight = 2; Global Weight = 10)
// TIME_DELTA 0.02 para PSO-TP sim
#define CONSTRICTION_FACTOR 0.8
#define COGNITIVE_WEIGTH 2
#define SOCIAL_WEIGTH 10

// PID Parameters 0.5 0.1 0.001 (I = 0.25 con LQR para simular PSO-TP)
#define K_PROPORTIONAL 0.5
#define K_INTEGRAL     0.1
#define K_DERIVATIVE   0.001

// HARDSTOP Filter Parameters
#define MAX_CHANGE 1.00

// Posiciones de robot y valores de Fitness Function ----------------------------------------------------
static const double *position_robot;            // Variable que recibe vector de posicion [X, Y, Z] retornado por GPS

// Posicion y velocidad nuevas del robot
static double new_position[] = {0,0};           // X_{i+1} de PSO
static double new_velocity[] = {0,0};           // V_{i+1} de PSO
static double theta_g = 0;                      // Orientacion de nueva posicion respecto a robot

// Posicion y velocidad actual del robot
static double actual_position[] = {0,0};        // X_{i} de PSO
static double old_velocity[] = {0,0};           // V_{i} de PSO
static double theta_o = 0;                      // Orientacion actual de robot
static double fitness_actual = 0;               // Costo de posicion actual

// Variables de mejor posicion local encontrada por particula
static double best_local[] = {0,0};
static double fitness_local = 0;

// Variables de mejor posicion global encontrada por particula
static double best_global[] = {0,0};
static double fitness_global = 0;

// Vector para recepcion de datos de otros robots
static double reception[] = {0,0};

// Parametros de PSO
static double epsilon = CONSTRICTION_FACTOR;      // Parametro PSO de constriccion
static double c1 = COGNITIVE_WEIGTH;              // Parametro PSO de escalamiento cognitivo
static double c2 = SOCIAL_WEIGTH;                 // Parametro PSO de escalamiento social
static double w = 0;                              // Parametro PSO de inercia
static double rho1 = 0;                           // Parametro PSO de uniformidad cognitiva
static double rho2 = 0;                           // Parametro PSO de uniformidad social
static double V_scaler = TIME_DELTA;              // Parametro de escalamiento especial de vector de velocidad PSO

// Variables de calculo de Parametro PSO de Inercia
static double w_min = 0.5;                        // Limite minimo de valor de parametro PSO de inercia
static double w_max = 1.0;                        // Limite maximo de valor de parametro PSO de inercia
static int MAXiter = 10000;                       // Cantidad maxima de iteraciones
static int iter = 0;                              // Contador de iteraciones ejecutadas

// Variables de Proportional Integral Derivative controller (PID)
static double KP = K_PROPORTIONAL;                // Constante de control proporcional
static double KI = K_INTEGRAL;                    // Constante de control integral
static double KD = K_DERIVATIVE;                  // Constante de control derivativo
static double e_old = 0;                          // Error de PID anterior
static double E_old = 0;                          // Error anterior acumulado de PID
static double e_o = 0;                            // Error de PID actual
static double E_o = 0;                            // Error actual acumulado de PID
static double e_D = 0;                            // Error diferencial

// Error de posicion entre robot y meta
static double e_x = 0;                            // Diferencial de posicion horizontal
static double e_y = 0;                            // Diferencial de posicion vertical
static double e_p = 0;                            // Distancia euclidiana entre robot y meta

// Variables de velocidades angulares pasadas de motores para Filtro de Picos
static double phi_r = 0;                          // Velocidad angular actual de motor derecho de robot
static double phi_l = 0;                          // Velocidad angular actual de motor izquierdo de robot
static double PhiR_old = 0;                       // Velocidad angular anterior de motor derecho de robot
static double PhiL_old = 0;                       // Velocidad angular anterior de motor izquierdo de robot

// Zero variable
static double zero = 0;
static int number_iteration = 0;

// Bandera de Inicializacion
bool state = false;

int rob_number = 0;

// Generador de numeros random 
double printRandoms(int lower, int upper, int count) { 
  int i;
  double num = 0;
  
  for (i = 0; i < count; i++) { 
    num = (rand() % (upper - lower + 1)) + lower; 
  } 
  return num;
}

// Generador de numeros random
double randfrac(void) {
  double res = (rand() % RAND_MAX) / (double)RAND_MAX;
  return res;
}

// Funcion para evaluar Fitness Function de espacio utilizando Benchmark Functions
double fitness(double x, double y){
  double r = pow(x,2) + pow(y,2);
  return r;
}


// Main function
int main() {

  // Inicializar WeBots Controlled Library
  wb_robot_init();
  

  // Obtener handle de GPS de robot
  WbNodeType gps = wb_robot_get_device("gps");
  wb_gps_enable(gps, TIME_STEP);
  
  // Obtener handle de brujula de robot
  WbNodeType compass = wb_robot_get_device("compass");
  wb_compass_enable(compass, TIME_STEP);
  
  // Obtener handle de motores de robot
  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
  
  // Configurar rotacion perpetua
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  
  // Configurar velocidades iniciales
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);
  
  // Configurar funciones de Emitter-Receiver
  WbDeviceTag emitter = wb_robot_get_device("emitter");
  wb_emitter_set_channel(emitter, COMMUNICATION_CHANNEL);       // COMMUNICATION_CHANNEL 1
  WbDeviceTag receiver = wb_robot_get_device("receiver");
  wb_receiver_enable(receiver, TIME_STEP);                      // TIME_STEP 32

  // Main loop de control (pasos de 32 milisegundos)
  while (wb_robot_step(TIME_STEP) != -1) {
    number_iteration++;
  
    // ----------------------------- CONFIGURACION INICIAL ---------------------------------------
  
    // Configurar valores iniciales de PSO local y global (Ejecutar solo una vez)
    if (state == false){
      position_robot = wb_gps_get_values(gps);
      actual_position[0] = position_robot[2];   // Coordenada en eje Z de simulacion es coordenada X
      actual_position[1] = position_robot[0];   // Coordenada en eje X de simulacion es coordenada Y
      best_local[0] = actual_position[0];
      best_local[1] = actual_position[1];
      best_global[0] = actual_position[0];
      best_global[1] = actual_position[1];
      
      // Calcular Fitness Value
      fitness_local = fitness(actual_position[0], actual_position[1]);
      fitness_global = fitness(actual_position[0], actual_position[1]);
      state = true;
    }

      // Obtener posicion del E-Puck
    position_robot = wb_gps_get_values(gps);                    // Devuelve [X,Y,Z] en vector, y aca ya se toma Z como X y X como Y
    
    // Obtener orientacion de E-Puck
    // Se utiliza atan2 para calcular el ángulo presente en cualquiera de los 4
    // cuadrantes, de lo contrario solo retorna el ángulo para los cuadrantes 1 y 4
    const double *north = wb_compass_get_values(compass);
    double rad = atan2(north[0], north[2]);
    
    // Orientacion obtenida como angulo de brujula (Phi en tesis)
    double bearing = ((rad) / M_PI) * 180.0;
    theta_o = bearing + 180;
    
    // Orientacion obtenida como angulo normal
    double bearing2 = ((rad) / M_PI) * 180.0;
    
    // Si el número retorna un valor negativo, se normaliza para que esté entre 0 y 360
    if (bearing2 < 0){   
      bearing2 = 360 + bearing2;           // Asegurar que angulo -> [0, 2*pi)
    } 
                                          
    double theta_o2 = 270 - bearing2;
    if (theta_o2 < 0){
      theta_o2 = theta_o2 + 360;           // Asegurar que angulo -> [0, 2*pi)
    }

// ------------------------- ACTUALIZACION DE LOCAL AND GLOBAL BESTS -------------------------
    
    // Calcular Fitness Value de posicion actiaul
    actual_position[0] = position_robot[2];
    actual_position[1] = position_robot[0];
    fitness_actual = fitness(actual_position[0], actual_position[1]);
    
    // Actualizar local best si posicion actual posee un mejor fitness value
    if (fitness_actual < fitness_local){
      best_local[0] = actual_position[0];
      best_local[1] = actual_position[1];
      fitness_local = fitness_actual;
    }
    
    // Transmision de Local Best a otros robots para poleo de Global Best
    double transmission[3] = {best_local[0],best_local[1],fitness_local};
    wb_emitter_send(emitter, transmission, 3 * sizeof(double));
    
    // Recepcion de otros datos de robots para polear todos los local bests a propio global best
    while (wb_receiver_get_queue_length(receiver) > 0) {
      const double *buffer = wb_receiver_get_data(receiver);
      reception[0] = buffer[0];
      reception[1] = buffer[1];
      reception[2] = buffer[2];
      
      // Actualizar global best
      if (reception[2] < fitness_global){
        best_global[0] = reception[0];
        best_global[1] = reception[1];
        fitness_global = reception[2];
      }
      wb_receiver_next_packet(receiver);
    }
    
    // Actualizar global best (si propio local best es mejor que el mejor global)
    if (fitness_local < fitness_global){
      best_global[0] = best_local[0];
      best_global[1] = best_local[1];
      fitness_global = fitness_local;
    }
    
        // ---------------------------------- MPSO ALGORITHM -----------------------------------

    // Generacion de valores de Parametros de Uniformidad (rho = [0,1])
    //rho1 = printRandoms(0, 1, 1);
    //rho2 = printRandoms(0, 1, 1);
    rho1 = randfrac();
    rho2 = randfrac();

    // Parametro de Inercia constante (Funcionalidad regular)
    w = 0.8;

    // Configuracion estandar de Parametros de Escalamiento PSO
  	c1 = 2.05;
  	c2 = 2.05;

    // Configuracion estandar de Parametro de Constriccion PSO
    double phi_T = c1 + c2;
  	epsilon = 2.0/fabs(2 - phi_T - sqrt(pow(phi_T,2) - 4*phi_T));

	// Configuracion de PSO Velocity Scaler
    V_scaler = 7.8125;

    // Ecuacion de Velocidad PSO para calculo de nueva velocidad de agente
    old_velocity[0] = new_velocity[0];
    old_velocity[1] = new_velocity[1];
    new_velocity[0] = epsilon * (w*old_velocity[0]  + c1*rho1*(best_local[0] - actual_position[0]) + c2*rho2*(best_global[0] - actual_position[0]));
    new_velocity[1] = epsilon * (w*old_velocity[1]  + c1*rho1*(best_local[1] - actual_position[1]) + c2*rho2*(best_global[1] - actual_position[1]));
  
  	// Ecuacion de Posicion PSO para calculo de nueva posicion de agente
    if (number_iteration % PSO_STEP == 0 || number_iteration == 1){
      new_position[0] = actual_position[0] + new_velocity[0] * V_scaler;
      new_position[1] = actual_position[1] + new_velocity[1] * V_scaler;
    }

    // ---------------------------- VARIABLES DE CONTROLADORES ---------------------------------

    // Inicializacion de velocidad lineal y angular
    double v = 0;                                               // Velocidad lineal de robot
    double w = 0;                                               // Velocidad angular de robot
    
    // Calculo de error de distancias
    e_x = new_position[0] - actual_position[0];                 // Error en posicion X
    e_y = new_position[1] - actual_position[1];                 // Error en posicion Y
    e_p = sqrt(pow(e_y,2) + pow(e_x,2));                        // Magnitud de error vectorial
    
    // Dimensiones de E-Puck
    double l = ROBOT_RADIUS/1000;                               // Distancia de centro a llantas en metros
    double r = WHEEL_RADIUS/1000;                               // Radio de llantas en metros
    double a = ROBOT_RADIUS/1000;                               // Distancia entre centro y punto de disomorfismo (E-puck front)
    
    // Constantes de ponderacion
    double K = 3.12*(1 - exp(-2*pow(e_p,1)))/e_p;                      // Ponderacion K en funcion de magnitud del error de posicion
    //K = 1.5;
    
    // Velocidad Lineal (aplicada con matriz de disomorfismo y 2*tanh(x) para acotar velocidades a MAX_SPEED de E-Puck (6.28 rad/s)
  	v = (2*tanh((K/MAX_SPEED)*e_x)) * cos((90 - theta_o)*M_PI/180) + 
	  	(2*tanh((K/MAX_SPEED)*e_y)) * sin((90 - theta_o)*M_PI/180);
                  
  	// Velocidad Angular (aplicada con matriz de disomorfismo y 2*tanh(x) para acotar velocidades a MAX_SPEED de E-Puck (6.28 rad/s)
  	w  =(2*tanh((K/MAX_SPEED)*e_x)) * (-sin((90 - theta_o)*M_PI/180)/a) +
		(2*tanh((K/MAX_SPEED)*e_y)) * (cos((90 - theta_o)*M_PI/180)/a);

   	// Angulo de meta calculado en orientacion de brujula desde vertical (+X en simulacion)
  	theta_g = atan2(new_position[0] - actual_position[0], new_position[1] - actual_position[1]);
    
    // Mantener angulo de orientacion de robot entre [-PI PI]
    double orientation_angle = 0;
    if (theta_o > 180){
      orientation_angle = theta_o - 360;
    } else {
      orientation_angle = theta_o;
    }
    
    // Calculo de error de distancia angular (Diferencia entre eje -Z de robot y norte, y angulo de la meta respecto a robot y norte)
    e_o = atan2(sin(theta_g - (orientation_angle*M_PI/180)), cos(theta_g - (orientation_angle*M_PI/180)));
    
    // PID de velocidad angular
    e_D = e_o - e_old;
    E_o = E_old + e_o;
    
    w = KP*e_o + KI*E_o + KD*e_D;
    

    e_old = e_o;
    E_old = E_o;

    // --------------- TRANSFORMACION DE VELOCIDADES CON MODELO DIFERENCIAL --------------------
              
    // Calculo de velocidades angulares de motores de E-Puck dependiendo la velocidad lineal y angular requeridas    
    phi_r = (v + w*l)/r;
    phi_l = (v - w*l)/r;
    
    // Truncar velocidades de rotacion de motor derecho a [-6.28, 6.28]
    if (phi_r > 0){
      if (phi_r > MAX_SPEED){
        phi_r = MAX_SPEED;
      }
    } else {
      if (phi_r < -MAX_SPEED){
        phi_r = -MAX_SPEED;
      }
    }
    
    // Truncar velocidades de rotacion de motor izquierdo a [-6.28, 6.28]
    if (phi_l > 0){
      if (phi_l > MAX_SPEED){
        phi_l = MAX_SPEED;
      }
    } else {
      if (phi_l < -MAX_SPEED){
        phi_l = -MAX_SPEED;
      }
    }
    
    // Limpieza de Picos
    if (HARDSTOP_FILTER){
      if(sqrt(pow(phi_r - PhiR_old,2)) > MAX_CHANGE){
        phi_r = (phi_r + 2*PhiR_old)/3;
      }
      PhiR_old = phi_r;
      if(sqrt(pow(phi_l - PhiL_old,2)) > MAX_CHANGE){
        phi_l = (phi_l + 2*PhiL_old)/3;
      }
      PhiL_old = phi_l;
    }
    
    // Actualizacion de velocidades de motores en simulacion
    wb_motor_set_velocity(left_motor, phi_l);
    wb_motor_set_velocity(right_motor, phi_r);
}
wb_robot_cleanup();

  return 0;
  }