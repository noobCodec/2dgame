#include "gfc_vector.h"
typedef struct Point Point;

struct Point
{
	int x;
	int y;
	Point *prev;
};


typedef struct {
	int front, rear, size;
	unsigned capacity;
	Point *array;
} Queue;

Point *init_point(int x,int y);
/*
 * Make a queue 
 */
Queue* createQueue(unsigned capacity);


/*
 * returns 1 if full or 0 otherwise
 */
int isFull(Queue* q);

/*
 * returns 1 if empty or 0 otherwise
 */
int isEmpty(Queue* q);

/*
 * push onto our queue
 */
void push(Queue* q,Point item);

/*
 * pop item from our queue
 */
Point *pop(Queue* q);

/*
 * return first elem in q
 */
Point *front(Queue* q);
/*
 * return last elem in q
 */
Point *rear(Queue* q);

void killqueue(Queue *q);

