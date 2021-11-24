#!/usr/bin/env python

import rospy
from nav_msgs.msg import Odometry

def Callback(msg):
    #print(ns)
    position = msg.pose.pose.position 
    print("x")
    print(position.x)
    print("y")
    print(position.y)
    
rospy.init_node('check_odometry')

# Para mas de 1 robot en uso, habilitar estas lineas
#ns = rospy.get_namespace()
#odom_sub = rospy.Subscriber('odom', Odometry, Callback)
#
#
odom_sub = rospy.Subscriber('robot1/odom', Odometry, Callback)
# esta linea de codigo mantiene el funcionamiento hasta que se detiene el nodo
rospy.spin()