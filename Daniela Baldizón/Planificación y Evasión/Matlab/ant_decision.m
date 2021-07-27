% Algoritmo 17.1 Artificial Ant Decision Process
% Computational Intelligence an Introduction
% Modificación
% Ecuación 8 An improved Ant Colony Algorithm for Robot Path Planning and
% Obstacle Avoidance
% Parámetros:
% alpha, beta y gamma son constantes definidas en el main
% G es el grafo
% id es el id del current node, tipo string. ejemplo:
% "1"
% vecinos del nodo tipo string array. ejemplo:
% ["2"]
% ["11"]
% ["12"]
% Output:
% String con el id del siguiente nodo del trayecto. ejemplo:
% "3"
% S es la distancia más corta sana entre la casilla actual y la casilla
% objetivo

function next_node = ant_decision(vecinos, alpha, beta, gamma, G, id, S)
index_edges = findedge(G, repmat(convertCharsToStrings(id), size(vecinos)), vecinos);
tau = G.Edges.Weight(index_edges);
eta = G.Edges.Eta(index_edges);
eta = eta.^(-1);
lambda = 1/S;
w = tau.^alpha.*eta.^beta*lambda^gamma;
probabilidad = w/sum(w);
I = rouletteWheel(probabilidad);
next_node = vecinos(I); % ID del vecino electo
end