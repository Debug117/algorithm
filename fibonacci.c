#include <stdio.h>
#define MAX		20

int fib(int seq)
{
		if(seq<2)
				return seq;
		else
				return fib(seq-1)+fib(seq-2);
}

int fib2(int seq)
{
	if(seq<2){
		return seq;
	}
	int n1=0,n2=1,nn=0;
	int c = 1;
	while(c++<seq){
		nn = n1+n2;
		n1 = n2;
		n2 = nn;
	}
	return nn;
}

int main()
{
		int i=0;
		for(i=0; i<MAX; i++){
				printf("%d: %d, %d\n",i,fib(i),fib2(i));
		}
		return 0;
}
