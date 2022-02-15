#include "gfc_vector.h"

typedef struct {
	int front, rear, size;
	unsigned capacity;
	Vector2D *array;
} Queue;

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
void push(Queue* q,Vector2D item);

/*
 * pop item from our queue
 */
Vector2D *pop(Queue* q);

/*
 * return first elem in q
 */
Vector2D *front(Queue* q);
/*
 * return last elem in q
 */
Vector2D *rear(Queue* q);

void killqueue(Queue *q);

