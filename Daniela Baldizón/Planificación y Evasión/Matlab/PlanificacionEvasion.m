% Algoritmo de planificación de trayectorias y evasión de obstáculos
% Daniela Baldizón
% 1/06/2021
% Se tomaron partes del código ACO de Gaby Iriarte

% Creamos grid cuadrado con la cantidad de nodos indicada:
    grid_size = 10;
    cost_diag = 0.5;
    tau_0 = 0.1;  % Valor de tau inicial
    G = graph_grid(grid_size);
    nodo_dest = '10';
    nodo_init = "1";
    plot_obstacles = 0;
    
    %% ACO init
t_max = 150; 
hormigas = 60;

% Rate de evaporación (puede tomar valores entre 0 y 1)
rho = 0.6; 
% Le da más peso a la feromona en la probabilidad
alpha = 1;
% Le da más peso al costo del link en la probabilidad
beta = 1;
% cte. positiva que regula el depósito de feromona
Q = 2.1; 
% Porcentaje de hormigas que queremos siguiendo la misma solución
epsilon = 0.9; 

% Preallocation
path_k = cell(hormigas, 1);
L = zeros(hormigas, t_max); % Lenght del path por hormiga e iteración
all_path = cell(hormigas, t_max);
ants(1:hormigas) = struct('blocked_nodes', [], 'last_node', nodo_init, 'current_node', nodo_init, 'path', nodo_init, 'L', zeros(1, t_max));
mode_plot = zeros(t_max, 1);
mean_plot = zeros(t_max, 1);

%% ACO loop
t = 1;
stop = 1;
% Gradient Color para la animación
% Solo es para personalizar los colores de la animación
map = [255 255 255
    245 215 250
    255 166 216
    255 111 150
    255 61 61]/255;

figure(1); clf;
h = plot(G, 'XData', G.Nodes.X-0.5, 'YData', G.Nodes.Y-0.5, 'NodeColor', 'k'); 
hold on 
nodos_especiales = [G.Nodes.X(str2double(nodo_init)), G.Nodes.Y(str2double(nodo_init)); G.Nodes.X(str2double(nodo_dest)), G.Nodes.Y(str2double(nodo_dest))];
scatter(nodos_especiales(1, 1)-0.5, nodos_especiales(1, 2)-0.5, 'g','filled')
scatter(nodos_especiales(2, 1)-0.5, nodos_especiales(2, 2)-0.5, 'xr','LineWidth', 5)

[X,Y]=meshgrid(0:grid_size);    % Creación de la cuadrícula
plot(X,Y,'k');          % Dibuja las líneas verticales
plot(Y,X,'k');          % Dibuja las líneas horizontales
% axis off

mapa = zeros(grid_size); % Creación del mapa
mapa(2,2) = 1;
mapa(5,3) = 1;
mapa(5,4) = 1;
[c,f] = find(mapa);

for i=1:size(f)
    rectangle('Position',[f(i)-1, c(i)-1, 1, 1], 'FaceColor',...
        [0 0 0], 'LineWidth',1)
end

% Coordenadas iniciales
P0 = [G.Nodes.X(str2double(nodo_init)), G.Nodes.Y(str2double(nodo_init))];
% coordenadas de los alrededores 
m = [P0(1)-1, P0(1), P0(1)+1; P0(1)-1, P0(1), P0(1)+1; P0(1)-1, P0(1), P0(1)+1];  % Coordenadas en X
n = [P0(2)+1, P0(2)+1, P0(2)+1; P0(2), P0(2), P0(2); P0(2)-1, P0(2)-1, P0(2)-1];   % Coordenadas en Y

