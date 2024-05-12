/*
Universidad de Pamplona
Facultad de Ingenierías y Arquitectura
Programa de Ingeniería de Sistemas
Asignatura: Fundamentos de Computación paralela y distribuida
Profesor: Jose Orlando Maldonado Bautista
*/


/*
En este programa, se genera una matriz de tamaño 4xN en el proceso maestro y distribuye cada fila de la matriz en un proceso diferente. Cada proceso debe calcular el máximo, el mínimo y el promedio de su fila asignada y retornar los resultados al proceso maestro, quien los debe reunir en un vector de máximos, un vector de mínimos y y un vector de promedios de las filas de la matriz original.

*/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

double promVector(int v[], int n)
{
   int suma = 0;
   for(int i=0;i<n;i++)
    	suma+= v[i];
   return suma/(double)(n);
}

int maxVector(int v[], int n)
{
   int max = v[0];
   for(int i=1;i<n;i++)
   	if (v[i]>max) max = v[i];
    	
   return max;
}

int minVector(int v[], int n)
{
   int min = v[0];
   for(int i=1;i<n;i++)
   	if (v[i]<min) min = v[i];
    	
   return min;
}

void imprimirMatriz(int M[], int fil, int col)
{
   for(int i=0;i<fil;i++)
   {
       for(int j=0;j<col;j++)
          printf("%d ",M[i*col+j]);
       printf("\n");
   }       
} 

void imprimirVector(int V[], int tam)
{
   for(int i=0;i<tam;i++)
      printf("%d ",V[i]);
}

void imprimirVectorReal(double V[], int tam)
{
   for(int i=0;i<tam;i++)
      printf("%f ",V[i]);
}

int main(int argc, char *argv[])
{
    int rank, size, *matriz, *fila;
    MPI_Status estado;
    // Se inicia el entorno de comunicación MPI
    MPI_Init(&argc, &argv); 
    // Se obtiene el total de procesos
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Se obtiene el identificador de cada proceso
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // variables de dimensiones de la matriz
    int fil = size;  // El número de filas coincide con el numero de procesos, por defecto puede ser 4.
    int col = 5;  // Este valor puede variarse, para verificar iniciamos con un numero pequeño
    int tam = fil * col;  // El tamaño de la matriz

    if (rank == 0)
    {
       srand(time(NULL));
       matriz = (int *)  malloc (tam * sizeof(int));
       for(int i=0;i<fil;i++)
          for(int j=0;j<col;j++)
             matriz[i*col+j]= rand()%80 + 10;
             
       imprimirMatriz(matriz,fil,col);
    }
     
    // Asignamos memoria para el vector local donde se va a guardar la fila que corresponda a cada proceso 
    fila = (int *)  malloc (col * sizeof(int));
     // Ahora se distribuyen las filas entre los procesos mediante MPI_scatter
    MPI_Scatter(matriz, col, MPI_INT, fila, col, MPI_INT, 0, MPI_COMM_WORLD);
    
   // En este bloque, cada proceso calcula su parte, es decir el min, max y prom local
    int min = minVector(fila,col);
    int max = maxVector(fila,col);
    double prom = promVector(fila,col);
    
     // Asignamos memoria para el vector de minimos
    int *minimos;
    minimos = (int * ) malloc ( fil * sizeof(int));
    
    // Ahora enviamos cada dato al proceso maestro quien los reune en un vector de minimos, usamos MPI_Gather
    MPI_Gather(&min, 1, MPI_INT, minimos, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Asignamos memoria para el vector de máximos
    int *maximos;
    maximos = (int * ) malloc ( fil * sizeof(int));
    // Ahora enviamos cada dato al proceso maestro quien los reune en un vector de máximo, usando MPI_Gather
    MPI_Gather(&max, 1, MPI_INT, maximos, 1, MPI_INT, 0, MPI_COMM_WORLD);
 
    // Asignamos memoria para el vector de promedios (completar)
    double *promedios;
    promedios = (double * ) malloc ( fil * sizeof(double));
    
    // Ahora enviamos cada dato al proceso maestro quien los reune en un vector de PROMEDIOS, usando MPI_Gather
    MPI_Gather(&prom, 1, MPI_DOUBLE, promedios, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
   // Para verificar el proceo 0 debe mostrar lo que recibió.
    if (rank==0)
    {
       printf(" \n Vector de mínimos: \n");           
       imprimirVector(minimos,fil);
       printf(" \n ");
       
       printf(" \n Vector de maximos: \n");           
       imprimirVector(maximos,fil);
       printf(" \n ");
       
       printf(" \n Vector de promedios: \n");           
       imprimirVectorReal(promedios,fil);
       printf(" \n ");
    }
    
    MPI_Finalize();
}

