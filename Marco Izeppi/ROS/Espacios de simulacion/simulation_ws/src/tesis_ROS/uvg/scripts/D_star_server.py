#!/usr/bin/env python

import rospy
from pp_msgs.srv import PathPlanningPlugin, PathPlanningPluginResponse
from geometry_msgs.msg import Twist
from gridviz import GridViz
from D_star import D_star

def make_plan(req):
  # costmap as 1-D array representation
  costmap = req.costmap_ros
  # number of columns in the occupancy grid
  width = req.width
  # number of rows in the occupancy grid
  height = req.height
  start_index = req.start
  goal_index = req.goal
  # size of each grid map square in meters
  resolution = 0.2
  # origin of grid map
  origin = [-7.4, -7.4, 0]

  viz = GridViz(costmap, resolution, origin, start_index, goal_index, width)

  # time statistics
  start_time = rospy.Time.now()

  # calculate the shortes path using Dijkstra
  path = D_star(start_index, goal_index, width, height, costmap, resolution, origin, viz)

  if not path:
    rospy.logwarn("No path returned by D_star shortes path algorithm")
    path = []
  else:
    data = indexar_al_mundo(goal_index, width, resolution, origin)
    print("D")
    print("\n")
    print(data)
    print("\n")
    print(path)
    print("\n")

    execution_time = rospy.Time.now() - start_time
    print("\n")
    rospy.loginfo('++++++++ D_star execution metrics ++++++++')
    rospy.loginfo('Total execution time: %s seconds', str(execution_time.to_sec()))
    rospy.loginfo('++++++++++++++++++++++++++++++++++++++++++++')
    print("\n")
    rospy.loginfo('a_star: Path sent to navigation stack')

  resp = PathPlanningPluginResponse()
  resp.plan = path
  return resp

def indexar_al_mundo(flatmap_index, map_width, map_resolution, map_origin = [0,0]):
    """
    Converts a flatmap index value to world coordinates (meters)
    flatmap_index: a linear index value, specifying a cell/pixel in an 1-D array
    map_width: number of columns in the occupancy grid
    map_resolution: side lenght of each grid map cell in meters
    map_origin: the x,y position in grid cell coordinates of the world's coordinate origin
    Returns a list containing x,y coordinates in the world frame of reference
    """
    # convert to x,y grid cell/pixel coordinates
    grid_cell_map_x = flatmap_index % map_width
    grid_cell_map_y = flatmap_index // map_width
    # convert to world coordinates
    x = map_resolution * grid_cell_map_x + map_origin[0]
    y = map_resolution * grid_cell_map_y + map_origin[1]

    return [x,y]


def clean_shutdown():
  cmd_vel.publish(Twist())
  rospy.sleep(1)

if __name__ == '__main__':
  rospy.init_node('dstar_path_planning_service_server', log_level=rospy.INFO, anonymous=False)
  make_plan_service = rospy.Service("/move_base/SrvClientPlugin/make_plan", PathPlanningPlugin, make_plan)
  cmd_vel = rospy.Publisher('/cmd_vel', Twist, queue_size=5)
  rospy.on_shutdown(clean_shutdown)

  while not rospy.core.is_shutdown():
    rospy.rostime.wallsleep(0.5)
  rospy.Timer(rospy.Duration(2), rospy.signal_shutdown('Shutting down'), oneshot=True)


