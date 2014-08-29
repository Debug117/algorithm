#include "stack.h"

extern void enqueue(qnode** q, type elem);

void push(qnode** stack, type elem)
{
		enqueue(stack,elem);
}

type pop(qnode** stack)
{
		int getv = -1;
		qnode* q = *stack;
		if(!q){
				return -1;
		}
		if(q->next == NULL){
				getv = q->data;
				free(q);
				*stack = NULL;
				return getv;
		}
		while(q->next->next){
				q = q->next;
		}
		getv = q->next->data;
		free(q->next);
		q->next=NULL;
		return getv;
}
