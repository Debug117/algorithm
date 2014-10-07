#include <stdio.h>
#include <stdlib.h>
#include <queue>

typedef struct _node{
	int				data;
	struct _node	*left, *right;
}node;

void insert(node** btree, int elem)
{
	node* p = *btree;
	if(p ==NULL){
		node* tmp = (node*)malloc(sizeof(node));
		tmp->data = elem;
		tmp->left = NULL;
		tmp->right= NULL;
		*btree = tmp;
		return ;
	}
	if(elem <= p->data){
		insert(&(p->left), elem);
	}else{
		insert(&(p->right), elem);
	}
}

void print_1(node* btree)
{
	if(btree){
		if(btree->left){
			print_1(btree->left);
		}

		printf("%d ", btree->data);

		if(btree->right){
			print_1(btree->right);
		}
	}
}

void print_2(node* btree)
{
	if(btree){
		printf("%d ",btree->data);

		if(btree->left){
			print_2(btree->left);
		}

		if(btree->right){
			print_2(btree->right);
		}
	}
}

void print_3(node* btree)
{
	if(btree){
		if(btree->left){
			print_3(btree->left);
		}

		if(btree->right){
			print_3(btree->right);
		}

		printf("%d ",btree->data);
	}
}

void print_4(node* btree)
{
	if(!btree){
		return ;
	}
	std::queue<node*> qn;
	qn.push(btree);
	while(!qn.empty()){
		node* tmp =qn.front();
		qn.pop();
		printf("%d ",tmp->data);

		if(tmp->left){
			qn.push(tmp->left);
		}
		if(tmp->right){
			qn.push(tmp->right);
		}
	}
}

void print(node* btree, char mode)
{
#define PRE		0
#define POS		1
#define MID		2
#define LAYER	3
	switch(mode){
		case PRE:
			print_2(btree);
			break;
		case POS:
			print_3(btree);
			break;
		case MID:
			print_1(btree);
			break;
		default:
			print_4(btree);
			break;
	}
	printf("\n");
}

int depth(node* btree)
{
	if(!btree){
		return 0;
	}
	int subl =0, subr=0;
	if(btree->left){
		subl = depth(btree->left);
	}
	if(btree->right){
		subr = depth(btree->right);
	}
	return 1+((subl>subr)?subl:subr);
}

int isContained(node* btree, int elem)
{
	if(!btree){
		return -1;
	}
	if(btree->data == elem){
		return 0;
	}else{
		if(elem<=btree->data){
			if(!isContained(btree->left, elem)){
				return 0;
			}else{
				return -1;
			}
		}else{
			if(!isContained(btree->right, elem)){
				return 0;
			}else{
				return -1;
			}
		}

	}
}

int leafs(node* btree)
{
	if(btree){
		if(btree->left ==NULL && btree->right ==NULL){
			return 1;
		}
	}else{
		return 0;
	}
	return leafs(btree->left) + leafs(btree->right);
}

node* getParent(node* btree,int elem1, int elem2)
{
	if(!btree || (!(btree->left) && !(btree->right))){
		return NULL;
	}
	if(isContained(btree,elem1) || isContained(btree,elem2)){
		return NULL;
	}
	if(!isContained(btree->left,elem1) && !isContained(btree->left, elem2)){
		return getParent(btree->left, elem1, elem2);
	}else if(!isContained(btree->right,elem1) && !isContained(btree->right, elem2)){
		return getParent(btree->right, elem1, elem2);
	}else{
		return btree;
	}
}

node* getParent2(node* btree, int elem)
{
	node* p =btree;
	node* pp =NULL;
	while(p && p->data != elem){
		if(elem <= p->data){
			pp = p;
			p = p->left;
		}else{
			pp =p;
			p = p->right;
		}
	}
	if(!p){
		return NULL;
	}
	return pp;
}

int isBalanced(node* btree)
{
	if(depth(btree)<=2){
		return 0;
	}
	int ld =depth(btree->left);
	int rd =depth(btree->right);
	if(ld-rd >=-1 && ld-rd<=1){
		if(!isBalanced(btree->left) && !isBalanced(btree->right)){
			return 0;
		}else{
			return -1;
		}
	}else{
		return -1;
	}
}

node* getMax(node* btree)
{
	node* p = btree;
	if(p){
		while(p->right){
			p = p->right;
		}
	}
	return p;
}

node* getMin(node* btree)
{
	node* p =btree;
	if(p){
		while(p->left){
			p = p->left;
		}
	}
	return p;
}

void delNode(node** btree, int val)
{
	node* p =*btree;
	node* pp = NULL;
	while(p && p->data !=val){
		if(val <= p->data){
			pp = p;
			p = p->left;
		}else{
			pp = p;
			p =p->right;
		}
	}
	if(!p){
		return ;
	}
	if(!(p->left) && !(p->right)){
		if(pp){
			if(pp->left == p){
				pp->left =NULL;
			}else if(pp->right == p){
				pp->right =NULL;
			}
		}
		free(p);
	}else{
		if(p->left){
			node* lmax = getMax(p->left);
			node* lmaxp = getParent2(p->left, lmax->data);
			if(lmaxp){
				lmaxp->right = NULL;
			}
			if(pp){
				if(pp->left == p){
					pp->left =lmax;
				}else if(pp->right ==p){
					pp->right =lmax;
				}
			}else{
				*btree = lmax;
			}
			if(p->right){
				lmax->right = p->right;
			}
			if(p->left != lmax){
				while(lmax->left){
					lmax = lmax->left;
				}
				lmax->left = p->left;
			}
			free(p);
		}else{
			node* rmin = getMin(p->right);
			node* rminp = getParent2(p->right, rmin->data);
			if(rminp){
				rminp->left =NULL;
			}
			if(pp){
				if(pp->left ==p){
					pp->left = rmin;
				}else if(pp->right == p){
					pp->right = rmin;
				}
			}else{
				*btree = rmin;
			}
			if(p->left){
				rmin->left = p->left;
			}
			if(p->right != rmin){
				while(rmin->right){
					rmin = rmin->right;
				}
				rmin->right = p->right;
			}
			free(p);
		}
	}
}

int main()
{
	node* btree =NULL;
	int c =0;
	int rand =0;
	srand(2003);
	while(c++<10){
		rand = random()%100;
		printf("%d ",rand);
		insert(&btree, rand);
	}

	printf("\n--------------------------------------\n");

	print(btree,0);
	print(btree,1);
	print(btree,2);
	print(btree,3);
	printf("depth: %d\n", depth(btree));
	printf("leafs: %d\n", leafs(btree));
	printf("balance? %d\n", isBalanced(btree));
	printf("left balance? %d\n", isBalanced(btree->left));
	printf("right balance? %d\n", isBalanced(btree->right));

	printf("%d is contained? %d\n",35,isContained(btree,35));
	printf("%d is contained? %d\n",10,isContained(btree,10));

	node* pp = getParent(btree,21,92);
	printf("%d and %d parent is %d\n",21,92,(pp)?pp->data:0);
	pp = getParent(btree,21,35);
	printf("%d and %d parent is %d\n",21,35,(pp)?pp->data:0);
	pp = getParent(btree,6,71);
	printf("%d and %d parent is %d\n",6,71,(!pp)?0:pp->data);

	delNode(&btree, 49);
	print(btree,2);

	return 0;
}
