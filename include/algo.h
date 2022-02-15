#include "smart_queue.h"
#include "gfc_vector.h"
int isValid(int *visited,int row,int col);
Queue* BFS(int *grid,int *visited,int row,int col,int goal_row,int goal_col);
