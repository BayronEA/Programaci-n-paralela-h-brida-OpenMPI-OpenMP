/*
Universidad de Pamplona
Facultad de Ingenierías y Arquitectura
Programa de Ingeniería de Sistemas
Asignatura: Fundamentos de Computación paralela y distribuida
Profesor: Jose Orlando Maldonado Bautista


Extender el programa para que realice el cálculo anterior para cualquier valor de N > NP.
*/

#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double promedioVector(int v[], int n) {
    int suma = 0;
    #pragma omp parallel for reduction(+:suma)
    for (int i = 0; i < n; i++)
        suma += v[i];
    return suma / (double)(n);
}

int maximoVector(int v[], int n) {
    int max = v[0];
    #pragma omp parallel for reduction(max:max)
    for (int i = 1; i < n; i++)
        if (v[i] > max)
            max = v[i];
    return max;
}

int minimoVector(int v[], int n) {
    int min = v[0];
    #pragma omp parallel for reduction(min:min)
    for (int i = 1; i < n; i++)
        if (v[i] < min)
            min = v[i];
    return min;
}

void imprimirVector(int V[], int tam) {
    for (int i = 0; i < tam; i++)
        printf("%d ", V[i]);
}

int main(int argc, char *argv[]) {
    int rank, size, *vector, *subvector, *extra_positions = NULL;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N = 22; // Tamaño total del vector
    int M = N / size; // Tamaño del vector para cada proceso
    int tam_vector = N; // Tamaño total del vector

    if (rank == 0) {
        srand(time(NULL));
        vector = (int *)malloc(tam_vector * sizeof(int));
        for (int i = 0; i < tam_vector; i++)
            vector[i] = rand() % 91 + 9;

        printf("Vector generado por el proceso maestro:\n");
        imprimirVector(vector, tam_vector);
        printf("\n");

        // Calcula las posiciones que quedan fuera de la distribución inicial
        int extra_positions_count = tam_vector % size;
        if (extra_positions_count > 0) {
            extra_positions = (int *)malloc(extra_positions_count * sizeof(int));
            for (int i = 0; i < extra_positions_count; i++) {
                extra_positions[i] = vector[M * size + i];
            }
        }
    }

    subvector = (int *)malloc(M * sizeof(int));

    // Distribuye el vector entre los procesos
    MPI_Scatter(vector, M, MPI_INT, subvector, M, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada proceso calcula el mínimo, el máximo y el promedio de sus elementos asignados
    int min = minimoVector(subvector, M);
    int max = maximoVector(subvector, M);
    double prom = promedioVector(subvector, M);

    // Recolectar los resultados en el proceso 0
    int *minimos = NULL, *maximos = NULL;
    double *promedios = NULL;

    if (rank == 0) {
        minimos = (int *)malloc(size * sizeof(int));
        maximos = (int *)malloc(size * sizeof(int));
        promedios = (double *)malloc(size * sizeof(double));
    }

    MPI_Gather(&min, 1, MPI_INT, minimos, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(&max, 1, MPI_INT, maximos, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(&prom, 1, MPI_DOUBLE, promedios, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Proceso 0 calcula y muestra los resultados de las posiciones extras
    if (rank == 0 && extra_positions != NULL) {
        int min_extra = minimoVector(extra_positions, N % size);
        int max_extra = maximoVector(extra_positions, N % size);
        double prom_extra = promedioVector(extra_positions, N % size);

        printf("Resultados de posiciones extras:\n");
        printf("Mínimo: %d\n", min_extra);
        printf("Máximo: %d\n", max_extra);
        printf("Promedio: %f\n", prom_extra);
    }

    // El proceso 0 imprime los resultados
    if (rank == 0) {
        printf("Resultados:\n");
        printf("Vector de mínimos:\n");
        imprimirVector(minimos, size);
        printf("\n");

        printf("Vector de máximos:\n");
        imprimirVector(maximos, size);
        printf("\n");

        printf("Vector de promedios:\n");
        for (int i = 0; i < size; i++)
            printf("%f ", promedios[i]);
        printf("\n");
    }

    MPI_Finalize();
}

