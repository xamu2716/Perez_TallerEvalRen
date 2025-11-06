#!/usr/bin/perl
#**************************************************************
#         		Pontificia Universidad Javeriana
#     Autor: David Esteban Beltrán Gómez
#     Fecha: 30 Octubre 2025
#     Materia: Sistemas Operativos
#     Tema: Taller de Evaluación de Rendimiento
#     Fichero: script automatización ejecución por lotes 
#****************************************************************/

use strict;
use warnings;

# Declaración explícita de variables (requerido por 'use strict')
my $Path;
my $Nombre_Ejecutable;
my @Size_Matriz;
my @Num_Hilos;
my $Repeticiones;
my $Directorio_Resultados;
my $file;
my $size;
my $hilo;
my $i;
my $p = 0;

# Se Obtiene el path del directorio actual
$Path = `pwd`;
# Si existe un salto de linea en el path se elimina
chomp($Path);

# Si se pasa un argumento, se usa como nombre del ejecutable; si no, se pide uno por defecto
if (@ARGV) {
	$Nombre_Ejecutable = $ARGV[0];
} else {
	print "No se recibió un nombre de ejecutable.\n";
	print "Usando el valor por defecto: mmClasicaOpenMP\n";
	$Nombre_Ejecutable = "mmClasicaOpenMP";
}

# Tamaños de las matrices a usar en un "Array"
@Size_Matriz = ("2","10","20");
# Cantidad de hilos con los que se ejecutara el programa en un "Array"
@Num_Hilos = (1,2,4,8,16,20);
# Cantidad de veces que se ejecutarán las combinaciones
$Repeticiones = 4;

# Crear carpeta de resultados específicos para el ejecutable si no existe
$Directorio_Resultados = "$Path/Resultados_$Nombre_Ejecutable";
mkdir $Directorio_Resultados unless -d $Directorio_Resultados;

print "\n\n   INICIO DE EJECUCIONES AUTOMATIZADAS\n";
print "   Ejecutable: $Nombre_Ejecutable\n";
print "   Resultados se guardarán en: $Directorio_Resultados\n";

# Primer ciclo que itera en los tamaños de matriz que se definieron
foreach $size (@Size_Matriz){

	# --- agregado ---
	print "\n------------------------------------------------------------\n";
	print " Ejecutando pruebas para MATRIZ de tamaño $size x $size\n";
	print "------------------------------------------------------------\n";

	# Segundo ciclo anindado que itera en los hilos que se definieron
	foreach $hilo (@Num_Hilos) {

		# --- agregado ---
		print "\n>> Configuración actual: $hilo hilo(s) | Tamaño matriz: $size x $size\n";

		# Genera el archivo de salida referente a la combinación de Matrices e Hilos
		$file = "$Directorio_Resultados/$Nombre_Ejecutable-".$size."-Hilos-".$hilo.".dat";

		# Tercer ciclo anidado que realiza las repeticiones indicadas
		for ($i=0; $i<$Repeticiones; $i++) {

			# Ejecuta el programa y lo que genere se guarda en el archivo
			system("$Path/$Nombre_Ejecutable $size $hilo >> $file");

			# Verificar si se ejecuto todo bien porque si no la funcion system() retorna un valor !=0
			if ($? != 0) {
    			print "\n Hubo un error al ejecutar: $Nombre_Ejecutable $size $hilo\n";
			} else {
				# --- agregado ---
				print "\n Ejecución repetición ".($i+1)." completada exitosamente.\n";
			}

			# Mostrar el comando ejecutado
			printf("  Comando: $Path/$Nombre_Ejecutable $size $hilo\n");
			
			# Al completar otra ejecucion aumentar $p
        	$p++;
		}

		# Mostrar donde se guardaron los resultados
		print "  ➤ Resultados guardados en: $file\n";
	}
}

# Mensaje final
print "\n\n   TODAS LAS EJECUCIONES FINALIZADAS EXITOSAMENTE\n";
print "   Total de ejecuciones realizadas: $p\n";
print "   Archivos generados en: $Directorio_Resultados\n";
