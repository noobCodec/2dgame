#include "smart_queue.h"

VectorSpecial specVec(Vector2D coords,VectorSpecial* parent)
{
	VectorSpecial vec;
	vec.coords = coords;
	vec.parent = parent;
	return vec;
}

SmartQueue* createQueueSpecial(unsigned capacity)
{
	SmartQueue* q = malloc(sizeof(SmartQueue));
	q->capacity = capacity;
	q->front = q->size = 0;
	
	q->rear = capacity-1;
	q->array = malloc(capacity * sizeof(VectorSpecial));
	return q;
}
void smart_killqueue(SmartQueue *q)
{
	if(!q)
		return;
	free(q->array);
	free(q);
}
int smart_isFull(SmartQueue *q)
{
	return (q->size == q->capacity);
}
int smart_isEmpty(SmartQueue *q)
{
	return q->size == 0;
}
void smart_push(SmartQueue *q, VectorSpecial item)
{
	if(smart_isFull(q)){
	 	q->capacity *= 2;
	 	VectorSpecial *tmp = realloc(q->array,q->capacity*sizeof(VectorSpecial));
	 	if(tmp)
	 		q->array = tmp;
	 	else
	 		return;
	}
	q->rear = (q->rear+1) % q->capacity;
	q->array[q->rear] = item;
	q->size++;
}
VectorSpecial *smart_pop(SmartQueue *q)
{
	if(smart_isEmpty(q)) return NULL;	
	VectorSpecial *item = &q->array[q->front];
	q->front = (q->front + 1) % q->capacity;
	q->size--;
	return item;
}
VectorSpecial *smart_front(SmartQueue *q)
{
	if(smart_isEmpty(q))
	{
	return NULL;	
	}
	return &q->array[q->front];
}
VectorSpecial *smart_rear(SmartQueue *q)
{
	if(smart_isEmpty(q)) return NULL;	
	return &q->array[q->rear];
}
