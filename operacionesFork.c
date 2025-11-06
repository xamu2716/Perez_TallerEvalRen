/*#######################################################################################
 #* Fecha: 30 Octubre 2025
 #* Autor: Xamuel Pérez Madrigal
 #* Programa:
 #*      Multiplicación de Matrices algoritmo clásico
 #* Versión:
 #*      Paralelismo con Procesos Fork
 #* Proposito:
 #*      Implementar la multiplicación de matrices utilizando procesos fork para paralelismo.
 #######################################################################################*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "operacionesFork.h"
#include "operacionesMatrices.h"

void multiplicarMatrix(double *mA, double *mB, double *mC, int D, int filaI, int filaF) {
    double Suma, *pA, *pB;
    for (int i = filaI; i < filaF; i++) {
        for (int j = 0; j < D; j++) {
            Suma = 0.0;
            pA = mA + i*D;
            pB = mB + j*D;
            for (int k = 0; k < D; k++, pA++, pB+=D) {
                Suma += *pA * *pB;
            }
            mC[i*D+j] = Suma;
        }
    }
}