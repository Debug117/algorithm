#include <stdio.h>
#include <stdlib.h>

typedef int type;
typedef struct _node {
			type	data;
	struct _node*	next;
}node;

void add(node** list, type elem)
{
	node* tmp = (node*)malloc(sizeof(node));
	if(tmp){
		tmp->data = elem;
		tmp->next = NULL;
	} else {
		printf("malloc fail! \n");
		return ;
	}

	node* p = *list;
	if(p){
		while(p->next){
			p = p->next;
		}
		p->next = tmp;
	} else {
		*list = tmp;
	}
}

void display(node** list)
{
	node* p = *list;
	while(p){
		printf("%d ", p->data);
		p = p->next;
	}
	printf("\n");
}

node* reverse(node** list)
{
	if(!(*list) || !((*list)->next))
		return *list;
	node* ph = *list;		/* point to head */
	node* pc = ph->next;	/* point to current node */
	ph->next = NULL;
	node* pn = NULL;		/* point to next node */

	while(pc){
		pn = pc->next;
		pc->next = ph;
		ph = pc;
		pc = pn;
	}
	//*list = ph;
	return ph;
}

node * reverse2(node** list)
{
	node* p =*list;
	if(!p ||!(p->next)){
		return p;
	}
	node* tmp = p->next;
	node* head = reverse2(&(p->next));
	tmp->next = p;
	p->next =NULL;
	return head;
}
																	 
void release(node** list)
{
	node* p = *list;
	node* tmp =NULL;
	while(p){
		tmp = p->next;
		free(p);
		p = tmp;
	}
	*list = NULL;
}

node* merge(node** list1, node** list2)
{
	if(*list1 == NULL || *list2==NULL)
		return NULL;
	node* p1 =(*list1)->data < (*list2)->data ? (*list1):(*list2);
	node* p2 =(*list1)->data < (*list2)->data ? (*list2):(*list1);
	node* result =p1;
	while(p2){
		while(p1->next && p1->next->data <= p2->data){
			p1 = p1->next;
		}
		node* tmp =p1->next;
		p1->next =p2;
		p2 =tmp;
	}
	return result;
}

int max(node* list)
{
	if(list == NULL){
		return -1;
	}else{
		int tmp = max(list->next);
		return (list->data >= tmp)?(list->data):tmp;
	}
}

int length(node* list)
{
	if(!list){
		return 0;
	}else{
		return 1+length(list->next);
	}
}

int sum(node* list)
{
	if(!list){
		return 0;
	}else{
		return list->data + sum(list->next);
	}
}

int main()
{
	node* list1 = NULL;
	node* list2 = NULL;
	int c = 0;
	while(c++ < 10){
		add(&list1, c*c+1);
		add(&list2, c*12+1);
	}
	display(&list1);
	printf("max:%d  length:%d  sum:%d\n",max(list1),length(list1),sum(list1));
	display(&list2);
	printf("max:%d  length:%d  sum:%d\n",max(list2),length(list2),sum(list2));

	node* list3 = merge(&list1, &list2);
	display(&list3);

	node* list4 =reverse(&list3);
	display(&list4);

	node* list5 = reverse2(&list4);
	display(&list5);

	release(&list5);

	return 0;
}

