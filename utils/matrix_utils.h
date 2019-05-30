#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>

#include "debug_print.h"

typedef std::vector<std::vector<double>> MAT;
typedef std::vector<double> MAT_ROW;

MAT     create_matrix(int row_num, int col_num, double val);
void    get_shape_matrix(MAT& mat, int& rows, int& cols);
MAT     padding_matrix(MAT& origin_mat, int rows, int cols, double pad_val);
MAT     remove_border_matrix(MAT& origin_mat, int rows, int cols);
void    print_matrix(MAT& mat);
void    copy_matrix(MAT& src, MAT& dst);
double* matrix2pointer(MAT& mat, int row_num, int col_num);
MAT     pointer2matrix(double* mat_tmp, int row_num, int col_num);