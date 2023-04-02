#include<stdio.h>
#include<sys/socket.h>
int main(int argc, char*argv[])
{
	int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_desc == -1)
	{
		printf("NO socket is created!\n");
	}
	else
	{
		printf("Socket is created!\n");
	}
	
	printf("Sock_desc:%d\n", sock_desc);
	return 0;
}	
