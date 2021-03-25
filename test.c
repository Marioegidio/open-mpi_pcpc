#include "library.h"
#include <string.h>
#define N 10

void printOut(char message[], int world_rank, int rank_aspected, int world_size)
{
    // check to print only one time
    if ((rank_aspected == 0 && world_rank == 0) || (rank_aspected == 1 && world_rank == (world_size - 1)))
    {
        printf("\n");
        printf("%s", message);
        printf("\n");
    }
}

int main(int argc, char **argv)
{
    int numbers[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int test[5] = {1, 2, 3, 4, 5};

    MPI_Status Stat;

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    printOut("Start broadcasting", world_rank, 0, world_size);
    myBrodcast(world_rank, numbers, N, world_size, 1, MPI_COMM_WORLD, &Stat);
    printOut("End broadcasting", world_rank, 1, world_size);

    printOut("Start gathering", world_rank, 0, world_size);
    myGathering(world_rank, numbers, test, N, world_size, 11, MPI_COMM_WORLD, &Stat);
    printOut("End gathering", world_rank, 1, world_size);

    printOut("Start scatter", world_rank, 0, world_size);
    myScatter(world_rank, numbers, N, world_size, 111, MPI_COMM_WORLD, &Stat);
    printOut("End scatter", world_rank, 1, world_size);

    // Finalize the MPI environment.
    MPI_Finalize();
}
