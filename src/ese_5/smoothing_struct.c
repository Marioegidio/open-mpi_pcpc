
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define MASTER 0
#define K 5
#define N 1000

typedef struct
    {
        int leftK[K];
        int portion[N]; 
        int rightK[K];
    } PortionToSend;

int main(int argc, char **argv)
{

    int a[N], blockcounts[1], finalArray[N];
    int portion, rest, recvNum, position, numtasks, rank, offset = 0;

    MPI_Status status;
    MPI_Request request;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    portion = N / (numtasks);
    rest = N % numtasks;
    recvNum = (rest <= rank) ? portion : portion + 1;

    MPI_Datatype portionType, oldtypes[1];
    MPI_Aint offsets[1];

    offsets[0] = 0;
    oldtypes[0] = MPI_INT;
    blockcounts[0] = 2 * K + N;

    MPI_Type_create_struct(1, blockcounts, offsets, oldtypes, &portionType);
    MPI_Type_commit(&portionType);

    PortionToSend recivedData;

    int resultArray[recvNum];

    if (rank == MASTER){

        for (int i =0; i <N; i++)
            a[i] = i + 1;

        for (int i = 0; i < numtasks; i++){

            int toSend = (rest <= i) ? portion : portion + 1;
            PortionToSend dataToSend;

            int index =0;
            for(int j=offset; j<offset+toSend;j++){
                dataToSend.portion[index] = a[j];
                index++;
            }
            index =0;
            for(int j=offset-K; j<offset;j++){
                dataToSend.leftK[index] = (j<0) ? a[N+j]:a[j] ;
                index++;
            }
            index =0;
            for(int j=offset+toSend; j<offset+toSend+K;j++){
                dataToSend.rightK[index] = (j>=N) ? a[j-N]:a[j];
                index++;

            }

            offset += toSend;

            MPI_Isend(&dataToSend, 1, portionType, i, 99, MPI_COMM_WORLD, &request);
        }
    }

    MPI_Recv(&recivedData, 1, portionType, MASTER, 99, MPI_COMM_WORLD, &status);

    for (int i = K; i < recvNum - K; i++){
        int sum = 0;
        for (int j = i - K; j <= i + K; j++)
            sum += recivedData.portion[j];

        resultArray[i] = sum / (2 * K + 1);
    }

    for (int i = 0; i < K; i++){
        int sum = 0;
        for (int j = K - 1; j >= i; j--)
            sum += recivedData.leftK[j];

        for (int j = 0; j <= i + K; j++)
            sum += recivedData.portion[j];

        resultArray[i] = sum / (2 * K + 1);
    }

    for (int i = recvNum - K; i < recvNum; i++){
        int sum = 0;
        int r = recvNum - i - 1;
        for (int j = i - K; j < recvNum; j++)
            sum += recivedData.portion[j];

        for (int j = 0; j < K - r; j++)
            sum += recivedData.rightK[j];
        resultArray[i] = sum / (2 * K + 1);
    }

    MPI_Gather(resultArray, recvNum, MPI_INT, finalArray, recvNum, MPI_INT, MASTER, MPI_COMM_WORLD);

    MPI_Finalize();

    if (rank == MASTER)
        for (int i = 0; i < N; i++)
            printf(" - %d - ", finalArray[i]);
}