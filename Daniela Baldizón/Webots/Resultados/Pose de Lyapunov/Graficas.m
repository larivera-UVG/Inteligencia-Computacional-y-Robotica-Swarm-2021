clc
clear

load analysis.mat

t = linspace(1,size(trajectory,1)-1,size(trajectory,1)-1)';

figure(1); clf;
plot(t, v_hist, 'b')
grid on
xlabel('tiempo (ms)')
ylabel('Velocidad (m/s)')
legend('Velocidad lineal del robot')

figure(2); clf;
plot(t, w_hist, 'r')
grid on
xlabel('tiempo (ms)')
ylabel('Velocidad angular (rad/s)')
legend('Velocidad angular del robot')

figure(3); clf;
plot(t, lwheel_hist, 'g')
hold on
plot(t, rwheel_hist, 'y')
grid on
xlabel('tiempo (ms)')
ylabel('Velocidad angular (rad/s)')
legend('Velocidad del motor izquierdo', 'velocidad del motor derecho')
ylim([0.5 3.5])

figure(4); clf
plot(trajectory(:,1), -trajectory(:,2), 'k')
grid on
xlabel('x (m)')
ylabel('z (m)')
xlim([-1 1])
ylim([-1 1])

% saveas(figure(1), 'velocidadPose.jpg')
% saveas(figure(2), 'velocidadangularPose.jpg')
saveas(figure(3), 'velocidadmotoresPose.jpg')
% saveas(figure(4), 'trayectoriaPose.jpg')