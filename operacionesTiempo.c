/*#######################################################################################
 #* Fecha: 30 Octubre 2025
 #* Autor: Xamuel Pérez Madrigal
 #* Programa:
 #*      operaciones para medición de tiempo
 #* Propósito:
 #*      Implementar funciones para iniciar y finalizar la medición de tiempo.
 #######################################################################################*/
#include <stdio.h>
#include <sys/time.h>
#include "operacionesTiempo.h"

// Definición de las variables para medir el tiempo
struct timeval inicio, fin;

void InicioMuestra(){
	gettimeofday(&inicio, (void *)0);
}

void FinMuestra() {
    gettimeofday(&fin, NULL);

    long segundos = fin.tv_sec - inicio.tv_sec;
    long useg     = fin.tv_usec - inicio.tv_usec;

    if (useg < 0) {
        useg += 1000000;
        segundos -= 1;
    }

    double tiempo = (double)segundos * 1000000.0 + (double)useg;
    printf("%.0f\n", tiempo);
}
