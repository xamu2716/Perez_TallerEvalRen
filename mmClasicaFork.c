/*#######################################################################################
#* Fecha: 30 Octubre 2025
#* Autor: Xamuel Pérez Madrigal
#* Programa:
#*      Multiplicación de Matrices algoritmo clásico
#* Versión:
#*      Paralelismo con Procesos Fork 
######################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include "operacionesFork.h"
#include "operacionesTiempo.h"
#include "operacionesMatrices.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("\n \t\tUse: $./nom_ejecutable Size Hilos \n");
        exit(0);
    }
    
    int N = (int)atoi(argv[1]);
    int num_P = (int)atoi(argv[2]);
    double *matA = (double *)calloc(N * N, sizeof(double));
    double *matB = (double *)calloc(N * N, sizeof(double));
    double *matC = (double *)calloc(N * N, sizeof(double));

    srand(time(0)); 
    
    // Inicializamos las matrices con valores aleatorios
    iniMatrix(matA, matB, N);
    impMatrix(matA, N, 0);
    impMatrix(matB, N, 1);

    int rows_per_process = N / num_P;

    // Inicia la medición de tiempo
    InicioMuestra();

    for (int i = 0; i < num_P; i++) {
        pid_t pid = fork();
        
        if (pid == 0) { 
            // Cada hijo calcula una parte de la matriz
            int start_row = i * rows_per_process;
            int end_row = (i == num_P - 1) ? N : start_row + rows_per_process;
            
            multiplicarMatrix(matA, matB, matC, N, start_row, end_row); 
            
            if (N < 9) {
                printf("\nChild PID %d calculated rows %d to %d:\n", getpid(), start_row, end_row - 1);
                for (int r = start_row; r < end_row; r++) {
                    for (int c = 0; c < N; c++) {
                        printf(" %.2f ", matC[N * r + c]);
                    }
                    printf("\n");
                }
            }
            exit(0); 
        } else if (pid < 0) {
            perror("fork failed");
            exit(1);
        }
    }
    
    for (int i = 0; i < num_P; i++) {
        wait(NULL);  // Espera que terminen todos los procesos hijos
    }
    
    // Finaliza la medición de tiempo
    FinMuestra(); 

    // Liberación de Memoria
    free(matA);
    free(matB);
    free(matC);

    return 0;
}
