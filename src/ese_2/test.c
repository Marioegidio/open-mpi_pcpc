#include "mycollective.h"
#include <mpi.h>
#define N 10

int main(int argc, char **argv)
{
    int numbers[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    char chars[N] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'l'};
    int test[N] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 10};
    char charsTest[N] = {'m', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v'};
    double start, end;
    bool toPrintOut = true;// TODO change here to hide printf
    int broadcast_arr_int[N];
    int gathering_arr_int[N];
    int scatter_arr_int[N];
    char broadcast_arr_char[N];
    char gathering_arr_char[N];
    char scatter_arr_char[N];
    int source = 0;
    int requestSize = 0;
    int sizeRet = 0;

    MPI_Datatype dataType = MPI_INT; // change to MPI_CHAR to send chars
    MPI_Status Stat;

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    MPI_Request requests[world_size];

    MPI_Barrier(MPI_COMM_WORLD);
    /* tutti i processi sono inizializzati */
    start = MPI_Wtime();

    myIbroadcast(numbers, N, dataType, source, MPI_COMM_WORLD, 99, requests, &requestSize, broadcast_arr_int);

    if (toPrintOut && world_rank != source)
    {
        printf("\n\n");
        printf("broadcast -> rank %d", world_rank);

        myWait(requests, requestSize);
        if (dataType == MPI_INT)
            for (int j = 0; j < N; j++)
                printf(" %d ", broadcast_arr_int[j]);

        else if (dataType == MPI_CHAR)
            for (int j = 0; j < N; j++)
                printf(" %c ", broadcast_arr_char[j]);

        printf("\n");
    }

    myIgather(numbers, N, dataType, source, MPI_COMM_WORLD, gathering_arr_int, &sizeRet, requests, &requestSize);

    if (toPrintOut && world_rank == source)
    {
        printf("\n\n\n");
        printf("gathering -> rank %d", world_rank);

        myWait(requests, requestSize);
        if (dataType == MPI_INT)
            for (int j = 0; j < sizeRet; j++)
                printf(" %d ", gathering_arr_int[j]);

        else if (dataType == MPI_CHAR)
            for (int j = 0; j < sizeRet; j++)
                printf(" %c ", gathering_arr_char[j]);

        printf("\n");
    }

    myIscatter(numbers, N, dataType, source, MPI_COMM_WORLD, scatter_arr_int, &sizeRet, requests, &requestSize);

    if (toPrintOut && world_rank != source)
    {
        printf("\n");
        printf("scatter -> rank %d", world_rank);

        myWait(requests, requestSize);
        if (dataType == MPI_INT)
            for (int j = 0; j < sizeRet; j++)
                printf(" %d ", scatter_arr_int[j]);

        else
            for (int j = 0; j < sizeRet; j++)
                printf(" %c ", scatter_arr_char[j]);

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
