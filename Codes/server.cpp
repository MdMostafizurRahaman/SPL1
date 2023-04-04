#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        fprintf(stderr,"Port no is nopt provided. Program Terminated\n");
        exit(1);
    }
    
    int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_desc == -1)
	{
		printf("NO socket is created!\n");
	}
	else
	{
		printf("Socket is created!\n");
	}

    int newsock_desc, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr,cli_addr;
    socklen_t clilen;

    bzero((char*)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if(bind(sock_desc, (struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0)
    {
        printf("Binding Failed...\n");
    }
    else
    {
        printf("Binding successful...\n");
    }

    listen(sock_desc, 5);
    clilen = sizeof(cli_addr);

    newsock_desc = accept(sock_desc, (struct sockaddr*)&cli_addr, &clilen);
    if(newsock_desc < 0)
    {
        printf("Error on accept\n");
    }
    else
    {
        printf("Accepted...\n");
    }

    while(1)
    {
        bzero(buffer, 255);
        n = read(newsock_desc, buffer, 255);
        if(n < 0)
        {
            printf("Error on Reading");
        }
        
        printf("Client:%s\n", buffer);
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);

       n = write(newsock_desc, buffer, strlen(buffer));
       if(n < 0)
       {
        printf("Error on writing\n");
       }
       

       int i = strncmp("Bye", buffer, 3);
       if(i == 0)
       break;

       close(newsock_desc);
       close(sock_desc);
       return 0;

    }






}