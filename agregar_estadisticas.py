#!/usr/bin/env python3
#######################################################################################
#* Fecha: 30 Octubre 2025
#* Autor: Xamuel Pérez Madrigal, Juan Felipe Gómez López
#* Programa:
#* agregar_estadisticas.py
#* Propósito:
#* Agrega estadísticas (media y desviación estándar) desde archivos .dat
#* y genera un archivo summary.csv con los resultados.
######################################################################################*/

import argparse # definir y manejar argumentos de línea de comandos.
import csv      # leer y escribir archivos CSV 
import math     # para funciones matemáticas, usado aquí para la raíz cuadrada
import re       # para expresiones regulares wn este caso regex, usado para parsear nombres de archivos.
from pathlib import Path # Clase para manejar rutas de archivos 
from typing import Dict, List, Tuple # Módulos para indicar los tipos de datos esperados.

# Definición de Expresión Regular para el nombre de archivo.
# Esta regex extrae el nombre del programa, la dimensión N y el número de hilos T.
# Espera archivos con el formato: <prog>_N<N>_T<hilos>.dat
# Ejemplo: mmClasicaOpenMP_N256_T4.dat
FILENAME_RE = re.compile(r'^(?P<prog>.+)_N(?P<N>\d+)_T(?P<hilos>\d+)\.dat$')

def read_times(dat_path: Path) -> List[float]:
    """
    Lee los tiempos de ejecución desde un archivo .dat.
    Se espera que cada tiempo esté en una línea separada.

    recibe el dat_path: Ruta del archivo .dat a leer
    retorna una lista de floats leídos del archivo.
    """
    times = []
    # Abre el archivo en modo lectura ('r')
    with dat_path.open("r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            line = line.strip() # Limpia espacios en blanco al inicio y final de la línea.
            if not line:
                continue # Ignora líneas vacías.
            try:
                # Intenta convertir la línea a un número float.
                times.append(float(line))
            except ValueError:
                # Si la línea no es un número válido (ej. encabezado de columna o basura), la ignora.
                continue
    return times

def mean_and_sigma(values: List[float]) -> Tuple[float, float]:
    """
    Calcula la media y la desviación estándar muestral de una lista de valores.

    recibe una lista de floats que son los tiempos de ejecución.

    Retorna una tupla con la media y la desviación estándar
    """
    n = len(values)
    if n == 0:
        # Retorna Not a Number (NaN) si no hay valores.
        return float("nan"), float("nan")

    # Cálculo de la Media: Suma de valores / Número de valores.
    mu = sum(values) / n

    if n == 1:
        # La desviación estándar es 0.0 si solo hay una muestra.
        return mu, 0.0

    # Cálculo de la Varianza Muestral: Suma de (x - mu)^2 / (n - 1)
    # Se usa (n - 1) en el denominador para obtener la desviación estándar muestral.
    var = sum((x - mu) ** 2 for x in values) / (n - 1)
    
    # La desviación estándar es la raíz cuadrada de la Varianza.
    return mu, math.sqrt(var) 

def aggregate(input_dir: Path, output_csv: Path) -> Dict[int, Dict[int, Dict[str, Tuple[float, float, int]]]]:
    """
    Procesa todos los archivos .dat en el directorio de entrada, calcula las estadísticas
    y guarda el resumen en un archivo CSV.

    recibe input_dir: Ruta del directorio que contiene los archivos *.dat.
    y output_csv: Ruta del archivo summary.csv de salida.

    Retorna un diccionario anidado con los resultados agregados.
    formato: results[N][hilos][prog] = (mean_us, sigma_us, n_samples)
    """
    # Inicialización del diccionario que almacenará los resultados jerárquicamente.
    results: Dict[int, Dict[int, Dict[str, Tuple[float, float, int]]]] = {}
    rows = [] # Lista temporal para construir las filas del CSV antes de escribirlas.

    # Itera sobre todos los archivos que terminan en ".dat" dentro del directorio de entrada.
    for path in input_dir.glob("*.dat"):
      m = FILENAME_RE.match(path.name) # Intenta hacer coincidir el nombre del archivo con la regex.
      if not m:
          continue # Ignora archivos que no cumplen con el patrón esperado.

      # Extrae los grupos nombrados de la expresión regular:
      prog = m.group("prog")       # Nombre del programa (ej: mmClasicaOpenMP)
      N = int(m.group("N"))        # Dimensión de la matriz (N)
      hilos = int(m.group("hilos")) # Número de hilos (T)

      times = read_times(path) # Lee los tiempos del archivo .dat.

      if not times:
          continue # Ignora archivos que no contienen datos válidos.

      mu, sigma = mean_and_sigma(times) # Calcula la media y desviación estándar.
      results.setdefault(N, {}).setdefault(hilos, {})[prog] = (mu, sigma, len(times))

      # Añade la fila a la lista que será escrita en el CSV. Se usa f-string para formatear sin decimales.
      rows.append([N, hilos, prog, f"{mu:.0f}", f"{sigma:.0f}", len(times)])

    # Ordena las filas antes de escribir el CSV. Criterio de ordenación:
    # 1. Dimensión N.
    # 2. Número de hilos.
    # 3. Nombre del programa (alfabético).
    rows.sort(key=lambda r: (int(r[0]), int(r[1]), str(r[2])))

    # Asegura que el directorio de salida exista.
    output_csv.parent.mkdir(parents=True, exist_ok=True)

    # Escribe el archivo CSV de salida.
    with output_csv.open("w", newline="") as f:
        w = csv.writer(f)
        # Escribe el encabezado del CSV.
        w.writerow(["N", "hilos", "prog", "mean_us", "sigma_us", "n"])
        # Escribe las filas de datos ordenadas.
        for row in rows:
            w.writerow(row)

    return results

def main():
    """
    Función principal del script: Define argumentos de línea de comandos, parsea la entrada,
    ejecuta la agregación de estadísticas e informa al usuario sobre el resultado.
    """
    # Configura el parser para manejar argumentos.
    parser = argparse.ArgumentParser(description="Agrega promedio y sigma desde archivos .dat y genera summary.csv")

    # Argumento --input_dir (-i): Ruta del directorio de entrada.
    parser.add_argument("--input_dir", "-i", type=Path, default=Path("resultados"),
                         help="Directorio con archivos *.dat que siguen el patrón: <prog>_N<N>_T<hilos>.dat")

    # Argumento --output (-o): Ruta del archivo de salida CSV.
    parser.add_argument("--output", "-o", type=Path, default=Path("resultados/summary.csv"),
                         help="Ruta completa donde se guardará el archivo CSV de resumen.")
    args = parser.parse_args()

    # Llama a la función principal de procesamiento de datos. El resultado retornado se ignora (_).
    _ = aggregate(args.input_dir, args.output)

    # Mensaje de finalización.
    print(f"[DONE] Resumen guardado en: {args.output}")

if __name__ == "__main__":
    # La ejecución comienza aquí cuando el script es llamado directamente.
    main()
