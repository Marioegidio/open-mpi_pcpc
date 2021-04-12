#include <stdio.h>
#include <mpi.h>


int main(int argc, char **argv){

    int rank;
    double start, end;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    MPI_Barrier(MPI_COMM_WORLD); /* tutti i processi sono inizializzati */
    start = MPI_Wtime();

    double array[4] = {1.1,1.2,1.3,1.4};

    array[0] = 2.4;

    MPI_Bcast(array,4,MPI_DOUBLE,0,MPI_COMM_WORLD);

    for(int i=0;i<4;i++){
        printf("rank: %d, res: %lf\n",rank,array[i]);
        fflush(stdout);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    MPI_Finalize();

    if (rank == 0) {
        printf("\n\n\nTime in ms = %f\n", end-start);
    }


}