%% Prueba de comuncación con Sockets
%% ----------- Cliente UDP --------------- 
% --- ENVIO DE COORDENADAS AL AGENTE -----
% ----------- [x, y, theta] --------------

pose1 = [5.5, 5.5, 55.0]; 
% pose2 = [6.0, 6.0, 66.0]; 

% ------- CONCATENAR EN UN STRING --------
indicador = 'T2,';
string1 = strcat(indicador, string(pose1(1)), ',');
string1 = strcat(string1, string(pose1(2)), ',');
string1 = strcat(string1, string(pose1(3)));

% ------ EN CASO QUE SE TENGAN MAS AGENTES ---------
% Agregar la cantidad de string necesarios para enviar la información a
% todos los agentes que se tengan. 

% string2 = strcat(indicador, string(pose2(1)), ',');
% string2 = strcat(string2, string(pose2(2)), ',');
% string2 = strcat(string2, string(pose2(3)));


% ----- DIRECCION IP DEL AGENTE PARA EL ENVIO ------
% address1 = '10.0.0.4';
address1 = '192.168.0.9';
% address2 = '192.168.0.12';

% --------- PUERTO DE COMUNICACION (FIJO) ----------
port = 2010; 
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
%     write(u,string1,"char",address1,port);
%     write(u,string2,"char",address2,port);
%% Enviar y recibir
    % fwrite(u,65:74)     % Envía como enteros
    % A = fread(u,10)     % Lee como enteros
    fprintf(u, string1); % Envía como cadenas
    msg = fscanf(u)     % Lee como cadenas

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