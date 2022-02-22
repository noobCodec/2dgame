#include "smart_queue.h"
#include "gfc_vector.h"
#include "gfc_list.h"
typedef struct Point Point;

struct Point
{
	int x;
	int y;
	Point *prev;
};
Point *init_point(int x, int y);
int isValid(int *visited,int row,int col,int col_size,int row_size);
List* backtrack(Point *x);
List* BFS(int *grid,int *visited,int row,int col,int goal_row,int goal_col,int col_size,int row_size);
