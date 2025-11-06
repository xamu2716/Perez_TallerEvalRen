#!/usr/bin/env python3
#######################################################################################
#* Fecha: 30 Octubre 2025
#* Autor: Xamuel Pérez Madrigal
#* Programa:
#*      graficar_summary.py
#* Propósito:
#*      Graficar estadísticas desde summary.csv generado por agregar_estadisticas.py
#*      Genera gráficos de barras de tiempo vs número de hilos para cada N (solo escala lineal).
######################################################################################*/
import argparse
from pathlib import Path

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

    # Sanitizar: reemplazar NaN y forzar no-negativos
    df["mean_s"]  = np.nan_to_num(df["mean_s"],  nan=0.0, posinf=None, neginf=0.0)
    df["sigma_s"] = np.nan_to_num(df["sigma_s"], nan=0.0, posinf=None, neginf=0.0)

    df["mean_s"]  = np.clip(df["mean_s"],  0, None)
    df["sigma_s"] = np.clip(df["sigma_s"], 0, None)

    return df.sort_values(["N","hilos","prog"]).reset_index(drop=True)

def plot_for_N(df: pd.DataFrame, N: int, out_dir: Path) -> bool:
    sub = df[df["N"] == N].copy()
    if sub.empty:
        return False

    means = sub.pivot(index="hilos", columns="prog", values="mean_s").sort_index()
    sigmas = sub.pivot(index="hilos", columns="prog", values="sigma_s").reindex(index=means.index, columns=means.columns)

    # Rellenar posibles NaN tras el pivot
    means = means.fillna(0.0)
    sigmas = sigmas.fillna(0.0)

    # Arrays base
    x = np.arange(len(means.index))
    progs = list(means.columns)
    width = 0.8 / max(1, len(progs))

    fig, ax = plt.subplots()

    for i, p in enumerate(progs):
        y    = means[p].to_numpy(dtype=float)
        yerr = sigmas[p].to_numpy(dtype=float)

        # Forzar no-negativos (por seguridad extra)
        y    = np.clip(np.nan_to_num(y,    nan=0.0), 0, None)
        yerr = np.clip(np.nan_to_num(yerr, nan=0.0), 0, None)

        # Recortar barra de error inferior a 0 (si sigma > mean, baja hasta 0)
        yerr_lower = np.minimum(y, yerr)   # cuánto puede bajar como máximo
        yerr_upper = yerr                  # cuánto puede subir
        yerr_asym  = np.vstack([yerr_lower, yerr_upper])

        ax.bar(x + (i - (len(progs)-1)/2) * width, y, width, yerr=yerr_asym, label=p)

    ax.set_title(f"Tiempo vs #Hilos (N={N})")
    ax.set_xlabel("# Hilos")
    ax.set_ylabel("Tiempo (s)")
    ax.set_xticks(x)
    ax.set_xticklabels([str(h) for h in means.index])
    ax.set_yscale("linear")
    ax.legend()
    fig.tight_layout()

    out_dir.mkdir(parents=True, exist_ok=True)
    out_path = out_dir / f"tiempo_vs_hilos_N{N}_linear.png"
    fig.savefig(out_path, dpi=150)
    plt.close(fig)
    print(f"[OK] {out_path}")
    return True

def main():
    ap = argparse.ArgumentParser(description="Graficar X=hilos, Y=mean_s (seg), 4 barras=prog por N (solo escala lineal)")
    ap.add_argument("-i","--input", type=Path, default=Path("resultados/summary.csv"), help="Ruta del summary.csv")
    ap.add_argument("-o","--out_dir", type=Path, default=Path("resultados/plots"), help="Directorio de salida")
    ap.add_argument("--solo", nargs="*", type=int, help="Opcional: graficar solo estos N (ej: --solo 64 256)")
    args = ap.parse_args()

    df = load_summary(args.input)
    Ns = sorted(df["N"].unique()) if not args.solo else args.solo

    ok = False
    for N in Ns:
        ok |= plot_for_N(df, int(N), args.out_dir)
    if not ok:
        print("[WARN] No se generaron gráficos.")

if __name__ == "__main__":
    main()
