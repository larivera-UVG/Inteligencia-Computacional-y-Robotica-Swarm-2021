% GRAFICAR RESULTADOS
% Daniela Baldizón
% 15-09-2021
% Grafica los resultados obtenidos con el mapa A

clc
clear
load mapaAC3.mat
formatSpec = 'iter: %d - t: %.2f - cost: %.2f \n';
fprintf(formatSpec, t-1, tiempofinal/(60*60), moda)

%% 

figure(1); clf;
hold on 
nodos_especiales = [G.Nodes.X(str2double(nodo_init)), G.Nodes.Y(str2double(nodo_init)); G.Nodes.X(str2double(nodo_dest)), G.Nodes.Y(str2double(nodo_dest))];
scatter(nodos_especiales(1, 1)+0.5, nodos_especiales(1, 2)+0.5, 'g','filled')
scatter(nodos_especiales(2, 1)+0.5, nodos_especiales(2, 2)+0.5, 'xr','LineWidth', 5)

[X,Y]=meshgrid(1:grid_size+1);    % Creación de la cuadrícula
plot(X,Y,'k');          % Dibuja las líneas verticales
plot(Y,X,'k');          % Dibuja las líneas horizontales
axis off

% Se le hace un borde al mapa para delimitarlo, este borde puede ser tomado
% como obstáculo
for i=1:size(mapa,1)
    mapa(1,i) = 1;
    mapa(i,1) = 1;
    mapa(size(mapa),i) = 1;
    mapa(i,size(mapa)) = 1;
end

% Colocación de obstáculos en el grid
for i=1:size(obstaculos,1)
    b = table2array(G.Nodes(obstaculos(i),[3,2]))+1;
    mapa(b(1),b(2))=1;
end
[c,f] = find(mapa);

% Dibuja los obstaculos
for i=1:size(f,1)
    rectangle('Position',[f(i)-1, c(i)-1, 1, 1], 'FaceColor',...
        [0 0 0], 'LineWidth',1)
end

plot(G.Nodes.X(best_path)+0.5, G.Nodes.Y(best_path)+0.5,'r')