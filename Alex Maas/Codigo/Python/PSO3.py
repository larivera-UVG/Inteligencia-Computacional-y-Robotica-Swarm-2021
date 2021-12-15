import random as rd
import numpy as np
import matplotlib.pyplot as plt
import math

#-------------------------- Parametros PSO -------------------------------
c1 = 2
c2 = 10

n_iterations = 100           #No. de iteraciones (100 funciona bien para comparacion con matlab)
n_particles = 100
target_error = 1e-6
dt = 0.1                    #Scaler de Velcidad
funcion = 1                 #(0-Sphere, 1-Himmelblau, 2-Booth, 3-Rosenbrock)

# Calculo de Inercia PSO 
w_min = 0.5
w_max = 1
Maxiter = 10000
iter = 0

Inertia_type = 2            #(0-Constante, 1-Linear, 2-Caotica, 3-Exponencial)
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
# ----------------------------------------------------------------


class Particula():
    def __init__(self):
        x = (-1) ** bool(rd.getrandbits(1))*rd.random()*5       #Limitar espacio de trabajo [-5,5]
        y = (-1) ** bool(rd.getrandbits(1))*rd.random()*5       #Limitar espacio de trabajo [-5,5]
        self.position = np.array([x,y])
        self.pBest_position = self.position
        self.pBest_value = float('inf')                         #Costo inicial infinito
        self.velocity = np.array([0,0])

    def update(self):
        self.position = self.position + self.velocity*dt        #Actualizacion de posicion

class Espacio():
    def __init__(self, target, target_error, n_particles):
        self.target = target
        self.target_error = target_error
        self.n_particles = n_particles
        self.particles = []
        self.gBest_value = float('inf')
        self.gBest_position = np.array([rd.random() * 50, rd.random() * 50])
            
    def fitness(self, particle):
        x = particle.position[0]
        y = particle.position[1]
        if (funcion==0):
            f = pow(x, 2) + pow(y, 2)                           #Sphere
        elif (funcion==1):                       
            f = pow(x*x + y -11,2) + pow(x + y*y -7,2)          #Himmelblau
        elif (funcion==2):
            f = pow(x + 2*y -7,2) + pow(2*x + y -5,2)           #Booth
        elif (funcion==3):
            f = pow(1-x,2) + 100*pow(y - pow(x,2),2)            #Rosenbrock
        return f
    
    def set_pBest(self):                                        #Costo Local
        for particle in self.particles:
            fitness_candidate = self.fitness(particle)
            if(particle.pBest_value > fitness_candidate):
                particle.pBest_value = fitness_candidate
                particle.pBest_position = particle.position

    def set_gBest(self):                                        #Costo Global
        for particle in self.particles:
            best_fitness_candidate = self.fitness(particle)
            if(self.gBest_value > best_fitness_candidate):
                self.gBest_value = best_fitness_candidate
                self.gBest_position = particle.position
                
    def update_particles(self):                                 #Actualizando Velocidad
        for particle in self.particles:
            global W
            new_velocity = ((W * particle.velocity) 
            + (c1 * rd.random() * (particle.pBest_position - particle.position))
            + (c2 * rd.random() * (self.gBest_position - particle.position)))
            particle.velocity = new_velocity
            particle.update()
            
    def visualizar(self, iteration):        
        print(iteration, 'iteraciones')
        print('La mejor posicion es:', self.gBest_position)
        print('El mejor costo es:', self.gBest_value)
        for particle in self.particles:
            plt.plot(particle.position[0], particle.position[1], 'ro')      #Plot particulas (rojo)
        plt.plot(self.gBest_position[0], self.gBest_position[1], 'bo')      #Plot mejor particula (azul)
        plt.suptitle("Himmelblau:100 agentes Inercia Caótica", 
         fontdict={'family': 'serif', 
                    'color' : 'darkblue',
                    'weight': 'bold',
                    'size': 16})
        plt.xlim(-5,5)
        plt.ylim(-5,5)
        feature_x = np.linspace(-5,5,100)
        feature_y = np.linspace(-5,5,100)
        [X , Y] = np.meshgrid(feature_x, feature_y)
        #fig, ax = plt.subplots(1,1)
        Z = pow(X*X + Y -11,2) + pow(X + Y*Y -7,2) 
        #Z = pow(X, 2) + pow(Y, 2)
        plt.contour(X, Y, Z, 20)
        plt.title("Global best: [ 3.000000 2.000000]")
        plt.show() #Caótica Himmelblau Sphere

search_space = Espacio(1, target_error, n_particles)
particle_vector = [Particula() for _ in range(search_space.n_particles)]
search_space.particles = particle_vector

iteration = 0
while(iteration < n_iterations):
    search_space.set_pBest()
    search_space.set_gBest()
    if(abs(search_space.gBest_value - search_space.target) <= search_space.target_error):
        break
        
    search_space.update_particles()
    iteration += 1
    
print("La mejor posicio Final es: ", search_space.gBest_position, " en ", iteration, "iteraciones")
search_space.visualizar(iteration)  
