#include "queue.h"
#include "simple_logger.h"
Queue* createQueue(unsigned capacity)
{
	Queue* q = malloc(sizeof(Queue));
	q->capacity = capacity;
	q->front = q->size = 0;
	
	q->rear = capacity-1;
	q->array = malloc(capacity * sizeof(Point));
	memset(q->array,0,capacity*sizeof(Point));
	return q;
}
Point *init_point(int x,int y)
{
	Point *new = malloc(sizeof(Point));
	new->x = x;
	new->y = y;
	new->prev = NULL;
	return new;
}
int isFull(Queue *q)
{
	return (q->size == q->capacity);
}
int isEmpty(Queue *q)
{
	return q->size == 0;
}
void push(Queue *q, Point item)
{
	if(isFull(q)){
	 	q->capacity *= 2;
	 	Point *tmp = realloc(q->array,q->capacity*sizeof(Point));
	 	if(tmp)
	 		q->array = tmp;
	 	else
	 		{
	 		slog("NO MEMORY");
	 		return;
	 		}
	}
	q->rear = (q->rear+1) % q->capacity;
	//Vector2D *live_on = malloc(sizeof(Vector2D));
	//live_on->x = item.x;
	//live_on->y = item.y;
	q->array[q->rear] = item;
	q->size = q->size+1;
}
Point *pop(Queue *q)
{
	if(isEmpty(q)) return NULL;
	Point *item = &q->array[q->front];
	q->front = (q->front + 1) % q->capacity;
	q->size = q->size - 1;
	return item;
}
Point *front(Queue *q)
{
	if(isEmpty(q)) return NULL;
	return &q->array[q->front];
}
Point *rear(Queue *q)
{
	if(isEmpty(q)) return NULL;
	return &q->array[q->rear];
}
void killqueue(Queue *q)
{
	if(!q)
		return;
	free(q->array);
	free(q);
	q = NULL;
}
