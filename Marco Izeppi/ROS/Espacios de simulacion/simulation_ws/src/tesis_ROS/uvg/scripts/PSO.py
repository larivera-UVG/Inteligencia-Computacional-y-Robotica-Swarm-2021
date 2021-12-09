#!/usr/bin/env python

import rospy
from nav_msgs.msg import Odometry
from std_msgs.msg import Float32
import random as rd
import numpy as np
import math

from tf.transformations import euler_from_quaternion
from geometry_msgs.msg import Point, Twist
from math import atan2

x = 0.0
y = 0.0 
theta = 0.0



c1 = 2
c2 = 2

funcion = 0 #(0-Sphere, 1-Himmelblau, 3-Booth, 3-Rosenbrock)

dt = 0.1                    #Scaler de Velcidad

w_min = 0.5
w_max = 1
tipo_inercia = 3
Maxiter = 10000
iter = 0

W = 0

velocity = np.array([0,0])

iteracion = 0        

pBest_value = float('inf')
pBest_position =  np.array([float('inf'),float('inf')])
gBest_value = float('inf')
gBest_position =  np.array([float('inf'),float('inf')])
new_pos = np.array([float('inf'),float('inf')])
contador = 0



Inertia_type = 0            #(0-Constante, 1-Linear, 2-Caotica, 3-Exponencial)

def inercia(Inertia_type,w_min,w_max,Maxiter,iter):
    if (Inertia_type== 0):
        W = 0.8                        
    elif (Inertia_type == 1):
        W = w_max - (w_max - w_min) * iter / Maxiter
    elif (Inertia_type == 2):
        zi = 0.2
        zii = 4 * zi * (1 - zi)
        W = (w_max - w_min) * ((Maxiter - iter) / Maxiter) * w_max * zii
        iter = iter + 1
    elif (Inertia_type == 3):
        W = w_min + (w_max - w_min) * math.exp((-1 * iter) / (Maxiter / 10))

    return W


def fitness(posicion,funcion = 0):
    x = posicion.x
    y = posicion.y
    if (funcion==0):
        f = pow(x, 2) + pow(y, 2)                           #Sphere
    elif (funcion==1):                       
        f = pow(x*x + y -11,2) + pow(x + y*y -7,2)          #Himmelblau
    elif (funcion==2):
        f = pow(x + 2*y -7,2) + pow(2*x + y -5,2)           #Booth
    elif (funcion==3):
        f = pow(1-x,2) + 100*pow(y - pow(x,2),2)            #Rosenbrock
    return f


'''
def set_pBest(pB_value,posicion,funcion = 0):                                        #Costo Local
    fitness_candidate = fitness(posicion,funcion)
    if(pBest_value > fitness_candidate):
        pBest_value = fitness_candidate
        pBest_position =  np.array([posicion.x,posicion.y])




def set_gBest(gB_value,posicion,funcion = 0):                                        #Costo Global
    best_fitness_candidate = fitness(posicion,funcion)
    if(gB_value > best_fitness_candidate):
        gBest_value = best_fitness_candidate
        gBest_position =  np.array([posicion.x,posicion.y])



def update_particles(velocity,posicion,funcion = 0):                                 #Actualizando Velocidad
    global W
    new_velocity = ((W * velocity) 
    + (c1 * rd.random() * (pBest_position -  np.array([posicion.x,posicion.y])))
    + (c2 * rd.random() * (gBest_position - np.array([posicion.x,posicion.y]))))
    print(new_velocity)
    return new_velocity
'''

def callback_2(data):
    global gBest_value
    gBest_value = data.data


def callback(msg):
    global x
    global y
    global theta
    global c1
    global c2
    global funcion
    global w_min
    global w_max
    global tipo_inercia
    global Maxite
    global iter
    global pBest_value
    global pBest_position
    global gBest_value
    global gBest_position
    global new_pos




    #print(ns)
    pos = msg.pose.pose.position 


    #PBest
    fitness_candidate = fitness(pos,funcion)
    if(pBest_value > fitness_candidate):
        pBest_value = fitness_candidate
        pBest_position =  np.array([pos.x,pos.y])

    #GBest
    if(gBest_value > pBest_value):
        gBest_value = pBest_value
        gBest_position =  np.array([pos.x,pos.y])

    #Nvel
    W = inercia(Inertia_type,w_min,w_max,Maxiter,iter)

    new_velocity = ((W * velocity) + (c1 * rd.random() * (pBest_position -  np.array([pos.x,pos.y]))) + (c2 * rd.random() * (gBest_position - np.array([pos.x,pos.y]))))

    new_pos = np.array([pos.x,pos.y]) + new_velocity * 0.25

    x = msg.pose.pose.position.x
    y = msg.pose.pose.position.y

    rot_q = msg.pose.pose.orientation
    (roll, pitch, theta) = euler_from_quaternion([rot_q.x, rot_q.y, rot_q.z, rot_q.w])

    print("best_global_value")
    print(gBest_value)
'''
    print('\n')
    print(ns)
    print("nueva posicion")
    print(new_pos)
    print("posicion actual")
    print(pos)
    print("velocidad")
    print(new_velocity)
    print("mejor personal")
    print(pBest_value)
    print("mejor global")
    print(gBest_value)
    print('\n')
'''


ns = rospy.get_namespace()
rospy.init_node('mejor_global_p')


od_sub = rospy.Subscriber('/mejor_global', Float32, callback_2)
odom_sub = rospy.Subscriber('odom', Odometry, callback)
odom_pub = rospy.Publisher('mejor_global_p', Float32,queue_size = 1)
pub = rospy.Publisher("cmd_vel", Twist, queue_size = 1)

msg = Float32()

r = rospy.Rate(4)

speed = Twist()

goal = Point()
goal.x = 0
goal.y = 0

while not rospy.is_shutdown():
    msg = gBest_value
    if contador < 10:
        goal.x = new_pos[0]
        goal.y = new_pos[1]
        contador = contador +1
    else:
        goal.x = 0
        goal.y = 0

    inc_x = goal.x -x
    inc_y = goal.y -y

    angle_to_goal = atan2(inc_y, inc_x)

    if abs(angle_to_goal - theta) > 0.1:
        speed.linear.x = 0.0
        speed.angular.z = 0.3
    else:
        speed.linear.x = 0.5
        speed.angular.z = 0.0

    pub.publish(speed)
    odom_pub.publish(msg)
    r.sleep() 


'''
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
    '''
