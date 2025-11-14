/*#######################################################################################
 #* Fecha: 30 Octubre 2025
 #* Autor: Xamuel Pérez Madrigal, Juan Felipe Gómez López, David Beltrán
 #* Programa:
 #* Multiplicación de Matrices algoritmo clásico
 #* Versión:
 #* Operaciones con Matrices
 #* Proposito:
 #* Implementar funciones para inicializar, imprimir y multiplicar matrices.
 #######################################################################################*/
// Interfaces necesarias 
#include <stdio.h> // Necesaria para operaciones de entrada y salida.
#include <stdlib.h> // Necesaria para utilidades generales, incluyendo la generación de números pseudoaleatorios ('rand').
#include "operacionesMatrices.h" // Archivo de encabezado que contiene las declaraciones de estas funciones.

/**
 * Inicializa dos matrices (m1 y m2) con valores aleatorios float
 *
 * m1 Puntero a la primera matriz.
 * m2 Puntero a la segunda matriz.
 * D Dimensión de las matrices.
 */
void iniMatrix(double *m1, double *m2, int D){
    // Recorre los D*D elementos de ambas matrices simultáneamente.
    for(int i = 0; i < D*D; i++, m1++, m2++){
        // Genera un valor aleatorio entre [1.0 y 5.0) para m1.
        *m1 = (double)rand()/RAND_MAX*(5.0-1.0);
        // Genera un valor aleatorio entre [5.0 y 9.0) para m2.
        *m2 = (double)rand()/RAND_MAX*(9.0-5.0);
        // La notación '*m1' accede al valor en la posición actual del puntero.
    }
}

/*
 * Imprime la matriz en consola, solo si su dimensión es menor a 6.
 * matrix Puntero a la matriz a imprimir.
 * D Dimensión de la matriz.
 */
void impMatrix(double *matrix, int D, int t) {
    int aux = 0;
    // La impresión se limita a matrices pequeñas para mantener el rendimiento del programa principal.
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
                    // El bucle interno (i+=D) avanza saltando D posiciones, simulando el descenso en una columna.
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

/**
 * Implementa el algoritmo estándar con optimización de punteros y paralelismo mediante OpenMP.
 *
 * mA Puntero a la matriz A.
 * mB Puntero a la matriz B.
 * mC Puntero a la matriz resultante C.
 * D Dimensión.
 */
void multiMatrix(double *mA, double *mB, double *mC, int D) {
    double Suma, *pA, *pB;
    // Directiva OpenMP: Marca el inicio de la región paralela. Un equipo de hilos es creado aquí.
    #pragma omp parallel
    {
    // Directiva OpenMP: Indica al compilador que distribuya las iteraciones del siguiente bucle for
    #pragma omp for
    // Bucle 1 (i - Filas de C): Cada hilo es responsable de un subconjunto de estas filas.
    for(int i=0; i<D; i++){
        // Bucle 2 (j - Columnas de C): Bucle interno que cada hilo ejecuta secuencialmente.
        for(int j=0; j<D; j++){
            // Inicialización de Punteros: Prepara los accesos rápidos a la memoria.
            pA = mA + i*D;      // Apunta al primer elemento de la Fila i de mA.
            pB = mB + j;        // Apunta al primer elemento de la Columna j de mB.
            Suma = 0.0;

            // Bucle 3 (k - Producto Punto)
            for(int k=0; k<D; k++, pA++, pB+=D){
                // pA++: Acceso a la fila de A.
                Suma += *pA * *pB;
            }
            // Almacena el resultado C[i][j]. No hay condiciones de carrera, cada hilo escribe en filas únicas.
            mC[i*D+j] = Suma;
        }
    }
    }
}

/**
 * @brief Multiplicación de Matrices Transpuesta Paralela (C = A x B^T).
 *
 * Versión optimizada que simula la transposición de la matriz B para convertir los accesos
 * de columna (saltos) en accesos secuenciales (óptimos para la caché de la CPU).
 *
 * @param mA Puntero a la matriz A.
 * @param mB Puntero a la matriz B (sus filas son tratadas como columnas).
 * @param mC Puntero a la matriz resultante C.
 * @param D Dimensión.
 */
void multiMatrixTrans(double *mA, double *mB, double *mC, int D) {
    double Suma, *pA, *pB;
    // Directiva OpenMP: Crea el equipo de hilos para ejecutar el bloque de código siguiente.
    #pragma omp parallel
    {
    // Directiva OpenMP: Paraleliza la distribución de las filas (bucle 'i').
    #pragma omp for
    for(int i=0; i<D; i++){
        for(int j=0; j<D; j++){
            // Inicialización de Punteros.
            pA = mA + i*D;      // Apunta a la Fila 'i' de mA.
            // CLAVE DE OPTIMIZACIÓN: pB ahora apunta a la FILA 'j' de mB.
            // Al multiplicar A x B^T, multiplicamos Fila(i) de A por Fila(j) de B, ¡ambos accesos son secuenciales!
            pB = mB + j*D;

            Suma = 0.0;
            // Bucle 3 (k - Producto Punto): Ambos punteros avanzan secuencialmente.
            for(int k=0; k<D; k++, pA++, pB++){
                // pA++ y pB++: Ambos acceden a la memoria de forma contigua.
                // Esto maximiza la eficiencia del sistema de caché de la CPU, mejorando el rendimiento.
                Suma += *pA * *pB;
            }
            mC[i * D + j] = Suma;
        }
    }
    }
}
