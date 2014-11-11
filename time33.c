#include <stdio.h>
#include <string.h>

unsigned int time33(const char* str)
{
	unsigned int hash =0;
	int i=0;
	while(str[i] != 0){
		hash = hash*33 + (unsigned int)str[i];
		i++;
	}
	return hash;
}

int main()
{
	const char* test1 = "hello,world";
	const char* test2 = "hello,world.";

	printf("time33 hash : %u\n", time33(test1));
	printf("time33 hash : %u\n", time33(test2));

	return 0;
}

