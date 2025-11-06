/*#######################################################################################
#* Fecha: 30 Octubre 2025
#* Autor: Xamuel Pérez Madrigal
#* Programa:
#*      Multiplicación de Matrices algoritmo clásico
#* Versión:
#*      Paralelismo con Hilos Pthreads "Posix" 
######################################################################################*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "operacionesMatrices.h"
#include "operacionesHilos.h"
#include "operacionesTiempo.h"

// Variables globales accesibles por todos los hilos
double *matrixA;
double *matrixB;
double *matrixC;
pthread_mutex_t MM_mutex;

int main(int argc, char *argv[]) {
    // Verifica que se ingresen exactamente 3 argumentos 
    if (argc != 3) {
        printf("Uso correcto:\n  $ ./ejecutableMM tamMatriz numHilos\n");
        printf("Ejemplo: ./ejecutableMM 4 2\n");
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);
    int n_threads = atoi(argv[2]);

    printf("\n--- Inicio Multiplicacion Matrices ---\n");
    printf("Tamaño de matriz: %d x %d\n", N, N);
    printf("Cantidad de hilos: %d\n\n", n_threads);

    pthread_t p[n_threads];
    pthread_attr_t atrMM;

    // Asignar memoria a las variables globales
    matrixA = (double *)calloc(N * N, sizeof(double));
    matrixB = (double *)calloc(N * N, sizeof(double));
    matrixC = (double *)calloc(N * N, sizeof(double));

    // Verificar si hubo errores al asignar memoria
    if (!matrixA || !matrixB || !matrixC) {
        printf("Error al asignar memoria para las matrices.\n");
        exit(EXIT_FAILURE);
    }

    // Inicializa las matrices con valores aleatorios
    iniMatrix(matrixA, matrixB, N);

    printf("Matriz A generada:\n");
    impMatrix(matrixA, N, 0);
    printf("Matriz B generada:\n");
    impMatrix(matrixB, N, 1);

    // Inicia la medición de tiempo
    InicioMuestra();

    pthread_mutex_init(&MM_mutex, NULL);
    pthread_attr_init(&atrMM);
    pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);

    // Crea estructuras para los parámetros de los hilos
    struct parametros *datos = malloc(n_threads * sizeof(struct parametros));

    printf("\nCreando hilos...\n");

    // Bucle para crear los hilos que realizarán la multiplicación de matrices
    for (int j = 0; j < n_threads; j++) {
        // Asigna un identificador único al hilo (por ejemplo: 0, 1, 2, ...)
        datos[j].idH = j;

        // Guarda el número total de hilos que se usarán
        datos[j].nH = n_threads;

        // Guarda el tamaño de la matriz (N x N) que se procesará
        datos[j].N = N;

        // Muestra por consola qué rango de filas procesará cada hilo
        printf(" > Hilo %d creado (procesará filas %d a %d)\n",
            j, (N/n_threads)*j, (N/n_threads)*(j+1)-1);

        // Crea el hilo j, asignándole la función que ejecutará (multiplicarMatrix)
        // y pasando como argumento la estructura con sus parámetros específicos
        pthread_create(&p[j], &atrMM, multiplicarMatrix, (void *)&datos[j]);
    }

    // Espera a que todos los hilos terminen su ejecución antes de continuar
    // Garantizando que toda la matriz C haya sido calculada completamente
    for (int j = 0; j < n_threads; j++) {
        pthread_join(p[j], NULL);
    }

    // Finaliza la medición del tiempo total de ejecución del programa
    FinMuestra();

    printf("\nMatriz resultante C = A x B:\n");
    impMatrix(matrixC, N, 0);

    // Liberación de memoria
    free(matrixA);
    free(matrixB);
    free(matrixC);
    free(datos);

    pthread_attr_destroy(&atrMM);
    pthread_mutex_destroy(&MM_mutex);

    printf("\n--- Finalizacion Exitosa ---\n");

    pthread_exit(NULL);
    return 0;
}