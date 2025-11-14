/*#######################################################################################
 #* Fecha: 30 Octubre 2025
 #* Autor: Xamuel Pérez Madrigal, Juan Felipe Gómez López, David Beltrán
 #* Programa:
 #* Multiplicación de Matrices algoritmo clásico
 #* Versión:
 #* Paralelismo con Hilos Pthreads "Posix"
 #* Proposito:
    #* Implementar la multiplicación de matrices utilizando hilos para paralelismo.
 #######################################################################################*/

// Interfaces requeridas
#include <pthread.h> // Hilos POSIX: Interfaz esencial para crear y gestionar la ejecución paralela.
#include <stdlib.h>  // Proporciona funciones generales, incluyendo manejo de memoria y utilidades.

// Archivos de encabezado con definiciones de estructuras y matrices
#include "operacionesHilos.h"    // Contiene la definición de 'struct parametros' (datos de entrada para cada hilo).
#include "operacionesMatrices.h" // Contiene las declaraciones de las matrices (matrixA, matrixB, matrixC).


// 'extern' indica que estas variables se definieron en el código principal.
extern double *matrixA, *matrixB, *matrixC; // Punteros a las Matrices A, B, y C (Resultado). Son datos COMPARTIDOS.
extern pthread_mutex_t MM_mutex;            // Mecanismo de Exclusión Mutua (Mutex): Se usa para proteger secciones de código críticas.

/**
 * Esta función es el punto de inicio para cada hilo Pthread creado. Su tarea es calcular
 * un rango de filas de la matriz resultante (matrixC),
 * implementando el concepto de "Divide y Vencerás" en el paralelismo.
 * Como parámetro recibe param variables Un puntero genérico (void*) que contiene la información única de trabajo 
 * para el hilo.
 * return void* Siempre retorna NULL, indicando la finalización de la rutina del hilo.
 */
 
void *multiplicarMatrix(void *variables) {
    // Se convierte el puntero genérico (void*) a la estructura 'parametros' esperada.
    struct parametros *data = (struct parametros *)variables;
    int idH = data->idH;     // ID del Hilo 
    int nH = data->nH;       // Número Total de Hilos
    int D = data->N;         // Dimensión de las matrices 

    
    // Se define qué filas de la matriz resultante C calculará este hilo específico.

    // Fila Inicial (filaI): El punto de partida de este hilo en la matriz C.
    // Ejemplo: Si D=100 y nH=10, el hilo con idH=3 comienza en la fila (100/10)*3 = fila 30.
    int filaI = (D / nH) * idH;

    // Fila Final filaF: El punto donde este hilo debe detenerse
    // Manejo de Residuo: La expresión ternaria asegura que el último hilo (idH == nH - 1)
    // tome todas las filas restantes (hasta D), garantizando que no queden filas sin calcular.
    int filaF = (idH == nH - 1) ? D : (D / nH) * (idH + 1);

   
    double Suma;    // Almacena el valor acumulado del producto punto para cada elemento C[i][j].
    double *pA, *pB; // Punteros auxiliares para recorrer eficientemente las matrices A y B.

   

    // Bucle 1 (Filas): Itera solo sobre las filas asignadas a este hilo (i = filaI hasta filaF - 1).
    for (int i = filaI; i < filaF; i++) {
        // Bucle 2 (Columnas): Itera sobre TODAS las columnas de la matriz B (j = 0 hasta D - 1).
        for (int j = 0; j < D; j++) {

            // Posicionamiento de Punteros: Prepara los punteros para el producto punto.
            pA = matrixA + i * D; // Puntero pA: Se coloca al inicio de la Fila i de matrixA.
            pB = matrixB + j;     // Puntero pB: Se coloca al inicio de la Columna j de matrixB.
            Suma = 0.0;           // Inicializa el acumulador para el nuevo elemento C[i][j].

            // Bucle 3 (Producto Punto): Multiplica Fila i de A por Columna j de B.
            for (int k = 0; k < D; k++, pA++, pB += D) {
                // pA++: Avanza al siguiente elemento en la Fila i de A.
                // pB += D: Avanza al siguiente elemento en la Columna 'j' de B (salta D elementos).
                Suma += (*pA) * (*pB); // Acumula el producto A[i][k] * B[k][j].
            }

            // Se usa el mutex para garantizar que la escritura en matrixC sea segura.
            //Cada hilo escribe en filas diferentes,
            pthread_mutex_lock(&MM_mutex);       // Bloquea el acceso para otros hilos.
            matrixC[i * D + j] = Suma;           // Escribe el resultado final C[i][j].
            pthread_mutex_unlock(&MM_mutex);     // Libera el acceso para otros hilos.
        }
    }

    pthread_exit(NULL); // El hilo termina su ejecución y se une al hilo principal.
}
