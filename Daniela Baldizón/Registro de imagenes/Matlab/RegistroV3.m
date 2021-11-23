% Algoritmo de Registro de imágenes
% Daniela Baldizón
% Se tomó como base el código desarrollado por Gabriela Iriarte
% Esta es la versión 3 del algoritmo de registro de imágenes. Es muy
% importante que las imágenes sean cuadradas 

clear
clc

tic % Tiempo
% Referencia

Ref = (imread('RodillaR.jpg')); % Imagen de referencia

% Template
Temp = (imread('RodillaT.jpg')); % Imagen distorsionada
% (1:10,1:10) Esto se agrega para tomar una parte de la imágen.
SRef = double(Ref); % Se convierten las imágenes para ya poder manipularlas.
STemp = double(Temp);
SReg = STemp; % Imagen registrada

% Intensidad total: Se suman las intensidades de las imágenes
int_ref = sum(sum(SRef));
int_temp = sum(sum(STemp));
int_reg = sum(sum(SReg));

% Se calcula la diferencia de intensidad entre la imagen de referencia y
% la distorsionada
desface = int_ref - int_temp;

% Se calcula el residuo de dividir el desface en cantidades iguales
% repartidas en los pixeles de la imagen.
Residuo = sign(desface)*mod(abs(desface),numel(SRef));

% Se calcula el desface sin residuo
desfaceM = sign(desface)*(abs(desface) - abs(Residuo));

% Pixel random donde se colocará el residuo
idesface = randi(numel(SReg));

% Se agrega el desface en cantidades iguales repartidas en los pixeles
SReg = SReg + desfaceM/numel(SReg);

% Se agrega el residuo en una casilla random
SReg(idesface) = SReg(idesface) + Residuo;

% Se verifica que las intensidades se hayan igualado
int_reg2 = sum(sum(SReg));

%% Para hacer pruebas rápidas
% Im = zeros(10);
% Im(4:7,4) = 155;
% Im(4:7,5) = 168;
% Im(4:7,6) = 161;
% 
% Im2 = zeros(10);
% Im2(4:7,2) = 155;
% Im2(4:7,4) = 168;
% Im2(4:7,9) = 161;
% 
% SRef = Im;
% STemp = Im2; % Distorsionada
% SReg = STemp; % Distorsionada


%% Alimento de las hormigas
Idiff = double(SRef - SReg);

%% Graph generation
% Creamos grid cuadrado con la cantidad de nodos indicada:
grid_size = size(SRef,1);
tau_0 = 0.1;  % Valor de tau inicial
G = graph_grid(grid_size, tau_0, Idiff);

%% ACO init
t_max = 750; 
hormigas = numel(SRef);

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

%% Se crea la estructura donde se almacena la información de cada hormiga
for i = 1:hormigas
    % Struct de las hormigas
    ants(i) = struct('blocked_nodes', [], 'last_node',...
        int2str(i), 'current_node', int2str(i), 'path', int2str(i));
end
%% ACO loop
t = 1;
stop = 1; % No tocar

% Inicialización variables
food = zeros(1,hormigas); % Representación de la comida que transportan
T_final = zeros(sqrt(hormigas)); % Transformación para llegar al resultado final
A_B = zeros(sqrt(hormigas)); % Matrices para cálculos estadísticos
A_AP = zeros(sqrt(hormigas));
B_BP = zeros(sqrt(hormigas));
CC_acumulado = zeros(t_max,1);
contador = 0; % No tocar
%%
while (t <= t_max && stop)
    
    contador = 1; % No tocar
    for n = 1:hormigas
        
        % Alimento de las hormigas
        Idiff = double(SRef - SReg);
        
        % Se modifica el índice k para hacer que las hormigas se muevan en
        % forma de "ondas" por la imágen. 
        if mod(contador,2) == 0
            if mod(t,2) == 0
                k = k + 1;
            else
                k = k - 1;
            end
        else
            if mod(t,2) == 0
                k = hormigas - n + 1;
            else
                k = n;
            end
        end

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
        
        if (isempty(vecinos))
            ants(k).path = ants(k).current_node;
            ants(k).current_node = ants(k).path(end);
        else
            
            
            vecinos_updated = vecinos;

            % La hormiga toma la decisión de a donde ir eq.(17.6)
            next_node = ant_decision(vecinos_updated, alpha, beta, G);
            nodo_actual = str2double(ants(k).current_node);

            % Se lleva la comida al nodo inicial
            food(str2double(ants(k).current_node)) = Idiff(str2double(next_node));
            % Se busca la comida que es negativa para que lo que se
            % mueva sean los pixeles en SReg
            if food(str2double(ants(k).current_node)) < 0
                % Se actualiza SReg quitandole o poniendo comida en los
                % nodos correspondientes
                SReg(str2double(ants(k).current_node)) = SReg(str2double(ants(k).current_node)) - food(str2double(ants(k).current_node));
                SReg(str2double(next_node)) = SReg(str2double(next_node)) + food(str2double(ants(k).current_node));
                Idiff = double(SRef - SReg);
                % Matriz que contiene las transformaciones utilizadas, se
                % suma o resta el valor de la comida que se pone o quita en
                % los nodos, según corresponda.
                T_final(str2double(ants(k).current_node)) = T_final(str2double(ants(k).current_node)) + food(str2double(ants(k).current_node));
                T_final(str2double(next_node)) = T_final(str2double(next_node)) - food(str2double(ants(k).current_node));
            end
            

            % Intensidad en los nodos, se actualiza con Idiff
            G.Nodes.Eta(str2double(ants(k).current_node)) = Idiff(str2double(ants(k).current_node));
            G.Nodes.Eta(str2double(next_node)) = Idiff(str2double(next_node));

            % Feromona: ecuación del paper modificacada. La modificación
            % hace que tengan más feromona los nodos donde las hormigas
            % pueden ir a traer comida.
            G.Nodes.Weight(str2double(ants(k).current_node)) = ...
                exp(-rho)*G.Nodes.Weight(str2double(ants(k).current_node)) +...
                tau_0*abs(Idiff(str2double(ants(k).current_node)))*heaviside(-Idiff(str2double(ants(k).current_node)));

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
        ants(k).path = int2str(k);
        
        % Esto también modifica a k para el movimiento deseado.
        if mod(n,grid_size) == 0
            contador = contador + 1;
            if mod(t,2) == 0
                k = contador*grid_size;
                k = hormigas - k;
            else
                k = contador*grid_size + 1;
            end
        end
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
    CC_acumulado(t,1) = CC;
    
    ref_zero = sum(any(Idiff));
    
    if isnan(CC) && ref_zero == 0
        CC = 1;
    end
    
    if CC >= 0.9
        % Condición de paro cuando el coeficiente de correlación sea lo
        % suficientemente bueno
        stop = 0;
        
        % Quita el desface dado para obtener la verdadera transformación
        T_final = T_final - desfaceM/numel(SReg);
        T_final(idesface) = T_final(idesface) - Residuo;
    end
    
    t = t + 1;
    
end
%% Best Path Calculation

if (t > t_max)
    disp("No hubo convergencia")
else
    disp("Listo")
    
%     figure;
%     imshow(uint8(SReg))
%     imwrite(uint8(SReg), 'Rodilla.jpg')
%     figure;
%     imshow(uint8(SRef))
%     figure;
%     imshow(uint8(STemp))
%     save Resultados.mat
end

    
tiempofinal = toc; 

subplot(1,3,1)
imshow(uint8(STemp))
title('Imagen plantilla')
subplot(1,3,2)
imshow(uint8(SRef))
title('Imagen de referencia')
subplot(1,3,3)
imshow(uint8(SReg))
title('Imagen registada')

%% Transformación luego del algoritmo
% Producto = STemp - T_final;
% figure;
% imshow(uint8(Producto))
