#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0
#define MASTER1 1

int main(int argc, char *argv[]) {
    int numtasks, rank, qtd_Vezes, tamanho, tag = 1;
    double start_time, end_time; // Variáveis para medir o tempo

    MPI_Status Stat;

    MPI_Init(&argc, &argv); // Inicia o MPI
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // Obtém o número de processos
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtém o rank do processo

    // Lê valores da linha de comando
    qtd_Vezes = atoi(argv[1]);
    tamanho = atoi(argv[2]);

    // Verifica se há dois processos
    if (numtasks != 2) {
        if (rank == MASTER) {
            printf("Este programa requer exatamente 2 processos.\n");
        }
        MPI_Finalize();
        exit(1);
    }

    double *vetor = (double *)malloc(tamanho * sizeof(double));

    for (int i = 0; i < tamanho; i++) {
        vetor[i] = -1.0;
    }

    start_time = MPI_Wtime(); // Início da medição de tempo
           
    while (qtd_Vezes > 0) {
        if (rank == MASTER) {
            if(vetor[0] != 2.0) {
                for (int j = 0; j < tamanho; j++) {
                    // Altera o vetor
                    vetor[j] = 2.0;
                }
            }

            // Envia o vetor e qtd_Vezes para MASTER1
            MPI_Send(vetor, tamanho, MPI_DOUBLE, MASTER1, tag, MPI_COMM_WORLD);
            MPI_Send(&qtd_Vezes, 1, MPI_INT, MASTER1, tag, MPI_COMM_WORLD);

            // Recebe o vetor e qtd_Vezes de MASTER1
            MPI_Recv(vetor, tamanho, MPI_DOUBLE, MASTER1, tag, MPI_COMM_WORLD, &Stat);
            MPI_Recv(&qtd_Vezes, 1, MPI_INT, MASTER1, tag, MPI_COMM_WORLD, &Stat);

        } else if (rank == MASTER1) {
            // Recebe o vetor e qtd_Vezes de MASTER
            MPI_Recv(vetor, tamanho, MPI_DOUBLE, MASTER, tag, MPI_COMM_WORLD, &Stat);
            MPI_Recv(&qtd_Vezes, 1, MPI_INT, MASTER, tag, MPI_COMM_WORLD, &Stat);

            if(vetor[0] == 2.0) {
                for (int j = 0; j < tamanho; j++) {
                    // Altera o vetor
                    vetor[j] = 4.0;
                }
                qtd_Vezes--;
            }

            // Envia o vetor e qtd_Vezes de MASTER1 para MASTER
            MPI_Send(vetor, tamanho, MPI_DOUBLE, MASTER, tag, MPI_COMM_WORLD);
            MPI_Send(&qtd_Vezes, 1, MPI_INT, MASTER, t
