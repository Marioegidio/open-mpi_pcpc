
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define MASTER 0
#define K 2
#define N 10

int main(int argc, char **argv)
{

    int a[N + 2 * K], blockcounts[1], finalArray[N];
    int portion, rest, recvNum, numtasks, rank, offset = K;
    typedef struct
    {
        int leftK[K];
        int toSend[N]; // qua non ci va N ma la size della porzione
        int rightK[K];
    } PortionToSend;

    MPI_Status status;
    MPI_Request request;
    MPI_Datatype portionType, oldtypes[1];
    // MPI_Aint type used to be consistent with syntax of
    // MPI_Type_extent routine
    MPI_Aint offsets[1], lb, extent;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    // setup description of the NOT-PREFIXED MPI_INT fields leftK, rightK & recvArray
    offsets[0] = 0;
    oldtypes[0] = MPI_INT;
    blockcounts[0] = 2 * K + N;

    // define structured type and commit it
    MPI_Type_create_struct(1, blockcounts, offsets, oldtypes, &portionType);
    MPI_Type_commit(&portionType);

    portion = N / (numtasks);
    rest = N % numtasks;
    recvNum = (rest <= rank) ? portion : portion + 1;

    int resultArray[recvNum + 2 * K];

    if (rank == MASTER)
    {

        for (int i = K; i < N + K; i++)
            a[i] = i + 1 - K;
        int j = N;
        for (int i = 0; i < K; i++)
        {
            a[i] = a[j];
            j++;
        }
        j = 0;
        for (int i = N + K; i < N + K * 2; i++)
        {
            a[i] = a[K + j];
            j++;
        }

        for (int i = 0; i < numtasks; i++)
        {
            PortionToSend array;

            int toSend = (rest <= i) ? portion : portion + 1;

            int index = 0;
            for (int j = offset; j < offset + toSend; j++)
            {
                array.toSend[index] = a[j];
                index++;
            }
            index = 0;
            for (int j = offset - K; j < offset; j++)
            {
                array.leftK[index] = a[j];
                index++;
            }
            index = 0;
            for (int j = offset + toSend; j < offset + toSend + K; j++)
            {
                array.rightK[index] = a[j];
                index++;
            }

            offset += toSend;

            MPI_Isend(&array, 1, portionType, i, 99, MPI_COMM_WORLD, &request);
            // MPI_Isend(message, N * 10, MPI_PACKED, i, 99, MPI_COMM_WORLD, &request);
        }
    }

    PortionToSend recivedArray;
    MPI_Irecv(&recivedArray, 1, portionType, MASTER, 99, MPI_COMM_WORLD, &request);

    for (int i = K; i < recvNum - K; i++)
    {
        int sum = 0;
        for (int j = i - K; j <= i + K; j++)
        {
            sum += recivedArray.toSend[j];
        }
        resultArray[i] = sum / (2 * K + 1);
    }

    for (int i = 0; i < K; i++)
    {
        int sum = 0;
        for (int j = K - 1; j >= i; j--)
        {
            sum += recivedArray.leftK[j];
        }
        for (int j = 0; j <= i + K; j++)
        {
            sum += recivedArray.toSend[j];
        }

        resultArray[i] = sum / (2 * K + 1);
    }

    for (int i = recvNum - K; i < recvNum; i++)
    {
        int sum = 0;
        int r = recvNum - i - 1;
        for (int j = i - K; j < recvNum; j++)
        {
            sum += recivedArray.toSend[j];
        }
        for (int j = 0; j < K - r; j++)
        {
            sum += recivedArray.rightK[j];
        }
        resultArray[i] = sum / (2 * K + 1);
    }

    MPI_Gather(resultArray, recvNum, MPI_INT, finalArray, recvNum, MPI_INT, MASTER, MPI_COMM_WORLD);

    if (rank == MASTER)
    {
        for (int i = 0; i < N; i++)
        {
            printf(" - %d - ", finalArray[i]);
            fflush(stdout);
        }

        printf("\n\n");
        fflush(stdout);

        for (int i = 0; i < N + 2 * K; i++)
        {
            printf(" - %d - ", a[i]);
            fflush(stdout);
        }

        printf("\n\n");
        fflush(stdout);
    }

    MPI_Finalize();
}