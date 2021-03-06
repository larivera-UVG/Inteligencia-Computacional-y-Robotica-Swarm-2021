% Algoritmo 17.1 Artificial Ant Decision Process
% Computational Intelligence an Introduction
% Parámetros:
% alpha y beta son constantes definidas en el main
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
function next_node = ant_decision(vecinos, alpha, beta, G)
tau = G.Nodes.Weight(str2double(vecinos));
eta = G.Nodes.Eta(str2double(vecinos));
w = tau.^alpha.*eta.^beta;
probabilidad = w/sum(w);
I = rouletteWheel(probabilidad);
next_node = vecinos(I); % ID del vecino electo
end


