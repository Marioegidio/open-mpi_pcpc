#include "mpi.h"
#include <stdio.h>
#include <stdbool.h>

int arrayMin(int arr[], int n);
int arrayMax(int arr[], int n);
int arrayAvg(int arr[], int n);

void myBroadcast(void *buf, int bufSize, MPI_Datatype datatype, int source, MPI_Comm comm, int tag, MPI_Status *Stat, void *out);
void myGather(void *buf, int bufSize, MPI_Datatype datatype, int root, MPI_Comm comm, void *returnBuffer, int *returnSize, MPI_Status *Stat);
void myWait(MPI_Request requests[], int size);
void myScatter(void *array, int bufSize, MPI_Datatype datatype, int source, MPI_Comm comm, void *returnBuffer, int *returnSize, MPI_Status *Stat);