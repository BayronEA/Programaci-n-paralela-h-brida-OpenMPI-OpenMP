/*
Universidad de Pamplona
Facultad de Ingenierías y Arquitectura
Programa de Ingeniería de Sistemas
Asignatura: Fundamentos de Computación paralela y distribuida
Profesor: Jose Orlando Maldonado Bautista


Escribir un programa paralelo, utilizando un enfoque híbrido. El programa debe realizar el cálculo del
mínimo, el máximo y el promedio de los elementos de un vector. Para ello, utilizando OpenMP para un
número NP de procesos, el proceso 0, genera un vector de números aleatorios de tamaño NPxM, (M, un
entero cualquiera ) y reparte el vector generado entre los NP procesos. Cada proceso por su parte recibe
los datos enviados, y los procesa localmente para determinar el máximo, el mínimo y el promedio de cada
vector local. Para esta tarea, utiliza programación de memoria compartida mediante OpenMP,
paralelizando el cálculo de dichos resultados en forma óptima (paralelizar el cálculo del mínimo, máximo
y promedio con OpenMP). Finalmente, cada proceso envía los resultados al proceso 0, para que se reúnan
y se impriman los resultados totales, es decir máximo, mínimo y promedio del vector original.
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
    int rank, size, *vector, *subvector;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int M = 7; // Tamaño del vector
    int tam_vector = size * M; // Tamaño total del vector

    if (rank == 0) {
        srand(time(NULL));
        vector = (int *)malloc(tam_vector * sizeof(int));
        for (int i = 0; i < tam_vector; i++)
            vector[i] = rand() % 91 + 9; // Genera números aleatorios entre 9 y 99

        printf("Vector generado por el proceso maestro:\n");
        imprimirVector(vector, tam_vector);
        printf("\n");
    }

    int elementos_por_proceso = M; // Número de elementos por proceso
    subvector = (int *)malloc(elementos_por_proceso * sizeof(int));

    // Distribuir el vector entre los procesos
    MPI_Scatter(vector, elementos_por_proceso, MPI_INT, subvector, elementos_por_proceso, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada proceso calcula el mínimo, el máximo y el promedio de sus elementos asignados
    int min = minimoVector(subvector, elementos_por_proceso);
    int max = maximoVector(subvector, elementos_por_proceso);
    double prom = promedioVector(subvector, elementos_por_proceso);

    // Recolectar los resultados en el proceso 0
    int minimos[size];
    MPI_Gather(&min, 1, MPI_INT, minimos, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int maximos[size];
    MPI_Gather(&max, 1, MPI_INT, maximos, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double promedios[size];
    MPI_Gather(&prom, 1, MPI_DOUBLE, promedios, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

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
    return 0;
}

