#include "mpi.h"
#include <stdio.h>
#include <stdbool.h>

void myBroadcast(void *buf, int bufSize, MPI_Datatype datatype, int source, MPI_Comm comm, int tag, MPI_Status *Stat, void *out)
{
    int rank, numtasks;

    MPI_Comm_size(comm, &numtasks);
    MPI_Comm_rank(comm, &rank);

    if (rank == source)
        for (int i = 0; i < numtasks; i++)
            MPI_Send(buf, bufSize, datatype, i, tag, comm);
    else
    {
        MPI_Recv(out, bufSize, datatype, source, tag, comm, Stat);
    }
}

void myGather(void *buf, int bufSize, MPI_Datatype datatype, int root, MPI_Comm comm, void *returnBuffer, int *returnSize, MPI_Status *Stat)
{
    int rank, numtasks, dataSize;

    MPI_Comm_size(comm, &numtasks);
    MPI_Comm_rank(comm, &rank);
    MPI_Type_size(datatype, &dataSize);

    if (rank == root)
    {
        int currentSender = 0;
        for (int i = 0; i < numtasks; i++)
        {
            if (i != root)
            {
                MPI_Recv(returnBuffer + (currentSender * dataSize), 1, datatype, i, 123, MPI_COMM_WORLD, Stat);
                currentSender++;
            }
        }
        *returnSize = numtasks - 1;
    }
    else
    {
        MPI_Send(buf + (rank * dataSize), 1, datatype, root, 123, MPI_COMM_WORLD);
    }
}

void myScatter(void *array, int bufSize, MPI_Datatype datatype, int source, MPI_Comm comm, void *returnBuffer, int *returnSize, MPI_Status *Stat)
{

    int rank, numtasks;

    MPI_Comm_size(comm, &numtasks);
    MPI_Comm_rank(comm, &rank);

    int rest = bufSize % (numtasks - 1);
    int portion = bufSize / (numtasks - 1);

    int size;
    MPI_Type_size(datatype, &size);

    if (rank == source)
    {
        int index = 0;
        int currentRevicer = 0;

        for (int i = 0; i < numtasks; i++)
        {
            if (i != source)
            {
                int toAdd = (rest <= currentRevicer) ? portion : portion + 1;
                MPI_Send(array + (index * size), toAdd, datatype, i, 123, MPI_COMM_WORLD);
                index += toAdd;
                currentRevicer++;
            }
        }
    }
    else
    {
        int correntReciver = (rank > source) ? rank - 1 : rank;
        int toRecive = (rest <= correntReciver) ? portion : portion + 1;
        *returnSize = toRecive;
        MPI_Recv(returnBuffer, toRecive, datatype, source, 123, MPI_COMM_WORLD, Stat);
    }
}
