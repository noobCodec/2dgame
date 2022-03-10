#ifndef __ALGO_H__
#define __ALGO_H__
#include "gfc_vector.h"
#include "gfc_list.h"
#include "queue.h"

int isValid(int *visited,int row,int col,int col_size,int row_size);
List* backtrack(Point *x);
List* BFS(int *grid,int *visited,int row,int col,int goal_row,int goal_col,int col_size,int row_size);

#endif
