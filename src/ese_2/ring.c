#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 10

int main(int argc, char **argv)
{

    int world_rank, world_size, tot;

    MPI_Status status;
    MPI_Request request;

    int isFirst = 1, next, prev;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int soglia;

    // la soglia va passata da linea di comando
    if (argc != 2)
    {
        if (world_rank == MASTER)
            printf("Ricordati di inserire la soglia da riga di comando!");
        exit(1);
    }
    else
        soglia = atoi(argv[1]);

    // Processo successivo
    next = (world_rank + 1) % world_size;

    // Processo precedente
    prev = (world_rank + world_size - 1) % (world_size);

    for (int i = 0; i < 10; i++)
    {
        if (isFirst)
        {
            srand(world_rank);
            isFirst = 0;

            if (world_rank == 0)
            {
                int val = rand() % 100;
                MPI_Isend(&val, 1, MPI_INT, next, 99, MPI_COMM_WORLD, &request);
            }
        }
        else
        {

            int res, tot, end = 1, val = rand() % 100;
            MPI_Irecv(&res, 1, MPI_INT, prev, 99, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);

            if (res >= soglia)
            {
                MPI_Isend(&res, 1, MPI_INT, next, 99, MPI_COMM_WORLD, &request);
                if (world_rank == 0)
                {
                    printf("\nNumero medio di round: %d, res: %d\n\n", i, res);
                    fflush(stdout);
                }
                break;
            }

            tot = res + val;
            MPI_Isend(&tot, 1, MPI_INT, next, 99, MPI_COMM_WORLD, &request);
        }
    }

    MPI_Finalize();
}
