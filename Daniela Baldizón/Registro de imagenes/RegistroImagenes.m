clear
clc

% Referencia
% Ref = rgb2gray(imread('Lena.jpg'));

% Template
% Temp = rgb2gray(imread('Lena Distorsion.jpg'));
% Temp = Ref;

% SRef = double(Ref(1:10,1:10));
% STemp = double(Temp(1:10,1:10));
% SReg = STemp;

Im = zeros(10);
Im(4:7,4) = 155;
Im(4:7,5) = 168;
Im(4:7,6) = 161;

Im2 = zeros(10);
Im2(4:7,3) = 155;
Im2(4:7,4) = 168;
Im2(4:7,7) = 161;

SRef = Im;
STemp = Im2;
SReg = STemp;
% figure;
% imshow(SubJ)
% 
% figure;
% imshow(JD)

%% Graph generation
tic
% Creamos grid cuadrado con la cantidad de nodos indicada:
grid_size = size(SRef,1);
cost_diag = 0.5;
tau_0 = 0.1;  % Valor de tau inicial
G = graph_grid(grid_size, tau_0);
% nodo_dest = '10';
nodo_init = "1";

%% ACO init
t_max = 20; 
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
% Escala de las distancias de avance
r = 0.0001;

% Preallocation
path_k = cell(hormigas, 1); % Inicializa el path de las hormigas en 0
L = zeros(hormigas, t_max); % Lenght del path por hormiga e iteración
all_path = cell(hormigas, t_max); % El path de todas las hormigas
% Coloca las hormigas de forma uniforme en el grid

%% Nuevo
% Si Idiff es negativo, la hormiga debe buscar intensidades más bajas con
% los vecinos. Si es mayor o igual a cero, va a buscar valores de
% intensidad más altos con los vecinos.
% Alimento de las hormigas
Idiff = double(SRef - STemp);

for i = 1:hormigas
    
%     % encuentra las coordenadas de los vecinos próximos y la propia por
%     % cada pixel
%     proximity = [convertCharsToStrings(neighbors(G, i));
%         i];
%     
%     % Posicion actual por hormiga (columnas y filas). La referencia es el
%     % origen de la imagen (esquina superior izquierda)
%     m = G.Nodes.X(double(proximity));
%     l = abs(-grid_size + G.Nodes.Y(double(proximity))) + 1;
%     
%     weight_int_vec = 0;
%     
%     % encuentra el peso de los pixeles en proximity. El peso está dado como
%     % la diferencia ente la imagen de referencia y el template.
%     for j = 1:size(m,1)
%         weight_int_vec(j) = Idiff(l(j),m(j));
%     end
%     
%     % Establece como nodo de destino al nodo con menor peso dentro de
%     % proximity
%     weight_dest = min(weight_int_vec);
%     index_dest = find(weight_int_vec == weight_dest);
%     index_dest = index_dest(end);
%     x = m(index_dest(1));
%     y = abs(-grid_size + l(index_dest)) + 1;
%     nodo_dest(i) = double(findNode(G,x,y));
    
    % Struct de las hormigas
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
% figure(1); clf;
% h = plot(G, 'XData', G.Nodes.X, 'YData', G.Nodes.Y, 'NodeColor', 'k'); 
% hold on 
% nodos_especiales = [G.Nodes.X(str2double(nodo_init)), G.Nodes.Y(str2double(nodo_init)); G.Nodes.X(str2double(nodo_dest)), G.Nodes.Y(str2double(nodo_dest))];
% scatter(nodos_especiales(1, 1), nodos_especiales(1, 2), 'g','filled')
% scatter(nodos_especiales(2, 1), nodos_especiales(2, 2), 'xr','LineWidth', 5)

% colormap(map);
contador = 0;
%%
while (t <= t_max && stop)
    
    parfor k = 1:hormigas
        %% Inicio debug
%         contador = contador + 1;
       % while (not(strcmp(ants(k).current_node, nodo_dest))) % Mientras no se haya llegado al destino
           
            % Si Idiff es negativo, la hormiga debe buscar intensidades más bajas con
            % los vecinos. Si es mayor o igual a cero, va a buscar valores de
            % intensidad más altos con los vecinos.
            % Alimento de las hormigas
            Idiff = double(SRef - SReg);
            
            % Posicion actual por hormiga (Se hace esto para que las
            % posiciones en la imagen coincidan con el grafo)
            m = G.Nodes.X(str2num(ants(k).current_node));
            l = G.Nodes.Y(str2num(ants(k).current_node));
            l = abs(-grid_size + l(1)) + 1; % Fila
            m = m(1); % Columna
            
            % encuentra los nodos vecinos
            proximity = [convertCharsToStrings(neighbors(G, k))];
            
            % Posicion actual por hormiga (columnas y filas). La referencia es el
            % origen de la imagen (esquina superior izquierda)
            colum_coor = G.Nodes.X(double(proximity));
            fila_coor = abs(-grid_size + G.Nodes.Y(double(proximity))) + 1;
            
            weight_int_vec = 0;
            
            % encuentra el peso de los pixeles en proximity. El peso está dado como
            % la diferencia ente la imagen de referencia y el template.
            for j = 1:size(colum_coor,1)
                weight_int_vec(j) = Idiff(fila_coor(j),colum_coor(j));
            end
            
            if (Idiff(l,m)>=0)
                % Bloquea los nodos que tengan peso menor a 0 para que solo
                % busque pesos más grandes
                nodos_bloqueados_menor = [];
                index_Idiff_menor = find(weight_int_vec<0);
                nodos_bloqueados_menor = proximity(index_Idiff_menor);
                ants(k).blocked_nodes = [ants(k).blocked_nodes; ...
                    string(nodos_bloqueados_menor)];
            else
                % Bloquea los nodos que tengan peso mayor o igual a 0, para
                % que solo busque pesos pequeños. 
                nodos_bloqueados_mayor = [];
                index_Idiff_mayor = find(weight_int_vec>=0);
                nodos_bloqueados_mayor = proximity(index_Idiff_mayor);
                ants(k).blocked_nodes = [ants(k).blocked_nodes; ...
                    string(nodos_bloqueados_mayor)];
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
                next_node = ant_decision(vecinos_updated, alpha, beta, G, ants(k).current_node);
                
                dirx = G.Nodes.X(str2num(next_node)) - G.Nodes.X(str2num(ants(k).current_node));
                diry = G.Nodes.Y(str2num(next_node)) - G.Nodes.Y(str2num(ants(k).current_node));
            
                % Gradiente de la imagen
                [Tx, Ty] = imgradientxy(SReg);

                % Distancia de avance eqn (9) 
                Dx = sign(dirx)*abs(r*Idiff(l,m)*Tx(l,m));
                Dy = sign(diry)*abs(r*Idiff(l,m)*Ty(l,m));

                % (Agregar signo del gradiente a las magnitudes de las
                % distancias)
                SRegX(k) = round(l+Dx);
                SRegY(k) = round(m+Dy);
                
                
                % Truncar los resultados para que no sean menores a 1 o
                % mayores a 10
                if SRegX(k)<1
                    SRegX(k) = 1;
                elseif SRegX(k)>10
                    SRegX(k) = 10;
                end
                
                if SRegY(k)<1
                    SRegY(k) = 1;
                elseif SRegY(k)>10
                    SRegY(k) = 10;
                end
                
                ants(k).last_node = [ants(k).last_node; ants(k).current_node];
                ants(k).current_node = next_node;
                ants(k).path = [ants(k).path; ants(k).current_node];
            end
            
     %   end

        % Le quitamos los loops al path y ahora los índices son números y
        % no strings.
        
        ants(k).path = loop_remover(str2double(ants(k).path));
        L(k, t) = sum(G.Edges.Eta(findedge(G, ants(k).path(1:end-1), ants(k).path(2:end))).^-1);
        all_path{k, t} = ants(k).path;  % Equivale a x_k(t)
        
        % Regresamos la hormiga k al inicio (k-esimo nodo)
        ants(k).current_node = int2str(k);
        ants(k).blocked_nodes = [];
        ants(k).last_node = int2str(k);
    %% Fin debug    
    end
    
    %% Actualización de foto
    for k = 1:hormigas
        % Posicion actual por hormiga
        m = G.Nodes.X(double(ants(k).current_node));
        l = G.Nodes.Y(double(ants(k).current_node));
        l = abs(-grid_size + l(1)) + 1; % Fila
        m = m(1); % Columna
        SReg(SRegX(k),SRegY(k)) = SRef(l,m);
%         STemp = SReg
    end
    
    %% Evaporación de Feromona
%     G.Edges.Weight = G.Edges.Weight * (1 - rho);
    
    %% Update de Feromona
    for k = 1:hormigas
%         dtau = Q/numel(ants(k).path);
%         edge_index = findedge(G, ants(k).path(1:end - 1), ants(k).path(2:end));
        if size(ants(k).path,1) ~= 1
            G.Nodes.WeightN((ants(k).path)) = ...
                exp(-rho)*G.Nodes.WeightN((ants(k).path)) + tau_0;
        else
            G.Nodes.WeightN((ants(k).path)) = 0.75*G.Nodes.WeightN((ants(k).path));
        end
        all_weight(k) = G.Nodes.WeightN(k);
        ants(k).path = int2str(k);
    end
    
    [mode_plot(t), F] = mode(L(:,t));
    mean_plot(t) = mean(L(:,t));
    
    if (F/hormigas >= epsilon) % condición de paro
        stop = 0;
    end
    
%     % Animación
%     G.Nodes.NormWeight = G.Nodes.WeightN/sum(G.Nodes.WeightN);
%     G.Edges.NormWeight = 0*G.Edges.Weight/sum(G.Edges.Weight);
%     h2.YData(t) = mean_plot(t);
%     h3.YData(t) = mode_plot(t);
%     h.MarkerSize = G.Nodes.NormWeight * 300;
%     h.EdgeCData = G.Edges.NormWeight*0;
%     drawnow limitrate
    t = t + 1;
    
    
end
%% Best Path Calculation

if (t > t_max)
    disp("Final")
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
    
%     % Gráfica
%     figure(2);
%     hold on;
%     plot(G.Nodes.X(best_path), G.Nodes.Y(best_path),'r')

end

%     WeightMatrix =  reshape(all_weight,grid_size,grid_size);
%     Regis = round(STemp.*(WeightMatrix./max(WeightMatrix)));
%     DiffR = SRef-Regis
% %     imtool(Regis)
%     figure;
%     imshow(Regis)
%     figure;
%     imshow(SRef)
% profile viewer % Es parte del profiler, descomentar para ver
tiempofinal = toc; 
