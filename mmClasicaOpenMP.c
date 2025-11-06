/*#######################################################################################
 #* Fecha: 30 Octubre 2025
 #* Autor: Xamuel Pérez Madrigal
 #* Programa: 
 #* 	 Multiplicación de Matrices algoritmo clásico
 #* Versión:
 #*	 Paralelismo con OpenMP
######################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#include "operacionesMatrices.h"
#include "operacionesTiempo.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("\n Use: $./clasicaOpenMP SIZE Hilos \n\n");
        exit(0);
    }

    int N = atoi(argv[1]);
    int TH = atoi(argv[2]);
    double *matrixA = (double *)calloc(N * N, sizeof(double));
    double *matrixB = (double *)calloc(N * N, sizeof(double));
    double *matrixC = (double *)calloc(N * N, sizeof(double));

    srand(time(NULL));

    // Configura el número de hilos en OpenMP
    omp_set_num_threads(TH);

    // Inicializa las matrices con valores aleatorios
    iniMatrix(matrixA, matrixB, N);

    // Imprime las matrices iniciales
    impMatrix(matrixA, N, 0);
    impMatrix(matrixB, N, 1);

    // Inicia la medición de tiempo
    InicioMuestra();

    // Realiza la multiplicación de matrices usando OpenMP
    multiMatrix(matrixA, matrixB, matrixC, N);

    // Finaliza la medición de tiempo
    FinMuestra();

    // Imprime la matriz resultante
    impMatrix(matrixC, N, 0);

    // Liberación de Memoria
    free(matrixA);
    free(matrixB);
    free(matrixC);

    return 0;
}