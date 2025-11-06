/*#######################################################################################
 #* Fecha: 30 Octubre 2025
 #* Autor: Xamuel Pérez Madrigal
 #* Programa:
 #*      Multiplicación de Matrices algoritmo clásico
 #* Versión:
 #*      Paralelismo con Hilos Pthreads "Posix"
 #######################################################################################*/
#ifndef OPERACIONES_HILOS_H
#define OPERACIONES_HILOS_H

#include <pthread.h>

// Declaración de MM_mutex como extern
extern pthread_mutex_t MM_mutex;

// Declaración de matrices como extern
extern double *matrixA;
extern double *matrixB;
extern double *matrixC;

// Estructura para pasar parámetros a los hilos
struct parametros {
    int idH;    // ID del hilo
    int nH;     // Número total de hilos
    int N;      // Tamaño de la matriz
};

// Función que multiplica las matrices en paralelo usando hilos
void *multiplicarMatrix(void *variables);

#endif // OPERACIONES_HILOS_H