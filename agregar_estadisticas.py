#!/usr/bin/env python3
#######################################################################################
#* Fecha: 30 Octubre 2025
#* Autor: Xamuel Pérez Madrigal
#* Programa:
#*      agregar_estadisticas.py
#* Propósito:
#*      Agrega estadísticas (media y desviación estándar) desde archivos .dat
#*      y genera un archivo summary.csv con los resultados.
######################################################################################*/
import argparse
import csv
import math
import re
from pathlib import Path
from typing import Dict, List, Tuple

# Espera archivos: <prog>_N<N>_T<hilos>.dat
# Ej: mmClasicaOpenMP_N256_T4.dat
FILENAME_RE = re.compile(r'^(?P<prog>.+)_N(?P<N>\d+)_T(?P<hilos>\d+)\.dat$')

def read_times(dat_path: Path) -> List[float]:
    times = []
    with dat_path.open("r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            try:
                times.append(float(line))
            except ValueError:
                # Ignora líneas no numéricas
                continue
    return times

def mean_and_sigma(values: List[float]) -> Tuple[float, float]:
    n = len(values)
    if n == 0:
        return float("nan"), float("nan")
    mu = sum(values) / n
    if n == 1:
        return mu, 0.0
    var = sum((x - mu) ** 2 for x in values) / (n - 1)  # desviación muestral (n-1)
    return mu, math.sqrt(var)

def aggregate(input_dir: Path, output_csv: Path) -> Dict[int, Dict[int, Dict[str, Tuple[float, float, int]]]]:
    """
    Devuelve: results[N][hilos][prog] = (mean_us, sigma_us, n)
    Escribe CSV aplanado con columnas: N,hilos,prog,mean_us,sigma_us,n
    """
    results: Dict[int, Dict[int, Dict[str, Tuple[float, float, int]]]] = {}
    rows = []
    for path in input_dir.glob("*.dat"):
      m = FILENAME_RE.match(path.name)
      if not m:
          continue
      prog = m.group("prog")
      N = int(m.group("N"))
      hilos = int(m.group("hilos"))
      times = read_times(path)
      if not times:
          continue
      mu, sigma = mean_and_sigma(times)
      results.setdefault(N, {}).setdefault(hilos, {})[prog] = (mu, sigma, len(times))
      rows.append([N, hilos, prog, f"{mu:.0f}", f"{sigma:.0f}", len(times)])

    rows.sort(key=lambda r: (int(r[0]), int(r[1]), str(r[2])))

    output_csv.parent.mkdir(parents=True, exist_ok=True)
    with output_csv.open("w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["N", "hilos", "prog", "mean_us", "sigma_us", "n"])
        for row in rows:
            w.writerow(row)
    return results

def main():
    parser = argparse.ArgumentParser(description="Agrega promedio y sigma desde archivos .dat y genera summary.csv")
    parser.add_argument("--input_dir", "-i", type=Path, default=Path("resultados"),
                        help="Dir con *.dat: <prog>_N<N>_T<hilos>.dat")
    parser.add_argument("--output", "-o", type=Path, default=Path("resultados/summary.csv"),
                        help="Ruta del CSV de salida")
    args = parser.parse_args()

    _ = aggregate(args.input_dir, args.output)
    print(f"[DONE] Resumen guardado en: {args.output}")

if __name__ == "__main__":
    main()
