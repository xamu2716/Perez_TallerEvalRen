#!/usr/bin/env perl
#######################################################################################
# Fecha: 30 Octubre 2025
# Autor: Xamuel Perez Madrigal, Juan Felipe Gomez Lopez, David Beltran
# Programa:
#       lanzador.pl
# Proposito:
#       Lanzar multiples programas de multiplicacion de matrices
#       se ejecutan los 4 experimentos 30 veces cada combinacion de N y T
#       y guardar las estadisticas en archivos .dat
######################################################################################
use strict;
use warnings;
use File::Path qw(make_path);

# Configuracion
my @programas = ("./mmClasicaOpenMP", "./mmClasicaPosix", "./mmClasicaFork", "./mmFilasOpenMP");
my @Ns        = (256, 512, 1024, 2048);
my @hilos     = (1, 2, 4, 8);
my $REPS      = 30;
my $OUT_DIR   = "resultados";
# --------------------------------

# Crea la carpeta de salida si no existe.
make_path($OUT_DIR) unless -d $OUT_DIR;

# Bucle principal: Itera sobre todos los tamanos de matriz (N)
for my $N (@Ns) {
  # Bucle anidado: Itera sobre el numero de hilos (T)
  for my $t (@hilos) {
    # Bucle anidado: Itera sobre cada uno de los 4 programas
    for my $prog (@programas) {

      # 1. Generacion del nombre del archivo de salida
      (my $tag = $prog) =~ s{^\./}{};
      my $outfile = "$OUT_DIR/${tag}_N${N}_T${t}.dat";
      open my $FH, ">", $outfile or die "No puedo crear $outfile: $!";

      # 2. Bucle de Repeticiones
      for (my $r = 1; $r <= $REPS; $r++) {
        my $cmd = "$prog $N $t";

        # Capturar stdout + stderr
        my $out = `$cmd 2>&1`;
        if ($? != 0) {
          die "Fallo ejecutando '$cmd' (status=$?):\n$out\n";
        }

        # 3. Extraccion del tiempo de ejecucion
        my @nums;
        for my $line (split /\R/, $out) {
          # Busca lineas que contengan solo un numero (entero o decimal)
          if ($line =~ /^\s*[+-]?\d+(?:\.\d+)?\s*$/) {
            $line =~ s/^\s+|\s+$//g;
            push @nums, $line;
          }
        }

        # 4. Seleccion del tiempo
        my $time_us;
        if (@nums) {
          # Filtra para quedarse solo con los numeros positivos
          my @positivas = grep { $_ + 0 > 0 } @nums;
          if (@positivas) {
            $time_us = $positivas[-1];
            for my $v (@positivas) { $time_us = $v if $v + 0 > $time_us + 0; }
          } else {
            $time_us = $nums[-1];
          }
        }

        die "No pude extraer el tiempo de la salida:\n$out\n"
          unless defined $time_us && $time_us =~ /-?\d/;

        print $FH "$time_us\n";
      }

      # 5. Finalizacion
      close $FH;
      print "[OK] Generado $outfile\n";
    }
  }
}

print "\nListo. Los .dat quedaron en '$OUT_DIR/'.\n";
