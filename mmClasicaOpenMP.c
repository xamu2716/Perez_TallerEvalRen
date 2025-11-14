/*#######################################################################################
# Fecha: 30 Octubre 2025
# Autor: Xamuel Perez Madrigal, Juan Felipe Gomez Lopez, David Beltran
# Programa: 
# Multiplicacion de Matrices algoritmo clasico
# Version:
#      Paralelismo con OpenMP
######################################################################################*/

// Inclusion de interfaes esenciales.
#include <stdio.h>    // Para funciones estandar de entrada y salida
#include <stdlib.h>   // Para funciones de proposito general como calloc, exit
#include <string.h>   // Para manipulacion de cadenas de caracteres 
#include <time.h>     // Para la funcion time(), usada en srand()
#include <sys/time.h> // Para funciones de medicion de tiempo
#include <omp.h>      // La interfaz de OpenMP

// Inclusion de cabeceras de funciones propias
#include "operacionesMatrices.h" // Funciones para inicializar e imprimir matrices.
#include "operacionesTiempo.h"   // Funciones para iniciar/finalizar la medicion de tiempo.

int main(int argc, char *argv[]) {
    // 1. Verificacion de argumentos de entrada
    if (argc != 3) {
        // Si no se reciben exactamente 3 argumentos, muestra un mensaje de uso y termina.
        printf("\n Use: $./clasicaOpenMP SIZE Hilos \n\n");
        exit(0);
    }

    // 2. Inicializacion de variables y asignacion de memoria
    // N es el tamano de la matriz (N x N)
    int N = atoi(argv[1]);
    // TH (Thread count) es el numero de hilos a usar.
    int TH = atoi(argv[2]);

    // Asignacion dinamica de memoria para las matrices A, B y C (Resultado).
    // Se usa calloc para inicializar toda la memoria a cero.
    double *matrixA = (double *)calloc(N * N, sizeof(double));
    double *matrixB = (double *)calloc(N * N, sizeof(double));
    double *matrixC = (double *)calloc(N * N, sizeof(double));

    // Inicializa la semilla para rand() para asegurar valores aleatorios diferentes cada vez.
    srand(time(NULL));

    // 3. Configuracion del Paralelismo
    // Configura explicitamente el numero de hilos que OpenMP debe usar
    // EL numero de hilos es el que se ingresa en la ejecucion
    omp_set_num_threads(TH);

    // Inicializa las matrices A y B con valores aleatorios.
    iniMatrix(matrixA, matrixB, N);

    // Muestra las matrices A y B.
    impMatrix(matrixA, N, 0);
    impMatrix(matrixB, N, 1);

    // Inicia la medicion de tiempo 
    InicioMuestra();

    // 4. Multiplicacion Paralela
    // Llama a la funcion que contiene la directiva OpenMP para dividir el trabajo
    // entre los TH hilos configurados en el paso 3.
    // Nota: El paralelismo real ocurre dentro de la funcion multiMatrix, ya que hay 
    //Se incluyen las directivas #pragma para que el compilador sepa que habra ejeucion paralela
    multiMatrix(matrixA, matrixB, matrixC, N);

    // Finaliza la medicion de tiempo. Esta funcion calcula e imprime el tiempo total.
    FinMuestra();

    // Imprime la matriz resultante C
    impMatrix(matrixC, N, 0);

    // 5. Liberacion de Recursos
    // Libera la memoria asignada a las matrices, evitando fugas de memoria.
    free(matrixA);
    free(matrixB);
    free(matrixC);

    return 0; // Termina el programa exitosamente.
}
