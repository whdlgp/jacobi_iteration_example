#include <mpi.h>
#include "utils/matrix_utils.h"

using namespace std;

enum
{
    MAT_SIZE = 8,
    MAT_ROWS = MAT_SIZE,
    MAT_COLS = MAT_SIZE,
};

int main(int argc, char** argv) 
{
    // start process execution time measure
    auto start = chrono::high_resolution_clock::now();

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number/rank of processes
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Show MPI info
    if(world_rank == 0)
    {
        DEBUG_PRINT_OUT("Processor_name " << processor_name);
        DEBUG_PRINT_OUT("Num of process " << world_size);
    }
    
    // Prepare input matrix
    MAT A = create_matrix(MAT_COLS-2, MAT_ROWS-2, 0.0);
    A = padding_matrix(A, MAT_COLS-2, MAT_ROWS-2, 1.0);

    // get matrix size
    int row_num, col_num;
    get_shape_matrix(A, row_num, col_num);

    // Prepare output matrix
    MAT B(A);

    // for each process, allocate A matrix divided by size
    int local_row_num;
    if(world_rank == (world_size - 1))
        local_row_num = (MAT_ROWS / world_size) + (MAT_ROWS % world_size);
    else
        local_row_num = (MAT_ROWS / world_size);
    
    // allocate local memory for each process
    // range = 'local_first_row' to 'local_end' - 1
    int local_first_row = world_rank * (MAT_ROWS / world_size);
    int local_end = local_first_row + local_row_num;
    MAT A_local = MAT(A.begin() + local_first_row, A.begin() + local_end);
    MAT B_local = MAT(B.begin() + local_first_row, B.begin() + local_end);

    bool converge = false;
    int iteration_num = 0;
    while(converge == false)
    {
        iteration_num++;
        double diffnorm = 0.0;

        // For convinience, padding A matrix 
        MAT A_local_padded = padding_matrix(A_local, local_row_num, col_num, 0);

        // Send and Recv border region of A
        if(world_rank > 0)
        {
            MPI_Send(A_local[0].data(), col_num, MPI_DOUBLE, world_rank-1, 11, MPI_COMM_WORLD);

            MPI_Status s;
            MAT_ROW tmp(col_num);
            MPI_Recv(tmp.data(), col_num, MPI_DOUBLE, world_rank-1, 22, MPI_COMM_WORLD, &s);
            for(int i = 0; i < col_num; i++)
                A_local_padded[0][i+1] = tmp[i];
        }
        if(world_rank < (world_size-1))
        {
            MPI_Send(A_local[local_row_num - 1].data(), col_num, MPI_DOUBLE, world_rank+1, 22, MPI_COMM_WORLD);

            MPI_Status s;
            MAT_ROW tmp(col_num);
            MPI_Recv(tmp.data(), col_num, MPI_DOUBLE, world_rank+1, 11, MPI_COMM_WORLD, &s);
            for(int i = 0; i < col_num; i++)
                A_local_padded[local_row_num+1][i+1] = tmp[i];
        }

        //Jacobi
        for(int i = 0; i < local_row_num; i++)
        {
            for(int j = 0; j < col_num; j++)
            {
                // because we do padding, index changed
                int idx_i_A = i+1;
                int idx_j_A = j+1;

                // Do jacobi
                B_local[i][j] = 0.25*(A_local_padded[idx_i_A+1][idx_j_A]
                                    + A_local_padded[idx_i_A-1][idx_j_A]
                                    + A_local_padded[idx_i_A][idx_j_A+1]
                                    + A_local_padded[idx_i_A][idx_j_A-1]);

                // simple converge test
                diffnorm += sqrt((B_local[i][j] - A_local[i][j])*(B_local[i][j] - A_local[i][j]));
            }
        }
        copy_matrix(B_local, A_local);

        // check converge
        double diffnorm_glov;
        MPI_Allreduce(&diffnorm, &diffnorm_glov, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        if(diffnorm_glov <= 0.0001)
        {
            if(world_rank == 0)
            {
                DEBUG_PRINT_OUT("Converge, iteration : " << iteration_num);
                DEBUG_PRINT_OUT("Error : " << diffnorm_glov);
            }
            converge = true;
        }
    }

    // To do MPI_Gather with std::vector<std::vector<...>>, need to convert type to pointer of double
    // It's tricky so if there a more good way. please share
    double* A_local_tmp = matrix2pointer(A_local, local_row_num, col_num);

    double* A_tmp = new double[row_num*col_num];
    MPI_Gather(A_local_tmp, local_row_num*col_num, MPI_DOUBLE, A_tmp, local_row_num*col_num, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if(world_rank == 0)
    {
        MAT A_result = pointer2matrix(A_tmp, row_num, col_num);
        print_matrix(A_result);
    }
    
    // Finalize the MPI environment.
    MPI_Finalize();

    // stop process execution time measure
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    DEBUG_PRINT_OUT("Process : " << world_rank << ", execution time : " << duration.count() << " us");
}