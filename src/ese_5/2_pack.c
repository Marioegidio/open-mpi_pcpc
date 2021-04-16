
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define MASTER 0
#define K 2
#define N 1000

int main(int argc, char **argv){

    int a[N],leftK[K],rightK[K];
    int portion,rest,recvNum,position,numtasks,rank,offset=0;
    int finalArray[N];

    char message[N+2*K];

    MPI_Status status;
    MPI_Request leftRequest,rightRequest,leftRequest1,rightRequest1,request;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);

    portion = N/(numtasks);
    rest = N%numtasks;
    recvNum =  (rest <= rank) ? portion : portion +1;

    int recvArray[recvNum];
    int resultArray[recvNum];

    if(rank==MASTER){
        for(int i=0; i<N; i++)
            a[i]=i+1;

        for(int i = 0; i<numtasks;i++){

            int toSend = (rest <= i) ? portion : portion +1;
            

            int arrayToSend[toSend];

            int index =0;
            for(int j=offset; j<offset+toSend;j++){
                arrayToSend[index] = a[j];
                index++;
            }
            index =0;
            for(int j=offset-K; j<offset;j++){
                leftK[index] = (j<0) ? a[N+j]:a[j] ;
                index++;
            }
            index =0;
            for(int j=offset+toSend; j<offset+toSend+K;j++){
                rightK[index] = (j>=N) ? a[j-N]:a[j];
                index++;

            }

            offset += toSend;
        
            position = 0;
            MPI_Pack(arrayToSend, toSend, MPI_INT, message, N*10, &position, MPI_COMM_WORLD); 
            MPI_Pack(leftK,K, MPI_INT, message, N*10, &position, MPI_COMM_WORLD);
            MPI_Pack(rightK,K, MPI_INT, message, N*10, &position, MPI_COMM_WORLD);

            MPI_Isend(message, N*10, MPI_PACKED, i, 99, MPI_COMM_WORLD,&request);


        }


    }

    MPI_Recv(message, N*10, MPI_PACKED, MASTER, 99, MPI_COMM_WORLD, &status);

    position =0;
    MPI_Unpack(message, N*10, &position, recvArray, recvNum,MPI_INT, MPI_COMM_WORLD);
    MPI_Unpack(message, N*10, &position, leftK, K,MPI_INT, MPI_COMM_WORLD);
    MPI_Unpack(message, N*10, &position, rightK, K,MPI_INT, MPI_COMM_WORLD);

    for(int i=K;i<recvNum-K;i++){
        int sum=0;
        for(int j=i-K;j<=i+K;j++){
            sum += recvArray[j];
        }
        resultArray[i] = sum/(2*K+1);
    }


    for(int i = 0; i<K; i++){
        int sum=0;
        for(int j=K-1;j>=i;j--){
            sum += leftK[j];
        }
        for(int j=0;j<=i+K;j++){
            sum += recvArray[j];
        }


        resultArray[i] = sum/(2*K+1);
    }


    for(int i = recvNum-K; i<recvNum; i++){
        int sum=0;
        int r = recvNum-i-1;
        for(int j=i-K;j<recvNum;j++){
            sum += recvArray[j];
          
        }
        for(int j=0;j<K-r;j++){
            sum += rightK[j];
        }
        resultArray[i] = sum/(2*K+1);
    }


    MPI_Gather(resultArray,recvNum,MPI_INT,finalArray,recvNum,MPI_INT,MASTER,MPI_COMM_WORLD);

    MPI_Finalize();

    if (rank == MASTER)
        for (int i = 0; i < N; i++)
            printf(" - %d - ", finalArray[i]);

}