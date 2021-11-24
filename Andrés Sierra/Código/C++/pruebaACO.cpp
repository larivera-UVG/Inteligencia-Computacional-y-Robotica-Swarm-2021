/*
                    TRABAJO DE GRADUACION 
                UNIVERSIDAD DEL VALLE DE GUATEMALA

    ROBOTICA DE ENJAMBRE - ALGORITMO ANT COLONY OPTIMIZATION
                        ALGORITMO ACO 
    ANDRES SIERRA
    CARNET 17025 

*/

#include "Randoms.cpp"              // Librería para generación de numeros aleatorios
#include <iostream>
#include <cstdlib>
#include <fstream>

#include <cmath>
#include <limits>
#include <climits>

#include <time.h>
#include <stdlib.h>

#include <cstdio>

// PARAMETROS DEL ALGORITMO

#define ITERACIONES (int)75
#define NUMEROHORMIGAS (int)40
#define NUMERONODOS (int)100

// Le da más peso a la feromona en la probabilidad -
#define ALPHA (double)1
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
#define NODOFINAL (int)72

// DEFINICION DE LAS VARIABLES/PUNTEROS A UTILIZAR 
double MEJORLONG;
int *MEJORRUTA;

int **GRAFO, **RUTAS;
double **COORDS, **FEROMONAS, **deltaFEROMONAS, **PROB;

int numITER[NUMEROHORMIGAS + 1];
Randoms *randoms;
using namespace std;
// -------------- DEFINICION DE TODAS LAS FUNCIONES ----------------
// INICIALIZA TODAS LAS VARIABLES RESERVANDO LOS ESPACIOS DE MEMORIA
void inicializar()
{
    GRAFO = new int *[NUMERONODOS];
    COORDS = new double *[NUMERONODOS];
    FEROMONAS = new double *[NUMERONODOS];
    deltaFEROMONAS = new double *[NUMERONODOS];
    PROB = new double *[NUMERONODOS - 1];
    for (int i = 0; i < NUMERONODOS; i++)
    {
        GRAFO[i] = new int[NUMERONODOS];
        COORDS[i] = new double[2];
        FEROMONAS[i] = new double[NUMERONODOS];
        deltaFEROMONAS[i] = new double[NUMERONODOS];
        PROB[i] = new double[2];
        for (int j = 0; j < 2; j++)
        {
            COORDS[i][j] = -1.0;
            PROB[i][j] = -1.0;
        }
        for (int j = 0; j < NUMERONODOS; j++)
        {
            GRAFO[i][j] = 0;
            FEROMONAS[i][j] = 0.0;
            deltaFEROMONAS[i][j] = 0.0;
        }
    }

    RUTAS = new int *[NUMEROHORMIGAS];
    for (int i = 0; i < NUMEROHORMIGAS; i++)
    {
        RUTAS[i] = new int[NUMERONODOS];
        for (int j = 0; j < NUMERONODOS; j++)
        {
            RUTAS[i][j] = -1;
        }
    }

    MEJORLONG = (double)INT_MAX;
    MEJORRUTA = new int[NUMERONODOS];

    for (int i = 0; i < NUMERONODOS; i++)
    {
        MEJORRUTA[i] = -1;
    }
}
// DEFINE LA CONEXION BIDIRECCIONAL DE LOS NODOS DEL GRAFO Y PONE UN VALOR DE FEROMONA 
// ALEATORIO A LOS NODOS INDICADOS

void conectarNODOS(int nodoi, int nodoj)
{
    GRAFO[nodoi][nodoj] = 1;
    FEROMONAS[nodoi][nodoj] = randoms->Uniforme() * TAUMAX;
    //FEROMONAS[nodoi][nodoj] = (rand() % (1)) * TAUMAX;
    GRAFO[nodoj][nodoi] = 1;
    FEROMONAS[nodoj][nodoi] = FEROMONAS[nodoi][nodoj];
}
// ASIGNA UNA COORDENADA A CADA NODO
void setCOORDENADAS(int nodo, double x, double y)
{
    COORDS[nodo][0] = x;
    COORDS[nodo][1] = y;
}
// PARA VERIFICAR SI EL NODO ES VALIDO (VERIFICA QUE ESTA 1)
bool nodoVALIDO(int nodoi, int nodoc)
{
    return (GRAFO[nodoi][nodoc] == 1);
}
// CALCULA LA DISTANCIA CON LA ECUACION EUCLIDIANA
double distancia(int nodoi, int nodoj)
{
    return (double)
        sqrt(pow(COORDS[nodoi][0] - COORDS[nodoj][0], 2) +
             pow(COORDS[nodoi][1] - COORDS[nodoj][1], 2));
}
// VERIFICA SI EL NODO YA FUE VISITADO 
bool visitado(int hormigaK, int c)
{
    for (int i = 0; i < NUMERONODOS; i++)
    {
        if (RUTAS[hormigaK][i] == -1)
        {
            break;
        }
        if (RUTAS[hormigaK][i] == c)
        {
            return true;
        }
    }
    return false;
}
// ECUACION DE PROBABILIDAD ANT SYSTEM 
double PHI(int nodoi, int nodoj, int hormigaK)
{
    double ETAij = (double)pow(1 / distancia(nodoi, nodoj), BETA);
    double TAUij = (double)pow(FEROMONAS[nodoi][nodoj], ALPHA);

    double sum = 0.0;
    for (int c = 0; c < NUMERONODOS; c++)
    {
        if (nodoVALIDO(nodoi, c))
        {
            if (!visitado(hormigaK, c))
            {
                double ETA = (double)pow(1 / distancia(nodoi, c), BETA);
                double TAU = (double)pow(FEROMONAS[nodoi][c], ALPHA);
                sum += ETA * TAU;
            }
        }
    }
    return (ETAij * TAUij) / sum;
}

// PARA ASIGANR EL NODO SIGUIENTE
int siguienteNODO()
{
    double xi = randoms->Uniforme();
    //double xi = rand() %(1);
    int i = 0;
    double sum = PROB[i][0];
    while (sum < xi)
    {
        i++;
        sum += PROB[i][0];
    }
    return (int)PROB[i][1];
}
// VERIFICA SI YA LLEGO AL FINAL 
int nodoFINAL(int nodoi)
{
    double cxi = COORDS[nodoi][0];
    double cxf = COORDS[NODOFINAL][0];
    double cyi = COORDS[nodoi][1];
    double cyf = COORDS[NODOFINAL][1];
    //cout << cxi << " " << cxf << " " << cyi << " " << cyf << " "<< endl;
    if ((cxi == cxf) && (cyi == cyf))
    {
        return 1;
    }
    return 0;
}
// CALCULA LA RUTA DE CADA HORMIGA 
void ruta(int hormigaK)
{
    RUTAS[hormigaK][0] = NODOINICIAL;
    for (int i = 0; i < NUMERONODOS - 1; i++)
    {
        int nodoi = RUTAS[hormigaK][i];
        int count = 0;
        //****************
        if (nodoFINAL(RUTAS[hormigaK][i]) == 1)
        {
            break;
        }

        for (int c = 0; c < NUMERONODOS; c++)
        {
            if (nodoi == c)
            {
                continue;
            }

            if (nodoVALIDO(nodoi, c))
            {
                // AQUI
                if (!visitado(hormigaK, c))
                {
                    PROB[count][0] = PHI(nodoi, c, hormigaK);
                    PROB[count][1] = (double)c;
                    count++;
                }
            }
        }

        RUTAS[hormigaK][i + 1] = siguienteNODO();
        //return i;
    }
    //return -1;
}

// VERIFICA SI EL NODO ES VALIDO  
int valid(int hormigaK, int iteracion)
{ // *
    for (int i = 0; i < iteracion - 1; i++)
    {
        int nodoi = RUTAS[hormigaK][i];
        int nodoj = RUTAS[hormigaK][i + 1];

        /*if (nodoFINAL(RUTAS[hormigaK][i])==1)
        {
            break;
        }*/

        if (nodoi < 0 || nodoj < 0)
        {
            return -1;
        }
        if (!nodoVALIDO(nodoi, nodoj))
        {
            return -2;
        }
        for (int j = 0; j < i - 1; j++)
        {
            if (RUTAS[hormigaK][i] == RUTAS[hormigaK][j])
            {
                return -3;
            }
        }
    }
    /*if (!nodoVALIDO(NODOINICIAL, RUTAS[hormigaK][iteracion - 1]))
    {
        return -4;
    }*/

    return 0;
}
// CALCULA LA LONGITUD DE LA RUTA SELECCONADA 
double longitud(int hormigaK, int numit)
{
    double sum = 0.0;
    for (int j = 0; j < numit - 1; j++)
    {
        sum += distancia(RUTAS[hormigaK][j], RUTAS[hormigaK][j + 1]);
    }
    return sum;
}
// ACTUALIZAR LA FEROMONA EN CADA NODO
void actualizarFEROMONAS()
{
    for (int k = 0; k < NUMEROHORMIGAS; k++)
    {
        double rlongitud = longitud(k, numITER[k]);
        for (int r = 0; r < numITER[k] - 1; r++)
        {
            int nodoi = RUTAS[k][r];
            int nodoj = RUTAS[k][r + 1];
            deltaFEROMONAS[nodoi][nodoj] += Q / rlongitud;
            deltaFEROMONAS[nodoj][nodoi] += Q / rlongitud;
        }
    }
    for (int i = 0; i < NUMERONODOS; i++)
    {
        for (int j = 0; j < NUMERONODOS; j++)
        {
            FEROMONAS[i][j] = (1 - RO) * FEROMONAS[i][j] + deltaFEROMONAS[i][j];
            deltaFEROMONAS[i][j] = 0.0;
        }
    }
}
// IMPRIMIR EL GRAFO EN LA CONSOLA 
void imprimirGRAFO()
{
    std::cout << " GRAFO: " << std::endl;
    std::cout << "  | ";
    for (int i = 0; i < NUMERONODOS; i++)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl
              << "- | ";
    for (int i = 0; i < NUMERONODOS; i++)
    {
        std::cout << "- ";
    }
    std::cout << std::endl;
    int count = 0;
    for (int i = 0; i < NUMERONODOS; i++)
    {
        std::cout << i << " | ";
        for (int j = 0; j < NUMERONODOS; j++)
        {
            if (i == j)
            {
                std::cout << "x ";
            }
            else
            {
                std::cout << GRAFO[i][j] << " ";
            }
            if (GRAFO[i][j] == 1)
            {
                count++;
            }
        }
        std::cout << std::endl;
    }
}
// IMPRIMIR LA FEROMONA EN LA CONSOLA 
void imprimirFEROMONAS()
{
    std::cout << " FEROMONAS: " << std::endl;
    std::cout << "  | ";
    for (int i = 0; i < NUMERONODOS; i++)
    {
        printf("%5d   ", i);
    }
    std::cout << std::endl
              << "- | ";
    for (int i = 0; i < NUMERONODOS; i++)
    {
        std::cout << "--------";
    }
    std::cout << std::endl;
    for (int i = 0; i < NUMERONODOS; i++)
    {
        std::cout << i << " | ";
        for (int j = 0; j < NUMERONODOS; j++)
        {
            if (i == j)
            {
                printf("%5s   ", "x");
                continue;
            }
            if (nodoVALIDO(i, j))
            {
                printf("%7.3f ", FEROMONAS[i][j]);
            }
            else
            {
                if (FEROMONAS[i][j] == 0.0)
                {
                    printf("%5.0f   ", FEROMONAS[i][j]);
                }
                else
                {
                    printf("%7.3f ", FEROMONAS[i][j]);
                }
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
// IMPRIME LOS RESULTADOS DEL ALGORITMO EN LA CONSOLA 
void imprimirRESULTADOS()
{
    //MEJORLONG += distancia(MEJORRUTA[NUMERONODOS - 1], NODOINICIAL);
    std::cout << " MEJOR RUTA:" << std::endl;
    for (int i = 0; i < numITER[NUMEROHORMIGAS]; i++)
    {
        std::cout << MEJORRUTA[i] + 1 << " ";
    }
    std::cout << std::endl
              << "Longitud: " << MEJORLONG << std::endl;
    std::cout << " " << std::endl;

    /*    std::cout << std::endl
              << " Ruta Ideal:" << std::endl;
    std::cout << "0 1 2 3 4 5 6 7 8 9" << std::endl;
    */
}

int main()
{
    randoms = new Randoms(15);
    srand(time(NULL));

    cout << "INICIO:" << endl;
    // INICIALIZAR EL ALGORITMO 
    inicializar();
    // ----- FILA 1 ------
    // CONECTAR LOS NODOS (SE ESTA UTILIZANDO 100 NODOS COMO SE VISUALIZA EN MATLAB)
    conectarNODOS(0, 1);
    conectarNODOS(0, 10);
    conectarNODOS(0, 11);
    conectarNODOS(1, 10);
    conectarNODOS(1, 11);
    conectarNODOS(1, 12);
    conectarNODOS(1, 2);

    conectarNODOS(2, 11);
    conectarNODOS(2, 12);
    conectarNODOS(2, 13);
    conectarNODOS(2, 3);
    conectarNODOS(3, 12);
    conectarNODOS(3, 13);
    conectarNODOS(3, 14);
    conectarNODOS(3, 4);
    
    conectarNODOS(4, 13);
    conectarNODOS(4, 14);
    conectarNODOS(4, 15);
    conectarNODOS(4, 5);
    conectarNODOS(5, 14);
    conectarNODOS(5, 15);
    conectarNODOS(5, 16);
    conectarNODOS(5, 6);

    conectarNODOS(6, 15);
    conectarNODOS(6, 16);
    conectarNODOS(6, 17);
    conectarNODOS(6, 7);
    conectarNODOS(7, 16);
    conectarNODOS(7, 17);
    conectarNODOS(7, 18);
    conectarNODOS(7, 6);

    conectarNODOS(8, 17);
    conectarNODOS(8, 18);
    conectarNODOS(8, 19);
    conectarNODOS(8, 9);
    conectarNODOS(9, 18);
    conectarNODOS(9, 19);
    // ----- FILA 2 ------
    conectarNODOS(10, 11);
    conectarNODOS(10, 20);
    conectarNODOS(10, 21);
    conectarNODOS(11, 20);
    conectarNODOS(11, 21);
    conectarNODOS(11, 22);
    conectarNODOS(11, 12);

    conectarNODOS(12, 21);
    conectarNODOS(12, 22);
    conectarNODOS(12, 23);
    conectarNODOS(12, 13);
    conectarNODOS(13, 22);
    conectarNODOS(13, 23);
    conectarNODOS(13, 24);
    conectarNODOS(13, 14);
    
    conectarNODOS(14, 23);
    conectarNODOS(14, 24);
    conectarNODOS(14, 25);
    conectarNODOS(14, 15);
    conectarNODOS(15, 24);
    conectarNODOS(15, 25);
    conectarNODOS(15, 26);
    conectarNODOS(15, 16);

    conectarNODOS(16, 25);
    conectarNODOS(16, 26);
    conectarNODOS(16, 27);
    conectarNODOS(16, 17);
    conectarNODOS(17, 26);
    conectarNODOS(17, 27);
    conectarNODOS(17, 28);
    conectarNODOS(17, 16);

    conectarNODOS(18, 27);
    conectarNODOS(18, 28);
    conectarNODOS(18, 29);
    conectarNODOS(18, 19);
    conectarNODOS(19, 28);
    conectarNODOS(19, 29);
    // ----- FILA 3 ------
    conectarNODOS(20, 21);
    conectarNODOS(20, 30);
    conectarNODOS(20, 31);
    conectarNODOS(21, 30);
    conectarNODOS(21, 31);
    conectarNODOS(21, 32);
    conectarNODOS(21, 22);

    conectarNODOS(22, 31);
    conectarNODOS(22, 32);
    conectarNODOS(22, 33);
    conectarNODOS(22, 23);
    conectarNODOS(23, 32);
    conectarNODOS(23, 33);
    conectarNODOS(23, 34);
    conectarNODOS(23, 24);
    
    conectarNODOS(24, 33);
    conectarNODOS(24, 34);
    conectarNODOS(24, 35);
    conectarNODOS(24, 25);
    conectarNODOS(25, 34);
    conectarNODOS(25, 35);
    conectarNODOS(25, 36);
    conectarNODOS(25, 26);

    conectarNODOS(26, 35);
    conectarNODOS(26, 36);
    conectarNODOS(26, 37);
    conectarNODOS(26, 27);
    conectarNODOS(27, 36);
    conectarNODOS(27, 37);
    conectarNODOS(27, 38);
    conectarNODOS(27, 26);

    conectarNODOS(28, 37);
    conectarNODOS(28, 38);
    conectarNODOS(28, 39);
    conectarNODOS(28, 29);
    conectarNODOS(29, 38);
    conectarNODOS(29, 39);
    // ----- FILA 4 ------
    conectarNODOS(30, 31);
    conectarNODOS(30, 40);
    conectarNODOS(30, 41);
    conectarNODOS(31, 40);
    conectarNODOS(31, 41);
    conectarNODOS(31, 42);
    conectarNODOS(31, 32);

    conectarNODOS(32, 41);
    conectarNODOS(32, 42);
    conectarNODOS(32, 43);
    conectarNODOS(32, 33);
    conectarNODOS(33, 42);
    conectarNODOS(33, 43);
    conectarNODOS(33, 44);
    conectarNODOS(33, 34);
    
    conectarNODOS(34, 43);
    conectarNODOS(34, 44);
    conectarNODOS(34, 45);
    conectarNODOS(34, 35);
    conectarNODOS(35, 44);
    conectarNODOS(35, 45);
    conectarNODOS(35, 46);
    conectarNODOS(35, 36);

    conectarNODOS(36, 45);
    conectarNODOS(36, 46);
    conectarNODOS(36, 47);
    conectarNODOS(36, 37);
    conectarNODOS(37, 46);
    conectarNODOS(37, 47);
    conectarNODOS(37, 48);
    conectarNODOS(37, 36);

    conectarNODOS(38, 47);
    conectarNODOS(38, 48);
    conectarNODOS(38, 49);
    conectarNODOS(38, 39);
    conectarNODOS(39, 48);
    conectarNODOS(39, 49);
    // ----- FILA 5 ------ 
    conectarNODOS(40, 41);
    conectarNODOS(40, 50);
    conectarNODOS(40, 51);
    conectarNODOS(41, 50);
    conectarNODOS(41, 51);
    conectarNODOS(41, 52);
    conectarNODOS(41, 42);

    conectarNODOS(42, 51);
    conectarNODOS(42, 52);
    conectarNODOS(42, 53);
    conectarNODOS(42, 43);
    conectarNODOS(43, 52);
    conectarNODOS(43, 53);
    conectarNODOS(43, 54);
    conectarNODOS(43, 44);
    
    conectarNODOS(44, 53);
    conectarNODOS(44, 54);
    conectarNODOS(44, 55);
    conectarNODOS(44, 45);
    conectarNODOS(45, 54);
    conectarNODOS(45, 55);
    conectarNODOS(45, 56);
    conectarNODOS(45, 46);

    conectarNODOS(46, 55);
    conectarNODOS(46, 56);
    conectarNODOS(46, 57);
    conectarNODOS(46, 47);
    conectarNODOS(47, 56);
    conectarNODOS(47, 57);
    conectarNODOS(47, 58);
    conectarNODOS(47, 46);

    conectarNODOS(48, 57);
    conectarNODOS(48, 58);
    conectarNODOS(48, 59);
    conectarNODOS(48, 49);
    conectarNODOS(49, 58);
    conectarNODOS(49, 59);
    // ----- FILA 6 ------
    conectarNODOS(50, 51);
    conectarNODOS(50, 60);
    conectarNODOS(50, 61);
    conectarNODOS(51, 60);
    conectarNODOS(51, 61);
    conectarNODOS(51, 62);
    conectarNODOS(51, 52);

    conectarNODOS(52, 61);
    conectarNODOS(52, 62);
    conectarNODOS(52, 63);
    conectarNODOS(52, 53);
    conectarNODOS(53, 62);
    conectarNODOS(53, 63);
    conectarNODOS(53, 64);
    conectarNODOS(53, 54);
    
    conectarNODOS(54, 63);
    conectarNODOS(54, 64);
    conectarNODOS(54, 65);
    conectarNODOS(54, 55);
    conectarNODOS(55, 64);
    conectarNODOS(55, 65);
    conectarNODOS(55, 66);
    conectarNODOS(55, 56);

    conectarNODOS(56, 65);
    conectarNODOS(56, 66);
    conectarNODOS(56, 67);
    conectarNODOS(56, 57);
    conectarNODOS(57, 66);
    conectarNODOS(57, 67);
    conectarNODOS(57, 68);
    conectarNODOS(57, 56);

    conectarNODOS(58, 67);
    conectarNODOS(58, 68);
    conectarNODOS(58, 69);
    conectarNODOS(58, 59);
    conectarNODOS(59, 68);
    conectarNODOS(59, 69);
    // ----- FILA 7 ------
    conectarNODOS(60, 61);
    conectarNODOS(60, 70);
    conectarNODOS(60, 71);
    conectarNODOS(61, 70);
    conectarNODOS(61, 71);
    conectarNODOS(61, 72);
    conectarNODOS(61, 62);

    conectarNODOS(62, 71);
    conectarNODOS(62, 72);
    conectarNODOS(62, 73);
    conectarNODOS(62, 63);
    conectarNODOS(63, 72);
    conectarNODOS(63, 73);
    conectarNODOS(63, 74);
    conectarNODOS(63, 64);
    
    conectarNODOS(64, 73);
    conectarNODOS(64, 74);
    conectarNODOS(64, 75);
    conectarNODOS(64, 65);
    conectarNODOS(65, 74);
    conectarNODOS(65, 75);
    conectarNODOS(65, 76);
    conectarNODOS(65, 66);

    conectarNODOS(66, 75);
    conectarNODOS(66, 76);
    conectarNODOS(66, 77);
    conectarNODOS(66, 67);
    conectarNODOS(67, 76);
    conectarNODOS(67, 77);
    conectarNODOS(67, 78);
    conectarNODOS(67, 66);

    conectarNODOS(68, 77);
    conectarNODOS(68, 78);
    conectarNODOS(68, 79);
    conectarNODOS(68, 69);
    conectarNODOS(69, 78);
    conectarNODOS(69, 79);
    // ----- FILA 8 ------
    conectarNODOS(70, 71);
    conectarNODOS(70, 80);
    conectarNODOS(70, 81);
    conectarNODOS(71, 80);
    conectarNODOS(71, 81);
    conectarNODOS(71, 82);
    conectarNODOS(71, 72);

    conectarNODOS(72, 81);
    conectarNODOS(72, 82);
    conectarNODOS(72, 83);
    conectarNODOS(72, 73);
    conectarNODOS(73, 82);
    conectarNODOS(73, 83);
    conectarNODOS(73, 84);
    conectarNODOS(73, 74);
    
    conectarNODOS(74, 83);
    conectarNODOS(74, 84);
    conectarNODOS(74, 85);
    conectarNODOS(74, 75);
    conectarNODOS(75, 84);
    conectarNODOS(75, 85);
    conectarNODOS(75, 86);
    conectarNODOS(75, 76);

    conectarNODOS(76, 85);
    conectarNODOS(76, 86);
    conectarNODOS(76, 87);
    conectarNODOS(76, 77);
    conectarNODOS(77, 86);
    conectarNODOS(77, 87);
    conectarNODOS(77, 88);
    conectarNODOS(77, 76);

    conectarNODOS(78, 87);
    conectarNODOS(78, 88);
    conectarNODOS(78, 89);
    conectarNODOS(78, 79);
    conectarNODOS(79, 88);
    conectarNODOS(79, 89);
    // ----- FILA 9 ------
    conectarNODOS(80, 81);
    conectarNODOS(80, 90);
    conectarNODOS(80, 91);
    conectarNODOS(81, 90);
    conectarNODOS(81, 91);
    conectarNODOS(81, 92);
    conectarNODOS(81, 82);

    conectarNODOS(82, 91);
    conectarNODOS(82, 92);
    conectarNODOS(82, 93);
    conectarNODOS(82, 83);
    conectarNODOS(83, 92);
    conectarNODOS(83, 93);
    conectarNODOS(83, 94);
    conectarNODOS(83, 84);
    
    conectarNODOS(84, 93);
    conectarNODOS(84, 94);
    conectarNODOS(84, 95);
    conectarNODOS(84, 85);
    conectarNODOS(85, 94);
    conectarNODOS(85, 95);
    conectarNODOS(85, 96);
    conectarNODOS(85, 86);

    conectarNODOS(86, 95);
    conectarNODOS(86, 96);
    conectarNODOS(86, 97);
    conectarNODOS(86, 87);
    conectarNODOS(87, 96);
    conectarNODOS(87, 97);
    conectarNODOS(87, 98);
    conectarNODOS(87, 86);

    conectarNODOS(88, 97);
    conectarNODOS(88, 98);
    conectarNODOS(88, 99);
    conectarNODOS(88, 89);
    conectarNODOS(89, 98);
    conectarNODOS(89, 99);
    // ----- FILA 10 -----
    conectarNODOS(90, 91);
    conectarNODOS(91, 92);
    conectarNODOS(92, 93);
    conectarNODOS(93, 94);
    conectarNODOS(94, 95);
    conectarNODOS(95, 96);
    conectarNODOS(96, 97);
    conectarNODOS(97, 98);
    conectarNODOS(98, 99);

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
    setCOORDENADAS(0, 0, 0);
    setCOORDENADAS(1, 10, 0);
    setCOORDENADAS(2, 20, 0);
    setCOORDENADAS(3, 30, 0);
    setCOORDENADAS(4, 40, 0);
    setCOORDENADAS(5, 50, 0);
    setCOORDENADAS(6, 60, 0);
    setCOORDENADAS(7, 70, 0);
    setCOORDENADAS(8, 80, 0);
    setCOORDENADAS(9, 90, 0);

    setCOORDENADAS(10, 0, 10);
    setCOORDENADAS(11, 10, 10);
    setCOORDENADAS(12, 20, 10);
    setCOORDENADAS(13, 30, 10);
    setCOORDENADAS(14, 40, 10);
    setCOORDENADAS(15, 50, 10);
    setCOORDENADAS(16, 60, 10);
    setCOORDENADAS(17, 70, 10);
    setCOORDENADAS(18, 80, 10);
    setCOORDENADAS(19, 90, 10);

    setCOORDENADAS(20, 0, 20);
    setCOORDENADAS(21, 10, 20);
    setCOORDENADAS(22, 20, 20);
    setCOORDENADAS(23, 30, 20);
    setCOORDENADAS(24, 40, 20);
    setCOORDENADAS(25, 50, 20);
    setCOORDENADAS(26, 60, 20);
    setCOORDENADAS(27, 70, 20);
    setCOORDENADAS(28, 80, 20);
    setCOORDENADAS(29, 90, 20);

    setCOORDENADAS(30, 0, 30);
    setCOORDENADAS(31, 10, 30);
    setCOORDENADAS(32, 20, 30);
    setCOORDENADAS(33, 30, 30);
    setCOORDENADAS(34, 40, 30);
    setCOORDENADAS(35, 50, 30);
    setCOORDENADAS(36, 60, 30);
    setCOORDENADAS(37, 70, 30);
    setCOORDENADAS(38, 80, 30);
    setCOORDENADAS(39, 90, 30);

    setCOORDENADAS(40, 0, 40);
    setCOORDENADAS(41, 10, 40);
    setCOORDENADAS(42, 20, 40);
    setCOORDENADAS(43, 30, 40);
    setCOORDENADAS(44, 40, 40);
    setCOORDENADAS(45, 50, 40);
    setCOORDENADAS(46, 60, 40);
    setCOORDENADAS(47, 70, 40);
    setCOORDENADAS(48, 80, 40);
    setCOORDENADAS(49, 90, 40);

    setCOORDENADAS(50, 0, 50);
    setCOORDENADAS(51, 10, 50);
    setCOORDENADAS(52, 20, 50);
    setCOORDENADAS(53, 30, 50);
    setCOORDENADAS(54, 40, 50);
    setCOORDENADAS(55, 50, 50);
    setCOORDENADAS(56, 60, 50);
    setCOORDENADAS(57, 70, 50);
    setCOORDENADAS(58, 80, 50);
    setCOORDENADAS(59, 90, 50);

    setCOORDENADAS(60, 0, 60);
    setCOORDENADAS(61, 10, 60);
    setCOORDENADAS(62, 20, 60);
    setCOORDENADAS(63, 30, 60);
    setCOORDENADAS(64, 40, 60);
    setCOORDENADAS(65, 50, 60);
    setCOORDENADAS(66, 60, 60);
    setCOORDENADAS(67, 70, 60);
    setCOORDENADAS(68, 80, 60);
    setCOORDENADAS(69, 90, 60);
   
    setCOORDENADAS(70, 0 , 70);
    setCOORDENADAS(71, 10, 70);
    setCOORDENADAS(72, 20, 70);
    setCOORDENADAS(73, 30, 70);
    setCOORDENADAS(74, 40, 70);
    setCOORDENADAS(75, 50, 70);
    setCOORDENADAS(76, 60, 70);
    setCOORDENADAS(77, 70, 70);
    setCOORDENADAS(78, 80, 70);
    setCOORDENADAS(79, 90, 70);

    setCOORDENADAS(80, 0 , 80);
    setCOORDENADAS(81, 10, 80);
    setCOORDENADAS(82, 20, 80);
    setCOORDENADAS(83, 30, 80);
    setCOORDENADAS(84, 40, 80);
    setCOORDENADAS(85, 50, 80);
    setCOORDENADAS(86, 60, 80);
    setCOORDENADAS(87, 70, 80);
    setCOORDENADAS(88, 80, 80);
    setCOORDENADAS(89, 90, 80);

    setCOORDENADAS(90, 0 , 90);
    setCOORDENADAS(91, 10, 90);
    setCOORDENADAS(92, 20, 90);
    setCOORDENADAS(93, 30, 90);
    setCOORDENADAS(94, 40, 90);
    setCOORDENADAS(95, 50, 90);
    setCOORDENADAS(96, 60, 90);
    setCOORDENADAS(97, 70, 90);
    setCOORDENADAS(98, 80, 90);
    setCOORDENADAS(99, 90, 90);

    //imprimirGRAFO();
    //imprimirFEROMONAS();

    // INICIA EL CICLO 
    for (int iteracion = 1; iteracion <= ITERACIONES; iteracion++)
    {
        cout << flush;
        cout << "ITERACION " << iteracion << " :" << endl
             << endl;

        for (int k = 0; k < NUMEROHORMIGAS; k++)                // PARA CADA HORMIGA
        {
            cout << " Hormiga " << k + 1 << " empieza" << endl;
            //ruta(k);
            int it = NUMERONODOS;                               // NUMERO INICIAL DE ITERACIONES 
            while (0 != valid(k, it))
            {
                cout << "  * calculando ruta ..." << endl;
                for (int i = 0; i < NUMERONODOS; i++)           // LIMPIO LA RUTA 
                {
                    RUTAS[k][i] = -1;
                }
                //int it = ruta(k);
                ruta(k);                                        // CALCULO LA NUEVA RUTA 
                for (int u = 0; u < NUMERONODOS; u++)           // DEFINO EL NUEVO NUMERO ITERACIONES (CANTIDAD 
                                                                // DE NODOS RECORRIDOS EN LA RUTA CALCULADA)
                {
                    if (RUTAS[k][u] < 0)
                    {
                        it = u;
                        break;
                    }
                }
            }
            cout << endl;
            cout << "  ruta seleccionada: " << flush;
            for (int i = 0; i < it; i++)                        // IMPRIMIR LA RUTA SELECCIONADA POR LA HORMIGA 
            {
                cout << RUTAS[k][i] + 1 << " ";
            }
            cout << endl;

            cout << "  ** RUTA CALCULADA" << endl; 
            double rlongitud = longitud(k, it);                 // LA LONGITUD DE LA RUTA 

            if (rlongitud < MEJORLONG)                          // VERIFICAR SI LA RUTA ES MAS CORTA
            {
                MEJORLONG = rlongitud;
                for (int i = 0; i < it; i++)
                {
                    MEJORRUTA[i] = RUTAS[k][i];                 // GUARDAMOS LA NUEVA MEJOR RUTA
                }
                for (int i = it; i < NUMERONODOS - it; i++)
                {
                    MEJORRUTA[i] = -1;                          // LIMPIAR LOS ESPACIOS PREVIOS DE LA MEJOR RUTA 
                }
                numITER[NUMEROHORMIGAS] = it;
            }
            cout << " * Hormiga " << k + 1 << " HA TERMINADO" << endl;
            cout << endl;
            numITER[k] = it;
        }

        cout << endl
             << "actualizando feromonas ..." << endl;
        actualizarFEROMONAS();
        cout << " Feromonas ACTUALIZADAS" << endl
             << endl;
        //imprimirFEROMONAS();

        for (int i = 0; i < NUMEROHORMIGAS; i++)                 // LIMPIAMOS LA RUTA 
        {
            for (int j = 0; j < NUMERONODOS; j++)
            {
                RUTAS[i][j] = -1;
            }
        }

        cout << endl
             << "ITERACION " << iteracion << " TERMINADA" << endl
             << endl;
    }
    cout << "EL ALGORITMO ACO HA CALCULADO LA MEJOR RUTA!" << endl;
    cout << " " << endl;
    imprimirRESULTADOS();

    std::ofstream outfile("ruta.txt");                          // INICIAMOS UN TXT PARA GUARDAR LA RUTA 
    for (int i = 0; i < numITER[NUMEROHORMIGAS]; i++)
    {
        double cordX = (double)COORDS[MEJORRUTA[i]][0];
        double cordY = (double)COORDS[MEJORRUTA[i]][1];
        double cordXnext = (double)COORDS[MEJORRUTA[i]][0];
        double cordYnext = (double)COORDS[MEJORRUTA[i]][1];
        if (i < numITER[NUMEROHORMIGAS] - 1)
        {
            cordXnext = (double)COORDS[MEJORRUTA[i + 1]][0];
            cordYnext = (double)COORDS[MEJORRUTA[i + 1]][1];
        }
        double x = cordX;
        double y = cordY;
        if ((cordX != cordXnext) && ((cordY != cordYnext)))
        {
            for (int j = 0; j < 10; j++)
            {
                x += 1;
                y += 1;
                outfile << x << " " << y << std::endl;
                //cout << x << " " << y << endl;
            }
        }
        if ((cordX == cordXnext) && ((cordY == cordYnext)) && (i < numITER[NUMEROHORMIGAS] -1))
        {
            for (int j = 0; j < 10; j++)
            {
                x += 1;
                y += 1;
                outfile << x << " " << y << std::endl;
                //cout << x << " " << y << endl;
            }
        }
        if ((cordX == cordXnext) && ((cordY != cordYnext)))
        {
            for (int j = 0; j < 10; j++)
            {
                y += 1;
                outfile << x << " " << y << std::endl;
                //cout << x << " " << y << endl;
            }
        }
        if ((cordX != cordXnext) && ((cordY == cordYnext)))
        {
            for (int j = 0; j < 10; j++)
            {
                x += 1;
                outfile << x << " " << y << std::endl;
                //cout << x << " " << y << endl;
            }
        }
    }
    outfile.close();                                                        // CERRAMOS EL TXT 

    // -------------------- LIMPIAMOS LA MEMORIA EN EL HEAP ----------------------
    /*
    for (int i = 0; i < NUMERONODOS-1; i++)
    {
        delete[] GRAFO[i];
        delete[] COORDS[i];
        delete[] FEROMONAS[i];
        delete[] deltaFEROMONAS[i];
        if (i < NUMERONODOS - 1)
        {
            delete[] PROB[i];
        }
    }
    delete[] GRAFO;
    delete[] COORDS;
    delete[] FEROMONAS;
    delete[] deltaFEROMONAS;
    delete[] PROB;
    */
    return 0;
}
