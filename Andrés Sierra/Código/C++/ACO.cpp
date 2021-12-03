/*
                    TRABAJO DE GRADUACION 
                UNIVERSIDAD DEL VALLE DE GUATEMALA

    ROBOTICA DE ENJAMBRE - ALGORITMO ANT COLONY OPTIMIZATION
                    DECLARACION FUNCIONES 
    ANDRES SIERRA
    CARNET 17025 

	Copyright:	Diogo A. B. Fernandes -	diogoabfernandes@gmail.com
	Modified by Andres Sierra  - sie17025@uvg.edu.gt
*/


#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <fstream>

#include <cmath>
#include <limits>
#include <climits>

#include "ACO.h"

using namespace std;

// PARAMETROS DEL ALGORITMO
ACO::ACO (int hormigas, int nodos, double alpha, double beta, double q, double ro, double taumax, int nodoinicial,int nodofinal) {
	NUMEROHORMIGAS 		= hormigas;
	NUMERONODOS 		= nodos;
	ALPHA 				= alpha;
	BETA 				= beta;
	Q 					= q;
	RO 					= ro;
	TAUMAX 				= taumax;
	NODOINICIAL			= nodoinicial;
	NODOFINAL			= nodofinal;

	randoms = new Randoms (21);
}

// INICIALIZACION
void ACO::inicializar(){
	GRAFO = new int *[NUMERONODOS];
    COORDS = new double *[NUMERONODOS];
    FEROMONAS = new double *[NUMERONODOS];
    deltaFEROMONAS = new double *[NUMERONODOS];
    PROB = new double *[NUMERONODOS - 1];
	numITER = new int[NUMEROHORMIGAS + 1];

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
 
void ACO::conectarNODOS (int nodoi, int nodoj) {
	GRAFO[nodoi][nodoj] = 1;
    FEROMONAS[nodoi][nodoj] = randoms->Uniforme() * TAUMAX;
    //FEROMONAS[nodoi][nodoj] = (rand() % (1)) * TAUMAX;
    GRAFO[nodoj][nodoi] = 1;
    FEROMONAS[nodoj][nodoi] = FEROMONAS[nodoi][nodoj];
}

void ACO::setCOORDENADAS (int nodo, double x, double y) {
	COORDS[nodo][0] = x;
	COORDS[nodo][1] = y;
}
bool ACO::nodoVALIDO(int nodoi, int nodoc)
{
	return (GRAFO[nodoi][nodoc] == 1);
}

double ACO::distancia(int nodoi, int nodoj)
{
    return (double)
        sqrt(pow(COORDS[nodoi][0] - COORDS[nodoj][0], 2) +
             pow(COORDS[nodoi][1] - COORDS[nodoj][1], 2));
}
// VERIFICA SI EL NODO YA FUE VISITADO 
bool ACO::visitado(int hormigaK, int c)
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
double ACO::PHI(int nodoi, int nodoj, int hormigaK)
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
int ACO::siguienteNODO()
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
int ACO::nodoFINAL(int nodoi)
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
void ACO::ruta(int hormigaK)
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
int ACO::valid(int hormigaK, int iteracion)
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
double ACO::longitud(int hormigaK, int numit)
{
    double sum = 0.0;
    for (int j = 0; j < numit - 1; j++)
    {
        sum += distancia(RUTAS[hormigaK][j], RUTAS[hormigaK][j + 1]);
    }
    return sum;
}
// ACTUALIZAR LA FEROMONA EN CADA NODO
void ACO::actualizarFEROMONAS()
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
void ACO::imprimirGRAFO()
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
void ACO::imprimirFEROMONA()
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
void ACO::imprimirRESULTADOS()
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

void ACO::optimizar(int ITERACIONES)
{
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

    std::ofstream outfile("rutaCLASE.txt");                          // INICIAMOS UN TXT PARA GUARDAR LA RUTA 
    //outfile << (double)COORDS[MEJORRUTA[0]][0] << " " << (double)COORDS[MEJORRUTA[0]][1] << std::endl;
    for (int i = 0; i < numITER[NUMEROHORMIGAS]; i++)
    {
        outfile << (double)COORDS[MEJORRUTA[i]][0] << " " << (double)COORDS[MEJORRUTA[i]][1] << std::endl;
    }
    /*
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
            for (int j = 0; j < 2; j++)
            {
                x += 5;
                y += 5;
                outfile << x << " " << y << std::endl;
                //cout << x << " " << y << endl;
            }
        }
        if ((cordX == cordXnext) && ((cordY == cordYnext)) && (i < numITER[NUMEROHORMIGAS] -1))
        {
            for (int j = 0; j < 2; j++)
            {
                x += 5;
                y += 5;
                outfile << x << " " << y << std::endl;
                //cout << x << " " << y << endl;
            }
        }
        if ((cordX == cordXnext) && ((cordY != cordYnext)))
        {
            for (int j = 0; j < 2; j++)
            {
                y += 5;
                outfile << x << " " << y << std::endl;
                //cout << x << " " << y << endl;
            }
        }
        if ((cordX != cordXnext) && ((cordY == cordYnext)))
        {
            for (int j = 0; j < 2; j++)
            {
                x += 5;
                outfile << x << " " << y << std::endl;
                //cout << x << " " << y << endl;
            }
        }
    }*/
    outfile.close();                                                        // CERRAMOS EL TXT 

    // -
}
