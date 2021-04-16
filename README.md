# Open-mpi_pcpc

## Exercise 1 

#### Implementing broadcasting, gathering and scatter operations with open-mpi for the pcpc course. 

<br>

### Execution 


###### [!important] You need open-mpi to execute


#### Steps:

- cd src/ese_1
- mpicc -c library.c -o library.o
- mpicc test.c  library.o -o test.o
- mpirun -np 4 test.o 

<br><br>
## Exercise 2

####  1) Given P processes the rank_i process sends an integer value to the rank_i+1 process. ( rank_P-1 process receives from rank_0 process). In each round, a random number is added to the total. Given a predetermined threshold S, the average number of rounds must be calculated to arrive at the convergence of P and S. 
####  2) Add function Reduce in library. 
####  3) Add non blocking broadcasting, gathering and scatter operations. 

<br>

### Execution 

###### [!important] You need open-mpi to execute

#### Steps:

- cd src/ese_2
- mpicc ring.c -o ring.o
- mpirun -np 4 ring.o 50 
