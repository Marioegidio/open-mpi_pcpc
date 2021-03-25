#include "mpi.h"
#include <stdio.h>

void myBrodcast(int world_rank, int numbers[], int N, int world_size, int tag, MPI_Comm MPI_COMMON_WORLD, MPI_Status *Stat);

void myGathering(int world_rank, int numbers_rcv[], int test[], int N, int world_size, int tag, MPI_Comm MPI_COMMON_WORLD, MPI_Status *Stat);

void myScatter(int world_rank, int numbers[], int N, int world_size, int tag, MPI_Comm MPI_COMMON_WORLD, MPI_Status *Stat);