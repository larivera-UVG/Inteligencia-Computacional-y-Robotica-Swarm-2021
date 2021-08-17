/*
 * File:          Epuck_PSO.c
 * Date:
 * Description:
 * Author:
 * Modifications:
 */

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


#define TIME_STEP 64
#define MAX_SPEED 2
#define USE_BEARING 0
#define COMMUNICATION_CHANNEL 1
#define ROBOT_RADIUS 35.0
#define WHEEL_RADIUS 20.5

#define TIME_DELTA 0.032

#define PSO_STEP 1

#define MAX_CHANGE 1.00

static const double *position_robot;            
static double new_position[] = {0,0};          
static double new_velocity[] = {0,0};        
static double theta_g = 0;                     

static double actual_position[] = {0,0};       
static double old_velocity[] = {0,0};        
static double theta_o = 0;                  
static double func_actual = 0;             

static double best_local[] = {0,0};
static double func_local = 0;

static double best_global[] = {0,0};
static double func_global = 0;

static double reception[] = {0,0};

static double epsilon = 0.8;

      
static double c1 = 2;             
static double c2 = 10;                 
static double w = 0;                             
static double rho1 = 0;                           
static double rho2 = 0;

                           
static double V_scaler = TIME_DELTA;              

static double w_min = 0.5;                        
static double w_max = 1.0;                        
static int MAXiter = 10000;                       
static int iter = 0;                             

// PID
static double KP = 0.5;               
static double KI = 0.1;                   
static double KD =  0.001; 

static double e_old = 0;                          
static double E_old = 0;                          
static double e_o = 0;                           
static double E_o = 0;                           
static double e_D = 0;                            

// LQI
static double delta_t = TIME_DELTA;

double Cr[2][2] = {{1,0},{0,1}};
               
static double XI_X = 0;                           
static double XI_Y = 0;                         

static double e_x = 0;                            
static double e_y = 0;                        
static double e_p = 0;   

static int num_sensors = 8; // número de sensores
char sensors_name[] = "ps0"; // nombre genérico de los sensores
static WbDeviceTag sensors[8]; 
double sensors_value[8];                        

// PID = 1 ; LQR = 2 ; LQI = 3
#define SELECTOR  2

// FUNCION COSTO (0 - Sphere, 1 - Rosenbrock, 2 - Booth, 3 - Himmelblau)
#define COST_FUNC 0

static double phi_r = 0;                         
static double phi_l = 0;    
int i;                     

double l = ROBOT_RADIUS/1000;                               
double r = WHEEL_RADIUS/1000;                               
double a = ROBOT_RADIUS/1000;                       

bool state = false;
static int number_iteration = 0;

double heuristics (double xstart, double ystart,double xgoal,double ygoal){
   return ((xgoal-xstart)*(xgoal-xstart)) + ((ygoal-ystart)*(ygoal-ystart));
};


double printRandoms(int lower, int upper, int count) { 
  int i;
  double num = 0;
  
  for (i = 0; i < count; i++) { 
    num = (rand() % (upper - lower + 1)) + lower; 
  } 
  return num;
}

double randfrac(void) {
  double res = (rand() % RAND_MAX) / (double)RAND_MAX;
  return res;
}

//FUNCIONES
double funcion(double x, double y){
  double r = 0;
  if (COST_FUNC == 0){
    r = pow(x,2) + pow(y,2);                            // Sphere 
  } else if (COST_FUNC == 1){
    r = pow(1 - x,2) + 100 * pow(y - pow(x,2),2);       // Rosenbrock 
  } else if (COST_FUNC == 2){
    r = pow(x + 2*y - 7,2) + pow(2*x + y - 5,2);        // Booth 
  } else if (COST_FUNC == 3){
    r = pow(x*x + y - 11,2) + pow(x + y*y - 7,2);       // Himmelblau 
  }
  return r;
}

int main() {

  // Inicializar WeBots Controlled Library
  wb_robot_init();
  
  // Se habilitan los sensores de distancia uno por uno 
  for (i = 0; i < num_sensors; i++) 
  {
    sensors[i] = wb_robot_get_device(sensors_name);
    wb_distance_sensor_enable(sensors[i], TIME_STEP);
    sensors_name[2]++;
  } 
  
  
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
    if (state == false){
      position_robot = wb_gps_get_values(gps);
      actual_position[0] = position_robot[2];   
      actual_position[1] = position_robot[0];  
      best_local[0] = actual_position[0];
      best_local[1] = actual_position[1];
      best_global[0] = actual_position[0];
      best_global[1] = actual_position[1];
      
      func_local = funcion(actual_position[0], actual_position[1]);
      func_global = funcion(actual_position[0], actual_position[1]);
      state = true;
    }
    
    position_robot = wb_gps_get_values(gps);                   
    
    const double *north = wb_compass_get_values(compass);
    double rad = atan2(north[0], north[2]);
    
    double bearing = ((rad) / M_PI) * 180.0;
    theta_o = bearing + 180;
    double bearing2 = ((rad) / M_PI) * 180.0;
    if (bearing2 < 0){   
      bearing2 = 360 + bearing2;           
    } 
                                          
    double theta_o2 = 270 - bearing2;
    if (theta_o2 < 0){
      theta_o2 = theta_o2 + 360;           
    }
    
    if (USE_BEARING == 0){
      theta_o = theta_o2;
    }                                                
     
    actual_position[0] = position_robot[2];
    actual_position[1] = position_robot[0];
    func_actual = funcion(actual_position[0], actual_position[1]);
    
    if (func_actual < func_local){
      best_local[0] = actual_position[0];
      best_local[1] = actual_position[1];
      func_local = func_actual;
    }
    
    double transmission[3] = {best_local[0],best_local[1],func_local};
    wb_emitter_send(emitter, transmission, 3 * sizeof(double));
    
    while (wb_receiver_get_queue_length(receiver) > 0) {
      const double *buffer = wb_receiver_get_data(receiver);
      reception[0] = buffer[0];
      reception[1] = buffer[1];
      reception[2] = buffer[2];
      
      if (reception[2] < func_global){
        best_global[0] = reception[0];
        best_global[1] = reception[1];
        func_global = reception[2];
      }
      wb_receiver_next_packet(receiver);
    }
    
    if (func_local < func_global){
      best_global[0] = best_local[0];
      best_global[1] = best_local[1];
      func_global = func_local;
    }
    
    // Se obtienen las mediciones de distancia de cada uno de los sensores
    for (i = 0; i < num_sensors; i++)
    {
      sensors_value[i] = wb_distance_sensor_get_value(sensors[i]);
    }
    
    //Definicion de constantes
    
    rho1 = randfrac();
    rho2 = randfrac();
    w = w_min + (w_max - w_min)*exp((-1*iter)/(MAXiter/10));
    iter = iter + 1;

    c1 = 2.05;
    c2 = 2.05;
    
    double phi_T = c1 + c2;
    epsilon = 2.0/fabs(2 - phi_T - sqrt(pow(phi_T,2) - 4*phi_T));
    
    V_scaler = 0.25;
    
    
    old_velocity[0] = new_velocity[0];
    old_velocity[1] = new_velocity[1];
    new_velocity[0] = epsilon * (w*old_velocity[0]  + c1*rho1*(best_local[0] - actual_position[0]) + c2*rho2*(best_global[0] - actual_position[0]));
    new_velocity[1] = epsilon * (w*old_velocity[1]  + c1*rho1*(best_local[1] - actual_position[1]) + c2*rho2*(best_global[1] - actual_position[1]));
  
    if (number_iteration % PSO_STEP == 0 || number_iteration == 1){
      new_position[0] = actual_position[0] + new_velocity[0] * V_scaler;
      new_position[1] = actual_position[1] + new_velocity[1] * V_scaler;
    }
    
    double v = 0;                                               
    double w = 0;                                               
    

    e_x = new_position[0] - actual_position[0];                 
    e_y = new_position[1] - actual_position[1];                
    e_p = sqrt(pow(e_y,2) + pow(e_x,2)); 
    printf("x pos: %2.2f - y pos: %2.2f \n",e_x,e_y);                    
    
                           
    double K = 3.12*(1 - exp(-2*pow(e_p,1)))/e_p;                      
    if (USE_BEARING == 0){
      v = (2*tanh((K/MAX_SPEED)*e_x)) * cos((theta_o)*M_PI/180) + 
          (2*tanh((K/MAX_SPEED)*e_y)) * sin((theta_o)*M_PI/180);
      w  = (2*tanh((K/MAX_SPEED)*e_x)) * (-sin((theta_o)*M_PI/180)/a) +
           (2*tanh((K/MAX_SPEED)*e_y)) * (cos((theta_o)*M_PI/180)/a);
    } else {
      v = (2*tanh((K/MAX_SPEED)*e_x)) * cos((90 - theta_o)*M_PI/180) + 
          (2*tanh((K/MAX_SPEED)*e_y)) * sin((90 - theta_o)*M_PI/180);
      w  = (2*tanh((K/MAX_SPEED)*e_x)) * (-sin((90 - theta_o)*M_PI/180)/a) +
           (2*tanh((K/MAX_SPEED)*e_y)) * (cos((90 - theta_o)*M_PI/180)/a);
    }


    if (USE_BEARING == 0){
      theta_g = atan2(new_position[1] - actual_position[1], new_position[0] - actual_position[0]);
    } else {
      theta_g = atan2(new_position[0] - actual_position[0], new_position[1] - actual_position[1]);
    }
    
    double orientation_angle = 0;
    if (theta_o > 180){
      orientation_angle = theta_o - 360;
    } else {
      orientation_angle = theta_o;
    }
  
    e_o = atan2(sin(theta_g - (orientation_angle*M_PI/180)), cos(theta_g - (orientation_angle*M_PI/180)));
    e_D = e_o - e_old;
    E_o = E_old + e_o;
    if (SELECTOR == 1){
      w = KP*e_o + KI*E_o + KD*e_D;
    }
    
   // Controlador LQR
    if (SELECTOR == 2){
      double K_x = 0.1;
      double K_y = 0.1;
      double u_1 = K_x*e_x;             
      double u_2 = K_y*e_y;
      
      u_1 = -K_x*(actual_position[0] - new_position[0]);
      u_2 = -K_y*(actual_position[1] - new_position[1]);
      
      v = u_1*cos((theta_o)*M_PI/180) + u_2*sin((theta_o)*M_PI/180);
      w = (-u_1*sin((theta_o)*M_PI/180) + u_2*cos((theta_o)*M_PI/180))/l;
    }
    
    // Controlador LQI
    if (SELECTOR == 3){
      double Klqr_x = 0.2127;
      double Klqr_y = 0.2127;
      double Klqi_x = -0.0224;
      double Klqi_y = -0.0224;
      
      double bv_p = 0.95;          
      double bv_i = 0.01;         
      double u_1 = -Klqr_x*(1 - bv_p)*(actual_position[0] - new_position[0]) - Klqi_x*XI_X;
      double u_2 = -Klqr_y*(1 - bv_p)*(actual_position[1] - new_position[1]) - Klqi_y*XI_Y;
      
      XI_X = XI_X + (best_global[0] - actual_position[0])*delta_t;
      XI_Y = XI_Y + (best_global[1] - actual_position[1])*delta_t;
      XI_X = (1 - bv_i)*XI_X;
      XI_Y = (1 - bv_i)*XI_Y;
      
      v = u_1*cos((theta_o)*M_PI/180) + u_2*sin((theta_o)*M_PI/180);
      w = (-u_1*sin((theta_o)*M_PI/180) + u_2*cos((theta_o)*M_PI/180))/l;
    }
    
    double dist_a_g = heuristics(actual_position[0],actual_position[1],new_position[0],new_position[1]);

    if (fabs(e_p) < 0.001){
      v = 0;
      w = 0;
    }

    phi_r = (v + w*l)/r;
    phi_l = (v - w*l)/r;
    
    if (phi_r > 0){
      if (phi_r > MAX_SPEED){
        phi_r = MAX_SPEED;
      }
    } else {
      if (phi_r < -MAX_SPEED){
        phi_r = -MAX_SPEED;
      }
    }
    
    if (phi_l > 0){
      if (phi_l > MAX_SPEED){
        phi_l = MAX_SPEED;
      }
    } else {
      if (phi_l < -MAX_SPEED){
        phi_l = -MAX_SPEED;
      }
    }
 
    if(sensors_value[6] > 80 ){
          phi_l = 6.28;
          phi_r = 0;
     }
     if(sensors_value[1] > 80){
          phi_r = 6.28;
          phi_l = 0;
     }        
    
    if (dist_a_g < 0.005){
        phi_r = 0;
        phi_l = 0;
    }
       
    wb_motor_set_velocity(left_motor, phi_l);
    wb_motor_set_velocity(right_motor, phi_r);
    
  }

  wb_robot_cleanup();

  return 0;
}