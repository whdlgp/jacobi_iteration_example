#pragma once

#ifndef MAT_SIZE
#define MAT_SIZE 40
#endif
enum
{
    MAT_ROWS = MAT_SIZE,
    MAT_COLS = MAT_SIZE,
};

#define PRINT_MAT false

#ifndef OMP_THREAD_NUM
#define OMP_THREAD_NUM 2
#endif
