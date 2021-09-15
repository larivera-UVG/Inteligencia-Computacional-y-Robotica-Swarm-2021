clc
clear

%% Creación del mapa 
grid_size = 2;
cost_diag = 0.5;
tau_0 = 0.1;  % Valor de tau inicial
G = graph_grid(grid_size);
obstaculos = [1,4]';
mapa = zeros(grid_size+2); % Creación del mapa

figure(1); clf;
% Se crea la imagen animada
% h = plot(G, 'XData', G.Nodes.X+0.5, 'YData', G.Nodes.Y+0.5, 'NodeColor', 'k'); 
hold on 

[X,Y]=meshgrid(1:grid_size+1);    % Creación de la cuadrícula
plot(X,Y,'k');          % Dibuja las líneas verticales
plot(Y,X,'k');          % Dibuja las líneas horizontales
axis off

% % Se le hace un borde al mapa para delimitarlo, este borde puede ser tomado
% % como obstáculo
% for i=1:size(mapa,1)
%     mapa(1,i) = 1;
%     mapa(i,1) = 1;
%     mapa(size(mapa),i) = 1;
%     mapa(i,size(mapa)) = 1;
% end

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