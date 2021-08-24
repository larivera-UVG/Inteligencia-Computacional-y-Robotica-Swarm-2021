% Algoritmo de planificación de trayectorias y evasión de obstáculos
% Daniela Baldizón
% 1/06/2021
% Se tomaron partes del código ACO de Gaby Iriarte

clc
clear

%% Creamos grid cuadrado con la cantidad de nodos indicada:
    grid_size = 10;
    cost_diag = 0.5;
    tau_0 = 0.1;  % Valor de tau inicial
    G = graph_grid(grid_size);
    G_cpy = G;
    nodo_dest = '10';
    nodo_init = "91";
    obstaculos = [4,5,6, 71, 73, 72, 78, 89, 99, 67,56,18,19,28,29]';
%     obstaculos = [ 9,13,17]';
        
    %% ACO init
t_max = 150; 
hormigas = 60;

rho = 0.8; 
alpha = 0.9;
beta = 1; 
Q = 2.1; 
epsilon = 0.8; 
gamma = 1; 
Sjo =1;

% Matriz peso
D = zeros(grid_size^2);
H = zeros(grid_size^2+grid_size+1);

% Inicialización de matrices
submapa = zeros(3);
OL = zeros(3);
OD = zeros(3);
mapa = zeros(grid_size+2); % Creación del mapa

% Preallocation
ants(1:hormigas) = struct('blocked_nodes', [], 'last_node', nodo_init,...
    'current_node', nodo_init, 'path', nodo_init, 'L', zeros(1, t_max));
% alpha_sweep_data = cell(numel(sweep) * repetitions + 1, 5);
alpha_sweep_data(1, :) = {'tiempo', 'costo', 'iteraciones', 'path', 'alpha'};
t_acumulado = 0;
mode_plot = zeros(t_max, 1);
mean_plot = zeros(t_max, 1);

%% ACO loop

for pruebas = 1:11
    timer = tic
    alpha = alpha + 0.1;
    all_path = cell(hormigas, t_max);
    L = zeros(hormigas, t_max); % Lenght del path por hormiga e iteración
    t = 1;
    stop = 1;
    G = G_cpy;
    pruebas
    
    % Gradient Color para la animación
    % Solo es para personalizar los colores de la animación
    map = [255 255 255
        245 215 250
        255 166 216
        255 111 150
        255 61 61]/255;

    figure(1); clf;
    % Se crea la imagen animada
    h = plot(G, 'XData', G.Nodes.X+0.5, 'YData', G.Nodes.Y+0.5, 'NodeColor', 'k'); 
    hold on 
    nodos_especiales = [G.Nodes.X(str2double(nodo_init)), G.Nodes.Y(str2double(nodo_init)); G.Nodes.X(str2double(nodo_dest)), G.Nodes.Y(str2double(nodo_dest))];
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
        mapa(b(1),b(2)) = 1;
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
%     t
        for k = 1:hormigas
            OL = 0;
            OD = 0;

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
                           OL(i,j) = 1;
                        elseif((Dx(i,j)==1 && Dy(i,j)==1)&&submapa(i,j)==1)
                            % Obstáculo en diagonal (Condición 2)
                           OD(i,j) = 1;
                        else
                            OL(i,j) = 0;
                            OD(i,j) = 0;
                        end
                    end
                end

                if isempty(find(OL))==0
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
                end

                if isempty(find(OD))==0
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
                if(visitado == 0)
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
            all_path{k, t} = ants(k).path;  % Equivale a x_k(t)

            % Regresamos la hormiga k al inicio
            ants(k).current_node = nodo_init;
            ants(k).blocked_nodes = [];
            ants(k).last_node = nodo_init;

            % Largo (aristas) de camino de cada hormiga
            caminos(k) = numel(ants(k).path);

        end

        %% Evaporación de Feromona
        G.Edges.Weight = G.Edges.Weight * (1 - rho);

        % Global best lenght
        lgb = min(caminos);
        lgb_index = find(caminos==lgb);
        Lmin = min(min(L)); %Costo min

        %% Update de Feromona
        for k = 1:hormigas
            dtau = Q/numel(ants(k).path);
            edge_index = findedge(G, ants(k).path(1:end - 1), ants(k).path(2:end));
            G.Edges.Weight(edge_index) = G.Edges.Weight(edge_index) + Q*dtau;
            if k == lgb_index
                G.Edges.Weight(edge_index) = G.Edges.Weight(edge_index)*2.5;
            end

            if L(k,t) == Lmin
                G.Edges.Weight(edge_index) = G.Edges.Weight(edge_index)*2.5;
            end

            ants(k).path = nodo_init;
        end
        
        [~,F] = mode(L(:,t));

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

    if (t > t_max)
        disp("No hubo convergencia")
        tiempofinal = toc(timer);
        alpha_sweep_data(pruebas + 1, :) = {tiempofinal, 0, t - 1, best_path, alpha};
    else
        moda =  mode(L(:, t-1));
        len_indx = L(:, t-1).*(L(:,t-1) == moda);
        len_prob = rouletteWheel(len_indx);
        best_path = all_path{len_prob, t-1};
        tiempofinal = toc(timer);
        alpha_sweep_data(pruebas + 1, :) = {tiempofinal, L(len_prob, t-1), t - 1, best_path, alpha};

    end
    t_acumulado = t_acumulado + tiempofinal;
    if t_acumulado >= 60
        disp("Guardando...")
        save('sweep_data', 'alpha_sweep_data', '-append')
        t_acumulado = 0;
    end
end
% Guardado final
save('sweep_data', 'alpha_sweep_data', '-append')
disp("Done.")
