/*#######################################################################################
 #* Fecha: 30 Octubre 2025
 #* Autor: Xamuel Pérez Madrigal
 #* Programa:
 #*      Multiplicación de Matrices algoritmo clásico
 #* Versión:
 #*      Operaciones con Matrices
 #*Proposito:
    #*      Implementar funciones para inicializar, imprimir y multiplicar matrices.
 #######################################################################################*/
#include <stdio.h>
#include <stdlib.h>
#include "operacionesMatrices.h"

void iniMatrix(double *m1, double *m2, int D){
    for(int i = 0; i < D*D; i++, m1++, m2++){
        *m1 = (double)rand()/RAND_MAX*(5.0-1.0); 
        *m2 = (double)rand()/RAND_MAX*(9.0-5.0); 
    }   
}

void impMatrix(double *matrix, int D, int t) {
    int aux = 0;
    if(D < 6)
        switch(t){
            case 0:
                for(int i=0; i<D*D; i++){
                    if(i%D==0) printf("\n");
                    printf("%.2f ", matrix[i]);
                }
                printf("\n  - \n");
                break;
            case 1:
                while(aux<D){
                    for(int i=aux; i<D*D; i+=D)
                        printf("%.2f ", matrix[i]);
                    aux++;
                    printf("\n");
                }    
                printf("\n  - \n");
                break;
            default:
                printf("Sin tipo de impresión\n");
        }
}

void multiMatrix(double *mA, double *mB, double *mC, int D) {
    double Suma, *pA, *pB;
    #pragma omp parallel
    {
    #pragma omp for
    for(int i=0; i<D; i++){
        for(int j=0; j<D; j++){
            pA = mA + i*D;    
            pB = mB + j; 
            Suma = 0.0;
            for(int k=0; k<D; k++, pA++, pB+=D){
                Suma += *pA * *pB;
            }
            mC[i*D+j] = Suma;
        }
    }
    }
}

void multiMatrixTrans(double *mA, double *mB, double *mC, int D) {
    double Suma, *pA, *pB;
    #pragma omp parallel
    {
    #pragma omp for
    for(int i=0; i<D; i++){
        for(int j=0; j<D; j++){
            pA = mA + i*D;    
            pB = mB + j*D;    
            Suma = 0.0;
            for(int k=0; k<D; k++, pA++, pB++){
                Suma += *pA * *pB;
            }
            mC[i * D + j] = Suma;
        }
    }
    }
}
