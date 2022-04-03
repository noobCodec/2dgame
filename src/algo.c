#define ROW 12
#define COL 20
#include "algo.h"
#include "gfc_list.h"
#include "simple_logger.h"
#include <math.h>


void debug(int *map,int cols,int rows)
{
	int i;
	for (i = 0; i < cols*rows;i++)
	{
	if(i!=0 && i%(cols)==0)
	{
		printf("\n");
	}
	printf("%2d ",map[i]);
	
	}
	printf("\n\n\n");
	return;
}
int isEmpty(Node **head) {return (*head) == NULL;}

Node* newNode(costPoint *p,int prio)
{
	Node* tmp = malloc(sizeof(Node));
	tmp->point = p;
	tmp->priority = prio;
	tmp->next = NULL;
	return tmp;
}


costPoint *pop(Node **head)
{
	if(isEmpty(head)) return NULL;
	Node *tmp = *head;
	costPoint *p = tmp->point;
	(*head) = (*head)->next;
	free(tmp);
	return p;
}

void push(Node** head,costPoint *p, int prio)
{
	Node *start = *head;
	Node *tmp = newNode(p,prio);
	if((*head) == NULL)
	{
		(*head) = tmp;
	}
	else if((*head)->priority > prio)
	{
		tmp->next = *head;
		(*head) = tmp;
	}
	else
	{
		while(start->next != NULL && start->next->priority <= prio)
		{
			start = start->next;
		}
		tmp->next = start->next;
		start->next = tmp;
	}
}


costPoint *init_cost_point(int x, int y, int cost)
{
	costPoint *tmp = malloc(sizeof(costPoint));
	tmp->x = x;
	tmp->y = y;
	tmp->prev = NULL;
	tmp->cost = cost;
	return tmp;
}


Point *init_point(int x,int y)
{
	Point *new = malloc(sizeof(Point));
//     slog("%ld",sizeof(Point));
	new->x = x;
	new->y = y;
	new->prev = NULL;
	return new;
}

void reprio(Node **head, costPoint *p, int prio)
{
	if((*head) == NULL)return;
	
	Node *start = *head;
	Node *prev = NULL;
	while(start->next != NULL)
	{
		if(start->point->x == p->x && start->point->y == p->y)
		{
			if(prev!=NULL)
			{
				prev->next = start->next;
			}
			free(start);
			break;
		}
		prev = start;
		start = start->next;
		
	}
	push(head,p,prio);
}
int isValid(int *visited,int row,int col,int col_size,int row_size)
{ 
	if(row < 0 || col < 0 || row>=row_size || col>=col_size) return 0;
	return 1;
}
List* backtrack(costPoint *x)
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

int heuristic(int row, int col, int goal_row,int goal_col)
{
	int x = goal_row - row;
	int y = goal_col - col;
	return sqrt(x*x + y*y);
}
List* BFS(int *grid,int row,int col,int goal_row, int goal_col,int col_size,int row_size)
{
//     slog("looking");
	if(grid[row*col_size+col]==-1 || grid[goal_row*col_size+goal_col]==-1) return NULL;
	int *visited = malloc(sizeof(int)*col_size*row_size);
	memset(visited,0,sizeof(int)*col_size*row_size);
	Node *pq;
	List *tobefreed = gfc_list_new();
	int length = row_size;
	costPoint *ptr = init_cost_point(row,col,0);
	tobefreed=gfc_list_append(tobefreed,ptr);
	int adjx[] = {-1,1,0,0};
	int adjy[] = {0,0,-1,1};
	pq = newNode(ptr,0);
	while(!isEmpty(&pq))
	{
		//slog("%d:%d",pq->point->x,pq->point->y);
		
		costPoint* cell = pop(&pq);
		int x = cell->x;
		int y = cell->y;
		if(x == goal_row && y == goal_col)
		{
/*		debug(visited,col_size,row_size);*/
			List *done = backtrack(cell);
			while(!isEmpty(&pq)) pop(&pq);
			for(int i =0; i<gfc_list_get_count(tobefreed);i++)
			{
				costPoint *tmp = gfc_list_get_nth(tobefreed,i);
				free(tmp);
			}
			gfc_list_delete(tobefreed);
			free(visited);
			return done;
		}
		for(int i = 0; i < 4; i ++)
		{
			int xcoord = x + adjx[i];
			int ycoord = y + adjy[i];
			int new_cost = cell->cost + grid[xcoord*col_size+ycoord];
			if(isValid(visited,xcoord,ycoord,col_size,length) && (grid[xcoord*col_size+ycoord]!=-1) && ((visited[xcoord*col_size+ycoord]==0) || new_cost < visited[xcoord*col_size+ycoord]))
			{
				ptr = init_cost_point(xcoord,ycoord,new_cost);
				tobefreed=gfc_list_append(tobefreed,ptr);
				ptr->prev = cell;
				push(&pq,ptr,heuristic(goal_row,goal_col,xcoord,ycoord));
				visited[xcoord*col_size+ycoord] = new_cost;
			}
		}
		
	}
	while(!isEmpty(&pq)) pop(&pq);
	for(int i =0; i<gfc_list_get_count(tobefreed);i++)
	{
		costPoint *tmp = gfc_list_get_nth(tobefreed,i);
		free(tmp);
	}
	gfc_list_delete(tobefreed);
	free(visited);
	return NULL;
}


