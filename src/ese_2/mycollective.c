#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MY_max 0
#define MY_min 1
#define MY_avg 2

int arrayMin(int arr[], int n)
{
    int min = arr[0];
    for (int i = 0; i < n; i++)
    {
        if (arr[i] < min)
            min = arr[i];
    }
    return min;
}

int arrayMax(int arr[], int n)
{
    int max = arr[0];
    for (int i = 0; i < n; i++)
    {
        if (arr[i] > max)
            max = arr[i];
    }
    return max;
}

int arrayAvg(int arr[], int n)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum = sum + arr[i];
    }
    return sum / n;
}

void myIbroadcast(void *buf, int bufSize, MPI_Datatype datatype, int source, MPI_Comm comm, int tag, MPI_Request requests[], int *requestSize)
{

    MPI_Status Stat;
    int rank, numtasks;

    MPI_Comm_size(comm, &numtasks);
    MPI_Comm_rank(comm, &rank);

    *requestSize = 0;

    if (rank == source)
        for (int i = 0; i < numtasks; i++)
        {
            MPI_Isend(buf, bufSize, datatype, i, tag, comm, &requests[*requestSize]);
            *requestSize = *requestSize + 1;
        }
    else
    {
        MPI_Irecv(buf, bufSize, datatype, source, tag, comm, &requests[0]);
        *requestSize = 1;
    }
}

void myIgather(void *buf, int bufSize, MPI_Datatype datatype, int root, MPI_Comm comm, void *returnBuffer, int *returnSize, MPI_Request requests[], int *requestsSize)
{

    MPI_Status status;
    int rank, numtasks, dataSize;

    MPI_Comm_size(comm, &numtasks);
    MPI_Comm_rank(comm, &rank);

    MPI_Type_size(datatype, &dataSize);

    *requestsSize = 0;

    if (rank == root)
    {
        int currentSender = 0;
        for (int i = 0; i < numtasks; i++)
        {
            if (i != root)
            {
                MPI_Irecv(returnBuffer + (currentSender * dataSize), bufSize, datatype, i, 123, MPI_COMM_WORLD, &requests[*requestsSize]);
                *requestsSize = *requestsSize + 1;
                *returnSize = *returnSize + bufSize;
                currentSender++;
            }
        }
    }
    else
    {
        MPI_Isend(buf, bufSize, datatype, root, 123, MPI_COMM_WORLD, &requests[0]);
        *requestsSize = 1;
    }
}

void myIscatter(void *array, int bufSize, MPI_Datatype datatype, int source, MPI_Comm comm, void *returnBuffer, int *returnSize, MPI_Request requests[], int *requestsSize)
{

    MPI_Status Stat;
    int rank, numtasks, dataSize;

    MPI_Comm_size(comm, &numtasks);
    MPI_Comm_rank(comm, &rank);

    int rest = bufSize % (numtasks - 1);
    int portion = bufSize / (numtasks - 1);

    MPI_Type_size(datatype, &dataSize);

    if (rank == source)
    {
        int index = 0;
        int currentRevicer = 0;

        for (int i = 0; i < numtasks; i++)
        {
            if (i != source)
            {
                int toAdd = (rest <= currentRevicer) ? portion : portion + 1;
                MPI_Isend(array + (index * dataSize), toAdd, datatype, i, 123, MPI_COMM_WORLD, &requests[*requestsSize]);
                *requestsSize = *requestsSize + 1;
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
        MPI_Irecv(returnBuffer, toRecive, datatype, source, 123, MPI_COMM_WORLD, &requests[*requestsSize]);
        *requestsSize = 1;
    }
}

void myWait(MPI_Request requests[], int size)
{
    int index;
    MPI_Status status;
    MPI_Waitany(size, requests, &index, &status);
}

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

void myReduce(int array[], int bufSize, int dest, MPI_Datatype datatype, MPI_Comm comm, int mode, int *result)
{

    MPI_Status status;

    int rank, numtasks;

    MPI_Comm_size(comm, &numtasks);
    MPI_Comm_rank(comm, &rank);

    int returnBuffer[bufSize];
    int returnSize;
    int source = 0;

    myScatter(array, bufSize, datatype, source, comm, returnBuffer, &returnSize, &status);

    if (rank != dest)
    {

        int res;

        switch (mode)
        {
        case MY_min:
            res = arrayMin(returnBuffer, returnSize);
            break;
        case MY_max:
            res = arrayMax(returnBuffer, returnSize);
            break;
        case MY_avg:
            res = arrayAvg(returnBuffer, returnSize);
            break;
        default:
            res = arrayMin(returnBuffer, returnSize);
            break;
        }

        MPI_Send(&res, 1, MPI_INT, dest, 12345, MPI_COMM_WORLD);
    }
    else
    {
        int returnBuffer[numtasks - 1];
        int index = 0;

        for (int i = 0; i < numtasks; i++)
        {

            if (i != dest)
            {

                int res;

                MPI_Recv(&res, 1, MPI_INT, i, 12345, MPI_COMM_WORLD, &status);

                returnBuffer[index] = res;

                index++;
            }
        }

        int res;
        switch (mode)
        {
        case MY_min:
            res = arrayMin(returnBuffer, numtasks - 1);
            break;
        case MY_max:
            res = arrayMax(returnBuffer, numtasks - 1);
            break;
        case MY_avg:
            res = arrayAvg(returnBuffer, numtasks - 1);
            break;
        default:
            res = arrayMin(returnBuffer, numtasks - 1);
            break;
        }
        *result = res;
    }
}
