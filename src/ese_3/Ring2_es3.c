#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include "mycollective.h"

int main(int argc, char **argv)
{

    int rank, numtasks, tot;
    int isFirst = 1;
    ;

    MPI_Status status;
    MPI_Status status2;
    MPI_Request request;
    MPI_Request request2;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int ranks[numtasks];

    int next = (rank + 1) % numtasks;

    int prec;

    if (rank - 1 >= 0)
        prec = (rank - 1) % numtasks;
    else
        prec = numtasks - 1;

    for (int i = 0; i < 10; i++)
    {

        if (isFirst)
        {
            isFirst = 0;

            if (rank == 0)
            {
                int size = 1;
                MPI_Isend(&size, 1, MPI_INT, next, 123, MPI_COMM_WORLD, &request2);
                MPI_Isend(&rank, 1, MPI_INT, next, 99, MPI_COMM_WORLD, &request2);
            }
        }
        else
        {

            int prevRank, size, end = 1;

            MPI_Recv(&size, 1, MPI_INT, prec, 123, MPI_COMM_WORLD, &status);

            MPI_Irecv(ranks, size, MPI_INT, prec, 99, MPI_COMM_WORLD, &request);

            if (size >= numtasks)
            {

                MPI_Isend(&size, 1, MPI_INT, next, 123, MPI_COMM_WORLD, &request2);
                MPI_Wait(&request, &status);
                MPI_Isend(ranks, size, MPI_INT, next, 99, MPI_COMM_WORLD, &request2);

                tot = 0;

                for (int j = 0; j < size; j++)
                {
                    tot = tot + ranks[j];
                }

                printf("rank: %d, size: %d\n", rank, size);
                fflush(stdout);

                printf("rank: %d, tot: %d\n", rank, tot);
                fflush(stdout);

                break;
            }
            else
            {
                ranks[size] = rank;
                size++;

                MPI_Isend(&size, 1, MPI_INT, next, 123, MPI_COMM_WORLD, &request2);
                MPI_Isend(ranks, size, MPI_INT, next, 99, MPI_COMM_WORLD, &request2);
            }
        }
    }

    MPI_Finalize();
}
