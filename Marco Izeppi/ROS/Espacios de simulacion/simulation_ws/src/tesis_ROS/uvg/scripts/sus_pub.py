#!/usr/bin/env python

import rospy
from nav_msgs.msg import Odometry
from std_msgs.msg import Float32

best_global = float('inf')
best_prev_global = float('inf')

def Callback(msg):
    global best_global

    best_prev_global = msg.data

    if(best_prev_global < best_global):
        best_global =  best_prev_global
        

    
rospy.init_node('mejor_global')


# Para mas de 1 robot en uso, habilitar estas lineas
#ns = rospy.get_namespace()
#odom_sub = rospy.Subscriber('odom', Odometry, Callback)
#
#
odom_sub = rospy.Subscriber('robot1/mejor_global_p', Float32, Callback)
odom_sub2 = rospy.Subscriber('robot2/mejor_global_p', Float32, Callback)
odom_sub3 = rospy.Subscriber('robot3/mejor_global_p', Float32, Callback)
odom_pub = rospy.Publisher('mejor_global', Float32,queue_size = 1)
odom_pub2 = rospy.Publisher('mejor_posicion', Odometry,queue_size = 10)

odom = Odometry()
odom.pose.pose.position.x = 2
odom.pose.pose.position.y = 2


msg = Float32()

rate = rospy.Rate(1)

while not rospy.is_shutdown():
    msg = best_global
    odom_pub.publish(msg)
    odom_pub2.publish(odom)
    rate.sleep()
