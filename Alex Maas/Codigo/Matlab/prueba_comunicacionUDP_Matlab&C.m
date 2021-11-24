%% Prueba de comuncación con Sockets
%% Cliente UDP 
%Valores a enviar
pose1 = [5.5, 5.5, 55.0]; % [x, y, theta]
pose2 = [6.0, 6.0, 66.0]; % [x, y, theta]
pose3 = [7.0, 7.0, 77.0]; % [x, y, theta]
pose4 = [8.0, 8.0, 88.0]; % [x, y, theta]

%Conversion
indicador = 'T2,';
string1 = strcat(indicador, string(pose1(1)), ',');
string1 = strcat(string1, string(pose1(2)), ',');
string1 = strcat(string1, string(pose1(3)));

string2 = strcat(indicador, string(pose2(1)), ',');
string2 = strcat(string2, string(pose2(2)), ',');
string2 = strcat(string2, string(pose2(3)));

string3 = strcat(indicador, string(pose3(1)), ',');
string3 = strcat(string3, string(pose3(2)), ',');
string3 = strcat(string3, string(pose3(3)));

string4 = strcat(indicador, string(pose4(1)), ',');
string4 = strcat(string4, string(pose4(2)), ',');
string4 = strcat(string4, string(pose4(3)));

%Direcciones
address1 = '10.0.0.4';
address2 = '10.0.0.3';
address3 = '10.0.0.5';
address4 = '10.0.0.6';
port = 2010;                %Puerto fijo
continueRunning = true;
u = udpport('byte',"IPV4");
while(continueRunning)
    write(u,string1,"char",address1,port);
    write(u,string2,"char",address2,port);
    write(u,string3,"char",address3,port);
    write(u,string4,"char",address4,port);
    continueRunning = input('Continue? ');
end
data = readline(u);
disp(data)