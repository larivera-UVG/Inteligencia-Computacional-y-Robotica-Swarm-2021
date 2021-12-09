/*
                    TRABAJO DE GRADUACION 
                UNIVERSIDAD DEL VALLE DE GUATEMALA

    ROBOTICA DE ENJAMBRE - ALGORITMO ANT COLONY OPTIMIZATION
                    CODIGO PRINCIPAL 
    ANDRES SIERRA
    CARNET 17025
*/

#include <iostream>
#include <cstdlib>

#include <cmath>
#include <limits>
#include <climits>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ACO.h"
#include <fstream>

#define MAX_LETRAS 100
#define MAX_CADENAS 60

using namespace std;

// PARAMETROS DEL ALGORITMO

#define ITERACIONES (int)85
#define NUMEROHORMIGAS (int)70
#define NUMERONODOS (int)100

// Le da más peso a la feromona en la probabilidad -
#define ALPHA (double)0.8
// Le da más peso al costo del link en la probabilidad -
#define BETA (double)0.1
// Constante positiva que regula el depósito de feromona -
#define Q (double)10
// Rate de evaporación de las feromonas (puede tomar valores entre 0 y 1)
#define RO (double)0.5
// Valor maximo de la feromona
#define TAUMAX (int)2
/*
// -------------- VALORES DE PRUEBA ----------------
// ------------- MEJORES RESULTADOS ----------------
# ALPHA (double)1
# BETA (double)0.1
# Q (double)10
# RO (double)0.5
# TAUMAX (int)2
*/

// Nodo inicial y final
#define NODOINICIAL (int)0
#define NODOFINAL (int)33

// ===== PRUEBAS =====
// Guardar el txt
//char *new_coord;
// new_coord = new string *[5];
// Nuevas posiciones del robot
double new_position[] = {0, 0};
char *token2;

// Variable para separar la informacion
double separar[2];

int main()
{
    ACO *ANTS = new ACO(NUMEROHORMIGAS, NUMERONODOS, ALPHA, BETA, Q, RO, TAUMAX, NODOINICIAL, NODOFINAL);

    ANTS->inicializar();
    // ----- FILA 1 ------
    // CONECTAR LOS NODOS (SE ESTA UTILIZANDO 100 NODOS COMO SE VISUALIZA EN MATLAB)
    ANTS->conectarNODOS(0, 1);
    ANTS->conectarNODOS(0, 10);
    ANTS->conectarNODOS(0, 11);
    ANTS->conectarNODOS(1, 10);
    ANTS->conectarNODOS(1, 11);
    ANTS->conectarNODOS(1, 12);
    ANTS->conectarNODOS(1, 2);

    ANTS->conectarNODOS(2, 11);
    ANTS->conectarNODOS(2, 12);
    ANTS->conectarNODOS(2, 13);
    ANTS->conectarNODOS(2, 3);
    ANTS->conectarNODOS(3, 12);
    ANTS->conectarNODOS(3, 13);
    ANTS->conectarNODOS(3, 14);
    ANTS->conectarNODOS(3, 4);

    ANTS->conectarNODOS(4, 13);
    ANTS->conectarNODOS(4, 14);
    ANTS->conectarNODOS(4, 15);
    ANTS->conectarNODOS(4, 5);
    ANTS->conectarNODOS(5, 14);
    ANTS->conectarNODOS(5, 15);
    ANTS->conectarNODOS(5, 16);
    ANTS->conectarNODOS(5, 6);

    ANTS->conectarNODOS(6, 15);
    ANTS->conectarNODOS(6, 16);
    ANTS->conectarNODOS(6, 17);
    ANTS->conectarNODOS(6, 7);
    ANTS->conectarNODOS(7, 16);
    ANTS->conectarNODOS(7, 17);
    ANTS->conectarNODOS(7, 18);
    ANTS->conectarNODOS(7, 6);

    ANTS->conectarNODOS(8, 17);
    ANTS->conectarNODOS(8, 18);
    ANTS->conectarNODOS(8, 19);
    ANTS->conectarNODOS(8, 9);
    ANTS->conectarNODOS(9, 18);
    ANTS->conectarNODOS(9, 19);
    // ----- FILA 2 ------
    ANTS->conectarNODOS(10, 11);
    ANTS->conectarNODOS(10, 20);
    ANTS->conectarNODOS(10, 21);
    ANTS->conectarNODOS(11, 20);
    ANTS->conectarNODOS(11, 21);
    ANTS->conectarNODOS(11, 22);
    ANTS->conectarNODOS(11, 12);

    ANTS->conectarNODOS(12, 21);
    ANTS->conectarNODOS(12, 22);
    ANTS->conectarNODOS(12, 23);
    ANTS->conectarNODOS(12, 13);
    ANTS->conectarNODOS(13, 22);
    ANTS->conectarNODOS(13, 23);
    ANTS->conectarNODOS(13, 24);
    ANTS->conectarNODOS(13, 14);

    ANTS->conectarNODOS(14, 23);
    ANTS->conectarNODOS(14, 24);
    ANTS->conectarNODOS(14, 25);
    ANTS->conectarNODOS(14, 15);
    ANTS->conectarNODOS(15, 24);
    ANTS->conectarNODOS(15, 25);
    ANTS->conectarNODOS(15, 26);
    ANTS->conectarNODOS(15, 16);

    ANTS->conectarNODOS(16, 25);
    ANTS->conectarNODOS(16, 26);
    ANTS->conectarNODOS(16, 27);
    ANTS->conectarNODOS(16, 17);
    ANTS->conectarNODOS(17, 26);
    ANTS->conectarNODOS(17, 27);
    ANTS->conectarNODOS(17, 28);
    ANTS->conectarNODOS(17, 16);

    ANTS->conectarNODOS(18, 27);
    ANTS->conectarNODOS(18, 28);
    ANTS->conectarNODOS(18, 29);
    ANTS->conectarNODOS(18, 19);
    ANTS->conectarNODOS(19, 28);
    ANTS->conectarNODOS(19, 29);
    // ----- FILA 3 ------
    ANTS->conectarNODOS(20, 21);
    ANTS->conectarNODOS(20, 30);
    ANTS->conectarNODOS(20, 31);
    ANTS->conectarNODOS(21, 30);
    ANTS->conectarNODOS(21, 31);
    ANTS->conectarNODOS(21, 32);
    ANTS->conectarNODOS(21, 22);

    ANTS->conectarNODOS(22, 31);
    ANTS->conectarNODOS(22, 32);
    ANTS->conectarNODOS(22, 33);
    ANTS->conectarNODOS(22, 23);
    ANTS->conectarNODOS(23, 32);
    ANTS->conectarNODOS(23, 33);
    ANTS->conectarNODOS(23, 34);
    ANTS->conectarNODOS(23, 24);

    ANTS->conectarNODOS(24, 33);
    ANTS->conectarNODOS(24, 34);
    ANTS->conectarNODOS(24, 35);
    ANTS->conectarNODOS(24, 25);
    ANTS->conectarNODOS(25, 34);
    ANTS->conectarNODOS(25, 35);
    ANTS->conectarNODOS(25, 36);
    ANTS->conectarNODOS(25, 26);

    ANTS->conectarNODOS(26, 35);
    ANTS->conectarNODOS(26, 36);
    ANTS->conectarNODOS(26, 37);
    ANTS->conectarNODOS(26, 27);
    ANTS->conectarNODOS(27, 36);
    ANTS->conectarNODOS(27, 37);
    ANTS->conectarNODOS(27, 38);
    ANTS->conectarNODOS(27, 26);

    ANTS->conectarNODOS(28, 37);
    ANTS->conectarNODOS(28, 38);
    ANTS->conectarNODOS(28, 39);
    ANTS->conectarNODOS(28, 29);
    ANTS->conectarNODOS(29, 38);
    ANTS->conectarNODOS(29, 39);
    // ----- FILA 4 ------
    ANTS->conectarNODOS(30, 31);
    ANTS->conectarNODOS(30, 40);
    ANTS->conectarNODOS(30, 41);
    ANTS->conectarNODOS(31, 40);
    ANTS->conectarNODOS(31, 41);
    ANTS->conectarNODOS(31, 42);
    ANTS->conectarNODOS(31, 32);

    ANTS->conectarNODOS(32, 41);
    ANTS->conectarNODOS(32, 42);
    ANTS->conectarNODOS(32, 43);
    ANTS->conectarNODOS(32, 33);
    ANTS->conectarNODOS(33, 42);
    ANTS->conectarNODOS(33, 43);
    ANTS->conectarNODOS(33, 44);
    ANTS->conectarNODOS(33, 34);

    ANTS->conectarNODOS(34, 43);
    ANTS->conectarNODOS(34, 44);
    ANTS->conectarNODOS(34, 45);
    ANTS->conectarNODOS(34, 35);
    ANTS->conectarNODOS(35, 44);
    ANTS->conectarNODOS(35, 45);
    ANTS->conectarNODOS(35, 46);
    ANTS->conectarNODOS(35, 36);

    ANTS->conectarNODOS(36, 45);
    ANTS->conectarNODOS(36, 46);
    ANTS->conectarNODOS(36, 47);
    ANTS->conectarNODOS(36, 37);
    ANTS->conectarNODOS(37, 46);
    ANTS->conectarNODOS(37, 47);
    ANTS->conectarNODOS(37, 48);
    ANTS->conectarNODOS(37, 36);

    ANTS->conectarNODOS(38, 47);
    ANTS->conectarNODOS(38, 48);
    ANTS->conectarNODOS(38, 49);
    ANTS->conectarNODOS(38, 39);
    ANTS->conectarNODOS(39, 48);
    ANTS->conectarNODOS(39, 49);
    // ----- FILA 5 ------
    ANTS->conectarNODOS(40, 41);
    ANTS->conectarNODOS(40, 50);
    ANTS->conectarNODOS(40, 51);
    ANTS->conectarNODOS(41, 50);
    ANTS->conectarNODOS(41, 51);
    ANTS->conectarNODOS(41, 52);
    ANTS->conectarNODOS(41, 42);

    ANTS->conectarNODOS(42, 51);
    ANTS->conectarNODOS(42, 52);
    ANTS->conectarNODOS(42, 53);
    ANTS->conectarNODOS(42, 43);
    ANTS->conectarNODOS(43, 52);
    ANTS->conectarNODOS(43, 53);
    ANTS->conectarNODOS(43, 54);
    ANTS->conectarNODOS(43, 44);

    ANTS->conectarNODOS(44, 53);
    ANTS->conectarNODOS(44, 54);
    ANTS->conectarNODOS(44, 55);
    ANTS->conectarNODOS(44, 45);
    ANTS->conectarNODOS(45, 54);
    ANTS->conectarNODOS(45, 55);
    ANTS->conectarNODOS(45, 56);
    ANTS->conectarNODOS(45, 46);

    ANTS->conectarNODOS(46, 55);
    ANTS->conectarNODOS(46, 56);
    ANTS->conectarNODOS(46, 57);
    ANTS->conectarNODOS(46, 47);
    ANTS->conectarNODOS(47, 56);
    ANTS->conectarNODOS(47, 57);
    ANTS->conectarNODOS(47, 58);
    ANTS->conectarNODOS(47, 46);

    ANTS->conectarNODOS(48, 57);
    ANTS->conectarNODOS(48, 58);
    ANTS->conectarNODOS(48, 59);
    ANTS->conectarNODOS(48, 49);
    ANTS->conectarNODOS(49, 58);
    ANTS->conectarNODOS(49, 59);
    // ----- FILA 6 ------
    ANTS->conectarNODOS(50, 51);
    ANTS->conectarNODOS(50, 60);
    ANTS->conectarNODOS(50, 61);
    ANTS->conectarNODOS(51, 60);
    ANTS->conectarNODOS(51, 61);
    ANTS->conectarNODOS(51, 62);
    ANTS->conectarNODOS(51, 52);

    ANTS->conectarNODOS(52, 61);
    ANTS->conectarNODOS(52, 62);
    ANTS->conectarNODOS(52, 63);
    ANTS->conectarNODOS(52, 53);
    ANTS->conectarNODOS(53, 62);
    ANTS->conectarNODOS(53, 63);
    ANTS->conectarNODOS(53, 64);
    ANTS->conectarNODOS(53, 54);

    ANTS->conectarNODOS(54, 63);
    ANTS->conectarNODOS(54, 64);
    ANTS->conectarNODOS(54, 65);
    ANTS->conectarNODOS(54, 55);
    ANTS->conectarNODOS(55, 64);
    ANTS->conectarNODOS(55, 65);
    ANTS->conectarNODOS(55, 66);
    ANTS->conectarNODOS(55, 56);

    ANTS->conectarNODOS(56, 65);
    ANTS->conectarNODOS(56, 66);
    ANTS->conectarNODOS(56, 67);
    ANTS->conectarNODOS(56, 57);
    ANTS->conectarNODOS(57, 66);
    ANTS->conectarNODOS(57, 67);
    ANTS->conectarNODOS(57, 68);
    ANTS->conectarNODOS(57, 56);

    ANTS->conectarNODOS(58, 67);
    ANTS->conectarNODOS(58, 68);
    ANTS->conectarNODOS(58, 69);
    ANTS->conectarNODOS(58, 59);
    ANTS->conectarNODOS(59, 68);
    ANTS->conectarNODOS(59, 69);
    // ----- FILA 7 ------
    ANTS->conectarNODOS(60, 61);
    ANTS->conectarNODOS(60, 70);
    ANTS->conectarNODOS(60, 71);
    ANTS->conectarNODOS(61, 70);
    ANTS->conectarNODOS(61, 71);
    ANTS->conectarNODOS(61, 72);
    ANTS->conectarNODOS(61, 62);

    ANTS->conectarNODOS(62, 71);
    ANTS->conectarNODOS(62, 72);
    ANTS->conectarNODOS(62, 73);
    ANTS->conectarNODOS(62, 63);
    ANTS->conectarNODOS(63, 72);
    ANTS->conectarNODOS(63, 73);
    ANTS->conectarNODOS(63, 74);
    ANTS->conectarNODOS(63, 64);

    ANTS->conectarNODOS(64, 73);
    ANTS->conectarNODOS(64, 74);
    ANTS->conectarNODOS(64, 75);
    ANTS->conectarNODOS(64, 65);
    ANTS->conectarNODOS(65, 74);
    ANTS->conectarNODOS(65, 75);
    ANTS->conectarNODOS(65, 76);
    ANTS->conectarNODOS(65, 66);

    ANTS->conectarNODOS(66, 75);
    ANTS->conectarNODOS(66, 76);
    ANTS->conectarNODOS(66, 77);
    ANTS->conectarNODOS(66, 67);
    ANTS->conectarNODOS(67, 76);
    ANTS->conectarNODOS(67, 77);
    ANTS->conectarNODOS(67, 78);
    ANTS->conectarNODOS(67, 66);

    ANTS->conectarNODOS(68, 77);
    ANTS->conectarNODOS(68, 78);
    ANTS->conectarNODOS(68, 79);
    ANTS->conectarNODOS(68, 69);
    ANTS->conectarNODOS(69, 78);
    ANTS->conectarNODOS(69, 79);
    // ----- FILA 8 ------
    ANTS->conectarNODOS(70, 71);
    ANTS->conectarNODOS(70, 80);
    ANTS->conectarNODOS(70, 81);
    ANTS->conectarNODOS(71, 80);
    ANTS->conectarNODOS(71, 81);
    ANTS->conectarNODOS(71, 82);
    ANTS->conectarNODOS(71, 72);

    ANTS->conectarNODOS(72, 81);
    ANTS->conectarNODOS(72, 82);
    ANTS->conectarNODOS(72, 83);
    ANTS->conectarNODOS(72, 73);
    ANTS->conectarNODOS(73, 82);
    ANTS->conectarNODOS(73, 83);
    ANTS->conectarNODOS(73, 84);
    ANTS->conectarNODOS(73, 74);

    ANTS->conectarNODOS(74, 83);
    ANTS->conectarNODOS(74, 84);
    ANTS->conectarNODOS(74, 85);
    ANTS->conectarNODOS(74, 75);
    ANTS->conectarNODOS(75, 84);
    ANTS->conectarNODOS(75, 85);
    ANTS->conectarNODOS(75, 86);
    ANTS->conectarNODOS(75, 76);

    ANTS->conectarNODOS(76, 85);
    ANTS->conectarNODOS(76, 86);
    ANTS->conectarNODOS(76, 87);
    ANTS->conectarNODOS(76, 77);
    ANTS->conectarNODOS(77, 86);
    ANTS->conectarNODOS(77, 87);
    ANTS->conectarNODOS(77, 88);
    ANTS->conectarNODOS(77, 76);

    ANTS->conectarNODOS(78, 87);
    ANTS->conectarNODOS(78, 88);
    ANTS->conectarNODOS(78, 89);
    ANTS->conectarNODOS(78, 79);
    ANTS->conectarNODOS(79, 88);
    ANTS->conectarNODOS(79, 89);
    // ----- FILA 9 ------
    ANTS->conectarNODOS(80, 81);
    ANTS->conectarNODOS(80, 90);
    ANTS->conectarNODOS(80, 91);
    ANTS->conectarNODOS(81, 90);
    ANTS->conectarNODOS(81, 91);
    ANTS->conectarNODOS(81, 92);
    ANTS->conectarNODOS(81, 82);

    ANTS->conectarNODOS(82, 91);
    ANTS->conectarNODOS(82, 92);
    ANTS->conectarNODOS(82, 93);
    ANTS->conectarNODOS(82, 83);
    ANTS->conectarNODOS(83, 92);
    ANTS->conectarNODOS(83, 93);
    ANTS->conectarNODOS(83, 94);
    ANTS->conectarNODOS(83, 84);

    ANTS->conectarNODOS(84, 93);
    ANTS->conectarNODOS(84, 94);
    ANTS->conectarNODOS(84, 95);
    ANTS->conectarNODOS(84, 85);
    ANTS->conectarNODOS(85, 94);
    ANTS->conectarNODOS(85, 95);
    ANTS->conectarNODOS(85, 96);
    ANTS->conectarNODOS(85, 86);

    ANTS->conectarNODOS(86, 95);
    ANTS->conectarNODOS(86, 96);
    ANTS->conectarNODOS(86, 97);
    ANTS->conectarNODOS(86, 87);
    ANTS->conectarNODOS(87, 96);
    ANTS->conectarNODOS(87, 97);
    ANTS->conectarNODOS(87, 98);
    ANTS->conectarNODOS(87, 86);

    ANTS->conectarNODOS(88, 97);
    ANTS->conectarNODOS(88, 98);
    ANTS->conectarNODOS(88, 99);
    ANTS->conectarNODOS(88, 89);
    ANTS->conectarNODOS(89, 98);
    ANTS->conectarNODOS(89, 99);
    // ----- FILA 10 -----
    ANTS->conectarNODOS(90, 91);
    ANTS->conectarNODOS(91, 92);
    ANTS->conectarNODOS(92, 93);
    ANTS->conectarNODOS(93, 94);
    ANTS->conectarNODOS(94, 95);
    ANTS->conectarNODOS(95, 96);
    ANTS->conectarNODOS(96, 97);
    ANTS->conectarNODOS(97, 98);
    ANTS->conectarNODOS(98, 99);

    /*
    for (int j = 0; j < 10; j++)
    {
        for (int h = 0; h < 10; h++)
        {
            setCOORDENADAS(j, h, 0);
        }
    }
    */
    // DEFINIMOS LA COORDENADA DE CADA NODO
    ANTS->setCOORDENADAS(0, 1, 1);
    ANTS->setCOORDENADAS(1, 10, 0);
    ANTS->setCOORDENADAS(2, 20, 0);
    ANTS->setCOORDENADAS(3, 30, 0);
    ANTS->setCOORDENADAS(4, 40, 0);
    ANTS->setCOORDENADAS(5, 50, 0);
    ANTS->setCOORDENADAS(6, 60, 0);
    ANTS->setCOORDENADAS(7, 70, 0);
    ANTS->setCOORDENADAS(8, 80, 0);
    ANTS->setCOORDENADAS(9, 90, 0);

    ANTS->setCOORDENADAS(10, 0, 10);
    ANTS->setCOORDENADAS(11, 10, 10);
    ANTS->setCOORDENADAS(12, 20, 10);
    ANTS->setCOORDENADAS(13, 30, 10);
    ANTS->setCOORDENADAS(14, 40, 10);
    ANTS->setCOORDENADAS(15, 50, 10);
    ANTS->setCOORDENADAS(16, 60, 10);
    ANTS->setCOORDENADAS(17, 70, 10);
    ANTS->setCOORDENADAS(18, 80, 10);
    ANTS->setCOORDENADAS(19, 90, 10);

    ANTS->setCOORDENADAS(20, 0, 20);
    ANTS->setCOORDENADAS(21, 10, 20);
    ANTS->setCOORDENADAS(22, 20, 20);
    ANTS->setCOORDENADAS(23, 30, 20);
    ANTS->setCOORDENADAS(24, 40, 20);
    ANTS->setCOORDENADAS(25, 50, 20);
    ANTS->setCOORDENADAS(26, 60, 20);
    ANTS->setCOORDENADAS(27, 70, 20);
    ANTS->setCOORDENADAS(28, 80, 20);
    ANTS->setCOORDENADAS(29, 90, 20);

    ANTS->setCOORDENADAS(30, 0, 30);
    ANTS->setCOORDENADAS(31, 10, 30);
    ANTS->setCOORDENADAS(32, 20, 30);
    ANTS->setCOORDENADAS(33, 30, 30);
    ANTS->setCOORDENADAS(34, 40, 30);
    ANTS->setCOORDENADAS(35, 50, 30);
    ANTS->setCOORDENADAS(36, 60, 30);
    ANTS->setCOORDENADAS(37, 70, 30);
    ANTS->setCOORDENADAS(38, 80, 30);
    ANTS->setCOORDENADAS(39, 90, 30);

    ANTS->setCOORDENADAS(40, 0, 40);
    ANTS->setCOORDENADAS(41, 10, 40);
    ANTS->setCOORDENADAS(42, 20, 40);
    ANTS->setCOORDENADAS(43, 30, 40);
    ANTS->setCOORDENADAS(44, 40, 40);
    ANTS->setCOORDENADAS(45, 50, 40);
    ANTS->setCOORDENADAS(46, 60, 40);
    ANTS->setCOORDENADAS(47, 70, 40);
    ANTS->setCOORDENADAS(48, 80, 40);
    ANTS->setCOORDENADAS(49, 90, 40);

    ANTS->setCOORDENADAS(50, 0, 50);
    ANTS->setCOORDENADAS(51, 10, 50);
    ANTS->setCOORDENADAS(52, 20, 50);
    ANTS->setCOORDENADAS(53, 30, 50);
    ANTS->setCOORDENADAS(54, 40, 50);
    ANTS->setCOORDENADAS(55, 50, 50);
    ANTS->setCOORDENADAS(56, 60, 50);
    ANTS->setCOORDENADAS(57, 70, 50);
    ANTS->setCOORDENADAS(58, 80, 50);
    ANTS->setCOORDENADAS(59, 90, 50);

    ANTS->setCOORDENADAS(60, 0, 60);
    ANTS->setCOORDENADAS(61, 10, 60);
    ANTS->setCOORDENADAS(62, 20, 60);
    ANTS->setCOORDENADAS(63, 30, 60);
    ANTS->setCOORDENADAS(64, 40, 60);
    ANTS->setCOORDENADAS(65, 50, 60);
    ANTS->setCOORDENADAS(66, 60, 60);
    ANTS->setCOORDENADAS(67, 70, 60);
    ANTS->setCOORDENADAS(68, 80, 60);
    ANTS->setCOORDENADAS(69, 90, 60);

    ANTS->setCOORDENADAS(70, 0, 70);
    ANTS->setCOORDENADAS(71, 10, 70);
    ANTS->setCOORDENADAS(72, 20, 70);
    ANTS->setCOORDENADAS(73, 30, 70);
    ANTS->setCOORDENADAS(74, 40, 70);
    ANTS->setCOORDENADAS(75, 50, 70);
    ANTS->setCOORDENADAS(76, 60, 70);
    ANTS->setCOORDENADAS(77, 70, 70);
    ANTS->setCOORDENADAS(78, 80, 70);
    ANTS->setCOORDENADAS(79, 90, 70);

    ANTS->setCOORDENADAS(80, 0, 80);
    ANTS->setCOORDENADAS(81, 10, 80);
    ANTS->setCOORDENADAS(82, 20, 80);
    ANTS->setCOORDENADAS(83, 30, 80);
    ANTS->setCOORDENADAS(84, 40, 80);
    ANTS->setCOORDENADAS(85, 50, 80);
    ANTS->setCOORDENADAS(86, 60, 80);
    ANTS->setCOORDENADAS(87, 70, 80);
    ANTS->setCOORDENADAS(88, 80, 80);
    ANTS->setCOORDENADAS(89, 90, 80);

    ANTS->setCOORDENADAS(90, 0, 90);
    ANTS->setCOORDENADAS(91, 10, 90);
    ANTS->setCOORDENADAS(92, 20, 90);
    ANTS->setCOORDENADAS(93, 30, 90);
    ANTS->setCOORDENADAS(94, 40, 90);
    ANTS->setCOORDENADAS(95, 50, 90);
    ANTS->setCOORDENADAS(96, 60, 90);
    ANTS->setCOORDENADAS(97, 70, 90);
    ANTS->setCOORDENADAS(98, 80, 90);
    ANTS->setCOORDENADAS(99, 90, 90);

    ANTS->optimizar(ITERACIONES);
    // char cadena[125];

    char Cadena[MAX_LETRAS + 1];                   // string (cadena de caracteres).
    char StringArray[MAX_CADENAS][MAX_LETRAS + 1]; // arreglo de cadenas
    int i, cont = 0;

    FILE *fp_original; // para los archivos
    // ifstream fe("/home/pi/Desktop/Ant Colony/rutaCLASE.txt");

    fp_original = fopen("rutaCLASE.txt", "r"); // abrir para lectura
    while (fgets(Cadena, MAX_LETRAS, fp_original) != NULL)
    {
        strcpy(StringArray[cont], Cadena); // strcpy = "string copy"
        //printf(StringArray[cont]);	                    // muestra la cadena copiada en la terminal
        fflush(stdout);
        cont++;
    }
    fclose(fp_original); // cierra el archivo original.
    //fe.close();
    cout << "Coordenadas" << endl;
    // cout << StringArray[1] << StringArray[2] << endl;

    token2 = strtok(StringArray[0], " ");
    separar[i] = atof(token2);
    while ((token2 = strtok(NULL, ",")) != NULL)
    {
        i++;
        separar[i] = atof(token2);
    }
    new_position[0] = separar[0];
    new_position[1] = separar[1];

    cout << new_position[0] << " " << new_position[1] << endl;

    return 0;
}
