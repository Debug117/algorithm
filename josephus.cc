#include <stdio.h>
#include <stdlib.h>
#include <set>

typedef struct _node{
	int		id;
	struct _node*	next;
}node;

void init(node** list, int count)
{
	node* p = *list;
	int i = 1;
	while(i<=count){
		node* tmp = (node*)malloc(sizeof(node));
		tmp->id = i;
		tmp->next = NULL;
		if(p == NULL){
			*list = tmp;
			p =	*list;
		}else{
			p->next = tmp;
			p = p->next;
		}
		i++;
	}
	if(!p->next){
		p->next =*list;
		printf("[Init OK]\n");
	}
}

void display(node* list)
{
	std::set<node*> trace;
	while(list){
		if(trace.count(list)>0){
			break;
		}
		trace.insert(list);
		printf("%d ",list->id);
		list = list->next;
	}
	printf("\n");
}

void josephus(node** list, int interval)
{
	if(interval >4 || interval<1){
		return ;
	}
	node* head =(node*)malloc(sizeof(node));
	head->next = *list;
	node* p = head;
	while(p){
		int i=1;
		while(i++<interval){
			p = p->next;
		}
		if(p ==p->next){
			printf("\n");
			break;
		}
		if(p->next){
			node* tmp = p->next;
			p->next = p->next->next;
			printf("[%d] ",tmp->id);
			if(*list == tmp){
				*list = p->next;
			}
			free(tmp);
		}
	}
	free(head);
}

int main()
{
	node* list =NULL;
	init(&list, 30);
	display(list);
	josephus(&list,3);
	display(list);

	return 0;
}
