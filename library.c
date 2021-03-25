#include "mpi.h"
#include <stdio.h>

void myBrodcast(int world_rank, int numbers[], int N, int world_size, int tag, MPI_Comm MPI_COMMON_WORLD, MPI_Status *Stat)
{
    if (world_rank == 0)
    {
        for (int i = 1; i < world_size; i++)
        {
            MPI_Send(numbers, N, MPI_INT, i, tag, MPI_COMM_WORLD);
        }
    }

    else if (world_rank > 0)
    {
        int numbers_rcv[N];
        MPI_Recv(&numbers_rcv, N, MPI_INT, 0, 1, MPI_COMM_WORLD, Stat);
        printf("\n");
        printf("rank %d", world_rank);
        for (int j = 0; j < N; j++)
        {
            printf(" %d ", numbers_rcv[j]);
        }
        printf("\n");
    }
}

void myGathering(int world_rank, int numbers_rcv[], int test[], int N, int world_size, int tag, MPI_Comm MPI_COMMON_WORLD, MPI_Status *Stat)
{
    if (world_rank == 0)
    {
        for (int i = 1; i < world_size; i++)
        {
            int temp = 0;
            MPI_Recv(&temp, 1, MPI_INT, i, tag, MPI_COMM_WORLD, Stat);
            numbers_rcv[i - 1] = temp;
        }

        printf("\n");
        printf("rank %d", world_rank);
        for (int j = 0; j < world_size - 1; j++)
        {
            printf(" %d ", numbers_rcv[j]);
        }
        printf("\n");
    }

    else if (world_rank > 0)
    {
        for (int i = 0; i < world_size - 1; i++)
        {
            MPI_Send(&test[world_rank - 1], 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
        }
    }
}

void myScatter(int world_rank, int numbers[], int N, int world_size, int tag, MPI_Comm MPI_COMMON_WORLD, MPI_Status *Stat)
{
    if (world_rank == 0)
    {
        if (world_size < N)
        {
            int split_size = N / (world_size - 1);

            int index = 0;
            for (int i = 1; i < world_size - 1; i++)
            {
                int temp[split_size];
                for (int j = 0; j < split_size; j++)
                {
                    temp[j] = numbers[index];
                    index++;
                }
                MPI_Send(temp, split_size, MPI_INT, i, tag, MPI_COMM_WORLD);
            }

            split_size = N - index;
            int temp[split_size];
            for (int i = index; i < N; i++)
            {
                temp[i - index] = numbers[i];
            }
            MPI_Send(temp, split_size, MPI_INT, world_size - 1, tag, MPI_COMM_WORLD);
        }
    }

    else if (world_rank > 0)
    {
        // calculate array dimension
        int dimension = 0;
        if (world_rank == world_size - 1)
            dimension = N - ((N / (world_size - 1)) * (world_size - 2));
        else
            dimension = N / (world_size - 1);
        int scatter_rcv[dimension];

        // reciving array
        MPI_Recv(&scatter_rcv, dimension, MPI_INT, 0, tag, MPI_COMM_WORLD, Stat);

        printf("\n");
        printf("rank %d", world_rank);
        for (int j = 0; j < dimension; j++)
        {
            printf(" %d ", scatter_rcv[j]);
        }
        printf("\n");
    }
}