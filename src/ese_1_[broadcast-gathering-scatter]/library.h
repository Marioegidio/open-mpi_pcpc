#include "mpi.h"
#include <stdio.h>
#include <stdbool.h>
void myBrodcast(int world_rank, const void *array, MPI_Datatype datatype, int source, int N, int world_size, int tag, MPI_Comm MPI_COMMON_WORLD, MPI_Status *Stat, int *int_out, char *chars_out);
void myGathering(int world_rank, MPI_Datatype datatype, int *testInt, char *testChar, int source, int N, int world_size, int tag, MPI_Comm MPI_COMMON_WORLD, MPI_Status *Stat, int *int_out, char *chars_out);
void myScatter(int world_rank, int arrayInt[], char arrayChar[], MPI_Datatype datatype, int source, int N, int world_size, int tag, MPI_Comm MPI_COMMON_WORLD, MPI_Status *Stat, int *int_out, char *chars_out);