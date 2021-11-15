% Cargar datos
load('datos0.mat')
load('datos1.mat')
load('datos2.mat')
load('datos3.mat')
load('datos4.mat')
load('datos5.mat')
load('datos6.mat')
load('datos7.mat')
load('datos8.mat')
load('datos9.mat')
i=1;

%Direcciones
address0 = '10.0.0.4';
address1 = '10.0.0.4';
address2 = '10.0.0.4';
address3 = '10.0.0.4';
address4 = '10.0.0.4';
address5 = '10.0.0.4';
address6 = '10.0.0.4';
address7 = '10.0.0.4';
address8 = '10.0.0.4';
address9 = '10.0.0.4';

port = 2010;                %Puerto fijo
continueRunning = true;
u = udpport('byte',"IPV4");
while(continueRunning)
    % Arrlego nececsario y envio de datos
    string0 = strcat(string(data0(i,1)), ',');
    string0 = strcat(string0, string(data0(i,2)), ',');
    string0 = strcat(string0, string(data0(i,3)));
    write(u,string0,"char",address0,port);
    
    string1 = strcat(string(data1(i,1)), ',');
    string1 = strcat(string1, string(data1(i,2)), ',');
    string1 = strcat(string1, string(data1(i,3)));
    write(u,string1,"char",address1,port);
    
    string2 = strcat(string(data2(i,1)), ',');
    string2 = strcat(string2, string(data2(i,2)), ',');
    string2 = strcat(string2, string(data2(i,3)));
    write(u,string2,"char",address2,port);
    
    string3 = strcat(string(data3(i,1)), ',');
    string3 = strcat(string3, string(data3(i,2)), ',');
    string3 = strcat(string3, string(data3(i,3)));
    write(u,string3,"char",address3,port);
    
    string4 = strcat(string(data4(i,1)), ',');
    string4 = strcat(string4, string(data4(i,2)), ',');
    string4 = strcat(string4, string(data4(i,3)));
    write(u,string4,"char",address4,port);
    
    string5 = strcat(string(data5(i,1)), ',');
    string5 = strcat(string5, string(data5(i,2)), ',');
    string5 = strcat(string5, string(data5(i,3)));
    write(u,string5,"char",address5,port);
    
    string6 = strcat(string(data6(i,1)), ',');
    string6 = strcat(string6, string(data6(i,2)), ',');
    string6 = strcat(string6, string(data6(i,3)));
    write(u,string6,"char",address6,port);
    
    string7 = strcat(string(data7(i,1)), ',');
    string7 = strcat(string7, string(data7(i,2)), ',');
    string7 = strcat(string7, string(data7(i,3)));
    write(u,string7,"char",address7,port);
    
    string8 = strcat(string(data8(i,1)), ',');
    string8 = strcat(string8, string(data8(i,2)), ',');
    string8 = strcat(string8, string(data8(i,3)));
    write(u,string8,"char",address8,port);
    
    string9 = strcat(string(data9(i,1)), ',');
    string9 = strcat(string9, string(data9(i,2)), ',');
    string9 = strcat(string9, string(data9(i,3)));
    write(u,string9,"char",address9,port);
    
    %pause(10);
    continueRunning = input('Continue? ');
    i=i+1;
%     string0
%     string1
%     string2
%     string3
%     string4
%     string5
%     string6
%     string7
%     string8
%     string9
end