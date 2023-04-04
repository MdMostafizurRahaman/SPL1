/*
filename server_ipaddress portno
argv[0] -> file name
argv[1] -> server_ipaddress
argv[2 -> portno
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

int main(int argc, char *argv[])
{
    int portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[255];

    if(argc < 3)
    {
        printf("Usage %s hostname port\n", argv[0]);
        exit(0);
    }
    
    portno = atoi(argv[2]);
    
    int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_desc < 0)
	{
		printf("NO socket is created!\n");
	}
	else
	{
		printf("Socket is created!\n");
	}

    server = gethostbyname(argv[1]);
    if(server == NULL);
    {
        fprintf(stderr, "Error, no such host...\n");
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if(connect(sock_desc, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Connection failed\n");
    }
    else
    {
        printf("Connected....\n");
    }
    
    while(1)
    {
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
       n = write(sock_desc, buffer, strlen(buffer));
       if(n < 0)
       {
            printf("Error on writing\n");
       }
       

       bzero(buffer, 255);
       n = read(sock_desc, buffer, 255);
       if(n < 0)
       {
            printf("Error on reading\n");
       }
       
       printf("Server:%s", buffer);
       int i = strncmp("Bye", buffer, 3);
       if(i == 0)
       break;
    }
    

    






}
