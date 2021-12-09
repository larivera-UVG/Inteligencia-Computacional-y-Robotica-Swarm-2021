#! /usr/bin/env python

import rospy
import math as math

def find_neighbors(index, width, height, costmap, orthogonal_step_cost):
  neighbors = []
  diagonal_step_cost = orthogonal_step_cost * math.sqrt(2)

  lethal_cost = 1

  upper = index - width
  if upper > 0:
    if costmap[upper] < lethal_cost:
      step_cost = orthogonal_step_cost + costmap[upper]/255
      neighbors.append([upper, step_cost])

  left = index - 1
  if left % width > 0:
    if costmap[left] < lethal_cost:
      step_cost = orthogonal_step_cost + costmap[left]/255
      neighbors.append([left, step_cost])

  upper_left = index - width - 1
  if upper_left > 0 and upper_left % width > 0:
    if costmap[upper_left] < lethal_cost:
      step_cost = diagonal_step_cost + costmap[upper_left]/255
      neighbors.append([index - width - 1, step_cost])

  upper_right = index - width + 1
  if upper_right > 0 and (upper_right) % width != (width - 1):
    if costmap[upper_right] < lethal_cost:
      step_cost = diagonal_step_cost + costmap[upper_right]/255
      neighbors.append([upper_right, step_cost])

  right = index + 1
  if right % width != (width + 1):
    if costmap[right] < lethal_cost:
      step_cost = orthogonal_step_cost + costmap[right]/255
      neighbors.append([right, step_cost])

  lower_left = index + width - 1
  if lower_left < height * width and lower_left % width != 0:
    if costmap[lower_left] < lethal_cost:
      step_cost = diagonal_step_cost + costmap[lower_left]/255
      neighbors.append([lower_left, step_cost])

  lower = index + width
  if lower <= height * width:
    if costmap[lower] < lethal_cost:
      step_cost = orthogonal_step_cost + costmap[lower]/255
      neighbors.append([lower, step_cost])

  lower_right = index + width + 1
  if (lower_right) <= height * width and lower_right % width != (width - 1):
    if costmap[lower_right] < lethal_cost:
      step_cost = diagonal_step_cost + costmap[lower_right]/255
      neighbors.append([lower_right, step_cost])

  return neighbors

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

def distancia_euclideana(a, b):
    distancia = 0
    for i in range(len(a)):
        distancia += (a[i] - b[i]) ** 2
    return distancia ** 0.5

def D_star(start_index, goal_index, width, height, costmap, resolution, origin, grid_viz):

  # lista de nodos abiertosS
  open_list = []

  #lista de nodos que conforman el camino mas corto
  shortest_path = []

  # diccionario de nodos ya visitados
  closed_list = set()

  # diccionario de mappeo de nodos padre
  parents = dict()

  # diccionario de costos para los costos g de cada nodo
  g_costs = dict()

  # diccionario de costos para los parametros de f de cada nodo
  f_costs = dict()

  # Se determina la heuristica desde el nodo incial hasta el nodo final
  from_xy = indexar_al_mundo(start_index, width, resolution, origin)
  to_xy = indexar_al_mundo(goal_index, width, resolution, origin)
  h_cost = distancia_euclideana(from_xy, to_xy)


  # se establece un costo de 0 para el nodo inicial
  g_costs[start_index] = 0

  # se establece el costo f como la heuristica desde el nodo incial al nodo final
  f_costs[start_index] = h_cost
  
  # se agrega el nodo incial a la lista de nodos abiertos con el costo g y f
  open_list.append([start_index, h_cost])

  # bandera que nos indica si el camino mas corto se ha encontrado
  path_found = False
  rospy.loginfo('D-Star: Inicializacion completada')

  # mientras hayan nodos en la lista de nodos no visitados
  while open_list:

    # organizar los nodos no visitados de acuerdo a su costo f 
    open_list.sort(key = lambda x: x[1]) 
    current_node = open_list.pop(0)[0]

    # se agrega el nodo con menor costo f a la lista de nodos ya visitados
    closed_list.add(current_node)

    grid_viz.set_color(current_node,"pale yellow")

    if current_node == goal_index:
      path_found = True
      break

    neighbors = find_neighbors(current_node, width, height, costmap, resolution)

    for neighbor_index, step_cost in neighbors:

      # revisar si el nodo ya ha sido visitado
      if neighbor_index in closed_list:
        continue

      # se calcula el costo g si el nodo avanza
      g_cost = g_costs[current_node] + step_cost

      # se determina la heuristica del nodo nuevo a el nodo meta
      from_xy = indexar_al_mundo(neighbor_index, width, resolution, origin)
      to_xy = indexar_al_mundo(goal_index, width, resolution, origin)
      h_cost = distancia_euclideana(from_xy, to_xy)

      # se calcula el costo total del nodo
      f_cost = g_cost + h_cost

      in_open_list = False

      for idx, element in enumerate(open_list):
        if element[0] == neighbor_index:
          in_open_list = True
          break

      # si el nodo ya esta en la lista de nodos no visitados
      if in_open_list:
        if f_cost < f_costs[neighbor_index]:
          g_costs[neighbor_index] = g_cost
          f_costs[neighbor_index] = f_cost
          parents[neighbor_index] = current_node
          open_list[idx] = [neighbor_index, f_cost]

      # si el nodo no esta en la lista de nodos no visitados
      else:
        
        g_costs[neighbor_index] = g_cost
        f_costs[neighbor_index] = f_cost
        parents[neighbor_index] = current_node
        open_list.append([neighbor_index, f_cost])

        grid_viz.set_color(neighbor_index,'orange')

  rospy.loginfo('D-Star: Finalizado')

  if not path_found:
    rospy.logwarn('D-Star: No se encontro un camino')
    return shortest_path

  # Reconstruccion del camino
  if path_found:
      node = goal_index
      shortest_path.append(goal_index)
      while node != start_index:
          shortest_path.append(node)
          node = parents[node]
  # reverse list
  shortest_path = shortest_path[::-1]
  rospy.loginfo('D-Star: Done reconstructing path')


  return shortest_path

