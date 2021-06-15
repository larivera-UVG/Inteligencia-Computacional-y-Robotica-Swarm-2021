% Algoritmo de planificación de trayectorias y evasión de obstáculos
% Daniela Baldizón
% 1/06/2021
% Se tomaron partes del código ACO de Gaby Iriarte

% Creamos grid cuadrado con la cantidad de nodos indicada:
    grid_size = 5;
    cost_diag = 0.5;
    tau_0 = 0.1;  % Valor de tau inicial
    G = graph_grid(grid_size);
    nodo_dest = '10';
    nodo_init = "12";
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
h = plot(G, 'XData', G.Nodes.X+0.5, 'YData', G.Nodes.Y+0.5, 'NodeColor', 'k'); 
hold on 
nodos_especiales = [G.Nodes.X(str2double(nodo_init)), G.Nodes.Y(str2double(nodo_init)); G.Nodes.X(str2double(nodo_dest)), G.Nodes.Y(str2double(nodo_dest))];
scatter(nodos_especiales(1, 1)+0.5, nodos_especiales(1, 2)+0.5, 'g','filled')
scatter(nodos_especiales(2, 1)+0.5, nodos_especiales(2, 2)+0.5, 'xr','LineWidth', 5)

[X,Y]=meshgrid(0:grid_size+2);    % Creación de la cuadrícula
plot(X,Y,'k');          % Dibuja las líneas verticales
plot(Y,X,'k');          % Dibuja las líneas horizontales
% axis off

mapa = zeros(grid_size+2); % Creación del mapa
% Se le hace un borde al mapa para delimitarlo, este borde puede ser tomado
% como obstáculo
for i=1:size(mapa)
    mapa(1,i) = 1;
    mapa(i,1) = 1;
    mapa(size(mapa),i) = 1;
    mapa(i,size(mapa)) = 1;
end
mapa(3,2)=1;
[c,f] = find(mapa);

for i=1:size(f)
    rectangle('Position',[f(i)-1, c(i)-1, 1, 1], 'FaceColor',...
        [0 0 0], 'LineWidth',1)
end

% Coordenadas iniciales
P0 = [G.Nodes.X(str2double(nodo_init)), G.Nodes.Y(str2double(nodo_init))]+1;

% mapa(P0(2),P0(1)) = 3;


% Coordenadas del punto
x0 = P0(2);
y0 = P0(1);

% Matriz de adyacencia (PAPER)
MAD = [mapa(x0-1,y0-1), mapa(x0-1,y0), mapa(x0-1,y0+1);
    mapa(x0,y0-1), mapa(x0,y0), mapa(x0,y0+1);
    mapa(x0+1,y0-1), mapa(x0+1,y0), mapa(x0+1,y0+1)];

% Matriz de distancia entre casillas
Dx = [1,0,1;1,0,1;1,0,1];
Dy = [1,1,1;0,0,0;1,1,1];

for i=1:size(MAD,1)
    for j=1:size(MAD,1)
        if(MAD(i,j)==1)
            % Si hay un obstáculo pone esa casilla en infinito
            Dx(i,j)=inf;
            Dy(i,j)=inf;
        end
    end
end

% Identificación de obstáculos
for i=1:size(MAD,1)
    for j=1:size(MAD,1)
        if(Dx(i,j)+Dy(i,j)==1)% Obstáculo hacia los lados (Condición 1)
           OL = 1;
        elseif(Dx(i,j)==1 && Dy(i,j)==1)% Obstáculo en diagonal (Condición 2)
           OD = 1;
        else
            OL = 0;
            OD = 0;
        end
    end
end

if OD==1
    if(mapa(x0,y0-1)~=1||mapa(x0-1,y0)~=1)
        UL = 1; % Narrow aisle upper left direction
    elseif(mapa(x0,y0-1)~=1||mapa(x0+1,y0)~=1)
        LL = 1; % Lower left
    elseif(mapa(x0,y0+1)~=1||mapa(x0+1,y0)~=1)
        RL = 1; % Rigth lower
    elseif(mapa(x0,y0+1)~=1||mapa(x0-1,y0)~=1)
        UR = 1; % Upper right
    else
        UL = 0;
        LL = 0;
        RL = 0;
        UR = 0;
    end
end

% Matriz peso
D = zeros(size(mapa)-2);
for i=2:size(mapa,1)-1
    for j=2:size(mapa,2)-1
        D(i-1,j-1)=sqrt((x0-i).^2+(y0-j).^2);
    end 
end