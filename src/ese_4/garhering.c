#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 10 

int main(int argc, char **argv){

    int rank,rest,recvNum,numtasks;
    double start,end;
    double array[ARRAY_SIZE] = {1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.1};
    double recvArray[ARRAY_SIZE];
    double random_value[2];



    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);

    MPI_Barrier(MPI_COMM_WORLD); /* tutti i processi sono inizializzati */
    start = MPI_Wtime();

    srand(rank);

    random_value[0] = (double)rand();

    random_value[1] = (double)rand();

    MPI_Gather(random_value,2,MPI_DOUBLE,recvArray,2,MPI_DOUBLE,0,MPI_COMM_WORLD);

    if(rank == 0)
        for(int i=0;i<(numtasks-1)*2;i++){
            printf("rank: %d, res: %lf\n",rank,recvArray[i]);
            fflush(stdout);
        }

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    MPI_Finalize();

    if (rank == 0) {
        printf("\n\n\nTime in ms = %f\n", end-start);
    }

}