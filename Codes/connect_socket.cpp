#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
int main(int argc, char *argv[])
{
    int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_desc == -1)
    {
        printf("No Socket is created!\n");
    }
    else
    {
        printf("Socket is created\n");
    }

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr("127.0.0.53");
    server.sin_family = AF_INET;
    server.sin_port = htons(53);

    if(connect(sock_desc, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        printf("Not Connected...\n");
    }
    else
    {
        printf("Connected!!!\n");
    }
    printf("%lu\n", sizeof(server));


    
}