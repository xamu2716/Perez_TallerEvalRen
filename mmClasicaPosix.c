/*#######################################################################################
# Fecha: 30 Octubre 2025
# Autor: Xamuel Perez Madrigal, Juan Felipe Gomez Lopez, David Beltran
# Programa:
#      Multiplicacion de Matrices algoritmo clasico
# Version:
#      Paralelismo con Hilos Pthreads Posix 
######################################################################################*/


// Inclusion de interfaces esenciales.
#include <stdio.h>    // Para funciones estandar de entrada y salida
#include <pthread.h>  // La interfaz para el manejo de hilos posix
#include <unistd.h>   // Para funciones de sistemas operativos
#include <stdlib.h>   // Para funciones de proposito general como calloc, exit
#include <time.h>     // Para funciones de tiempo
#include <sys/time.h> // Para funciones de medicion de tiempo

// Inclusion de cabeceras de funciones propias
#include "operacionesMatrices.h" // Funciones para inicializar e imprimir matrices.
#include "operacionesHilos.h"    // Contiene la funcion que ejecutan los hilos y la estructura 'parametros'.
#include "operacionesTiempo.h"   // Funciones para iniciar y finalizar la medicion de tiempo.

// 1. Variables globales
// Las matrices se declaran como globales para que todos los hilos puedan acceder a ellas directamente
double *matrixA;
double *matrixB;
double *matrixC;
// Mutex para proteger secciones criticas si varios hilos escribieran en el mismo lugar
pthread_mutex_t MM_mutex;

int main(int argc, char *argv[]) {
    // 2. Verificacion y lectura de argumentos
    // Verifica que se ingresen exactamente 3 argumentos: ./ejecutable N T
    if (argc != 3) {
        printf("Uso correcto:\n  $ ./ejecutableMM tamMatriz numHilos\n");
        printf("Ejemplo: ./ejecutableMM 4 2\n");
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);         // N: Tamano de la matriz (N x N)
    int n_threads = atoi(argv[2]); // n_threads: Numero de hilos a crear

    printf("\n--- Inicio Multiplicacion Matrices ---\n");
    printf("Tamano de matriz: %d x %d\n", N, N);
    printf("Cantidad de hilos: %d\n\n", n_threads);

    // Declaracion de un array para almacenar los IDs de los hilos y sus atributos
    pthread_t p[n_threads];
    pthread_attr_t atrMM;

    // 3. Asignacion de memoria (variables globales)
    // Asignacion dinamica de memoria para las matrices, accesible por todos los hilos.
    matrixA = (double *)calloc(N * N, sizeof(double));
    matrixB = (double *)calloc(N * N, sizeof(double));
    matrixC = (double *)calloc(N * N, sizeof(double));

    // Verificar si hubo errores al asignar memoria
    if (!matrixA || !matrixB || !matrixC) {
        printf("Error al asignar memoria para las matrices.\n");
        exit(EXIT_FAILURE);
    }

    // 4. Inicializacion y pre-ejecucion
    // Inicializa las matrices A y B con valores aleatorios
    iniMatrix(matrixA, matrixB, N);

    // Muestra las matrices A y B 
    printf("Matriz A generada:\n");
    impMatrix(matrixA, N, 0);
    printf("Matriz B generada:\n");
    impMatrix(matrixB, N, 1);

    // Inicia la medicion de tiempo.
    InicioMuestra();

    // 5. Inicializacion de pthreads y atributos
    // Inicializa el mutex (bloqueo)
    pthread_mutex_init(&MM_mutex, NULL);
    // Inicializa el objeto de atributos para los hilos
    pthread_attr_init(&atrMM);
    // Configura los hilos para que sean joinable, permitiendo que el hilo principal espere por ellos.
    pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);

    // Asigna memoria para el array de estructuras que contienen los parametros de cada hilo.
    struct parametros *datos = malloc(n_threads * sizeof(struct parametros));

    printf("\nCreando hilos...\n");

    // 6. Creacion de Hilos y Asignacion de Tareas
    for (int j = 0; j < n_threads; j++) {
        // Prepara la estructura 'parametros' para el hilo j
        datos[j].idH = j;          // Identificador del hilo (0 a n_threads-1)
        datos[j].nH = n_threads;   // Total de hilos
        datos[j].N = N;            // Tamano de la matriz

        // Muestra que rango de filas procesara este hilo.
        printf(" > Hilo %d creado (procesara filas %d a %d)\n",
            j, (N/n_threads)*j, (N/n_threads)*(j+1)-1);

        // Crea el hilo:
        // &p[j]: Donde se almacena el ID del nuevo hilo.
        // &atrMM: Atributos del hilo.
        // multiplicarMatrix: La funcion que el hilo comenzara a ejecutar.
        // (void *)&datos[j]: El argumento que se pasa a la funcion (sus parametros de trabajo).
        pthread_create(&p[j], &atrMM, multiplicarMatrix, (void *)&datos[j]);
    }

    // 7. Sincronizacion
    // Espera a que todos los hilos terminen su ejecucion.
    for (int j = 0; j < n_threads; j++) {
        pthread_join(p[j], NULL);
    }

    // Finaliza la medicion del tiempo total de ejecucion.
    FinMuestra();

    // 8. Resultados y Limpieza
    printf("\nMatriz resultante C = A x B:\n");
    impMatrix(matrixC, N, 0);

    // Liberacion de memoria de las matrices y la estructura de parametros.
    free(matrixA);
    free(matrixB);
    free(matrixC);
    free(datos);

    // Destruccion de atributos y mutex de Pthreads
    pthread_attr_destroy(&atrMM);
    pthread_mutex_destroy(&MM_mutex);

    printf("\n--- Finalizacion Exitosa ---\n");

    // Termina el hilo principal.
    pthread_exit(NULL);
    return 0;
}
