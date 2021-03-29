#include "mpi.h"
#include <stdio.h>
#include <stdbool.h>

void myBrodcast(int world_rank, const void *array, MPI_Datatype datatype, int N, int world_size, int tag, MPI_Comm MPI_COMMON_WORLD, MPI_Status *Stat, bool toPrintOut);
void myGathering(int world_rank, MPI_Datatype datatype, const void *test, int N, int world_size, int tag, MPI_Comm MPI_COMMON_WORLD, MPI_Status *Stat, bool toPrintOut);
void myScatter(int world_rank, int arrayInt[], char arrayChar[], MPI_Datatype datatype, int source, int N, int world_size, int tag, MPI_Comm MPI_COMMON_WORLD, MPI_Status *Stat, bool toPrintOut);