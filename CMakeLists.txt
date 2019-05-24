cmake_minimum_required(VERSION 2.8)
project( mpi_helloworld_test )

set( SRCS )
aux_source_directory( "./utils" SRCS)

find_package(MPI REQUIRED)

set(CMAKE_C_COMPILER mpicc)
set(CMAKE_CXX_COMPILER mpicxx)

set( include_flag ${MPI_INCLUDE_PATH} )
set( libs ${MPI_LIBRARIES} )

include_directories( ${include_flag} )

add_executable( mpi_single_cpu.out ${SRCS} mpi_single_cpu.cpp )
add_executable( mpi_multi_cpu.out ${SRCS} mpi_multi_cpu.cpp )

target_link_libraries( mpi_single_cpu.out ${libs} )
target_link_libraries( mpi_multi_cpu.out ${libs} )