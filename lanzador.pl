#!/usr/bin/env perl
#######################################################################################
#* Fecha: 30 Octubre 2025
#* Autor: Xamuel Pérez Madrigal
#* Programa:
#*      agregar_estadisticas.py
#* Propósito:
#*      Lanzar múltiples programas de multiplicación de matrices
#* 	    se ejecutan los 4 experimentos 30 veces cada combinación de N y T
#*      y guardar las estadísticas en archivos .dat
######################################################################################*/
use strict;
use warnings;
use File::Path qw(make_path);

# Configuración
my @programas = ("./mmClasicaOpenMP", "./mmClasicaPosix", "./mmClasicaFork", "./mmFilasOpenMP");
my @Ns        = (64, 128, 256, 512);   # tamaños de matriz
my @hilos     = (1, 2, 4, 8);          # números de hilos a probar
my $REPS      = 30;                    # repeticiones por combinación
my $OUT_DIR   = "resultados";          # carpeta de salida
# --------------------------------

make_path($OUT_DIR) unless -d $OUT_DIR;

for my $N (@Ns) {
  for my $t (@hilos) {
    for my $prog (@programas) {

      (my $tag = $prog) =~ s{^\./}{};
      my $outfile = "$OUT_DIR/${tag}_N${N}_T${t}.dat";
      open my $FH, ">", $outfile or die "No puedo crear $outfile: $!";

      for (my $r = 1; $r <= $REPS; $r++) {
        my $cmd = "$prog $N $t";

        # Capturar stdout + stderr
        my $out = `$cmd 2>&1`;
        if ($? != 0) {
          die "Fallo ejecutando '$cmd' (status=$?):\n$out\n";
        }

        # Extraer todas las líneas que sean SOLO un número (con o sin signo/decimales)
        my @nums;
        for my $line (split /\R/, $out) {
          if ($line =~ /^\s*[+-]?\d+(?:\.\d+)?\s*$/) {
            $line =~ s/^\s+|\s+$//g;   # limpiar espacios
            push @nums, $line;
          }
        }

        # Elegir el tiempo: preferir la mayor positiva (usualmente el tiempo en µs)
        my $time_us;
        if (@nums) {
          my @positivas = grep { $_ + 0 > 0 } @nums;
          if (@positivas) {
            $time_us = $positivas[-1];
            # opcional: si varias positivas, quedarse con la mayor
            for my $v (@positivas) { $time_us = $v if $v + 0 > $time_us + 0; }
          } else {
            # si no hay positivas, usar la última numérica encontrada
            $time_us = $nums[-1];
          }
        }

        die "No pude extraer el tiempo de la salida:\n$out\n"
          unless defined $time_us && $time_us =~ /-?\d/;

        print $FH "$time_us\n";
      }

      close $FH;
      print "[OK] Generado $outfile\n";
    }
  }
}

print "\nListo. Los .dat quedaron en '$OUT_DIR/'.\n"
