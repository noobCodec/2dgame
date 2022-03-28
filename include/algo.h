#ifndef __ALGO_H__
#define __ALGO_H__
#include "gfc_vector.h"
#include "gfc_list.h"
typedef struct Point Point;

struct Point
{
	int x;
	int y;
	Point *prev;
};

typedef struct point_p
{
	int x;
	int y;
	int cost;
	struct point_p *prev;
} costPoint;

typedef struct node_s
{
	costPoint *point;
	int priority;
	struct node_s *next;
} Node;

costPoint *init_cost_point(int x, int y, int cost);
int isEmpty(Node **head);
Node* newNode(costPoint *p,int prio);
costPoint *pop(Node **head);
void push(Node** head,costPoint *p, int prio);
Point *init_point(int x,int y);
int isValid(int *visited,int row,int col,int col_size,int row_size);
void reprio(Node **head, costPoint *p, int prio);
List* backtrack(costPoint *x);
List* BFS(int *grid,int row,int col,int goal_row,int goal_col,int col_size,int row_size);

#endif
