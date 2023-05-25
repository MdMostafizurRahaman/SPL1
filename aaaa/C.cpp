#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6100
#define BUFFER_SIZE 1024

int clientSocket;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* receiveMessages(void* arg) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    while (1) {
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            printf("Connection to server closed.\n");
            break;
        }

        printf("%s\n", buffer);

        memset(buffer, 0, sizeof(buffer));
    }

    pthread_exit(NULL);
}

int main() {
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
    serverAddress.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(serverAddress.sin_addr)) <= 0) {
        perror("Error converting IP address");
        exit(1);
    }

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error connecting to server");
        exit(1);
    }

    // Create a thread to receive messages from the server
    pthread_create(&thread, NULL, receiveMessages, NULL);

    // Prompt the user for a username
    char username[50];
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    // Send the username to the server
    send(clientSocket, username, strlen(username), 0);
    send(clientSocket, " Connected", 10, 0);
    // Start the chat loop
    char message[BUFFER_SIZE];
    while (1) {
        memset(message, 0, sizeof(message));
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = '\0';

        // Send the message to the server
        pthread_mutex_lock(&mutex);
        send(clientSocket, message, strlen(message), 0);
        pthread_mutex_unlock(&mutex);
    }

    // Close the client socket
    close(clientSocket);

    return 0;
}
