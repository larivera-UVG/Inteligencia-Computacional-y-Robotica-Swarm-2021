Im = zeros(10);

Im(4:7,4) = 155;
Im(4:7,5) = 168;
Im(4:7,6) = 161;

Im = uint8(Im);
% imshow(Im)
imwrite(Im, 'Pruebas.jpg')

Im2 = zeros(10);

Im2(4:7,3) = 155;
Im2(4:7,4) = 168;
Im2(4:7,7) = 161;

Im2 = uint8(Im2)
imwrite(Im2, 'PruebasD.jpg')