# jacobi_iteration_example
Jacobi iteration test, single process and multi process

## Build

Need 
* CMake
* MPICH

```
cmake .
make
```

## Test

### Test with single process
```
./mpi_single_cpu.out
```

### Test with Multi process
```
mpirun -np 4 ./mpi_multi_cpu.out
```
