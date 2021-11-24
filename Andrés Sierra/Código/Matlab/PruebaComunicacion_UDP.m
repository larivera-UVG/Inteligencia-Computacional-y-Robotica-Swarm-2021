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
address1 = '10.0.0.4';
% address2 = '192.168.0.12';

% --------- PUERTO DE COMUNICACION (FIJO) ----------
port = 2010;  
% ------------- VARIABLE PARA ENVIAR ---------------
continueRunning = true;
% ---------- CREAR EL SOCKET DE ENVIO --------------
u = udpport('byte',"IPV4");

% CICLO PARA ENVIAR LA INFORMACION - Siempre que la variable
% continueRunning sea igual a true, se enviará la información contenida en
% el string1 a la dirección address1
while(continueRunning)
    write(u,string1,"char",address1,port);
%     write(u,string2,"char",address2,port);
    continueRunning = input('Continue? ');
end
data = readline(u);
disp(data)