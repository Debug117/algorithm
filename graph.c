#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

#define MAX		10
#define true	1
#define false	0

typedef struct _node {
		int		id;
		int		data;
} node;

typedef struct _graph{
		int		amount;
		node	point[MAX];
		char	arch[MAX][MAX];
}graph;

static char*    IsVisited = NULL;
static graph*			g = NULL;
static qnode*		queue = NULL;

void visit(graph* g, int id)
{
		if(g){
				printf("[ID] %d      [DATA] %c\n",id,(g->point[id]).data);
		}
}

int check_visited(graph* g)
{
		int i=0;
		for(i=0; i<g->amount; i++){
				if(*(IsVisited+i)==0)
						return false;
		}
		return true;
}

int DFS(graph* g, int ID)
{
		int i=0;
		if(ID > g->amount){
				printf("node id error\n");
				return -2;
		} else {
				visit(g,ID);
				*(IsVisited+ID)=1;
		}

		for(i=0; i<g->amount; i++){
				if(g->arch[ID][i]==1){
					if(*(IsVisited+i) ==false)
							DFS(g,i);
				}
		}
		return 0;
}

int BFS(graph* g, int ID)
{
		int i=0,d=0;
		if(ID > g->amount){
				printf("node id error\n");
				return -2;
		} else {
				if(*(IsVisited+ID)==false){
						enqueue(&queue,ID);
				} else {
						printf("first id has been visited!\n");
						return -1;
				}
		}

		while((d=dequeue(&queue)) != -1){
				if(*(IsVisited+d) == false){
						visit(g,d);
						*(IsVisited+d)=true;
				}
				for(i=0; i<g->amount; i++){
						if(g->arch[d][i]==1){
							if(*(IsVisited+i) ==false)
									enqueue(&queue,i);
						}
				}
		}

		return 0;
}

void set_arch(graph* g,int a, int b){
		if(g!=NULL){
				g->arch[a][b]=true;
				g->arch[b][a]=true;
		}
}

void init(graph** p)
{
		int i=0,j=0;
		graph* g = (graph*)malloc(sizeof(graph));
		g->amount=8;
		for(i=0; i<g->amount; i++){
				(g->point[i]).id = i;
				(g->point[i]).data = 'A'+i;
				for(j=0; j<g->amount; j++){
						g->arch[i][j]=false;
				}
		}
		set_arch(g,0,1);
		set_arch(g,0,2);
		set_arch(g,1,4);
		set_arch(g,1,3);
		set_arch(g,2,6);
		set_arch(g,2,5);
		set_arch(g,3,7);
		set_arch(g,4,7);
		set_arch(g,5,7);
		set_arch(g,6,7);
		
		*p = g;

		IsVisited = (char*)malloc(sizeof(char)*(g->amount));
		for(i=0; i<g->amount; i++){
				*(IsVisited+i)=false;
		}
}

int main()
{
		graph* p=NULL;
		init(&p);
		//DFS(p,0);
		BFS(p,0);
		return 0;
}
