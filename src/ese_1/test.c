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
    int broadcast_arr_int[N];
    int gathering_arr_int[N];
    int scatter_arr_int[N];
    char broadcast_arr_char[N];
    char gathering_arr_char[N];
    char scatter_arr_char[N];
    int source = 0;

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

    myBroadcast(numbers, N, dataType, source, MPI_COMM_WORLD, 1, &Stat, broadcast_arr_int);

    if (toPrintOut && world_rank != source)
    {
        printf("\n");
        printf("broadcast -> rank %d", world_rank);

        if (dataType == MPI_INT)
            for (int j = 0; j < N; j++)
            {
                printf(" %d ", broadcast_arr_int[j]);
            }
        else if (dataType == MPI_CHAR)
            for (int j = 0; j < N; j++)
            {
                printf(" %c ", broadcast_arr_char[j]);
            }
        printf("\n");
    }

    int sizeRet=0;
    myGather(numbers, N, dataType, source, MPI_COMM_WORLD, gathering_arr_int, &sizeRet, &Stat);
   
    if (toPrintOut && world_rank == source)
    {
        printf("\n");
        printf("gathering -> rank %d", world_rank);
        if (dataType == MPI_INT)
            for (int j = 0; j < sizeRet; j++)
            {
                printf(" %d ", gathering_arr_int[j]);
            }
        else if (dataType == MPI_CHAR)
            for (int j = 0; j < sizeRet; j++)
            {
                printf(" %c ", gathering_arr_char[j]);
            }
        printf("\n");
    }

    //myScatter(world_rank, numbers, chars, dataType, source, N, world_size, 111, MPI_COMM_WORLD, &Stat, scatter_arr_int, scatter_arr_char);

    if (toPrintOut)
    {
        // calculate array dimension
        int dimension = 0;
        if (world_rank == world_size - 1)
            dimension = N - ((N / (world_size - 1)) * (world_size - 2));
        else
            dimension = N / (world_size - 1);

        printf("\n");
        printf("scatter -> rank %d", world_rank);
        if (dataType == MPI_INT)
            for (int j = 0; j < dimension; j++)
            {
                printf(" %d ", scatter_arr_int[j]);
            }
        else
            for (int j = 0; j < dimension; j++)
            {
                printf(" %c ", scatter_arr_char[j]);
            }
        printf("\n");
    }

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
