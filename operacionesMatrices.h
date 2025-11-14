/*#######################################################################################
 #* Fecha: 30 Octubre 2025
 #* Autor: Xamuel Perez Madrigal, Felipe Gomez, David Beltran
 #* Programa:
 #*      Multiplicación de Matrices algoritmo clásico
 #* Versión:
 #*      Operaciones con Matrices
 #######################################################################################*/
#ifndef OPERACIONES_MATRICES_H
#define OPERACIONES_MATRICES_H

// Inicializa las matrices con valores aleatorios
void iniMatrix(double *m1, double *m2, int D);

// Imprime la matriz dependiendo de un tipo de impresión
void impMatrix(double *matrix, int D, int t);

// Realiza la multiplicación de matrices (version transpuesta o normal)
void multiMatrix(double *mA, double *mB, double *mC, int D);
void multiMatrixTrans(double *mA, double *mB, double *mC, int D);

#endif // OPERACIONES_MATRICES_H
