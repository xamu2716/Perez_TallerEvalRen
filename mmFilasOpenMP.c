/*#######################################################################################
 #* Fecha: 30 Octubre 2025
 #* Autor: Xamuel Pérez Madrigal
 #* Programa:
 #*      Multiplicación de Matrices algoritmo matriz Transpuesta (Filas x Filas) 
 #* Versión:
 #*      Paralelismo con OpenMP
 #######################################################################################*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#include "operacionesMatrices.h"
#include "operacionesTiempo.h"

int main(int argc, char *argv[]) {
    if(argc != 3){
        printf("\n Use: $./clasicaOpenMP SIZE Hilos \n\n");
        exit(0);
    }

    int N = atoi(argv[1]);
    int TH = atoi(argv[2]);
    double *matrixA = (double *)calloc(N*N, sizeof(double));
    double *matrixB = (double *)calloc(N*N, sizeof(double));
    double *matrixC = (double *)calloc(N*N, sizeof(double));
    
    srand(time(NULL));
    omp_set_num_threads(TH);

    iniMatrix(matrixA, matrixB, N);
    impMatrix(matrixA, N, 0);
    impMatrix(matrixB, N, 1);

    InicioMuestra();
    multiMatrixTrans(matrixA, matrixB, matrixC, N);
    FinMuestra();

    impMatrix(matrixC, N, 0);

    // Liberación de Memoria
    free(matrixA);
    free(matrixB);
    free(matrixC);
    
    return 0;
}