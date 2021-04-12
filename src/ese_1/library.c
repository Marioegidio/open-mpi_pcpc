#include "mpi.h"
#include <stdio.h>
#include <stdbool.h>

void myBrodcast(int world_rank, const void *array, MPI_Datatype datatype, int source, int N, int world_size, int tag, MPI_Comm MPI_COMMON_WORLD, MPI_Status *Stat, int *int_out, char *chars_out)
{
    if (world_rank == source)
    {
        for (int i = 1; i < world_size; i++)
        {
            if (datatype == MPI_INT)
                MPI_Send(array, N, MPI_INT, i, tag, MPI_COMM_WORLD);

            else if (datatype == MPI_CHAR)
                MPI_Send(array, N, MPI_CHAR, i, tag, MPI_COMM_WORLD);
        }
    }

    else if (world_rank != source)
    {
        if (datatype == MPI_INT)
            MPI_Recv(int_out, N, MPI_INT, source, tag, MPI_COMM_WORLD, Stat);

        else if (datatype == MPI_CHAR)
            MPI_Recv(chars_out, N, MPI_CHAR, source, tag, MPI_COMM_WORLD, Stat);
    }
}

void myGathering(int world_rank, MPI_Datatype datatype, int *testInt, char *testChar, int source, int N, int world_size, int tag, MPI_Comm MPI_COMMON_WORLD, MPI_Status *Stat, int *int_out, char *chars_out)
{
    if (world_rank == source)
    {
        if (datatype == MPI_INT)
        {
            for (int i = 1; i < world_size; i++)
            {
                int temp = 0;
                MPI_Recv(&temp, 1, MPI_INT, i, tag, MPI_COMM_WORLD, Stat);
                int_out[i - 1] = temp;
            }
        }
        else if (datatype == MPI_CHAR)
        {
            for (int i = 1; i < world_size; i++)
            {
                char temp = 0;
                MPI_Recv(&temp, 1, MPI_CHAR, i, tag, MPI_COMM_WORLD, Stat);
                chars_out[i - 1] = temp;
            }
        }
    }
    else
    {
        if (datatype == MPI_INT)
        {
            for (int i = 0; i < world_size - 1; i++)
            {
                MPI_Send(&testInt[world_rank], 1, MPI_INT, source, tag, MPI_COMM_WORLD);
            }
        }
        else if (datatype == MPI_CHAR)
        {
            MPI_Send(&testChar[world_rank], 1, MPI_CHAR, source, tag, MPI_COMM_WORLD);
        }
    }
}

void myScatter(int world_rank, int arrayInt[], char arrayChar[], MPI_Datatype datatype, int source, int N, int world_size, int tag, MPI_Comm MPI_COMMON_WORLD, MPI_Status *Stat, int *int_out, char *chars_out)
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
            // reciving arrays
            MPI_Recv(&int_out, dimension, MPI_INT, 0, tag, MPI_COMM_WORLD, Stat);
        }
        else if (datatype == MPI_CHAR)
        {
            // reciving array
            MPI_Recv(&chars_out, dimension, MPI_CHAR, 0, tag, MPI_COMM_WORLD, Stat);
        }
    }
}