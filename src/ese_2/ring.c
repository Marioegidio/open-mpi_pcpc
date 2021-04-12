#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){

    int rank, numtasks,tot;

    MPI_Status status;
    MPI_Request request;
    MPI_Request request2;

    int isFirst = 1;




    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int soglia;

    if(argc != 2){ 

        if(rank == 1){
            printf("Inserire soglia da riga di comando");

        }

        exit(1);
        
    }else{

        soglia = atoi(argv[1]);

    }


    int next = (rank+1)%numtasks;

    int prec;

    if(rank -1 >=0)
     prec = (rank-1)%numtasks;
    else 
     prec = numtasks-1;




    for(int i=0; i<10;i++){

        if(isFirst){
            srand(rank);
            isFirst = 0;

            if(rank == 0){

                int val = rand() % 100;
                MPI_Isend(&val,1,MPI_INT,next,99,MPI_COMM_WORLD,&request2);
            }

        }else{

            int res,tot,end = 1;

            int val = rand() % 100;

            MPI_Irecv(&res,1,MPI_INT,prec,99,MPI_COMM_WORLD,&request);

            printf("ciao %d\n",rank);
            fflush(stdout);


            MPI_Wait(&request,&status);
            printf("Ricevuto %d\n",rank);
            fflush(stdout);





            if(res >= soglia){
                MPI_Isend(&res,1,MPI_INT,next,99,MPI_COMM_WORLD,&request2);
                if(rank == 0){
                    printf("Numero medio di round: %d, res: %d\n",i, res);
                    fflush(stdout);
                }
                break;
            }

            tot = res + val;


            MPI_Isend(&tot,1,MPI_INT,next,99,MPI_COMM_WORLD,&request2);
  

        }


    }


     MPI_Finalize();

}
