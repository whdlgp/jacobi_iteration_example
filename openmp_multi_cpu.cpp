#include "utils/matrix_utils.h"
#include "common_setup.h"
#include <fstream>
#include <omp.h>

using namespace std;

int main(int argc, char** argv) 
{
    // start process execution time measure
    auto start = chrono::high_resolution_clock::now();

    // Prepare input matrix
    MAT A = create_matrix(MAT_COLS-2, MAT_ROWS-2, 0.0);
    A = padding_matrix(A, MAT_COLS-2, MAT_ROWS-2, 1.0);

    // get matrix size
    int row_num, col_num;
    get_shape_matrix(A, row_num, col_num);

    // Prepare output matrix
    MAT B(A);

    bool converge = false;
    int iteration_num = 0;
    //THREAD_NUM
    omp_set_num_threads(OMP_THREAD_NUM);
    while(converge == false)
    {
        iteration_num++;
        double diffnorm = 0.0;

        // For convinience, padding A matrix
        MAT A_padded = padding_matrix(A, row_num, col_num, 0);

        // 2 Nested for loop, local sum 'diffnorm' and reduction with operator '+' 
        #pragma omp parallel for reduction(+:diffnorm)
        for(int i = 0; i < MAT_ROWS; i++)
        {
            for(int j = 0; j < MAT_COLS; j++)
            {
                // because we do padding, index changed
                int idx_i_A = i+1;
                int idx_j_A = j+1;

                // Do jacobi
                B[i][j] = 0.25*(A_padded[idx_i_A+1][idx_j_A]
                              + A_padded[idx_i_A-1][idx_j_A]
                              + A_padded[idx_i_A][idx_j_A+1]
                              + A_padded[idx_i_A][idx_j_A-1]);

                // simple converge test
                diffnorm += sqrt((B[i][j] - A[i][j])*(B[i][j] - A[i][j]));
            }
        }
        copy_matrix(B, A);

        if(diffnorm <= 0.0001)
        {
            DEBUG_PRINT_OUT("Converge, iteration : " << iteration_num);
            DEBUG_PRINT_OUT("Error : " << diffnorm);
            converge = true;
        }
    }

    if(PRINT_MAT)
        print_matrix(A);

    // stop process execution time measure
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    DEBUG_PRINT_OUT("Execution time : " << duration.count() << " us");

    ofstream outfile;
    outfile.open("log.txt", std::ios_base::app);
    outfile << "openmp," << "jacobi," << OMP_THREAD_NUM << ',' << MAT_SIZE << ',' << converge << ',' << iteration_num << ',' << duration.count() << endl; 
}