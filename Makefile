#---------------------------------------------------------
# Makefile - Multiplicación de matrices (Fork, Posix, OpenMP)
# Autor: Xamuel Pérez Madrigal
# Fecha: 30 Octubre 2025
# Propósito:
#   Compilar los diferentes programas de multiplicación de matrices
#---------------------------------------------------------

GCC = gcc
CFLAGS = -Wall -O3 -lm
FOPENMP = -fopenmp
POSIX = -lpthread

# Ejecutables a generar
PROGRAMAS = mmClasicaFork mmClasicaPosix mmClasicaOpenMP mmFilasOpenMP

# Archivos fuente comunes
FUENTES_COMUNES = operacionesMatrices.c operacionesTiempo.c
HEADERS_COMUNES = operacionesMatrices.h operacionesTiempo.h

# Regla por defecto
all: $(PROGRAMAS)

# --- Implementación con Fork ---
mmClasicaFork: mmClasicaFork.c operacionesFork.c $(FUENTES_COMUNES) $(HEADERS_COMUNES)
	$(GCC) $(CFLAGS) $^ -o $@

# --- Implementación con POSIX threads ---
mmClasicaPosix: mmClasicaPosix.c operacionesHilos.c $(FUENTES_COMUNES) $(HEADERS_COMUNES)
	$(GCC) $(CFLAGS) $^ -o $@ $(POSIX)

# --- Implementación clásica OpenMP ---
mmClasicaOpenMP: mmClasicaOpenMP.c $(FUENTES_COMUNES) $(HEADERS_COMUNES)
	$(GCC) $(CFLAGS) $^ -o $@ $(FOPENMP)

# --- Implementación por filas con OpenMP ---
mmFilasOpenMP: mmFilasOpenMP.c $(FUENTES_COMUNES) $(HEADERS_COMUNES)
	$(GCC) $(CFLAGS) $^ -o $@ $(FOPENMP)

# Limpieza de binarios
clean:
	rm -f $(PROGRAMAS) *.o