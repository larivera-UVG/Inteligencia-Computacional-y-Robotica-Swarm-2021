% Diseño e Innovación 2
% Ant Colony Optimization 
% Nombre: Andres Sierra 
% Fecha: 2021

% Descripción: Visualizar la ruta obtenida al correr el algoritmo ACO en
% la rasp en lenguaje C++
%%
% ----- CREAR EL GRAFO A UTILIZAR -------
grid_size_w = 10;
grid_size_h = 10;
G = graph_grid(grid_size_h,grid_size_w);
% ------ NODO INICIAL ------
nodo_init = "1";

% --- CASOS A VALIDAR ------
caso = 7;
switch caso
    case 1
        nodo_dest = '23';
        best_path = [1;12;23];
    case 2 
        nodo_dest = '34';
        best_path = [1;12;23;34];
    case 3
        nodo_dest = '24';
        best_path = [1;2;13;24];
    case 4
        nodo_dest = '45';
        best_path = [1;12;23;34;45];
    case 5
        nodo_dest = '56';
        best_path = [1;12;23;34;45;56];
    case 6
        nodo_dest = '48';
        best_path = [1;12;13;14;25;36;37;48];
    case 7 
        nodo_dest = '73';
        best_path = [1;11;21;31;42;53;63;73];
end


figure(3); clf; 
h = plot(G, 'XData', G.Nodes.X, 'YData', G.Nodes.Y, 'NodeColor', 'k'); 

hold on 
nodos_especiales = [G.Nodes.X(str2double(nodo_init)), G.Nodes.Y(str2double(nodo_init)); G.Nodes.X(str2double(nodo_dest)), G.Nodes.Y(str2double(nodo_dest))];
scatter(nodos_especiales(1, 1), nodos_especiales(1, 2), 'g','filled')
scatter(nodos_especiales(2, 1), nodos_especiales(2, 2), 'xr','LineWidth', 5)

hold on;
plot(G.Nodes.X(best_path), G.Nodes.Y(best_path),'r','LineWidth', 2)

