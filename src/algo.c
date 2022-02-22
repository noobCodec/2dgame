#define ROW 12
#define COL 20
#include "algo.h"
#include "gfc_list.h"
#include "simple_logger.h"

Point *init_point(int x,int y)
{
	Point *new = malloc(sizeof(Point));
	new->x = x;
	new->y = y;
	new->prev = NULL;
	return new;
}

int isValid(int *visited,int row,int col,int col_size,int row_size)
{ 
	if(row < 0 || col < 0 || row>=row_size || col>=col_size) return 0;
	
	if(visited[row*col_size + col])return 0;
	
	return 1;
}
List* backtrack(Point *x)
{
	List *q = gfc_list_new();
	
	while(x->prev)
	{
		q = gfc_list_append(q,init_point(x->x,x->y));
		x = x->prev;
	}
	q = gfc_list_append(q,init_point(x->x,x->y));
	return q;
}
List* BFS(int *grid,int *visited,int row,int col,int goal_row, int goal_col,int col_size,int row_size)
{
	List *q = gfc_list_new();
	
	List *tobefreed = gfc_list_new();
	int length = row_size;
	Point *ptr = init_point(row,col);
	tobefreed=gfc_list_append(tobefreed,ptr);
	q = gfc_list_append(q,ptr);
	while(gfc_list_get_count(q)!=0)
	{
		Point* cell = gfc_list_get_nth(q,0);
		gfc_list_delete_nth(q,0);
		int x = cell->x;
		int y = cell->y;
		if(x == goal_row && y == goal_col)
		{
			List *done = backtrack(cell);
			for(int i =0; i<gfc_list_get_count(tobefreed);i++)
			{
				Point *tmp = gfc_list_get_nth(tobefreed,i);
				free(tmp);
			}
			gfc_list_delete(tobefreed);
			gfc_list_delete(q);
			return done;
		}
		//x+1 y
		int adjx = x + 1;
		int adjy = y;
		if(isValid(visited,adjx,adjy,col_size,length) && (grid[adjx*col_size+adjy]!=-1))
		{
			ptr = init_point(adjx,adjy);
			tobefreed=gfc_list_append(tobefreed,ptr);
			//gfc_list_get_nth(grid,adjx*col_size+adjy);
			ptr->prev = cell;
			q = gfc_list_append(q,ptr);
			visited[adjx*col_size+y] = 1;
		}
		//x y + 1
		adjx = x;
		adjy = y + 1;
		if(isValid(visited,adjx,adjy,col_size,length) && (grid[adjx*col_size+adjy]!=-1))
		{
			ptr = init_point(adjx,adjy);
			tobefreed=gfc_list_append(tobefreed,ptr);
			//gfc_list_get_nth(grid,adjx*col_size+adjy);
			ptr->prev = cell;
			q = gfc_list_append(q,ptr);
			visited[adjx*col_size+y] = 1;
		}
		//x - 1  y
		adjx = x-1;
		adjy = y;
		if(isValid(visited,adjx,adjy,col_size,length) && (grid[adjx*col_size+adjy]!=-1))
		{
			ptr = init_point(adjx,adjy);
			tobefreed=gfc_list_append(tobefreed,ptr);
			//gfc_list_get_nth(grid,adjx*col_size+adjy);
			ptr->prev = cell;
			q = gfc_list_append(q,ptr);
			visited[adjx*col_size+y] = 1;
		}
		//x  y - 1
		adjx = x;
		adjy = y - 1;
		if(isValid(visited,adjx,adjy,col_size,length) && (grid[adjx*col_size+adjy]!=-1))
		{
			ptr = init_point(adjx,adjy);
			tobefreed=gfc_list_append(tobefreed,ptr);
			//gfc_list_get_nth(grid,adjx*col_size+adjy);
			ptr->prev = cell;
			q = gfc_list_append(q,ptr);
			visited[adjx*col_size+y] = 1;
		}
	}
	for(int i =0; i<gfc_list_get_count(tobefreed);i++)
	{
	Point *tmp = gfc_list_get_nth(tobefreed,i);
	free(tmp);
	}
	gfc_list_delete(tobefreed);
	gfc_list_delete(q);
	return NULL;
}


