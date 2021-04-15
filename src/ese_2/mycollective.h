#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MY_max 0
#define MY_min 1
#define MY_avg 2

int arrayMin(int arr[], int n);
int arrayMax(int arr[], int n);
int arrayAvg(int arr[], int n);

void myReduce(int array[], int bufSize, int dest, MPI_Datatype datatype, MPI_Comm comm, int mode, int *result);
void myIbroadcast(void *buf, int bufSize, MPI_Datatype datatype, int source, MPI_Comm comm, int tag, MPI_Request requests[], int *requestSize);
void myIgather(void *buf, int bufSize, MPI_Datatype datatype, int root, MPI_Comm comm, void *returnBuffer, int *returnSize, MPI_Request requests[], int *requestsSize);
void myIscatter(void *array, int bufSize, MPI_Datatype datatype, int source, MPI_Comm comm, void *returnBuffer, int *returnSize, MPI_Request requests[], int *requestsSize);
void myWait(MPI_Request requests[], int size);
void myBroadcast(void *buf, int bufSize, MPI_Datatype datatype, int source, MPI_Comm comm, int tag, MPI_Status *Stat, void *out);
void myGather(void *buf, int bufSize, MPI_Datatype datatype, int root, MPI_Comm comm, void *returnBuffer, int *returnSize, MPI_Status *Stat);
void myScatter(void *array, int bufSize, MPI_Datatype datatype, int source, MPI_Comm comm, void *returnBuffer, int *returnSize, MPI_Status *Stat);
