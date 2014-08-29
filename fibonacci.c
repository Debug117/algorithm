#include <stdio.h>
#define MAX		20

int fib(int seq)
{
		if(seq<2)
				return seq;
		else
				return fib(seq-1)+fib(seq-2);
}

int main()
{
		int i=0;
		for(i=0; i<MAX; i++){
				printf("%d ",fib(i));
		}
		printf("\n");
		double limit=((double)fib(30))/((double)fib(29));
		printf("limit: %f\n",limit);
		limit=((double)fib(40))/((double)fib(39));
		printf("limit: %f\n",limit);
		limit=((double)fib(45))/((double)fib(44));
		printf("limit: %f\n",limit);
		return 0;
}
