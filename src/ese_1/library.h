#include "mpi.h"
#include <stdio.h>
#include <stdbool.h>

int arrayMin(int arr[], int n);
int arrayMax(int arr[], int n);
int arrayAvg(int arr[], int n);
void myBroadcast(void *buf, int bufSize, MPI_Datatype datatype, int source, MPI_Comm comm, int tag, MPI_Status *Stat, void *out);
void myIbroadcast(void *buf, int bufSize, MPI_Datatype datatype, int source, MPI_Comm comm, int tag, MPI_Request requests[], int *requestSize);
void myGather(void *buf, int bufSize, MPI_Datatype datatype, int root, MPI_Comm comm, void *returnBuffer, int *returnSize, MPI_Status *Stat);
void myIgather(void *buf, int bufSize, MPI_Datatype datatype, int root, MPI_Comm comm, void *returnBuffer, int *returnSize, MPI_Request requests[], int *requestsSize);
void myWait(MPI_Request requests[], int size);
void myScatter(void *array, int bufSize, MPI_Datatype datatype, int source, MPI_Comm comm, void *returnBuffer, int *returnSize, MPI_Status *Stat);
void myIscatter(void *array, int bufSize, MPI_Datatype datatype, int source, MPI_Comm comm, void *returnBuffer, int *returnSize, MPI_Request requests[], int *requestsSize);