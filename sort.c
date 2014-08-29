#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RESET		"\033[0m"
#define HIGHLIGHT	"\033[1m"
#define UNDERLINE	"\033[4m"
#define FLASH		"\033[5m"
#define REVERSE		"\033[7m"
#define HIDE		"\033[8m"

#define RED			"\033[31m"
#define GREEN		"\033[32m"
#define YELLOW		"\033[33m"
#define BLUE		"\033[34m"
#define PURPLE		"\033[35m"
#define WHITE		"\033[37m"
#define BLACK		"\033[30m"

void swap1(int *a, int* b)
{
		int t=*a;
		*a = *b;
		*b = t;
}

void swap2(int* a, int* b)
{
		*a = *a- *b;
		*b = *a + *b;
		
}

void print(int* num,int n)
{
		int i=0;
		printf(HIGHLIGHT);
		printf("[%d]  ",n);
		printf(RED);
		for(i=0; i<n; i++)
				printf("%-3d",*(num+i));
		printf(RESET"\n");
}

//插入排序
void sort1(int *num, int n)
{
		int i=1,k=0;
		for(i=1; i<n;i++){
				for(k=i;k-1>=0;k--) {
						if(*(num+k) < *(num+k-1)){
								swap1(num+k,num+k-1);
						}
				}
		};
}

//选择排序
void sort2(int *num, int n)
{
		int pos=0,i=0,k=0;
		int min=0;
		for(i=0; i<n; i++){
				min = *(num+i);
				for(k=i; k<n; k++){
						if(*(num+k)< min){
								min = *(num+k);
								pos = k;
						}
				}
				if(min != *(num+i)){
						swap1(num+pos, num+i);
				}
		}
}

//冒泡排序
void sort3(int* num, int n)
{
		int i=0,j=0;
		for(i=n-1; i>=0; i--){
				for(j=0; j<i; j++){
						if(*(num+j) > *(num+j+1)){
								swap1(num+j, num+j+1);
						}
				}
		}
}

//快速排序
void sort4(int*num, int n)
{
		print(num,n);
		int i=0,j=n-1;
		int av = (num[0] +num[n-1])/2;
		while(i < j){
				while(num[i] <= av && i<j) i++;
				while(num[j] > av && i<j) j--;
				if(num[i] != num[j] && i<j){
						printf("(%d<-->%d) %d<-->%d\n",i,j,num[i],num[j]);
						swap1(num+i,num+j);
				}
				if(num[i+1]<av && i<j)i++;
				if(num[j-1]>av && i<j)j--;
		}
		//print(num,n);
		//printf("(i=%d,j=%d)\n",i,j);
		if(i >= 2 && i<n){
				sort4(num,i);
		}
		if(n-j >= 2 && j>0){
				sort4(num+j,n-j);
		}
}

//归并排序
void sort5_merge(int* a, int an, int* b, int bn)
{
		int i=0,j=0,s=0;
		int* tmp = (int*)malloc((sizeof(int))*(an+bn));
		while(i<an || j<bn){
				if(a[i]<b[j]){
						tmp[s] = a[i];
						i++;
				} else {
						tmp[s] = b[j];
						j++;
				}
				s++;
				if(i == an){
						while(j < bn){
								tmp[s] = b[j];
								s++;
								j++;
						}
				}
				if(j == bn){
						while(i < an){
								tmp[s] = a[i];
								s++;
								i++;
						}
				}
		}
		memcpy((void*)a,(void*)tmp,(sizeof(int))*(an+bn));
		free(tmp);
}
void sort5(int* num, int n)
{
		int d = 0;
		if(n>1){
				d = n/2;
				sort5(num,d);
				sort5(num+d,n-d);
				sort5_merge(num,d,num+d,n-d);
		}
}

//堆排序
void sort6_maxtopheap(int* num, int n)
{
		int i = 0,p=0,s=0;
		int tmp=0;
		for(i=1; i<n; i++){
				s=i;
				while(s > 0 && p>=0){
						p=(s-1)/2;
						if(num[p]>num[s]){
								break;
						} else {
								swap1(num+s,num+p);
								s=p;
						}
				}
		}
}
void sort6(int* num, int n)
{
		int i=0;
		for(i=n; i>0; i--){
				sort6_maxtopheap(num,i);
				swap1(num,num+i-1);
		}
}

int main()
{
		int test[]={12,22,16,0,99,64,23,88,55,29,94,44,9,68,72,1,49,65,38,82,15,4,55,24,25,22,78,88,33,51,29,65};
		print(test,sizeof(test)/sizeof(test[0]));
		sort6(test,sizeof(test)/sizeof(test[0]));
		print(test,sizeof(test)/sizeof(test[0]));

		/*
		int test2[]={2,7,23,42,4,15,22,55,68,83,88,98};
		print(test2,12);
		//sort5_merge(test2,4,test2+4,8);
		sort6_maxtopheap(test2,12);
		print(test2,12);
		*/

		return 0;
}
