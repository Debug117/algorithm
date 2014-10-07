#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <set>

typedef struct _node{
			  int	data;
	struct _node*	next;
}node;

void add(node** list, int elem)
{
	node* p =*list;
	node* tmp = (node*)malloc(sizeof(node));
	if(!tmp){
		return ;
	}
	tmp->data =elem;
	tmp->next =NULL;

	if(p){
		while(p->next){
			p = p->next;
		}
		p->next =tmp;
	}else{
		*list =tmp;
	}
}

int del(node** target)
{
	node* p =*target;
	if(p==NULL || p->next ==NULL){
		return -1;
	}
	node * tmp =p->next;
	p->data = tmp->data;
	p->next = tmp->next;
	free(tmp);
	return 0;
}

void print(node** list)
{
	node* p =*list;
	while(p){
		printf("%d ",p->data);
		p = p->next;
	}
	printf("\n");
}

node* merge(node** list1, node** list2)
{
	if(*list1 == NULL || NULL== *list2){
		return NULL;
	}
	node* p1 =(*list1)->data < (*list2)->data? (*list1):(*list2);
	node* p2 =(*list1)->data < (*list2)->data? (*list2):(*list1);
	node* result =p1;

	while(p2){
		while(p1->next && (p1->next->data) <= (p2->data)){
			p1 = p1->next;
		}
		node* tmp = p1->next;
		p1->next = p2;
		p2 = tmp;
	}
	return result;
}

int crossed(node**list1, node**list2)
{
	node* p1 =*list1;
	node* p2 =*list2;
	if(p1){
		while(p1->next){
			p1 = p1->next;
		}
	}
	if(p2){
		while(p2->next){
			p2 = p2->next;
		}
	}
	if(p1 == p2){
		return 0;
	}else{
		return -1;
	}
}

int loop(node**list, int pos)
{
	node* p =*list;
	int n=0;
	while(n<pos && p){
		p = p->next;
		n++;
	}
	if(n<pos){
		return -1;
	}
	node* cur = p;
	while(p->next){
		p = p->next;
	}
	p->next = cur;
	return 0;
}

int isloop(node** list)
{
	node* p1 = *list;
	node* p2 = p1;
	while(p1 && p2){
		if(p1->next){
			p1 = p1->next;
		}
		if(p2->next && p2->next->next){
			p2 = p2->next->next;
		}else{
			break;
		}
		if(p1 == p2 && p1 != NULL ){
			return 0;
		}
	}
	return -1;
}

int count(node** list)
{
	node* p =*list;
	std::set<node*> marked;
	while(p){
		marked.insert(p);
		p = p->next;
		if(p==NULL || marked.count(p)>0){
			break;
		}
	}
	return marked.size();
}

void unloop(node** list)
{
	if(isloop(list)!=0)
		return ;
	node* p =*list;
	std::set<node*> mark;
	if(p){
		mark.insert(p);
	}
	while(p->next){
		if(mark.count(p->next)>0){
			break;
		}
		p = p->next;
	}
	p->next =NULL;
}

node* reverse(node** list)
{
	node* p = *list;
	node* tmp = NULL;
	if(p){
		p->next = NULL;
	}
	if(p->next){
		tmp = reverse(&(p->next));
		tmp ->next =  p;
	}else{
		return p;
	}
}

node* insert_sort(node** list)
{
	node* p = *list;
	if(!p||!(p->next)){
		return p;
	}
	node *pos = p->next;
	/*
	while(pos){
		node* start = p;

		while(start->data < pos->data && start!=pos){
			start = start->next;
		}
		pos = pos->next;
	}
	*/
}

int main()
{
	node* list1 = NULL;
	node* list2 = NULL;
	node* list3 = NULL;
	node* list4 = NULL;
	int i =0;
	while(i++<10){
		add(&list1, i*i*i+1);
		add(&list2, i+100);
		add(&list4, random()%100);
	}
	print(&list1);
	print(&list2);

	if(isloop(&list1)){
		printf("list1[%d]: ",count(&list1));
		print(&list1);
	}

	loop(&list2,0);
	if(!isloop(&list2)){
		printf("list2:[%d] \n",count(&list2));
		unloop(&list2);
		print(&list2);
	}
	

	/*
	printf("crossed: %d\n",crossed(&list1, &list2));

	list3 = merge(&list1, &list2);
	print(&list3);
	*/

	print(&list4);

	return 0;
}
