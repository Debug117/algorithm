#include <stdio.h>
#include <stdlib.h>


typedef int type;
typedef struct node {
		type			data;
		struct node *	next;
}qnode;

void enqueue(qnode** queue, type elem);
void display(qnode** queue);
type dequeue(qnode** queue);
