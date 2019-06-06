cmake_minimum_required(VERSION 2.8)
project( mpi_helloworld_test )

set( MAT_SIZE )
set( OMP_THREAD_NUM )
add_definitions( ${MAT_SIZE} ${OMP_THREAD_NUM} )

set( SRCS )
aux_source_directory( "./utils" SRCS)

find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)

set(CMAKE_C_COMPILER mpicc)
set(CMAKE_CXX_COMPILER mpicxx)

set( include_flag ${MPI_INCLUDE_PATH} )
set( libs ${MPI_LIBRARIES} OpenMP::OpenMP_CXX)

include_directories( ${include_flag} )

add_executable( mpi_single_cpu.out ${SRCS} mpi_single_cpu.cpp )
add_executable( mpi_multi_cpu.out ${SRCS} mpi_multi_cpu.cpp )
add_executable( openmp_multi_cpu.out ${SRCS} openmp_multi_cpu.cpp )
add_executable( mpi_single_cpu_gauss_seidel.out ${SRCS} mpi_single_cpu_gauss_seidel.cpp )
add_executable( mpi_multi_cpu_gauss_seidel.out ${SRCS} mpi_multi_cpu_gauss_seidel.cpp )
add_executable( openmp_multi_cpu_gauss_seidel.out ${SRCS} openmp_multi_cpu_gauss_seidel.cpp )

target_link_libraries( mpi_single_cpu.out ${libs} )
target_link_libraries( mpi_multi_cpu.out ${libs} )
target_link_libraries( openmp_multi_cpu.out ${libs} )
target_link_libraries( mpi_single_cpu_gauss_seidel.out ${libs} )
target_link_libraries( mpi_multi_cpu_gauss_seidel.out ${libs} )
target_link_libraries( openmp_multi_cpu_gauss_seidel.out ${libs} )
