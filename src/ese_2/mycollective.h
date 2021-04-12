#include <mpi.h>
#include <stdio.h>


#define MY_max 0
#define MY_min 1
#define MY_avg 2



/*void myBroadcast(int arr[],int arrSize,int numtasks, MPI_Comm commmunicator, int tag, int rank);
void myBroadcastChar(char arr[],int arrSize,int numtasks, MPI_Comm commmunicator, int tag, int rank);
void myGatheting(int numtasks, MPI_Comm commmunicator, int tag, int rank);
void myGathetingChar(int numtasks, MPI_Comm commmunicator, int tag, int rank);*/

void myBroadcast(const void *buf,int bufSize,MPI_Datatype datatype, int source, MPI_Comm comm, int tag);
void myGatheting(MPI_Datatype datatype, int dest, MPI_Comm comm, int tag);
void myScaatter(const void *buf,int bufSize, int source, MPI_Comm comm, int returnBuffer[], int *returnSize);
void myReduce(int array[],int bufSize, int dest, MPI_Comm comm, int mode, int *result);

void myIbroadcast(int buf[],int bufSize,MPI_Datatype datatype, int source, MPI_Comm comm, int tag, MPI_Request *request, int retBuffer[]);
void myIscaatter(int array[],int bufSize, int source, MPI_Comm comm, int returnBuffer[], int *returnSize, MPI_Request requests[], int *requestSize);

void myWait(MPI_Request requests[],int size);