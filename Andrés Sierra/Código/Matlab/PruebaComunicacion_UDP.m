%% -- Prueba de comuncación con Sockets --
%% ----------- Cliente UDP --------------- 
% --- ENVIO DE COORDENADAS AL AGENTE -----
% ----------- [x, y, theta] --------------
pose1 = [8.8, 8.8, 88.8]; 

% ------- ENVIO DEL GRAFO AL AGENTE ------
% ---------- [nodo 1, nodo 2] ------------
conectar  = [0, 1];
conectar2 = [1, 2];
conectar3 = [3, 4];

conectar4  = [0,1;
              0,4;
              0,5;
              1,4;
              1,5;
              1,6;
              1,2;
              2,5;
              2,6;
              2,7;
              2,3;
              3,6;
              3,7;
              4,5;
              4,8;
              4,9;
              5,6;
              5,8;
              5,9;
              5,10;
              6,7;
              6,9;
              6,10;
              6,11;
              7,10;
              7,11;
              8,9;
              8,12;
              8,13;
              9,10;
              9,12;
              9,13;
              9,14;
              10,11;
              10,13;
              10,14;
              10,15;
              11,14;
              11,15;
              12,13;
              13,14;
              14,15];


% -- [nodo, coordenada x ,coordenada y] --
coord_nodos = [1, 20, 20];
coord_nodos2 = [0, 0, 0;
                1,10,0;
                2,20,0;
                3,30,0;
                4,0,10;
                5,10,10;
                6,20,10;
                7,30,10;
                8,0,20;
                9,10,20;
                10,20,20;
                11,30,20;
                12,0,30;
                13,10,30;
                14,20,30;
                15,30,30];

% ------- CONCATENAR EN UN STRING --------
% Pruebas de mensajes
stringPRUEBA = "String no aceptado";

% Enviar la posicion del robot
indicador = 'P,';
string_robot = strcat(indicador, string(pose1(1)), ',');
string_robot = strcat(string_robot, string(pose1(2)), ',');
string_robot = strcat(string_robot, string(pose1(3)));

% Enviar informacion del grafo (conexion)
indicador2 = 'G,';

string_conexion = strcat(indicador2, string(conectar(1)), ',');
string_conexion = strcat(string_conexion, string(conectar(2)), ',');

string_conexion2 = strcat(indicador2, string(conectar2(1)), ',');
string_conexion2 = strcat(string_conexion2, string(conectar2(2)), ',');

string_conexion3 = strcat(indicador2, string(conectar3(1)), ',');
string_conexion3 = strcat(string_conexion3, string(conectar3(2)), ',');

% Enviar informacion del grafo (coordenadas)
indicador3 = 'C,';
string_coord = strcat(indicador3, string(coord_nodos(1)), ',');
string_coord = strcat(string_coord, string(coord_nodos(2)), ',');
string_coord = strcat(string_coord, string(coord_nodos(3)));

% ------ EN CASO QUE SE TENGAN MAS AGENTES ---------
% Agregar la cantidad de string necesarios para enviar la información a
% todos los agentes que se tengan. 

% string2 = strcat(indicador, string(pose2(1)), ',');
% string2 = strcat(string2, string(pose2(2)), ',');
% string2 = strcat(string2, string(pose2(3)));


% ----- DIRECCION IP DEL AGENTE PARA EL ENVIO ------
address1 = '10.0.0.2';
% address1 = '192.168.0.10';
% address2 = '192.168.0.12';

% --------- PUERTO DE COMUNICACION (FIJO) ----------
port = 2050; 
% echoudp('on', port);  % Si queremos enviar y recibir aquí mismo.
                        % Comentarlo si queremos intercambiar mensajes con otro 

% -------- VARIABLE PARA SEGUIR ENVIANDO -----------
continueRunning = true;

% ================ CREAR EL SOCKET DE ENVIO =====================
% ---------------------- PARA MATLAB 2021 -----------------------
% u = udpport('byte',"IPV4");
% ---------------------- PARA MATLAB 2019 -----------------------
% -- USANDO LOCAL HOST --
u = udp(address1,port,'Timeout', 20);
% HOST REMOTO. Colocar la IP del otro lado (esta u otra computadora)
% u = udp('192.168.1.15', puerto_enviar, 'LocalPort', port, 'Timeout', 20);

fopen(u);

% Con esto, se termina la recepción hasta detectar el '\n'
% u.DatagramTerminateMode = 'off';  

% CICLO PARA ENVIAR LA INFORMACION - Siempre que la variable
% continueRunning sea igual a true, se enviará la información contenida en
% el string1 a la dirección address1

while(continueRunning)
% ------------------ MATLAB 2021 -------------------
%     write(u,string1,"char",address1,port);
%     write(u,string2,"char",address2,port);
%% --------- Enviar y recibir MATLAB 2019 ----------
    % fwrite(u,65:74)           % Envía como enteros
    % A = fread(u,10)           % Lee como enteros
    caso = 2;
    switch caso
        case 1 
            for it = 1:42
                string_conexion4 = strcat(indicador2, string(conectar4(it,1)), ',');
                string_conexion4 = strcat(string_conexion4, string(conectar4(it,2)), ',');
                fprintf(u, string_conexion4);            % Envía como cadenas
                n = 0.1;
                pause(n)
            end 
            for no = 1:16
                string_coord2 = strcat(indicador3, string(coord_nodos2(no,1)), ',');
                string_coord2 = strcat(string_coord2, string(coord_nodos2(no,2)), ',');
                string_coord2 = strcat(string_coord2, string(coord_nodos2(no,3)));
                fprintf(u, string_coord2);               % Envía como cadenas
                n = 0.1;
                pause(n)
            end
        case 2       
            fprintf(u, string_robot);             % Envía como cadenas  
    end
%     fprintf(u, string_conexion);            % Envía como cadenas
%     fprintf(u, string_conexion2);           % Envía como cadenas
%     fprintf(u, string_conexion3);           % Envía como cadenas
%     fprintf(u, string_coord);               % Envía como cadenas
%     
%     fprintf(u, string_robot);             % Envía como cadenas
    
    
%     fprintf(u, stringPRUEBA);             % Envía como cadenas
    
    msg = fscanf(u)             % Lee como cadenas

%     fprintf(u, 'Hasta luego'); % Envía como cadenas
    continueRunning = input('Continue? ');
end
%% Desconectar y limpiar
% ---- FUNCIONA UNICAMENTE MATLAB 2021 -------
% data = readline(u);
% disp(data)
% --------- FUNCIONA PARA MATLAB 2019 --------
%echoudp('off')
fclose(u);
delete(u)
clear u