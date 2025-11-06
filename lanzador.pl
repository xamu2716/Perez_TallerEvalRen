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
        my $out = `$cmd`;
        if ($? != 0) {
          die "Fallo ejecutando '$cmd' (status=$?):\n$out\n";
        }
        # Extrae el ÚLTIMO número de la salida (tiempo en microsegundos)
        my ($time_us) = ($out =~ /(\d+(?:\.\d+)?)(?!.*\d)/);
        die "No pude extraer el tiempo de la salida:\n$out\n" unless defined $time_us;

        print $FH "$time_us\n";
      }

      close $FH;
      print "[OK] Generado $outfile\n";
    }
  }
}

print "\nListo. Los .dat quedaron en '$OUT_DIR/'.\n"


