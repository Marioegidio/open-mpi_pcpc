
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define MASTER 0
#define K 2
#define N 10

int main(int argc, char **argv){

    int a[N+2*K],leftK[K],rightK[K];
    int portion,rest,recvNum,position,numtasks,rank,offset=K;
    int finalArray[N];

    char message[N*10];

    MPI_Status status;
    MPI_Request leftRequest,rightRequest,leftRequest1,rightRequest1,request;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);

    portion = N/(numtasks);
    rest = N%numtasks;
    recvNum =  (rest <= rank) ? portion : portion +1;

    int recvArray[recvNum];
    int resultArray[recvNum+2*K];

    if(rank==MASTER){
        for(int i=K; i<N+K; i++)
            a[i]=i+1-K;
        int j=N;
        for(int i=0; i<K; i++){
            a[i]=a[j];
            j++;
        }
        j =0;
        for(int i=N+K; i<N+K*2; i++){
            a[i]=a[K+j];
            j++;
        }

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
                leftK[index] = a[j];
                index++;
            }
            index =0;
            for(int j=offset+toSend; j<offset+toSend+K;j++){
                rightK[index] = a[j];
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

    if (rank == MASTER){
        for (int i = 0; i<N; i++){
            printf(" - %d - ", finalArray[i]);
            fflush(stdout);
        }

        printf("\n\n");
            fflush(stdout);

        for (int i = 0; i<N+2*K; i++){
            printf(" - %d - ", a[i]);
            fflush(stdout);
        }

         printf("\n\n");
            fflush(stdout);


    }

    MPI_Finalize();

}