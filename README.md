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

Ejercicio 9
Escribir un programa paralelo con OpenMpi, que genere una matriz de tamaño 4xN en el proceso maestro y 
distribuya cada fila de la matriz en un proceso diferente. Cada proceso debe calcular el máximo, el mínimo y 
el promedio de su fila asignada y retornar los resultados al proceso maestro, quien debe reunir los datos 
recibidos en un vector de máximos, un vector de mínimos y un vector de promedios de las filas de la matriz 
original.
Solución en el ejer9.c
