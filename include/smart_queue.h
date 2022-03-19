#ifndef __SMART_QUEUE_H__
#define __SMART_QUEUE_H__

#include "gfc_vector.h"
#include "queue.h"
typedef struct VectorSpecial VectorSpecial;
struct VectorSpecial {
	Vector2D coords;
	VectorSpecial *parent;
};
typedef struct {
	int front, rear, size;
	unsigned capacity;
	VectorSpecial *array;
} SmartQueue;

void free_specVec(VectorSpecial* ptr);

VectorSpecial specVec(Vector2D,VectorSpecial* parent);
/*
 * Make a queue 
 */
SmartQueue* createQueueSpecial(unsigned capacity);


/*
 * returns 1 if full or 0 otherwise
 */
int smart_isFull(SmartQueue* q);

/*
 * returns 1 if empty or 0 otherwise
 */
int smart_isEmpty(SmartQueue* q);

/*
 * push onto our queue
 */
void smart_push(SmartQueue* q,VectorSpecial item);

/*
 * pop item from our queue
 */
VectorSpecial* smart_pop(SmartQueue* q);

/*
 * return first elem in q
 */
VectorSpecial* smart_front(SmartQueue* q);
/*
 * return last elem in q
 */
VectorSpecial* smart_rear(SmartQueue* q);

void smart_killqueue(SmartQueue *q);
#endif
