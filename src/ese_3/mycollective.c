#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MY_max 0
#define MY_min 1
#define MY_avg 2


int arrayMin(int arr[], int n)
{
    int min = arr[0];
    for(int i=0; i<n; i++){
        if (arr[i] < min)
            min = arr[i];
    }
    return min;
}

int arrayMax(int arr[], int n)
{
    int max = arr[0];
    for(int i=0; i<n; i++){
        if (arr[i] > max)
            max = arr[i];
    }
    return max;
}

int arrayAvg(int arr[], int n)
{
    int sum = 0;
    for(int i=0; i<n; i++){
            sum = sum + arr[i];
    }
    return sum/n;
}



void myBroadcast(const void *buf,int bufSize,MPI_Datatype datatype, int source, MPI_Comm comm, int tag){

    MPI_Status Stat;

    int rank,numtasks;


    MPI_Comm_size(comm, &numtasks);
    MPI_Comm_rank(comm, &rank);


    if( rank == source ){

        for(int i = 0; i < numtasks; i++){


            if(datatype == MPI_INT){

                MPI_Send(buf, bufSize, MPI_INT, i, tag, comm);

            }else if(datatype == MPI_CHAR){

                MPI_Send(buf, bufSize, MPI_CHAR, i, tag, comm);

            }

        }

        
    }else{

        if(datatype == MPI_INT){

            int recivedArray[bufSize];

            MPI_Recv(recivedArray, bufSize, MPI_INT, source, tag, comm, &Stat);

            for(int i=0; i < bufSize; i++){

            printf("Task %d, element %d: %d\n", rank,i,recivedArray[i]);
            fflush(stdout);

            }

        }else if(datatype == MPI_CHAR){

            char recivedArray[bufSize];

            MPI_Recv(recivedArray, bufSize, MPI_CHAR, source, tag, comm, &Stat);

            for(int i=0; i < bufSize; i++){

            printf("Task %d, element %d: %c\n", rank,i,recivedArray[i]);
            fflush(stdout);

            }

        }
        
    }

}





void myGatheting(MPI_Datatype datatype, int dest, MPI_Comm comm, int tag){

    MPI_Status Stat;

    int rank,numtasks;

    MPI_Comm_size(comm, &numtasks);
    MPI_Comm_rank(comm, &rank);

    int array[numtasks];


    if( rank == dest ){

        for(int i = 1; i < numtasks; i++){

            int res;

            MPI_Recv(&res, 1, MPI_INT, i, 123, MPI_COMM_WORLD,&Stat);
            array[i] = res;

        }

        
    }else{

        srand(rank);

        int val = rand() % 100;

        MPI_Send(&val, 1, MPI_INT, dest, 123, MPI_COMM_WORLD);

        
 
    }

}


void myScaatter(int array[],int bufSize, int source, MPI_Comm comm, int returnBuffer[], int *returnSize){

    MPI_Status Stat;

    int rank,numtasks;

    MPI_Comm_size(comm, &numtasks);
    MPI_Comm_rank(comm, &rank);




    if( rank == source ){

        int rest = bufSize%(numtasks-1);
        int added, index = 0;
        int portion=bufSize/(numtasks-1);

        for(int i = 0; i < numtasks; i++){

            if(i != source){

                int toAdd;

                if(rest != 0){
                    toAdd = portion + 1;
                    rest--;
                } else 
                    toAdd = portion;

                int arrayPortion[toAdd];

                for(int j = 0; j < toAdd; j++){
                    arrayPortion[j] = array[index];
                    index++;
                }

                MPI_Send(&toAdd, 1, MPI_INT, i, 99, MPI_COMM_WORLD);

                MPI_Send(arrayPortion, toAdd, MPI_INT, i, 123, MPI_COMM_WORLD);

            }

        }

        

    }else if( rank != 0){

        int size;

        MPI_Recv(&size, 1, MPI_INT, source, 99, MPI_COMM_WORLD, &Stat);


        *returnSize = size;
        int recivedArray[size];

        MPI_Recv(recivedArray, size, MPI_INT, source, 123, MPI_COMM_WORLD, &Stat);

        for(int i = 0; i<size;i++){
            returnBuffer[i] = recivedArray[i];
        }

    }

}



void myReduce(int array[],int bufSize, int dest, MPI_Comm comm, int mode, int *result){

    MPI_Status status;

    int rank,numtasks;

    MPI_Comm_size(comm, &numtasks);
    MPI_Comm_rank(comm, &rank);

    int arrayRes[bufSize];
    int size;


    myScaatter(array,bufSize,dest,MPI_COMM_WORLD,arrayRes, &size);

     if(rank != dest){

        int res;

        switch(mode){
            case MY_min: res = arrayMin(arrayRes,size); break;
            case MY_max: res = arrayMax(arrayRes,size); break;
            case MY_avg: res = arrayAvg(arrayRes,size); break;
            default: res = arrayMin(arrayRes,size); break;
        }

        MPI_Send(&res,1,MPI_INT,dest,12345,MPI_COMM_WORLD);
    }else{
        int arrayResult[numtasks-1];
        int index = 0; 

        for(int i = 0; i<numtasks;i++){

            if(i != dest){

                int res;

                MPI_Recv(&res,1,MPI_INT,i,12345,MPI_COMM_WORLD,&status);

                arrayResult[index] = res;

                index++;
            }

        }

        int res;
        switch(mode){
            case MY_min: res = arrayMin(arrayResult,numtasks-1); break;
            case MY_max: res = arrayMax(arrayResult,numtasks-1); break;
            case MY_avg: res = arrayAvg(arrayResult,numtasks-1); break;
            default: res = arrayMin(arrayResult,numtasks-1); break;
        }
        *result = res;
    }

   

}


void myIbroadcast(int buf[],int bufSize,MPI_Datatype datatype, int source, MPI_Comm comm, int tag, MPI_Request *request, int retBuffer[]){

    int rank,numtasks;

    MPI_Comm_size(comm, &numtasks);
    MPI_Comm_rank(comm, &rank);

    if( rank == source ){

        for(int i = 0; i < numtasks; i++){

            if(i != source){

                MPI_Isend(buf, bufSize, MPI_INT, i, tag, comm,request);

            }

        }

        
    }else{

        int recivedArray[bufSize];

        MPI_Irecv(recivedArray, bufSize, MPI_INT, source, tag, comm, request);

        retBuffer = recivedArray;

        for(int i=0; i<bufSize; i++){

            retBuffer[i] = recivedArray[i];

        }


        
    }

}




void myWait(MPI_Request requests[],int size){
    int index;
    MPI_Status status;
    MPI_Waitany(size,requests,&index, &status);
}






void myIscaatter(int array[],int bufSize, int source, MPI_Comm comm, int returnBuffer[], int *returnSize, MPI_Request requests[], int *requestSize){

    MPI_Status Stat;

    int rank,numtasks;

    MPI_Comm_size(comm, &numtasks);
    MPI_Comm_rank(comm, &rank);

    //MPI_Request request;

    int reqSize = 0;


    if( rank == source ){

        int rest = bufSize%(numtasks-1);
        int added, index = 0;
        int portion=bufSize/(numtasks-1);
        *returnSize = 0;

        for(int i = 0; i < numtasks; i++){

            if(i != source){

                int toAdd;

                if(rest != 0){
                    toAdd = portion + 1;
                    rest--;
                } else 
                    toAdd = portion;

                int arrayPortion[toAdd];

                for(int j = 0; j < toAdd; j++){
                    arrayPortion[j] = array[index];
                    index++;
                }

                MPI_Send(&toAdd, 1, MPI_INT, i, 99, comm);

                MPI_Isend(arrayPortion, toAdd, MPI_INT, i, 123, comm,&requests[reqSize]);
                reqSize++;


            }

        }

        *requestSize = reqSize;

        
    }else if( rank != 0){

        int size;

        MPI_Recv(&size, 1, MPI_INT, source, 99, comm, &Stat);


        *returnSize = size;

        MPI_Irecv(returnBuffer, size, MPI_INT, source, 123, comm, &requests[reqSize]);
        reqSize++;
        *requestSize = reqSize;
    }

}