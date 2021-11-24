% GRAFICAR RESULTADOS
% Daniela Baldizón
% 15-09-2021
% Grafica los resultados obtenidos con el mapa A

clc
clear
load MapaDC3B1.mat
formatSpec = 'iter: %d - t: %.2f - cost: %.2f \n';
fprintf(formatSpec, t-1, tiempofinal/(60), moda)
