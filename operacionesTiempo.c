/*#######################################################################################
 #* Fecha: 30 Octubre 2025
 #* Autor: Xamuel Pérez Madrigal, Juan Felipe Gómez López, David Esteban Beltrán
 #* Programa:
 #*      operaciones para medición de tiempo
 #* Propósito:
 #*      Implementar funciones para iniciar y finalizar la medición de tiempo.
 #######################################################################################*/
#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#include "operacionesTiempo.h"

static struct timespec t0, t1;

void InicioMuestra(void){
    clock_gettime(CLOCK_MONOTONIC, &t0);
}

void FinMuestra(void){
    clock_gettime(CLOCK_MONOTONIC, &t1);
    long sec  = t1.tv_sec  - t0.tv_sec;
    long nsec = t1.tv_nsec - t0.tv_nsec;
    if (nsec < 0) { nsec += 1000000000L; sec -= 1; }
    if (sec < 0) { sec = 0; nsec = 0; }
    double us = (double)sec * 1000000.0 + (double)(nsec / 1000);
    printf("%.0f\n", us);
}

