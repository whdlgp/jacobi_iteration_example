#include "utils/matrix_utils.h"
#include "common_setup.h"
#include <fstream>

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


    // For convinience, padding A matrix
    MAT A_padded = padding_matrix(A, row_num, col_num, 0);

    bool converge = false;
    int iteration_num = 0;
    while(converge == false)
    {
        iteration_num++;
        double diffnorm = 0.0;

        for(int i = 0; i < row_num; i++)
        {
            for(int j = 0; j < col_num; j++)
            {
                // because we do padding, index changed
                int idx_i_A = i+1;
                int idx_j_A = j+1;

                double pre_val, current_val;

                // Do Gauss-Seidel
                pre_val = A_padded[idx_i_A][idx_j_A];
                A_padded[idx_i_A][idx_j_A] = 0.25*(A_padded[idx_i_A+1][idx_j_A]
                                                 + A_padded[idx_i_A-1][idx_j_A]
                                                 + A_padded[idx_i_A][idx_j_A+1]
                                                 + A_padded[idx_i_A][idx_j_A-1]);
                current_val = A_padded[idx_i_A][idx_j_A];

                // simple converge test
                diffnorm += sqrt((pre_val - current_val)*(pre_val - current_val));
            }
        }

        if(diffnorm <= 0.0001)
        {
            DEBUG_PRINT_OUT("Converge, iteration : " << iteration_num);
            DEBUG_PRINT_OUT("Error : " << diffnorm);

            A = remove_border_matrix(A_padded, row_num+2, col_num+2);
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
    outfile << "single," << "gauss_seidel," << 1 << ',' << MAT_SIZE << ',' << converge << ',' << iteration_num << ',' << duration.count() << endl; 
}