#define ROW 12
#define COL 20
#include "algo.h"
#include "gfc_list.h"
int isValid(int *visited,int row,int col)
{
	if(row < 0 || col < 0 || row>=12 || col>=20) return 0;
	
	if(visited[row*20 + col])return 0;
	
	return 1;
}
Queue* backtrack(VectorSpecial sol)
{
	Queue *q = createQueue(1000);
	while(sol.parent)
	{
		push(q,sol.coords);
		sol = *sol.parent;
	}
	push(q,sol.coords);
	return q;
}
Queue* BFS(int *grid, int *visited,int row,int col,int goal_row, int goal_col)
{
	SmartQueue *q = createQueueSpecial(20000);
	Queue *tmq;
	smart_push(q,specVec(vector2d(row,col),NULL));
	while(!smart_isEmpty(q))
	{
		VectorSpecial* cell = smart_pop(q);
		int x = cell->coords.x;
		int y = cell->coords.y;
		if(x == goal_row && y == goal_col)
		{
		tmq = backtrack(*cell);
		smart_killqueue(q);
		return tmq;
		}
		//x+1 y
		int adjx = x + 1;
		int adjy = y;
		if(isValid(visited,adjx,adjy) && grid[adjx*20+adjy]!= -1)
		{
			smart_push(q,specVec(vector2d(adjx,adjy),cell));
			visited[adjx*20+y] = 1;
		}
		//x y + 1
		adjx = x;
		adjy = y + 1;
		if( isValid(visited,adjx,adjy) && grid[adjx*20+adjy]!= -1)
		{
			smart_push(q,specVec(vector2d(adjx,adjy),cell));
			visited[adjx*20+y] = 1;
		}
		//x - 1  y
		adjx = x-1;
		adjy = y;
		if( isValid(visited,adjx,adjy) && grid[adjx*20+adjy]!= -1)
		{
			smart_push(q,specVec(vector2d(adjx,adjy),cell));
			visited[adjx*20+y] = 1;
		}
		//x  y - 1
		adjx = x;
		adjy = y - 1;
		if(isValid(visited,adjx,adjy)&& grid[adjx*20+adjy]!= -1)
		{
			smart_push(q,specVec(vector2d(adjx,adjy),cell));
			visited[adjx*20+y] = 1;
		}
	}
	smart_killqueue(q);
	return NULL;
}


