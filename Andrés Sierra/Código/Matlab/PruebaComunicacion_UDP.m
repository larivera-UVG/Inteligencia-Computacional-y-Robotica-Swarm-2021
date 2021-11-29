%% -- Prueba de comuncación con Sockets --
%% ----------- Cliente UDP --------------- 
% --- ENVIO DE COORDENADAS AL AGENTE -----
% ----------- [x, y, theta] --------------
pose1 = [8.8, 8.8, 88.8]; 
% pose2 = [6.6, 6.6, 66.6]; 
% ------- ENVIO DEL GRAFO AL AGENTE ------
% ---------- [nodo 1, nodo 2] ------------
conectar = [0, 1];
% -- [nodo, coordenada x ,coordenada y] --
coord_nodos = [1, 20, 20];
% ------- CONCATENAR EN UN STRING --------
% Pruebas de mensajes
stringPRUEBA = "String no aceptado";

% Enviar la posicion del robot
indicador = 'P,';
string1 = strcat(indicador, string(pose1(1)), ',');
string1 = strcat(string1, string(pose1(2)), ',');
string1 = strcat(string1, string(pose1(3)));

% Enviar informacion del grafo (conexion)
indicador2 = 'G,';
string2 = strcat(indicador2, string(conectar(1)), ',');
string2 = strcat(string2, string(conectar(2)), ',');


% Enviar informacion del grafo (coordenadas)
indicador3 = 'C,';
string3 = strcat(indicador3, string(coord_nodos(1)), ',');
string3 = strcat(string3, string(coord_nodos(2)), ',');
string3 = strcat(string3, string(coord_nodos(3)));

% ------ EN CASO QUE SE TENGAN DATOS QUE ENVIAR ---------
% Agregar la cantidad de string necesarios para enviar la información. 

indicadorN ='N,';
% stringN = strcat(indicadorN, string(pose2(1)), ',');
% stringN = strcat(stringN, string(pose2(2)), ',');
% stringN = strcat(stringN, string(pose2(3)));


% ----- DIRECCION IP DEL AGENTE PARA EL ENVIO ------
% address1 = '10.0.0.5';
address1 = '192.168.0.12';
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
    
    fprintf(u, string1);        % Envía como cadenas
    fprintf(u, string2);        % Envía como cadenas
    fprintf(u, string3);        % Envía como cadenas
    fprintf(u, stringPRUEBA);        % Envía como cadenas
    
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
