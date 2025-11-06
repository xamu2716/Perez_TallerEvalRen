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

void FinMuestra(){
	gettimeofday(&fin, (void *)0);
	// Calcula la diferencia en microsegundos
	fin.tv_usec -= inicio.tv_usec;
	fin.tv_sec  -= inicio.tv_sec;
	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec); 
	// Imprime el tiempo que ha pasado en microsegundos
	printf("%9.0f \n", tiempo);
}