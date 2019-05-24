#include "matrix_utils.h"
using namespace std;

// Create matrix with std vector
MAT create_matrix(int row_num, int col_num, double val)
{
    MAT mat(row_num, MAT_ROW(col_num, val));
    return mat;
}

// Get size of matrix, rows and columns
void get_shape_matrix(MAT& mat, int& rows, int& cols)
{
    rows = mat.size();
    cols = mat[0].size();
}

// Padding Matrix, impemented with std::vector
// origin_mat: origin_mat matrix to pad
// rows, cols: original size
// pad_val: border regions value
MAT padding_matrix(MAT& origin_mat, int rows, int cols, double pad_val)
{
    // Output matrix
    vector<vector<double>> padded;

    // For padding, prepare top/bottom vectors
    vector<double> row_top_bottom(cols+2, pad_val);
    
    // Padding first row
    padded.push_back(row_top_bottom);

    // Padding first column and last column
    for(int i = 0; i < rows; i++)
    {
        // For padding, prepare buffer for column wize padding
        vector<double> tmp(cols+2);
        // Add padding value
        tmp[0] = pad_val; 
        tmp[cols + 1] = pad_val;
        //insert original matrix i's row values
        for(int col_idx = 0; col_idx < cols; col_idx++)
            tmp[col_idx+1] = origin_mat[i][col_idx];
        //insert padded rows
        padded.push_back(tmp);
    }

    // Padding last row
    padded.push_back(row_top_bottom);

    return padded;
}

// Print out Matrix implemented with std::vector
void print_matrix(MAT& mat)
{
    int rows_num = mat.size();
    int cols_num = mat[0].size();

    for(int rows = 0; rows < rows_num; rows++)
    {
        for(int cols = 0; cols < cols_num; cols++)
        {
            cout << mat[rows][cols] << "\t";
        }
        cout << endl;
    }
}

// Copy src matrix to dst matrix
void copy_matrix(MAT& src, MAT& dst)
{
    for(int i = 0; i < src.size(); i++)
        dst[i].assign(src[i].begin(), src[i].end());
}

// Copy MAT(std::vector) to array pointer
double* matrix2pointer(MAT& mat, int row_num, int col_num)
{
    double* mat_tmp = new double[row_num*col_num];
    for(int i = 0; i < row_num; i++)
        memcpy(mat_tmp + i*col_num, mat[i].data(), sizeof(double)*col_num);

    return mat_tmp;
}

// Copy array pointer to MAT(std::vector)
MAT pointer2matrix(double* mat_tmp, int row_num, int col_num)
{
    MAT mat;
    for(int i = 0; i < row_num; i++)
    {
        MAT_ROW tmp(mat_tmp + i*col_num, mat_tmp + i*col_num + col_num);
        mat.push_back(tmp);
    }

    return mat;
}
