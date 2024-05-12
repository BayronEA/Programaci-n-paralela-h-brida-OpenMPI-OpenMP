/*
Universidad de Pamplona
Facultad de Ingenierías y Arquitectura
Programa de Ingeniería de Sistemas
Asignatura: Fundamentos de Computación paralela y distribuida
Profesor: Jose Orlando Maldonado Bautista


Escribir una variante del programa propuesto en los talleres 9 y 10, utilizando un enfoque híbrido de
paralelización. Así, para una matriz de M filas (x N columnas), con M>NP, donde NP es el número de
procesos activos se debe realizar el procesamiento en paralelo de los datos. Usando inicialmente OpenMPI
para NP procesos, el proceso 0 debe generar la matriz aleatoria, y repartir las filas en partes iguales entre
el número de procesos. Las filas sobrantes se asignan al proceso 0. Cada proceso calcula el mínimo,
máximo y promedio de sus filas asignadas utilizando procesamiento paralelo con OpenMP, y devuelve los
datos al proceso 0, quien debe reunir la información en los vectores de mínimos, máximos y promedio de
cada fila e imprimirlos.

*/
#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double promVector(int v[], int n) {
    int suma = 0;
    #pragma omp parallel for reduction(+:suma)
    for (int i = 0; i < n; i++)
        suma += v[i];
    return suma / (double)(n);
}

int maxVector(int v[], int n) {
    int max = v[0];
    #pragma omp parallel for reduction(max:max)
    for (int i = 1; i < n; i++)
        if (v[i] > max) max = v[i];
    return max;
}

int minVector(int v[], int n) {
    int min = v[0];
    #pragma omp parallel for reduction(min:min)
    for (int i = 1; i < n; i++)
        if (v[i] < min) min = v[i];
    return min;
}

void imprimirMatriz(int M[], int fil, int col) {
    for (int i = 0; i < fil; i++) {
        for (int j = 0; j < col; j++)
            printf("%d ", M[i * col + j]);
        printf("\n");
    }
}

void imprimirVector(int V[], int tam) {
    for (int i = 0; i < tam; i++)
        printf("%d ", V[i]);
}

void imprimirVectorReal(double V[], int tam) {
    for (int i = 0; i < tam; i++)
        printf("%f ", V[i]);
}

int main(int argc, char *argv[]) {
    int rank, size, *matriz, *filas, *extra_filas = NULL;
    MPI_Status estado;
    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int M = 10; // Número total de filas en la matriz
    int N = 5; // Número de columnas

    int K = M / size; // Número de filas por proceso
    int filas_totales = M; // Número total de filas en la matriz
    int tam_matriz = M * N; // Tamaño total de la matriz

    if (rank == 0) {
        srand(time(NULL));
        matriz = (int *)malloc(tam_matriz * sizeof(int));
        for (int i = 0; i < tam_matriz; i++)
            matriz[i] = rand() % 80 + 10;

        printf("Matriz generada por el proceso maestro:\n");
        imprimirMatriz(matriz, filas_totales, N);
        printf("\n");

        // Calcular las filas extras que no se distribuyeron inicialmente
        int extra_filas_count = M % size;
        if (extra_filas_count > 0) {
            extra_filas = (int *)malloc(N * extra_filas_count * sizeof(int));
            for (int i = 0; i < extra_filas_count; i++) {
                for (int j = 0; j < N; j++) {
                    extra_filas[i * N + j] = matriz[(K * size + i) * N + j];
                }
            }
        }
    }

    filas = (int *)malloc(K * N * sizeof(int));

    // Distribuir las filas de la matriz a los procesos
    MPI_Scatter(matriz, K * N, MPI_INT, filas, K * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada proceso calcula el mínimo, el máximo y el promedio de sus filas asignadas
    int min = minVector(filas, K * N);
    int max = maxVector(filas, K * N);
    double prom = promVector(filas, K * N);

    // Recolectar los resultados de las filas asignadas en el proceso maestro
    int *minimos = (int *)malloc(size * sizeof(int));
    MPI_Gather(&min, 1, MPI_INT, minimos, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    int *maximos = (int *)malloc(size * sizeof(int));
    MPI_Gather(&max, 1, MPI_INT, maximos, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    double *promedios = (double *)malloc(size * sizeof(double));
    MPI_Gather(&prom, 1, MPI_DOUBLE, promedios, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Proceso maestro procesa las filas extras
    if (rank == 0 && extra_filas != NULL) {
        int min_extra = minVector(extra_filas, N * (M % size));
        int max_extra = maxVector(extra_filas, N * (M % size));
        double prom_extra = promVector(extra_filas, N * (M % size));

        printf("Resultados de filas extras procesadas por el proceso maestro:\n");
        printf("Mínimo: %d\n", min_extra);
        printf("Máximo: %d\n", max_extra);
        printf("Promedio: %f\n", prom_extra);
    }

    // El proceso 0 imprime los resultados
    if (rank == 0) {
        printf("Resultados de las filas asignadas:\n");
        printf("Vector de mínimos:\n");
        imprimirVector(minimos, size);
        printf("\n");

        printf("Vector de máximos:\n");
        imprimirVector(maximos, size);
        printf("\n");

        printf("Vector de promedios:\n");
        imprimirVectorReal(promedios, size);
        printf("\n");
    }
    MPI_Finalize();
}

