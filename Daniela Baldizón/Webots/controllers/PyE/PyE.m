% MATLAB controller for Webots
% File:          PyE.m
% Date:
% Description:
% Author:        Daniela Baldizon
% Modifications:

% uncomment the next two lines if you want to use
% MATLAB's desktop to interact with the controller:
%desktop;
%keyboard;
load('webots_test.mat');

TIME_STEP = 64;
ell = 71/2000;  % Distance from center en metros
r = 20.5/1000;  % Radio de las llantas en metros
MAX_SPEED = 6.28;
MAX_CHANGE = 0.001;  % rad/s
goal_points = webots_path;

controlador = 1;

%% Sensores del e-Puck

% Pose
position_sensor = wb_robot_get_device('gps');
orientation_sensor = wb_robot_get_device('compass');

left_motor = wb_robot_get_device('left wheel motor');
right_motor = wb_robot_get_device('right wheel motor');
wb_motor_set_position(left_motor, inf);
wb_motor_set_position(right_motor, inf);
wb_motor_set_velocity(left_motor, 0);
wb_motor_set_velocity(right_motor, 0);

% Enables GPS and compass
wb_gps_enable(position_sensor, TIME_STEP);
wb_compass_enable(orientation_sensor, TIME_STEP);

%% Matrices del sistema linealizado alrededor del punto de operación
A = zeros(2); 
B = eye(2);

% LQR 
Q = eye(2);
R = 2000*eye(2);

% LQR + Integradores (LQI)
Cr = eye(2);
Dr = zeros(2);
AA = [A, zeros(size(Cr')); Cr, zeros(size(Cr,1))];
BB = [B; Dr];
QQ = eye(size(A,1) + size(Cr,1)); QQ(3,3) = 5; QQ(4,4) = 0.0002; 
klqi = lqr(AA, BB, QQ, R);
sigma = [0,0];

% Modificaciones de Aldo para el LQI:
bv_p = 0.95;          % Reducir velocidad de control proporcional en 95% evitando aceleracion brusca por actualizacion PSO
bv_i = 0.01;          % Reducir velocidad de control integrador en 1% cada iteracion para frenado al acercarse a Meta PSO
yn_1 = [0, 0];
yn = [0, 0];
x_n = [0, 0];

% Control de pose
k_rho = 0.8;
k_alpha = 20;
k_beta = -0.05; 

%Control de pose de Lyapunov
k_rho2 = 0.8;
k_alpha2 = 20;

stop_counter = 0;
path_node = 1;

% Contador para moverse en el path
cont = 1;

% Tolerancia para cambiar de punto en el path
tol = 0.05;

% Variables para las gráficas
pos = wb_gps_get_values(position_sensor);
xi = pos(1);  zi = pos(3);

trajectory = [xi, zi];
v_hist = [];
w_hist = [];
rwheel_hist = [];
lwheel_hist = [];

%% main loop:
% perform simulation steps of TIME_STEP milliseconds
% and leave the loop when Webots signals the termination
%
while wb_robot_step(TIME_STEP) ~= -1
    
    pos = wb_gps_get_values(position_sensor);
    xi = pos(1);  zi = pos(3);

    north = wb_compass_get_values(orientation_sensor);
    rad = atan2(north(1), north(3));

    if rad < 0
    rad = rad + 2*pi; 
    end
    
    theta = pi - rad;  % Se corrige el �ngulo para que est� igual que theta_g
    
%     if sqrt((xg - xi)^2 + (zg - zi)^2) <= epsilon
%         controlador = 0;
%     end
% 
%     % ------------- OFF -------------
%     if controlador == 0
%         if xg == goals(length(goals), 1) && zg == goals(length(goals), 2)
%             v = 0;
%             w = 0;
%         end
%     end
    
    xg = goal_points(cont,1);
    zg = goal_points(cont,2);

    % read the sensors, e.g.:
    %  rgb = wb_camera_get_image(camera);

    % Process here sensor data, images, etc.

    % send actuator commands, e.g.:
    %  wb_motor_set_postion(motor, 10.0);

    % if your code plots some graphics, it needs to flushed like this:
    if controlador == 1
        % Controlador LQI
        elqi = [xi-xg, zi-zg];
        % Error de posición
        eP = sqrt(elqi(1)^2+elqi(2)^2);
        e = [xi - xg; zi - zg];
        thetag = atan2(elqi(2),elqi(1));
        theta = thetag - rad;

        sigma = [sigma(1) + (xi-xg)*TIME_STEP/1000; ...
        sigma(2) + (zi-zg)*TIME_STEP/1000];

       % mu = [-(klqi(1,1)*xi+klqi(1,2)*zi+klqi(1,3)*sigma(1)+klqi(1,4)*sigma(2)), ...
        %-(klqi(2,1)*xi+klqi(2,2)*zi+klqi(2,3)*sigma(1)+klqi(2,4)*sigma(2))];
        mu = -klqi*[e*(1-bv_p); sigma];
        v = 0.5*(mu(1)*cos(theta)+mu(2)*sin(theta));
        w = 0.5*(-mu(1)*sin(theta)+mu(2)*cos(theta))/ell;
    elseif controlador == 2
        % Controlador de pose
        % Error total de posicion
        rho = sqrt((xg - xi)^2 + (zg - zi)^2);
        eP = rho; % Error de posición para los puntos
        
        % Error de orientacion
        theta_g = -atan2((zg - zi), (xg - xi));
        alpha = -theta + theta_g;
        beta = -theta - alpha;
        % disp(theta_g*180/pi)
        
        theta_g = -atan2((zg - zi), (xg - xi));
        eO = atan2(sin(theta_g - theta), cos(theta_g - theta));

        %formatSpec = 'xi: %.2f, zi: %.2f  | theta: %.2f theta g: %.2f eO:%.2f sin: %.2f cos: %.2f \n';
        %fprintf(formatSpec, xi, zi, theta*180/pi, theta_g*180/pi, eO, sin(theta_g - theta), cos(theta_g - theta));
        
        
        if (alpha < -pi)
            alpha = alpha + (2*pi);
        elseif (alpha > pi)
            alpha = alpha - (2*pi);
        end
        
        if (beta < -pi)
            beta = beta + (2*pi);
        elseif (beta > pi)
            beta = beta - (2*pi);
        end
        
        v = k_rho*rho;
        w = k_alpha*alpha + k_beta*beta;
    elseif controlador == 3
        % Controlador de pose de Lyapunov
        % Error total de posicion
        rho = sqrt((xg - xi)^2 + (zg - zi)^2);
        eP = rho; % Error de posición para los puntos
        
        % Error de orientacion
        theta_g = -atan2((zg - zi), (xg - xi));
        alpha = -theta + theta_g;
        
        if (alpha < -pi)
            alpha = alpha + (2*pi);
        elseif (alpha > pi)
            alpha = alpha - (2*pi);
        end
               
        v = k_rho2 * rho * cos(alpha);
        w = k_rho2 * sin(alpha) * cos(alpha) + k_alpha2*alpha;
        
        if alpha <= -pi/2 || alpha > pi/2
            v = -v;
        end
    end

    [controlador xi zi xg zg]
    % mapeo de velocidades
    rd = (v+w*ell)/r;
    ri = (v-w*ell)/r;
    speed = [ri, rd];
    
    % Se trunca la velocidad
    for i = 1:2
        
%         if controlador == 2
%             if (abs(speed(i)) < 1) && (controlador ~= 0)
%                 if path_node >= length(goals)-1
%                     speed(k) = (speed(i)+ MAX_SPEED/2)*exp(-stop_counter);
%                     stop_counter = stop_counter + 0.0375;
%                 else
%                     speed(i) = speed(i) + MAX_SPEED/2;
%                 end
%             end
            
        if speed(i) < -MAX_SPEED
            speed(i) = -MAX_SPEED;
        elseif speed(i) > MAX_SPEED
            speed(i) = MAX_SPEED;
        end
    end

    wb_motor_set_velocity(right_motor,speed(2));
    wb_motor_set_velocity(left_motor, speed(1));
    
    % Verificar que se acercó lo suficiente al siguiente punto
    if eP<tol
        cont = cont + 1;
    end
    
    trajectory = [trajectory; [xi, zi]];
    v_hist = [v_hist; v];
    w_hist = [w_hist; w];
    rwheel_hist = [rwheel_hist; speed(2)];
    lwheel_hist = [lwheel_hist; speed(1)];
    goal = [xg, zg];
    save('analysis.mat', 'trajectory', 'v_hist', 'w_hist', 'rwheel_hist', 'lwheel_hist', 'goal','-append')
    
    drawnow;
  
end


% cleanup code goes here: write data to files, etc.
