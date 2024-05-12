# Programacion-paralela-hibrida-OpenMPI-OpenMP
Repositorio en donde se resuelven ejercicios de programación paralela con OPENMPI y OPENMP con linux en este caso ubuntu

Luego de tener la distribución debemos instalar todos los complementos necesarios
Primero actualizamos ubuntu
```
sudo apt-get update
```
E instalamos openmpi ya que openmp ya está listo en nuestra terminal
```
sudo apt-get install openmpi-bin libopenmpi-dev
```
Compilamos el ejercicio desde la terminal de la siguiente forma, el -fopenmp incluye las bibliotecas y funciones necesarias para admitir el modelo de programación paralela de OpenMP en el binario resultante
```
mpicc nombredelarchivo.c -o nombrequequeremosdarlealcompilador.1 -fopenmp
```
Y ejecutamos de la siguiente forma, -np es el numero de nucleos que tenemos disponible en nuestra terminal
```
mpirun -np 4 nombredelcompilador
```

##### Ejercicio 9
Escribir un programa paralelo con OpenMpi, que genere una matriz de tamaño 4xN en el proceso maestro y 
distribuya cada fila de la matriz en un proceso diferente. Cada proceso debe calcular el máximo, el mínimo y 
el promedio de su fila asignada y retornar los resultados al proceso maestro, quien debe reunir los datos 
recibidos en un vector de máximos, un vector de mínimos y un vector de promedios de las filas de la matriz 
original.
- Solución en el ejer9.c

##### Ejercicio 10
- 1. Escribir una variante del programa anterior (Taller 9) para una matriz de 2xNP filas (x N columnas), donde 
NP es el número de procesos activos. Similarmente el proceso 0 debe generar la matriz aleatoria, repartir 
equitativamente las filas a cada proceso. Después, cada proceso calcula el mínimo, máximo y promedio 
de sus filas asignadas, y devuelve los datos al proceso 0, quien debe reunir la información en los vectores
de mínimos, máximos y promedio de cada fila e imprimirlos.
- 2. Modifique el programa anterior para una matriz de KxNP filas (x N columnas), donde K,N son números
enteros y NP es el número de procesos.
- 3. Escribir una variante del programa anterior (punto 1) para una matriz de M filas (x N columnas), con 
M>NP, donde NP es el número de procesos activos. El proceso 0 debe generar la matriz aleatoria, y 
repartir las filas en partes iguales entre el número de procesos. Las filas sobrantes se asignan al proceso 0. 
Cada proceso calcula el mínimo, máximo y promedio de sus filas asignadas, y devuelve los datos al 
proceso 0, quien debe reunir la información en los vectores de mínimos, máximos y promedio de cada fila 
e imprimirlos.

-Solución en ejer10_1 ejer10_2 ejer10_3

##### Ejercicio 11
- 1. Escribir un programa paralelo, utilizando un enfoque híbrido. El programa debe realizar el cálculo del 
mínimo, el máximo y el promedio de los elementos de un vector. Para ello, utilizando OpenMP para un 
número NP de procesos, el proceso 0, genera un vector de números aleatorios de tamaño NPxM, (M, un 
entero cualquiera ) y reparte el vector generado entre los NP procesos. Cada proceso por su parte recibe 
los datos enviados, y los procesa localmente para determinar el máximo, el mínimo y el promedio de cada 
vector local. Para esta tarea, utiliza programación de memoria compartida mediante OpenMP, 
paralelizando el cálculo de dichos resultados en forma óptima (paralelizar el cálculo del mínimo, máximo 
y promedio con OpenMP). Finalmente, cada proceso envía los resultados al proceso 0, para que se reúnan 
y se impriman los resultados totales, es decir máximo, mínimo y promedio del vector original.
- 2. Extender el programa para que realice el cálculo anterior para cualquier valor de N > NP.
- 3. Escribir una variante del programa propuesto en los talleres 9 y 10, utilizando un enfoque híbrido de 
paralelización. Así, para una matriz de M filas (x N columnas), con M>NP, donde NP es el número de 
procesos activos se debe realizar el procesamiento en paralelo de los datos. Usando inicialmente OpenMPI
para NP procesos, el proceso 0 debe generar la matriz aleatoria, y repartir las filas en partes iguales entre 
el número de procesos. Las filas sobrantes se asignan al proceso 0. Cada proceso calcula el mínimo, 
máximo y promedio de sus filas asignadas utilizando procesamiento paralelo con OpenMP, y devuelve los 
datos al proceso 0, quien debe reunir la información en los vectores de mínimos, máximos y promedio de 
cada fila e imprimirlos.

-Solución enm ejer11_1 ejer11_2 ejer11_3

