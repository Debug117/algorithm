#include "queue.h"

void enqueue(qnode** queue, type elem)
{
		qnode*	tmp = (qnode*)malloc(sizeof(qnode));
		tmp->data = elem;
		tmp->next = NULL;

		qnode*	q = *queue;
		if(q){
				while(q->next) {
						q = q->next;
				}
				q->next = tmp;
		} else {
				*queue = tmp;
		}
}

void display(qnode** queue)
{
		qnode* q = *queue;
		if(!q){
				printf("queue empty!\n");
				return ;
		}
		while(q){
				printf("%d ",q->data);
				q = q->next;
		}
		printf("\n");
}

type dequeue(qnode** queue)
{
		qnode* q = *queue;
		type  getv= -1;
		if(q){
				getv = q->data;
				*queue = q->next;
				free(q);
		}
		return getv;
}

