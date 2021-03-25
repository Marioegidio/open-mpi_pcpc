# Open-mpi_pcpc

## Implementing broadcasting, gathering and scatter operations with open-mpi for the pcpc course. 

<br>
<br>

# Execution

##### You need open-mpi to execute

<br>

### Steps:

- mpicc -c library.c -o library.o
- mpicc test.c  library.o -o test.o
- mpirun -np 4 test.o 