#include<stdio.h>
int main(int argc, char*argv[])
{	
	int i;
	printf("Total Argument::%d\n\n",argc);
	for(i = 0; i < argc; i++)
	{
		printf("No of Argument:%d, Argument is::%s\n",i,argv[i]);
	}
}
	
