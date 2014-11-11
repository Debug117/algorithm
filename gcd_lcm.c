#include <stdio.h>
#include <stdlib.h>

int gcd(int a, int b)
{
	int min = a<b?a:b;
	int max = a<b?b:a;
	if(max%min==0){
		return min;
	}
	return gcd(max%min,min);
}

int gcd2(int a, int b)
{	
	int min = a<b?a:b;
	int max = a<b?b:a;
	while(max%min!=0){
		int tmp = min;
		min = max%min;
		max = tmp;
	}
	return min;
}

int lcm(int a, int b)
{
	int g = gcd(a,b);
	return (a*b)/g;
}

int main(int argc, char*argv[])
{
	if(argc<3){
		return 0;
	}
	int a= atoi(argv[1]);
	int b= atoi(argv[2]);
	printf("gcd(%d,%d)=%d\n",a,b,gcd(a,b));
	printf("gcd2(%d,%d)=%d\n",a,b,gcd2(a,b));
	printf("lcm(%d,%d)=%d\n",a,b,lcm(a,b));

	return 0;
}
