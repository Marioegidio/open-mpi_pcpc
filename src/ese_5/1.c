
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define MASTER 0
#define K 2
#define N 10



int main(int argc, char **argv){

    int a[N];
    int finalArray[N];


    int rank,numtasks,portion,rest,recvNum,left,right,offset=0;
    double start,end;


    MPI_Status status;
    MPI_Request leftRequest,rightRequest,leftRequest1,rightRequest1;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    int sendCount[numtasks];
    int displs[numtasks];

    portion = N/(numtasks);
    rest = N%numtasks;
    left = (rank+numtasks-1)%(numtasks);
    right = (rank+1)%(numtasks);
    recvNum =  (rest <= rank) ? portion : portion +1;

    int recvArray[recvNum];
    int resultArray[recvNum];

    if(rank==MASTER){
        for(int i=0; i<N; i++){
            a[i]=i+1;
        }

        for(int i = 0; i<numtasks;i++){

            sendCount[i] = (rest <= i) ? portion : portion +1;
            displs[i] = offset;
            offset += sendCount[i];
        }
    }

    MPI_Scatterv(a,sendCount,displs,MPI_INT,recvArray,recvNum,MPI_DOUBLE,MASTER,MPI_COMM_WORLD);

    int leftK[K];
    int rightK[K];

    MPI_Isend(recvArray,K,MPI_INT,left,99,MPI_COMM_WORLD,&leftRequest1);
    MPI_Isend(&recvArray[recvNum - K],K,MPI_INT,right,9,MPI_COMM_WORLD,&rightRequest1);

    MPI_Irecv(leftK, K, MPI_INT,left, 9, MPI_COMM_WORLD, &leftRequest);
    MPI_Irecv(rightK, K, MPI_INT,right, 99, MPI_COMM_WORLD, &rightRequest);
   

    for(int i=K;i<recvNum-K;i++){

        int sum=0;

        for(int j=i-K;j<=i+K;j++){
            sum += recvArray[j];
        }

       
        
        resultArray[i] = sum/(2*K+1);

        
    }


    MPI_Request requestsArray[2]={leftRequest, rightRequest};
    int index=0;
    MPI_Waitany(1,requestsArray,&index,&status);
  

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

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();
    
    if (rank == MASTER)  
        for (int i = 0; i<N; i++){
            printf(" - %d - ", finalArray[i]);
            fflush(stdout);
        }

    MPI_Finalize();

}

    