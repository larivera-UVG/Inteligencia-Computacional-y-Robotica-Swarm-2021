/*
 * File:          lab10_controller.c
 * Date:
 * Description:
 * Author:
 * Modifications:
 */

/*
 * You may need to add include files like <webots/distance_sensor.h> or
 * <webots/motor.h>, etc.
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <webots/motor.h>
#include <webots/robot.h>
#include <webots/distance_sensor.h>
#include <webots/gps.h>

#include <webots/compass.h>

/*
 * You may want to add macros here.
 */
#define TIME_STEP 64

// maximal speed allowed
#define MAX_SPEED 5.24

// robot wheel radius
#define WHEEL_RADIUS (195/2.0)

// robot's wheel distance from center
#define DISTANCE_FROM_CENTER (381/2.0)

//tamaño en metros de la atena
#define RECTANGLE_ARENA_SIZE 2

// 1m por cuadrado/ 0.5 por tile
#define TILES_PER_M 2 

#define GRID_SIZE (RECTANGLE_ARENA_SIZE*TILES_PER_M)
// 1 para cada lado - 0.5 por cada cuadrado de la cuadricula

#define SIZE ((GRID_SIZE*GRID_SIZE)-1)


static int num_sensors = 8; // número de sensores
char sensors_name[] = "ps0"; // nombre genérico de los sensores
static WbDeviceTag sensors[8];

// Cambia entre PID, LQI y no lineal moderno.
char tipo_control = 1;

// PID exponencial
double v0 = MAX_SPEED;
double alpha =100;
double eO_1 = 0, EO = 0;
double EP =0;
double eP_1 = 0;

// Constantes PIDs
double kpO = 0.2;
double kiO = 0.001;
double kdO = 0.0005;

// LQI
double Cr[2][2] = {{1,0},{0,1}};

// No lineal moderno
double krho = 0.3;
double kalpha = 0.8;
double kbeta = -0.5;

// Especifico del robot
double l = 190.5/100.0;
double r = 97.5/100.0;
int i;
int j;
int c;
int posmin1 = 0;
int posmin2 = 0;
double k_m = 0;

//QUEUE para los puntos del algoritmo
double Queue[SIZE][2];
double g[SIZE][SIZE];
double rhs[SIZE][SIZE];

double grid[SIZE][SIZE][2];

double start[2] = {-0.875,-0.875};
double goal[2] = {0,0};
double last[2] = {0,0};


double min(double g[], double rhs[]){

  for (c = 0; c < SIZE; c++){
    if (g[c] < g[posmin1]){
      posmin1 = c;
    }
  }
  
  for (c = 0; c < SIZE; c++){
    if (rhs[c] < rhs[posmin2]){
      posmin2 = c;
    }
  }

  if(g[posmin1] < rhs[posmin2]){
    return g[posmin1];  
  }
  else{
    return rhs[posmin2];
  } 
};

double heuristics (double xstart, double ystart,double xgoal,double ygoal){
   return ((xgoal-xstart)*(xgoal-xstart)) + ((ygoal-ystart)*(ygoal-ystart));
};

//bool calculate_key(double s[]){
//  k1 = min(g[s],rhs[s] + heuristics(x,y,s[0],s[1]) + k_m);
//}

double sort (double array[]){
  int counter, counter1, swap_var;
  for (counter = 0 ; counter < SIZE; counter++)
  {
    for (counter1 = 0 ; counter1 < SIZE - counter; counter1++)
    {
      if (array[counter1] > array[counter1+1]) /* For decreasing order use < */
      {
        swap_var        = array[counter1];
        array[counter1]   = array[counter1+1];
        array[counter1+1] = swap_var;
      }
    }
  }
  return array[SIZE];
};

/*
 * This is the main program.
 * The arguments of the main function can be specified by the
 * "controllerArgs" field of the Robot node
 */
int main(int argc, char **argv) {
  /* necessary to initialize webots stuff */
  wb_robot_init();

  /*
   * You should declare here WbDeviceTag variables for storing
   * robot devices like this:
   *  WbDeviceTag my_sensor = wb_robot_get_device("my_sensor");
   *  WbDeviceTag my_actuator = wb_robot_get_device("my_actuator");
   */
  // device IDs for the wheels
  WbDeviceTag left_wheel = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_wheel = wb_robot_get_device("right wheel motor");
   
  // device ID for the GPS and the compass
  WbDeviceTag position_sensor = wb_robot_get_device("gps");
  WbDeviceTag orientation_sensor = wb_robot_get_device("compass");
  
  // Se habilitan los sensores de distancia uno por uno 
  for (i = 0; i < num_sensors; i++) 
  {
    sensors[i] = wb_robot_get_device(sensors_name);
    wb_distance_sensor_enable(sensors[i], TIME_STEP);
    sensors_name[2]++;
    printf(sensors_name);
  } 
   
  // sets up wheels
  wb_motor_set_position(left_wheel, INFINITY);
  wb_motor_set_position(right_wheel, INFINITY);
  wb_motor_set_velocity(left_wheel, 0.0);
  wb_motor_set_velocity(right_wheel, 0.0);
  
  // enables GPS and compass
  wb_gps_enable(position_sensor, TIME_STEP);
  wb_compass_enable(orientation_sensor, TIME_STEP);
  
  /* main loop
   * Perform simulation steps of TIME_STEP milliseconds
   * and leave the loop when the simulation is over
   */
   
  //printf("Grid:\n");
  for (i = 0; i < SIZE; i++)
  {
    for (j = 0; j < SIZE; j++)
    {
      grid[i][j][0] = start[0]+0.125*i;
      grid[i][j][1] = start[1]+0.125*j;
      //printf("{%1.3f,%1.3f},",grid[i][j][0],grid[i][j][1]);
    }
    //printf("\n");
  }
  //printf("END\n");
  // inicializamos el queue en infinito
  for (i = 0; i < SIZE; i++)
  {
      Queue[i][0]= INFINITY;
      Queue[i][1]= INFINITY;
  };

  //inicializamos toda la matriz en 1
  //printf("rhs:\n");
  for (i = 0; i < SIZE; i++)
  {
    for (j = 0; j < SIZE; j++)
    {
        rhs[i][j] = INFINITY;
        g[i][j] = INFINITY;
        if(grid[i][j][0] == goal[0] && grid[i][j][1] == goal[1]){
          rhs[i][j] = 0;
          g[i][j] = 0;
        }
        //printf("{%1.3f},",rhs[i][j]);
    };
    //printf("\n");
  };
  //printf("END\n");

  //definimos las posiciones
  for (i = 0; i < SIZE; i++)
  {
    for (j = 0; j < SIZE; j++)
    {
        grid[i][j][0] = j*0.25 + 0.125;
        grid[i][j][1] = i*0.25 + 0.125;
    };
  };
  
  last[0] = start[0];
  last[1] = start[1];

  
   
  while (wb_robot_step(TIME_STEP) != -1) {
    /*
     * Read the sensors :
     * Enter here functions to read sensor data, like:
     *  double val = wb_distance_sensor_get_value(my_sensor);
     */
     
     const double *gps_val = wb_gps_get_values(position_sensor);

     const double *north = wb_compass_get_values(orientation_sensor);
     double bearing = atan2(north[0], north[2]);

     //Meta establecida en (1,1) <---------------------------------------
     double xg = 0.75;
     double yg = 0.75;

     //posicion actual
     double x = gps_val[0];
     double y = gps_val[2];
     start[0] = x;
     start[1] = y;
     
     double dist_s_g = heuristics(x,y,xg,yg);
     
     double theta = bearing;
     
     double sensors_value[8];
    
    // Se obtienen las mediciones de distancia de cada uno de los sensores
    for (i = 0; i < num_sensors; i++)
    {
      sensors_value[i] = wb_distance_sensor_get_value(sensors[i]);
      printf("sensor %i = %f\n ",i,sensors_value[i]);
    }
    /* Process sensor data here */
    /*
     * Enter here functions to send actuator commands, like:
     * wb_motor_set_position(my_actuator, 10.0);
     */
     

      /* Enter your cleanup code here */
     // Controladores  
     
     if (tipo_control == 1)
     {
     
      // PID exponencial  
      double ex = xg - x;
      double ey = yg - y;
      double thetag = atan2(ey,ex);
      
      double eP = sqrt(ex*ex+ey*ey);
      double eO = thetag - theta;
             eO = atan2(sin(eO), cos(eO));
             
      // Control de velocidad lineal
      double kP = v0*(1-exp(-alpha*eP*eP))/eP;
      double v = kP*eP;
      
      // Control de velocidad angular
      double eO_D = eO - eO_1;
      double val = EO;
      double EO = val + eO;
      double w = kpO*eO + kiO*EO + kdO*eO_D;
             eO_1 = eO;
             
         
      double rw = (v+w*l)/r;
      double lw = (v-w*l)/r;
      
      if(sensors_value[6] > 80){
        lw = 6.28;
        rw = 0;
        
        }
      if(sensors_value[1] > 80){
        rw = 6.28;
        lw = 0;
        
        }
      
      
      if(rw > 6.28){
        rw = 6.28;
      }
      
      if(lw > 6.28){
        lw = 6.28;
      }
      
      if(dist_s_g < 0.0001){
        lw = 0;
        rw = 0;
      }
      
      wb_motor_set_velocity(right_wheel, rw);
      wb_motor_set_velocity(left_wheel, lw);
      }
    
    };

  /* This is necessary to cleanup webots resources */
  wb_robot_cleanup();
  return 0;
};

