% Algoritmo de planificación de trayectorias y evasión de obstáculos
% Daniela Baldizón
% 1/06/2021
% Se tomaron partes del código ACO de Gaby Iriarte

clc
clear

%% Detectar funciones lentas
% profile on % El profiler se utiliza para detectar funciones lentas
% Para que funcione hay que descomentarlo y descomentar el profile viewer
% de abajo (casi al final del código)
tic  % Para medir el tiempo que se tarda el algoritmo en correr.

%% Creamos grid cuadrado con la cantidad de nodos indicada:
    grid_size = 10;
    cost_diag = 0.5;
    tau_0 = 0.1;  % Valor de tau inicial
    G = graph_grid(grid_size);
    nodo_dest = '100';
    nodo_init = "1";
    
    
    escenario = 2;
   
    switch escenario
        case 1
            % Mapa A
            obstaculos = [12,13,22,23,37,38,47,48,75,76,85,86]'; % Nodos donde se colocan los obstaculos
        case 2
            % Mapa B
            obstaculos = [57,66,75,44,35,26,17]';
        case 3
            % Mapa C
            obstaculos = [4,5,6,51,52,58,59,67,68,69,71,72,75,78,79,83,84,87,...
                88,89,96,97,98,99,103,104,111,112,113,117,118,119,120,131,132,...
                133,144,145,146,156,157,158,159,168,169,188,189,213,214,215,216,...
                222,223,224,226,233,234,235,236,242,243,244,247,248,249,253,254,...
                255,256,262,263,264,267,268,269,282,283,284,293,294,295,307,308,...
                309,313,314,315,324,325,326,344,345,315,351,352,353,384,385]';
        case 4
            % Mapa D
            obstaculos = [4,5,6,51,52,58,59,67,68,69,71,72,78,79,83,84,87,88,89,...
                96,97,98,99,103,104,111,112,113,117,118,119,120,131,132,133,144,...
                145,146,156,157,158,159,168,169,188,189,213,214,215,216,222,223,...
                224,233,234,235,236,242,243,244,247,248,249,253,254,255,256,262,...
                263,264,267,268,269,282,283,284,293,294,295,307,308,309,313,314,...
                315,324,325,326,344,345,351,352,353,384,385]';
        case 5
            % Mapa E
            obstaculos = [53,54,55,71,72,73,74,75,77,78,87,88,89,91,92,97,98,...
                103,104,107,108,109,123,124,132,133,134,136,152,153,154,156,172,...
                173,174,188,189,208,209,211,212,213,214,231,232,233,234,242,243,...
                244,247,248,249,251,252,253,254,262,263,264,267,268,269,271,272,...
                273,274,282,283,284,287,288,289,307,308,309,327,328,329,342,343,...
                347,348,349,362,363]';
        case 6
            % Mapa Webots
            obstaculos = [23,24,27,28,32,62,67,72,73,74,78,79]'; 
        case 7
            % Barrido
            obstaculos = [4,5,6, 71, 73, 72, 78, 89, 99, 67,56,18,19,28,29]';
    end
        
    %% ACO init
t_max = 1000; 

constantes = 3;
switch constantes
    case 1
        % Constantes Gaby
        rho = 0.6;
        alpha = 1;
        beta = 1;
        Q = 2.1;
        gamma = 1;
        hormigas = 60;
    case 2
        % Artículo
        rho = 0.43;
        alpha = 1;
        beta = 7;
        Q = 100;
        gamma = 3;
        hormigas = 30;
    case 3
        % Barrido 1
        rho = 0.8;
        alpha = 2.2;
        beta = 0.5;
        gamma = 3.4;
        Q = 65;
        hormigas = 15;
    case 4
        % Barrido 2
        % Rate de evaporación (puede tomar valores entre 0 y 1)
        rho = 0.9;
        % Le da más peso a la feromona en la probabilidad
        alpha = 2;
        % Le da más peso al costo del link en la probabilidad
        beta = 0.5;
        % Factor de guía
        gamma = 2.2;
        % cte. positiva que regula el depósito de feromona
        Q = 40;
        hormigas = 60;
end

% Porcentaje de hormigas que queremos siguiendo la misma solución
epsilon = 0.95; 

% Matriz peso
D = zeros(grid_size^2);
H = zeros(grid_size^2+grid_size+1);

% Inicialización de matrices
submapa = zeros(3);
OL = zeros(3);
OD = zeros(3);
mapa = zeros(grid_size+2); % Creación del mapa

% Preallocation
path_k = cell(hormigas, 1); % Crea un array de celdas de tamaño hormigasx1
L = zeros(hormigas, t_max); % Length del path por hormiga e iteración
all_path = cell(hormigas, t_max);
ants(1:hormigas) = struct('blocked_nodes', [], 'last_node', nodo_init, ...
    'current_node', nodo_init, 'path', nodo_init, 'L', zeros(1, t_max));
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
h2 = plot((1:t_max)', mean_plot, 'Color', [0.8, 0.05, 0], 'LineWidth', 1.5);
hold on
h3 = plot((1:t_max)', mode_plot, 'Color', [0, 0, 0.8], 'LineWidth', 1.5);
title('Global Cost', 'interpreter', 'latex', 'FontSize', 17)
xlabel('Generations', 'interpreter', 'latex', 'FontSize', 12)
ylabel('Path Lenght', 'interpreter', 'latex', 'FontSize', 12)
leg1 = legend('$\bar{x}$', '$\hat{x}$');
set(leg1, 'Interpreter', 'latex');
set(leg1, 'FontSize', 17);
%%
figure(2); clf;
% Se crea la imagen animada
h = plot(G, 'XData', G.Nodes.X+0.5, 'YData', G.Nodes.Y+0.5, 'NodeColor', 'k' ); 
% labelnode(h, str2double(G.Nodes.Name'), G.Nodes.Name'); % Colocar nombres a los nodos
hold on 
nodos_especiales = [G.Nodes.X(str2double(nodo_init)), G.Nodes.Y(str2double(nodo_init)); ...
    G.Nodes.X(str2double(nodo_dest)), G.Nodes.Y(str2double(nodo_dest))];
scatter(nodos_especiales(1, 1)+0.5, nodos_especiales(1, 2)+0.5, 'g','filled')
scatter(nodos_especiales(2, 1)+0.5, nodos_especiales(2, 2)+0.5, 'xr','LineWidth', 5)

[X,Y]=meshgrid(0:grid_size+2);    % Creación de la cuadrícula
plot(X,Y,'k');          % Dibuja las líneas verticales
plot(Y,X,'k');          % Dibuja las líneas horizontales
% axis off

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

% Matriz de distancia entre casillas
Dx = [1,0,1;1,0,1;1,0,1];
Dy = [1,1,1;0,0,0;1,1,1];

%%
colormap(map);
while (t <= t_max && stop)
  
    for k = 1:hormigas
%         OL = 0;
%         OD = 0;
        
        while (not(strcmp(ants(k).current_node, nodo_dest))) % Mientras no se haya llegado al destino
            
            % Lista tabú
            ants(k).blocked_nodes = [ants(k).blocked_nodes; convertCharsToStrings(ants(k).current_node)];
                   
            % Posicion actual por hormiga
            x = G.Nodes.X(double(ants(k).current_node))+1;
            y = G.Nodes.Y(double(ants(k).current_node))+1;
            
            % Creación del submapa
            submapa = [mapa(y-1,x-1), mapa(y-1,x), mapa(y-1,x+1); ...
                mapa(y,x-1), mapa(y,x), mapa(y, x+1); ...
                mapa(y+1,x-1), mapa(y+1,x), mapa(y+1,x+1)];
            
            % Identificación de obstáculos
            for i=1:size(submapa,1)
                for j=1:size(submapa,1)
                    if((Dx(i,j)+Dy(i,j)==1)&&submapa(i,j)==1)
                       % Obstáculo hacia los lados (Condición 1)
                        if(submapa(1,2)==1) % D Obstacle
                            nodob = findNode(G,x,y-1);
                            ants(k).blocked_nodes = [ants(k).blocked_nodes; ...
                                convertCharsToStrings(nodob)];
                        end 

                        if(submapa(2,1)==1) % L Obstacle
                            nodob = findNode(G,x-1,y);
                            ants(k).blocked_nodes = [ants(k).blocked_nodes; ...
                                convertCharsToStrings(nodob)];
                        end

                        if(submapa(2,3)==1) % R Obstacle
                            nodob = findNode(G,x+1,y);
                            ants(k).blocked_nodes = [ants(k).blocked_nodes; ...
                                convertCharsToStrings(nodob)];
                        end

                        if(submapa(3,2)==1) % U Obstacle
                            nodob = findNode(G,x,y+1);
                            ants(k).blocked_nodes = [ants(k).blocked_nodes; ...
                                convertCharsToStrings(nodob)];
                        end

                        % Encuentra en qué dirección está la narrow aisle (Condiciones
                        % 3-6)
                        if(submapa(1,2)==1&&submapa(2,3)==1)
                            LR = 1; % Narrow aisle lower rigth direction
                        else 
                            LR = 0;
                        end

                        if(submapa(1,2)==1&&submapa(2,1)==1)
                            LL = 1; % Lower left
                        else
                            LL = 0;
                        end

                        if(submapa(3,2)==1&&submapa(2,1)==1)
                            UL = 1; % Upper Left
                        else
                            UL = 0;
                        end

                        if((submapa(3,2)==1&&submapa(2,3)==1))
                            UR = 1; % Upper right
                        else
                            UR = 0;
                        end

                        % BLoquea el nodo que se encuentra después de la narrow aisle
                        if(UR==1)
                            xf = x+1;
                            yf = y+1;
                            nodob = findNode(G,xf,yf);
                            nodoa = findNode(G,x,yf);
                            nodol = findNode(G,xf,y);
                            ants(k).blocked_nodes = [ants(k).blocked_nodes; ...
                                convertCharsToStrings(nodob); ...
                                convertCharsToStrings(nodoa);
                                convertCharsToStrings(nodol)];
                        end

                        if(UL==1)
                            xf = x-1;
                            yf = y+1;
                            nodob = findNode(G,xf,yf);
                            nodoa = findNode(G,x,yf);
                            nodol = findNode(G,xf,y);
                            ants(k).blocked_nodes = [ants(k).blocked_nodes; ...
                                convertCharsToStrings(nodob); ...
                                convertCharsToStrings(nodoa);
                                convertCharsToStrings(nodol)];
                        end

                        if(LL==1)
                            xf = x-1;
                            yf = y-1;
                            nodob = findNode(G,xf,yf);
                            nodoa = findNode(G,x,yf);
                            nodol = findNode(G,xf,y);
                            ants(k).blocked_nodes = [ants(k).blocked_nodes; ...
                                convertCharsToStrings(nodob); ...
                                convertCharsToStrings(nodoa);
                                convertCharsToStrings(nodol)];
                        end

                        if(LR==1)
                            xf = x+1;
                            yf = y-1;
                            nodob = findNode(G,xf,yf);
                            nodoa = findNode(G,x,yf);
                            nodol = findNode(G,xf,y);
                            ants(k).blocked_nodes = [ants(k).blocked_nodes; ...
                                convertCharsToStrings(nodob); ...
                                convertCharsToStrings(nodoa);
                                convertCharsToStrings(nodol)];
                        end
                    elseif((Dx(i,j)==1 && Dy(i,j)==1)&&submapa(i,j)==1)
                        % Obstáculo en diagonal (Condición 2)
                       if(submapa(1,1)==1) % LL Obstacle
                            nodob = findNode(G,x-1,y-1);
                            ants(k).blocked_nodes = [ants(k).blocked_nodes; ...
                                convertCharsToStrings(nodob)];
                        end

                        if(submapa(3,3)==1) % UR Obstacle
                            nodob = findNode(G,x+1,y+1);
                            ants(k).blocked_nodes = [ants(k).blocked_nodes; ...
                                convertCharsToStrings(nodob)];
                        end

                        if(submapa(1,3)==1) % LR Obstacle
                            nodob = findNode(G,x+1,y-1);
                            ants(k).blocked_nodes = [ants(k).blocked_nodes; ...
                                convertCharsToStrings(nodob)];
                        end

                        if(submapa(3,1)==1) % UL Obstacle
                            nodob = findNode(G,x-1,y+1);
                            ants(k).blocked_nodes = [ants(k).blocked_nodes; ...
                                convertCharsToStrings(nodob)];
                        end
                    end
                end
            end
            
            % Quita de vecinos los nodos que ya fueron visitados (Lista
            % tabú)
            vecinos = setdiff(convertCharsToStrings(neighbors(G, ants(k).current_node)),...
                ants(k).blocked_nodes, 'rows','stable');
            
            while (isempty(vecinos))
                ants(k).path = ants(k).path(1:end-1);
                ants(k).current_node = ants(k).path(end);
                vecinos = setdiff(convertCharsToStrings(neighbors(G,ants(k).current_node)),...
                    ants(k).blocked_nodes, 'rows','stable');
            end
            vecinos_updated = vecinos;
            
            % Matriz peso
            visitado = sum(H((y-1)*grid_size+x,:));
            if(visitado == 0) % Primera vez en el nodo actual
                for i=2:size(mapa,1)-1
                    for j=2:size(mapa,2)-1
                        H((y-1)*grid_size+x,(j-1)*grid_size+i)=sqrt((x-i).^2+(y-j).^2);
                    end
                end
                % Calcula las distancias entre todo el grid 
                D = [H(grid_size+2:size(H,1),grid_size+2:size(H,2))];
            end
            
            % Distancia del grid actual con el grid objetivo
            Sjo = D(str2double(ants(k).current_node),str2double(nodo_dest));
            
            
            % La hormiga toma la decisión de a donde ir eq.(17.6)
            next_node = ant_decision(vecinos_updated, alpha, beta, gamma, ...
                G, ants(k).current_node, Sjo);
            ants(k).last_node = [ants(k).last_node; ants(k).current_node];
            ants(k).current_node = next_node;
            ants(k).path = [ants(k).path; ants(k).current_node];
            ants(k).blocked_nodes = [];
        end
        
        % Le quitamos los loops al path y ahora los índices son números y
        % no strings.
        
        ants(k).path = loop_remover(str2double(ants(k).path));
        L(k, t) = sum(G.Edges.Eta(findedge(G, ants(k).path(1:end-1), ants(k).path(2:end))).^-1);
%         Largo(k,t) = sum(G.Edges.Eta(findedge(G, ants(k).path(1:end-1), ants(k).path(2:end))));
        all_path{k, t} = ants(k).path;  % Equivale a x_k(t)
        
        % Regresamos la hormiga k al inicio
        ants(k).current_node = nodo_init;
        ants(k).blocked_nodes = [];
        ants(k).last_node = nodo_init;

    end
    
    %% Evaporación de Feromona
    G.Edges.Weight = G.Edges.Weight * (1 - rho);
    
    %% Update de Feromona
    for k = 1:hormigas
        dtau = Q/(L(k,t)*numel(ants(k).path));
        edge_index = findedge(G, ants(k).path(1:end - 1), ants(k).path(2:end));
        
        largos = find(L~=0);
        largos = L(largos);
        min_cost = min(largos);
        
        
        [mode_plot(t), F] = mode(L(:,t));
        if (F/hormigas >= epsilon*0.4) 
            disp("Nivel de convergencia mayor a e*0.4")
            if(L(k,t)<=min_cost)
                G.Edges.Weight(edge_index) = G.Edges.Weight(edge_index) + Q*dtau;
                disp("Segunda Fermona")
            end
        else
            G.Edges.Weight(edge_index) = G.Edges.Weight(edge_index) + dtau;
        end
        
        ants(k).path = nodo_init;
    end
    
    [mode_plot(t), F] = mode(L(:,t));
    mean_plot(t) = mean(L(:,t));
    
    if (F/hormigas >= epsilon) % condición de paro
        stop = 0;
    end
    
    % Animación
    G.Edges.NormWeight = G.Edges.Weight/sum(G.Edges.Weight);
    h2.YData(t) = mean_plot(t);
    h3.YData(t) = mode_plot(t);
    h.LineWidth = G.Edges.NormWeight * 35;
    h.EdgeCData = G.Edges.NormWeight;
    drawnow limitrate
    t = t + 1;
    
    
end

if (t > t_max)
    disp("No hubo convergencia")
else

% Con la MODA vemos qué largo es el que más se repite
    moda =  mode(L(:, t-1));
    % Tomamos los index de todos los largos que son iguales a la moda (en
    % la última iteración)
    len_indx = L(:, t-1).*(L(:,t-1) == moda);
    % Tomamos probabilidad random de qué camino tomar (si hubiese varios
    % casos con el mismo largo pero ruta diferente). Esta función nos
    % devuelve el index (la hormiga) que produjo el best path
    len_prob = rouletteWheel(len_indx);
    best_path = all_path{len_prob, t-1};
    
    % Gráfica
    figure(2);
    hold on;
    plot(G.Nodes.X(best_path)+0.5, G.Nodes.Y(best_path)+0.5,'r')

end

% profile viewer % Es parte del profiler, descomentar para ver
tiempofinal = toc;
formatSpec = 'iter: %d - t: %.2f - cost: %.2f \n';
fprintf(formatSpec, t-1, tiempofinal, moda)
bpath = [G.Nodes.X(best_path), G.Nodes.Y(best_path)];
webots_path = (bpath - (grid_size/2+0.5)).*[1/5 -1/5];
wb_pc_path = 'C:\Users\Daniela Baldizon\Documents\GitHub\Inteligencia-Computacional-y-Robotica-Swarm-2021\Daniela Baldizón\Webots\controllers\PyE\';
save(strcat(wb_pc_path, 'webots_test.mat'), 'bpath', 'webots_path')


