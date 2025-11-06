/*#######################################################################################
 #* Fecha: 30 Octubre 2025
 #* Autor: Xamuel Pérez Madrigal
 #* Programa:
 #*      Multiplicación de Matrices algoritmo clásico
 #* Versión:
 #*      Paralelismo con Hilos Pthreads "Posix"
 #* Proposito:
    #*      Implementar la multiplicación de matrices utilizando hilos para paralelismo.
 #######################################################################################*/
#include <pthread.h>
#include <stdlib.h>
#include "operacionesHilos.h"
#include "operacionesMatrices.h"

extern double *matrixA, *matrixB, *matrixC;
extern pthread_mutex_t MM_mutex;

void *multiplicarMatrix(void *variables) {
    struct parametros *data = (struct parametros *)variables;
    int idH = data->idH;
    int nH = data->nH;
    int D = data->N;

    int filaI = (D / nH) * idH;
    int filaF = (idH == nH - 1) ? D : (D / nH) * (idH + 1); // último hilo toma el resto

    double Suma, *pA, *pB;

    for (int i = filaI; i < filaF; i++) {
        for (int j = 0; j < D; j++) {
            pA = matrixA + i * D;
            pB = matrixB + j;
            Suma = 0.0;
            for (int k = 0; k < D; k++, pA++, pB += D) {
                Suma += *pA * *pB;
            }

            // No hay necesidad real de mutex, pero se deja por claridad académica
            pthread_mutex_lock(&MM_mutex);
            matrixC[i * D + j] = Suma;
            pthread_mutex_unlock(&MM_mutex);
        }
    }

    pthread_exit(NULL);
}
