/*
                    TRABAJO DE GRADUACION 
                UNIVERSIDAD DEL VALLE DE GUATEMALA

    ROBOTICA DE ENJAMBRE - ALGORITMO ANT COLONY OPTIMIZATION
                    	HEADER FILE 
    ANDRES SIERRA
    CARNET 17025

*/
#include "Randoms.cpp"
#ifndef __ACO_H__
#define __ACO_H__

class ACO {
public:
	ACO (int hormigas, int nodos, double alpha, double beta, double q, double ro, double taumax, int nodoinicial,int nodofinal);
	void inicializar();
	void imprimirGRAFO();
	void imprimirFEROMONA();
	void imprimirRESULTADOS();

	void conectarNODOS(int nodox, int nodoy);
	void setCOORDENADAS (int nodo, double x, double y);
	void optimizar (int ITERATIONS);
	 
private:

	int NUMEROHORMIGAS,NUMERONODOS, NODOINICIAL,NODOFINAL;
	double ALPHA, BETA, Q, RO, TAUMAX;

	double MEJORLONG;
	int *MEJORRUTA, *numITER;

	int **GRAFO, **RUTAS;
	double **COORDS, **FEROMONAS, **deltaFEROMONAS, **PROB;
	
	bool nodoVALIDO(int nodoi, int nodoc);
	double distancia(int nodoi, int nodoj);
	bool visitado(int hormigaK, int c);
	double PHI(int nodoi, int nodoj, int hormigaK);
	int siguienteNODO();
	int nodoFINAL(int nodoi);
	void ruta(int hormigaK);
	int valid(int hormigaK, int iteracion);
	double longitud(int hormigaK, int numit);
	void actualizarFEROMONAS();


	Randoms *randoms;
};

#endif  /* __ACO_H__ */
