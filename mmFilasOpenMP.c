/*#######################################################################################
# Fecha: 30 Octubre 2025
# Autor: Xamuel Perez Madrigal, Juan Felipe Gomez Lopez, David Esteban Beltran
# Programa:
#      Multiplicacion de Matrices algoritmo matriz Transpuesta (Filas x Filas) 
# Version:
#      Paralelismo con OpenMP
#######################################################################################*/

// Inclusion de interfaces esenciales.
#include <stdio.h>    // Para funciones estandar de entrada y salida
#include <stdlib.h>   // Para funciones de proposito general como calloc, exit
#include <string.h>   // Para manipulacion de cadenas de caracteres
#include <time.h>     // Para la funcion time(), usada en srand()
#include <sys/time.h> // Para funciones de medicion de tiempo
#include <omp.h>      // La libreria OpenMP para el paralelismo en CPU.

// Inclusion de cabeceras de funciones propias
#include "operacionesMatrices.h" // Funciones para inicializar, transponer, e imprimir matrices.
#include "operacionesTiempo.h"   // Funciones para iniciar/finalizar la medicion de tiempo.

int main(int argc, char *argv[]) {
    // 1. Verificacion de argumentos de entrada
    if(argc != 3){
        // Si no se reciben exactamente 3 argumentos, muestra un mensaje de uso y termina.
        printf("\n Use: $./clasicaOpenMP SIZE Hilos \n\n");
        exit(0);
    }

    // 2. Inicializacion de variables y asignacion de memoria
    int N = atoi(argv[1]);  // N: Tamano de la matriz (N x N)
    int TH = atoi(argv[2]); // TH: Numero de hilos a usar.

    // Asignacion dinamica de memoria para las matrices A, B y C (Resultado).
    double *matrixA = (double *)calloc(N*N, sizeof(double));
    double *matrixB = (double *)calloc(N*N, sizeof(double));
    double *matrixC = (double *)calloc(N*N, sizeof(double));
    
    // 3. Configuracion
    // Inicializa la semilla para rand().
    srand(time(NULL));
    // Configura explicitamente el numero de hilos que OpenMP debe usar.
    omp_set_num_threads(TH);

    // Inicializa las matrices A y B con valores aleatorios.
    iniMatrix(matrixA, matrixB, N);

    // Muestra las matrices iniciales 
    impMatrix(matrixA, N, 0);
    impMatrix(matrixB, N, 1);

    // Inicia la medicion de tiempo (registra el tiempo de inicio).
    InicioMuestra();

    // 4. Multiplicacion Paralela con Transposicion
    multiMatrixTrans(matrixA, matrixB, matrixC, N);

    // Finaliza la medicion de tiempo. Calcula e imprime el tiempo total de ejecucion.
    FinMuestra();

    // Imprime la matriz resultante C.
    impMatrix(matrixC, N, 0);

    // 5. Liberacion de Recursos
    // Libera la memoria asignada a las matrices.
    free(matrixA);
    free(matrixB);
    free(matrixC);
    
    return 0; // Termina el programa exitosamente.
}
