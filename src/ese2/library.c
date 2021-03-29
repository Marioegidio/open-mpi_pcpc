#include "mpi.h"
#include <stdio.h>
#include <stdbool.h>

void myBrodcast(int world_rank, const void *array, MPI_Datatype datatype, int N, int world_size, int tag, MPI_Comm MPI_COMMON_WORLD, MPI_Status *Stat, bool toPrintOut)
{
    if (world_rank == 0)
    {
        for (int i = 1; i < world_size; i++)
        {
            if (datatype == MPI_INT)
            {
                MPI_Send(array, N, MPI_INT, i, tag, MPI_COMM_WORLD);
            }
            else if (datatype == MPI_CHAR)
            {
                MPI_Send(array, N, MPI_CHAR, i, tag, MPI_COMM_WORLD);
            }
        }
    }

    else if (world_rank > 0)
    {
        if (datatype == MPI_INT)
        {
            int array_rcv[N];
            MPI_Recv(&array_rcv, N, MPI_INT, 0, 1, MPI_COMM_WORLD, Stat);

            if (toPrintOut)
            {
                printf("\n");
                printf("broadcast -> rank %d", world_rank);
                for (int j = 0; j < N; j++)
                {
                    printf(" %d ", array_rcv[j]);
                }
                printf("\n");
            }
        }
        else
        {
            char array_rcv[N];
            MPI_Recv(&array_rcv, N, MPI_CHAR, 0, 1, MPI_COMM_WORLD, Stat);

            if (toPrintOut)
            {
                printf("\n");
                printf("broadcast -> rank %d", world_rank);
                for (int j = 0; j < N; j++)
                {
                    printf(" %c ", array_rcv[j]);
                }
                printf("\n");
            }
        }
    }
}

void myGathering(int world_rank, MPI_Datatype datatype, const void *test, int N, int world_size, int tag, MPI_Comm MPI_COMMON_WORLD, MPI_Status *Stat, bool toPrintOut)
{
    if (world_rank == 0)
    {
        if (datatype == MPI_INT)
        {
            char array_rcv[N];
            for (int i = 1; i < world_size; i++)
            {
                int temp = 0;
                MPI_Recv(&temp, 1, MPI_INT, i, tag, MPI_COMM_WORLD, Stat);
                array_rcv[i - 1] = temp;
            }

            if (toPrintOut)
            {
                printf("\n");
                printf("gathering -> rank %d", world_rank);
                for (int j = 0; j < world_size - 1; j++)
                {
                    printf(" %d ", array_rcv[j]);
                }
                printf("\n");
            }
        }
        else if (datatype == MPI_CHAR)
        {
            int array_rcv[N];
            for (int i = 1; i < world_size; i++)
            {
                char temp = 0;
                MPI_Recv(&temp, 1, MPI_CHAR, i, tag, MPI_COMM_WORLD, Stat);
                array_rcv[i - 1] = temp;
            }

            if (toPrintOut)
            {
                printf("\n");
                printf("gathering -> rank %d", world_rank);
                for (int j = 0; j < world_size - 1; j++)
                {
                    printf(" %c ", array_rcv[j]);
                }
                printf("\n");
            }
        }
    }

    else if (world_rank > 0)
    {
        if (datatype == MPI_INT)
        {
            for (int i = 0; i < world_size - 1; i++)
            {
                MPI_Send(&test[world_rank - 1], 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
            }
        }
        else if (datatype == MPI_CHAR)
        {
            MPI_Send(&test[world_rank - 1], 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
        }
    }
}

void myScatter(int world_rank, int arrayInt[], char arrayChar[], MPI_Datatype datatype, int source, int N, int world_size, int tag, MPI_Comm MPI_COMMON_WORLD, MPI_Status *Stat, bool toPrintOut)
{
    if (world_rank == source)
    {
        if (world_size < N)
        {
            int split_size = N / (world_size - 1);

            int index = 0;

            for (int i = 1; i < world_size - 1; i++)
            {
                if (datatype == MPI_INT)
                {
                    int temp[split_size];
                    for (int j = 0; j < split_size; j++)
                    {
                        temp[j] = arrayInt[index];
                        index++;
                    }
                    MPI_Send(temp, split_size, MPI_INT, i, tag, MPI_COMM_WORLD);
                }
                else if (datatype == MPI_CHAR)
                {
                    char temp[split_size];
                    for (int j = 0; j < split_size; j++)
                    {
                        temp[j] = arrayChar[index];
                        index++;
                    }
                    MPI_Send(temp, split_size, MPI_CHAR, i, tag, MPI_COMM_WORLD);
                }
            }

            split_size = N - index;
            if (datatype == MPI_INT)
            {
                int temp[split_size];
                for (int i = index; i < N; i++)
                {
                    temp[i - index] = arrayInt[i];
                }
                MPI_Send(temp, split_size, MPI_INT, world_size - 1, tag, MPI_COMM_WORLD);
            }
            else if (datatype == MPI_CHAR)
            {
                char temp[split_size];
                for (int i = index; i < N; i++)
                {
                    temp[i - index] = arrayChar[i];
                }
                MPI_Send(temp, split_size, MPI_CHAR, world_size - 1, tag, MPI_COMM_WORLD);
            }
        }
    }
    else
    {
        // calculate array dimension
        int dimension = 0;
        if (world_rank == world_size - 1)
            dimension = N - ((N / (world_size - 1)) * (world_size - 2));
        else
            dimension = N / (world_size - 1);

        if (datatype == MPI_INT)
        {
            int scatter_rcv[dimension];
            // reciving arrays
            MPI_Recv(&scatter_rcv, dimension, MPI_INT, 0, tag, MPI_COMM_WORLD, Stat);

            if (toPrintOut)
            {
                printf("\n");
                printf("scatter -> rank %d", world_rank);
                for (int j = 0; j < dimension; j++)
                {
                    printf(" %d ", scatter_rcv[j]);
                }
                printf("\n");
            }
        }
        else if (datatype == MPI_CHAR)
        {
            char scatter_rcv[dimension];
            // reciving array
            MPI_Recv(&scatter_rcv, dimension, MPI_CHAR, 0, tag, MPI_COMM_WORLD, Stat);

            if (toPrintOut)
            {
                printf("\n");
                printf("scatter -> rank %d", world_rank);
                for (int j = 0; j < dimension; j++)
                {
                    printf(" %c ", scatter_rcv[j]);
                }
                printf("\n");
            }
        }
    }
}