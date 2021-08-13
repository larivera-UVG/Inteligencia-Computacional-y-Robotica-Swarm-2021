clear
clc

% Referencia
Ref = rgb2gray(imread('Lena.jpg'));

% Template
Temp = rgb2gray(imread('Lena Distorsion.jpg'));

SRef = double(Ref(1:10,1:10));
STemp = double(Temp(1:10,1:10));
% figure;
% imshow(SubJ)
% 
% figure;
% imshow(JD)

