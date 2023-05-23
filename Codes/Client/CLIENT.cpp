//without @/all working well

#include "header.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 65425
#define BUFFER_SIZE 1024

void* receiveMessages(void* arg) {
    int socket = *((int*)arg);
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    while (1) {
        int bytesRead = recv(socket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            printf("Disconnected from server.\n");
            break;
        }

        printf("%s\n", buffer);

        memset(buffer, 0, sizeof(buffer));
    }

    pthread_exit(NULL);
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddress;
    pthread_t thread;

    // Create client socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Configure server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddress.sin_port = htons(SERVER_PORT);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error connecting to server");
        exit(1);
    }

    // Receive the initial prompt to enter username
    char prompt[50];
    memset(prompt, 0, sizeof(prompt));
    int bytesRead = recv(clientSocket, prompt, sizeof(prompt), 0);
    if (bytesRead > 0) {
        printf("%s", prompt);
    }

    // Read the username from the user
    char username[50];
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    // Send the username to the server
    send(clientSocket, username, strlen(username), 0);

    // Create a new thread to receive messages from the server
    if (pthread_create(&thread, NULL, receiveMessages, &clientSocket) != 0) {
        perror("Error creating thread");
        exit(1);
    }

    // Read and send user messages
    char message[BUFFER_SIZE];
    while (1) {
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = '\0';

        // Check if user wants to exit
        if (strcmp(message, "/exit") == 0) {
            printf("Disconnected from server.\n");
            break;
        }

        // Send the message to the server
        send(clientSocket, message, strlen(message), 0);
    }

    // Close the client socket
    close(clientSocket);

    return 0;
}