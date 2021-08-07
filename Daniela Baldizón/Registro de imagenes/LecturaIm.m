clear
clc

% Referencia
Ref = rgb2gray(imread('Lena.jpg'));

% Template
Temp = rgb2gray(imread('Lena Distorsion.jpg'));

SRef = Ref(1:10,1:10);
STemp = Temp(1:10,1:10);
% figure;
% imshow(SubJ)
% 
% figure;
% imshow(JD)

%% Graph generation

% Creamos grid cuadrado con la cantidad de nodos indicada:
grid_size = size(SRef,1);
cost_diag = 0.5;
tau_0 = 0.1;  % Valor de tau inicial
G = graph_grid(grid_size);
nodo_dest = '10';
nodo_init = "1";

%% ACO init
t_max = 150; 
hormigas = sum(size(SRef));

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
% Escala de las distancias de avance
r = 1;

% Preallocation
path_k = cell(hormigas, 1); % Inicializa el path de las hormigas en 0
L = zeros(hormigas, t_max); % Lenght del path por hormiga e iteración
all_path = cell(hormigas, t_max); % El path de todas las hormigas
% Coloca las hormigas de forma uniforme en el grid
for i = 1:hormigas
    ants(i) = struct('blocked_nodes', [], 'last_node',...
        int2str(i), 'current_node', int2str(i), 'path', int2str(i), 'L', ...
        zeros(1, t_max));
end
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

% Figura del grafo
figure(1); clf;
h = plot(G, 'XData', G.Nodes.X, 'YData', G.Nodes.Y, 'NodeColor', 'k'); 
hold on 
% nodos_especiales = [G.Nodes.X(str2double(nodo_init)), G.Nodes.Y(str2double(nodo_init)); G.Nodes.X(str2double(nodo_dest)), G.Nodes.Y(str2double(nodo_dest))];
% scatter(nodos_especiales(1, 1), nodos_especiales(1, 2), 'g','filled')
% scatter(nodos_especiales(2, 1), nodos_especiales(2, 2), 'xr','LineWidth', 5)

colormap(map);

while (t <= t_max && stop)
    
    parfor k = 1:hormigas
        while (not(strcmp(ants(k).current_node, nodo_dest))) % Mientras no se haya llegado al destino
            
            ants(k).blocked_nodes = [ants(k).blocked_nodes; convertCharsToStrings(ants(k).current_node)];
            
            vecinos = setdiff(convertCharsToStrings(neighbors(G, ants(k).current_node)),...
                ants(k).blocked_nodes, 'rows','stable');
            
            while (isempty(vecinos))
                ants(k).path = ants(k).path(1:end-1);
                ants(k).current_node = ants(k).path(end);
                vecinos = setdiff(convertCharsToStrings(neighbors(G,ants(k).current_node)),...
                    ants(k).blocked_nodes, 'rows','stable');
            end
            vecinos_updated = vecinos;
            
            % La hormiga toma la decisión de a donde ir eq.(17.6)
            next_node = ant_decision(vecinos_updated, alpha, beta, G, ants(k).current_node);
            
            % Si Idiff es negativo, la hormiga debe buscar intensidades más bajas con
            % los vecinos. Si es mayor o igual a cero, va a buscar valores de
            % intensidad más altos con los vecinos.
            % Alimento de las hormigas
            Idiff = SRef - STemp;
            
            % Gradiente de la imagen
            [Tx, Ty] = imgradientxy(STemp);
            
            % Distancia de avance
            Dx = abs(r*Idiff*Tx);
            Dy = abs(r*Idiff*Ty);
            
            ants(k).last_node = [ants(k).last_node; ants(k).current_node];
            ants(k).current_node = next_node;
            ants(k).path = [ants(k).path; ants(k).current_node];
        end

        % Le quitamos los loops al path y ahora los índices son números y
        % no strings.
        
        ants(k).path = loop_remover(str2double(ants(k).path));
        L(k, t) = sum(G.Edges.Eta(findedge(G, ants(k).path(1:end-1), ants(k).path(2:end))).^-1);
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
        dtau = Q/numel(ants(k).path);
        edge_index = findedge(G, ants(k).path(1:end - 1), ants(k).path(2:end));
        G.Edges.Weight(edge_index) = G.Edges.Weight(edge_index) + dtau;
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
    h.LineWidth = G.Edges.NormWeight * 50;
    h.EdgeCData = G.Edges.NormWeight;
    drawnow limitrate
    t = t + 1;
    
    
end
%% Best Path Calculation

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
    plot(G.Nodes.X(best_path), G.Nodes.Y(best_path),'r')

end

% profile viewer % Es parte del profiler, descomentar para ver
tiempofinal = toc; 
