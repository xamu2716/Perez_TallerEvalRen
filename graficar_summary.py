#!/usr/bin/env python3
#######################################################################################
#* Fecha: 30 Octubre 2025
#* Autor: Xamuel Pérez Madrigal
#* Programa:
#*      agregar_estadisticas.py
#* Propósito:
#*      Graficar estadísticas desde summary.csv generado por agregar_estadisticas.py
#*      Genera gráficos de barras de tiempo vs número de hilos para cada N.
######################################################################################*/
import argparse
from pathlib import Path
from typing import Literal

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def load_summary(csv_path: Path) -> pd.DataFrame:
    df = pd.read_csv(csv_path)
    required = {"N","hilos","prog","mean_us","sigma_us","n"}
    missing = required - set(df.columns)
    if missing:
        raise ValueError(f"Faltan columnas en el CSV: {sorted(missing)}")
    df["N"] = df["N"].astype(int)
    df["hilos"] = df["hilos"].astype(int)
    # µs -> s para graficar
    df["mean_s"]  = df["mean_us"]  / 1_000_000.0
    df["sigma_s"] = df["sigma_us"] / 1_000_000.0
    return df.sort_values(["N","hilos","prog"]).reset_index(drop=True)

def plot_for_N(df: pd.DataFrame, N: int, out_dir: Path, yscale: Literal["linear","log"]="linear") -> bool:
    sub = df[df["N"] == N].copy()
    if sub.empty:
        return False

    means = sub.pivot(index="hilos", columns="prog", values="mean_s").sort_index()
    sigmas = sub.pivot(index="hilos", columns="prog", values="sigma_s").reindex(index=means.index, columns=means.columns)

    x = np.arange(len(means.index))
    progs = list(means.columns)
    width = 0.8 / max(1, len(progs))

    fig, ax = plt.subplots()

    for i, p in enumerate(progs):
        y = means[p].values.astype(float)
        yerr = sigmas[p].values.astype(float)

        # --- Recortar errorbars para no ir por debajo de 0 ---
        yerr_minus = np.minimum(y, yerr)  # si yerr > y, baja hasta 0 como mínimo
        yerr_plus  = yerr
        yerr_asym  = np.vstack([yerr_minus, yerr_plus])

        ax.bar(x + (i - (len(progs)-1)/2) * width, y, width, yerr=yerr_asym, label=p)

    ax.set_title(f"Tiempo vs #Hilos (N={N})  [{yscale}]")
    ax.set_xlabel("# Hilos")
    ax.set_ylabel("Tiempo (s)")
    ax.set_xticks(x)
    ax.set_xticklabels([str(h) for h in means.index])
    ax.set_yscale(yscale)
    ax.legend()
    fig.tight_layout()

    out_dir.mkdir(parents=True, exist_ok=True)
    out_path = out_dir / f"tiempo_vs_hilos_N{N}_{yscale}.png"
    fig.savefig(out_path, dpi=150)
    plt.close(fig)
    print(f"[OK] {out_path}")
    return True

def main():
    ap = argparse.ArgumentParser(description="Graficar X=hilos, Y=mean_s (seg), 4 barras=prog por N, lineal+log, yerr recortado en 0")
    ap.add_argument("-i","--input", type=Path, default=Path("resultados/summary.csv"), help="Ruta del summary.csv")
    ap.add_argument("-o","--out_dir", type=Path, default=Path("resultados/plots"), help="Directorio de salida")
    ap.add_argument("--solo", nargs="*", type=int, help="Opcional: graficar solo estos N (ej: --solo 64 256)")
    args = ap.parse_args()

    df = load_summary(args.input)
    Ns = sorted(df["N"].unique()) if not args.solo else args.solo

    ok = False
    for N in Ns:
        ok |= plot_for_N(df, int(N), args.out_dir, yscale="linear")
        ok |= plot_for_N(df, int(N), args.out_dir, yscale="log")
    if not ok:
        print("[WARN] No se generaron gráficos.")

if __name__ == "__main__":
    main()
