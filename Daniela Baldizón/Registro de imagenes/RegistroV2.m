% Algoritmo de Registro de imágenes
% Daniela Baldizón
% Se tomó como base el código desarrollado por Gabriela Iriarte

clear
clc

% % Referencia
% Ref = rgb2gray(imread('Lena.jpg'));
% 
% % Template
% Temp = rgb2gray(imread('Lena Distorsion.jpg'));
% 
% SRef = double(Ref(1:10,1:10));
% STemp = double(Temp(1:10,1:10));
% SReg = STemp;

Im = zeros(10);
Im(4:7,4) = 155;
Im(4:7,5) = 168;
Im(4:7,6) = 161;

Im2 = zeros(10);
Im2(4:7,1) = 155;
Im2(4:7,4) = 168;
Im2(4:7,9) = 161;

SRef = Im;
STemp = Im2; % Distorsionada
SReg = STemp; % Distorsionada
% figure;
% imshow(SubJ)
% 
% figure;
% imshow(JD)

% Si Idiff es negativo, la hormiga debe buscar intensidades más bajas con
% los vecinos. Si es mayor o igual a cero, va a buscar valores de
% intensidad más altos con los vecinos.
% Alimento de las hormigas
Idiff = double(SRef - STemp);

%% Graph generation
tic
% Creamos grid cuadrado con la cantidad de nodos indicada:
grid_size = size(SRef,1);
tau_0 = 1;  % Valor de tau inicial
G = graph_grid(grid_size, tau_0, Idiff);

%% ACO init
t_max = 150; 
hormigas = (size(SRef,1))^2;

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
path_k = cell(hormigas, 1); % Inicializa el path de las hormigas en 0
all_path = cell(hormigas, t_max); % El path de todas las hormigas
all_weight = cell(hormigas, t_max);

%% Nuevo

for i = 1:hormigas
    % Struct de las hormigas
    ants(i) = struct('blocked_nodes', [], 'last_node',...
        int2str(i), 'current_node', int2str(i), 'path', int2str(i));
end
%% ACO loop
t = 1;
stop = 1; % No tocar

% Inicialización variables
food = zeros(1,hormigas);
A_B = zeros(sqrt(hormigas));
A_AP = zeros(sqrt(hormigas));
B_BP = zeros(sqrt(hormigas));
%%
while (t <= t_max && stop)
    
    
    parfor k = 1:hormigas

        % Si Idiff es negativo, la hormiga debe buscar intensidades más bajas con
        % los vecinos. Si es mayor o igual a cero, va a buscar valores de
        % intensidad más altos con los vecinos.
        % Alimento de las hormigas
        Idiff = double(SRef - SReg);


        % Posicion actual por hormiga (Se hace esto para que las
        % posiciones en la imagen coincidan con el grafo)
        l = G.Nodes.X(str2double(ants(k).current_node)); % Fila
        m = G.Nodes.Y(str2double(ants(k).current_node)); % Columna

        % encuentra los nodos vecinos
        proximity = [convertCharsToStrings(neighbors(G, k))];

        % Posicion actual por nodo (columnas y filas). La referencia es el
        % origen de la imagen (esquina superior izquierda)
        colum_coor = G.Nodes.Y(double(proximity));
        fila_coor = G.Nodes.X(double(proximity));

        weight_int_vec = zeros(1,size(colum_coor,1));

        % encuentra el peso de los pixeles en proximity. El peso está dado como
        % la diferencia ente la imagen de referencia y el template.
        for j = 1:size(colum_coor,1)
            weight_int_vec(j) = Idiff(fila_coor(j),colum_coor(j));
        end

        ants(k).blocked_nodes = [ants(k).blocked_nodes; convertCharsToStrings(ants(k).current_node)];

        vecinos = setdiff(convertCharsToStrings(neighbors(G, ants(k).current_node)),...
            ants(k).blocked_nodes, 'rows','stable');
        
        % En las iteraciones par se bloquean los nodos mayores al
        % actual, para dar el efecto de retroceso
%         if (mod(t,2)==0 && ~isempty(vecinos))
%             vecinos_v2 = str2double(vecinos);
%             for i = 1:vecinos_v2
%                 ants(k).blocked_nodes = [ants(k).blocked_nodes; ...
%                     vecinos_v2(i)];
%             end
%             vecinos = setdiff(convertCharsToStrings(neighbors(G, ants(k).current_node)),...
%                 ants(k).blocked_nodes, 'rows','stable');
%         end
        
        if (isempty(vecinos))
            ants(k).path = ants(k).current_node;
            ants(k).current_node = ants(k).path(end);
        else
            
            
            vecinos_updated = vecinos;

            % La hormiga toma la decisión de a donde ir eq.(17.6)
            next_node = ant_decision(vecinos_updated, alpha, beta, G);

            ants(k).last_node = [ants(k).last_node; ants(k).current_node];
            ants(k).current_node = next_node;
            ants(k).path = [ants(k).path; ants(k).current_node];

        end

        % Le quitamos los loops al path y ahora los índices son números y
        % no strings.

        ants(k).path = loop_remover(str2double(ants(k).path));
%         all_path{k, t} = ants(k).path;  % Equivale a x_k(t)

        % Regresamos la hormiga k al inicio (k-esimo nodo)
        ants(k).current_node = int2str(k);
        ants(k).blocked_nodes = [];
        ants(k).last_node = int2str(k);

    end
    
    for k = 1:hormigas
        Idiff = double(SRef - SReg);
        nodo_actual = ants(k).path(end);
        % Se lleva la comida al nodo inicial
        food(k) = Idiff(nodo_actual);
        % Se busca la comida que es negativa para que lo que se
        % mueva sean los pixeles en SReg
        if food(k) < 0
            SReg(k) = SReg(k) - food(k);
            SReg(nodo_actual) = SReg(nodo_actual) + food(k);
            Idiff(k) = double(SRef(k) - SReg(k));
            G.Nodes.Eta(k) = Idiff(k);
        end
        
        % Feromona
        G.Nodes.Weight(str2double(ants(k).current_node)) = (1-rho)*G.Nodes.Weight(k) + tau_0;
        
        ants(k).path = int2str(k);
    end
        
    %% Cálculos SSD y CC
    % Se tomó A como la referencia y B como la registrada
    
    % Intensidad promedio de la referencia
    AP = mean2(SRef);
    
    % Intensidad primedio de la registrada
    BP = mean2(SReg);
    
    for i = 1:size(SReg,1)
        for j = 1:size(SReg,2)
            A_B(i,j) = (norm(SRef(i,j) - SReg(i,j))^2)/hormigas;
            A_AP(i,j) = SRef(i,j) - AP;
            B_BP(i,j) = SReg(i,j) - BP;
        end
    end
    
    
    SSD = sum(sum(A_B));
    CC = (sum(sum(A_AP.*B_BP)))/sqrt(sum(sum(A_AP.^2))*sum(sum(B_BP.^2)));
    
    if CC >= 0.9
        % Condición de paro cuando el coeficiente de correlación sea lo
        % suficientemente bueno
        stop = 0;
    end
    
    t = t + 1;
    
end
%% Best Path Calculation

if (t > t_max)
    disp("No hubo convergencia")
else
    disp("Listo")
    figure;
    imshow(uint8(SReg))
    figure;
    imshow(uint8(SRef))
%     figure;
%     imshow(uint8(STemp))
end

    
tiempofinal = toc; 