#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <queue>
#include <set>
using namespace std;
#define MAX	9

typedef struct _edge{
	int		weight;
	int		to;
	struct _edge*	next;
} edge;

typedef struct _node{
	int		info;
	struct _edge* firstedge;
} node;

typedef enum graphType{
	UNDIRECTED =0,
	DIRECTED
}gtype;

typedef struct _Graph{
	int		nodes;
	int		edges;
	gtype	type;
	node	node[MAX];
} Graph;


void addEdge(Graph** G, int n1, int n2, int wt)
{
	Graph* g = *G;
	if(!g){
		return ;
	}
	if(n1>g->nodes || n2> g->nodes){
		return ;
	}
	edge* tmp = (edge*)malloc(sizeof(edge));
	tmp->to = n2;
	tmp->next = NULL;
	tmp->weight = wt;
	edge* e1 = (g->node[n1]).firstedge;
	if(!e1){
		(g->node[n1]).firstedge = tmp;
	}else{
		while(e1->next){
			e1 = e1->next;
		}
		e1->next = tmp;
	}

	if(g->type == UNDIRECTED){
		tmp = (edge*)malloc(sizeof(edge));
		tmp->to = n1;
		tmp->next = NULL;
		tmp->weight = wt;
		edge* e2 = (g->node[n2]).firstedge;
		if(!e2){
			(g->node[n2]).firstedge = tmp;
		}else{
			while(e2->next){
				e2 = e2->next;
			}
			e2->next = tmp;
		}
	}
}

void visitNode(Graph* g, int node)
{
	if(!g){
		return ;
	}
	printf("%d  ", (g->node[node]).info);
}

int indegree(Graph* g, int id)
{
	if(!g || id>=g->nodes ||g->type != DIRECTED){
		return -1;
	}
	int indegree =0;
	for(int i=0; i<g->nodes; i++){
		edge* tmp = (g->node[i]).firstedge;
		while(tmp){
			if(tmp->to == id){
				indegree++;
				break;
			}
			tmp = tmp->next;
		}
	}
	return indegree;
}

void BFS(Graph* g, int firstNode)
{
	if(!g || firstNode>=g->nodes){
		return ;
	}
	bool* isVisited = new bool[g->nodes];
	for(int i =0; i<g->nodes; i++){
		isVisited[i] = false;
	}

	queue<int> qu;
	qu.push(firstNode);
	isVisited[firstNode] = true;

	while(qu.empty()==false){
		int front = qu.front();
		visitNode(g, front);
		qu.pop();

		edge* tmp = (g->node[front]).firstedge;
		while(tmp){
			if(isVisited[tmp->to]==false){
				qu.push(tmp->to);
				isVisited[tmp->to] = true; /* important */
			}
			tmp = tmp->next;
		}
	}
	delete []isVisited;
}

static bool visited[9]={0,0,0,0,0,0,0,0,0};
void DFS(Graph* g, int id)
{
	if(!g || id>=g->nodes){
		return ;
	}
	if((g->node[id]).firstedge == NULL){
		return ;
	}
	if(visited[id]==false){
		visitNode(g, id);
		visited[id] = true;
	}

	edge* tmp = (g->node[id]).firstedge;
	int toVisit = tmp->to;
	while(tmp){
		toVisit = tmp->to;
		if(visited[toVisit] == false){
			DFS(g, toVisit); /* important */
		}
		tmp = tmp->next;
	}
}

void topSort(Graph* g)
{
	if(!g || g->type!=DIRECTED){
		return ;
	}
	queue<int>qu;
	int *indegrees =new int[g->nodes];
	for(int i=1; i<g->nodes; i++){
		if((indegrees[i] = indegree(g,i))==0){
			qu.push(i);
		}
	}
	while(qu.empty() ==false)
	{
		int tmpN = qu.front();
		qu.pop();
		visitNode(g, tmpN);

		edge* tmp = (g->node[tmpN]).firstedge;
		while(tmp){
			indegrees[tmp->to]--;
			if(indegrees[tmp->to] ==0){
				qu.push(tmp->to);
			}
			tmp = tmp->next;
		}
	}
	for(int i=1;i<g->nodes; i++){
		if(indegrees[i]!=0){
			cout<<"[ERROR] Graph has a cycle"<<endl;
			break;
		}
	}
}

int prim(Graph* g, int first)
{
	if(!g || first>=g->nodes){
		return -1;
	}
	int minWeight = 0;
	set<int> st;
	set<int>::iterator it;
	st.insert(first);

	while(st.size() < g->nodes-1){
		it = st.begin();
		int minW = 100;//((g->node[*it]).firstedge)->weight;
		int minFrom = *it;
		int minTo = ((g->node[*it]).firstedge)->to;
		while(it != st.end()){
			edge* tmp = (g->node[*it]).firstedge;
			while(tmp){
				if(tmp->weight < minW && st.count(tmp->to)==0 ){
					minFrom = *it;
					minTo = tmp->to;
					minW = tmp->weight;
				}
				tmp =tmp->next;
			}
			it++;
		}
		st.insert(minTo);
		minWeight += minW;
	}
	return minWeight;
}

void initG_1(Graph** G)
{
	if(!(*G)){
		*G = (Graph*)malloc(sizeof(Graph));
	}
	(*G)->nodes = 8;
	(*G)->type = UNDIRECTED;
	int i=0;
	for(i=0; i<(*G)->nodes; i++){
		((*G)->node[i]).info = i;
		((*G)->node[i]).firstedge = NULL;
	}

	addEdge(G, 1,2,2);
	addEdge(G, 1,3,4);
	addEdge(G, 1,4,1);
	addEdge(G, 2,4,3);
	addEdge(G, 2,5,10);
	addEdge(G, 3,4,2);
	addEdge(G, 3,6,5);
	addEdge(G, 4,5,7);
	addEdge(G, 4,6,8);
	addEdge(G, 4,7,4);
	addEdge(G, 5,7,6);
	addEdge(G, 6,7,1);
}

void initG_2(Graph** G)
{
	if(!(*G)){
		*G = (Graph*)malloc(sizeof(Graph));
	}
	(*G)->nodes = 8;
	(*G)->type = DIRECTED;
	int i=0;
	for(i=0; i<(*G)->nodes; i++){
		((*G)->node[i]).info = i;
		((*G)->node[i]).firstedge = NULL;
	}

	addEdge(G, 1,2,0);
	addEdge(G, 1,3,0);
	addEdge(G, 1,4,0);
	addEdge(G, 2,4,0);
	addEdge(G, 2,5,0);
	addEdge(G, 3,6,0);
	addEdge(G, 4,3,0);
	addEdge(G, 4,6,0);
	addEdge(G, 4,7,0);
	addEdge(G, 5,4,0);
	addEdge(G, 5,7,0);
	addEdge(G, 7,6,0);
}

int main()
{
	Graph * g1 =NULL;
	initG_1(&g1);
	cout<<"Breadth First Search:"<<endl;	BFS(g1, 1);		cout<<endl;
	cout<<"Depth First Search:"<<endl;		DFS(g1, 1);		cout<<endl;
	cout<<"prim: "<<prim(g1, 1)<<endl;
	
	Graph * g2 =NULL;
	initG_2(&g2);
	cout<<"TopSort:"<<endl;		topSort(g2);  cout<<endl;
	return 0;
}
