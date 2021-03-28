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
    char chars[N] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'l'};
    int test[N] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 10};
    char charsTest[N] = {'m', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v'};
    double start, end;
    MPI_Datatype dataType = MPI_INT; // change to MPI_CHAR to send chars
    bool toPrintOut = true;
    MPI_Status Stat;

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);
    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    MPI_Barrier(MPI_COMM_WORLD); 
    /* tutti i processi sono inizializzati */
    start = MPI_Wtime();

    myBrodcast(world_rank, numbers, dataType, N, world_size, 1, MPI_COMM_WORLD, &Stat, toPrintOut);
    //myBrodcast(world_rank, chars, dataType, N, world_size, 1, MPI_COMM_WORLD, &Stat,toPrintOut);

    myGathering(world_rank, dataType, test, N, world_size, 11, MPI_COMM_WORLD, &Stat, toPrintOut);
    //myGathering(world_rank, dataType, charsTest, N, world_size, 11, MPI_COMM_WORLD, &Stat,toPrintOut);

    myScatter(world_rank, numbers, chars, dataType, 0, N, world_size, 111, MPI_COMM_WORLD, &Stat, toPrintOut);

    MPI_Barrier(MPI_COMM_WORLD); 
    /* tutti i processi hanno terminato */
    end = MPI_Wtime();

    // Finalize the MPI environment.
    MPI_Finalize();

    if (world_rank == 0)
    { /* Master node scrive su stdout il tempo o su file */
        fflush(stdout);
        printf("\n\n");
        printf("\nTime in ms = %f\n", end - start);
        printf("\n\n");
    }
}
