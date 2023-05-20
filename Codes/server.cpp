#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_PORT 65425
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    int socket;
    char username[50];
} Client;

Client clients[MAX_CLIENTS];
int numClients = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* handleClient(void* arg) {
    int clientSocket = *((int*)arg);
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    // Find the client's username
    char username[50];
    for (int i = 0; i < numClients; i++) {
        if (clients[i].socket == clientSocket) {
            strcpy(username, clients[i].username);
            break;
        }
    }

    while (1) {
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            pthread_mutex_lock(&mutex);
            for (int i = 0; i < numClients; i++) {
                if (clients[i].socket == clientSocket) {
                    printf("Client disconnected: %s\n", username);
                    close(clients[i].socket);
                    for (int j = i; j < numClients - 1; j++) {
                        clients[j] = clients[j + 1];
                    }
                    numClients--;
                    break;
                }
            }
            pthread_mutex_unlock(&mutex);
            break;
        }

        if (strcmp(buffer, "@all") == 0) {
            // Send the message to all connected clients
            //printf("Message from %s: %s\n", username, buffer);

            pthread_mutex_lock(&mutex);
            for (int i = 0; i < numClients; i++) {
                if (clients[i].socket != clientSocket) {
                    send(clients[i].socket, username, strlen(username), 0);
                    send(clients[i].socket, ": ", 2, 0);
                    send(clients[i].socket, buffer, strlen(buffer), 0);
                }
            }
            pthread_mutex_unlock(&mutex);
        } else if (buffer[0] == '@') {
            // Extract the recipient's username
            char recipient[50];
            sscanf(buffer, "@%s", recipient);

            // Remove '@' from the recipient's username
            memmove(recipient, recipient + 1, strlen(recipient));

            //printf("Message from %s: %s\n", username, buffer);

            // Send the message to the specified client
            pthread_mutex_lock(&mutex);
            //printf("Message from %s: %s\n", username, buffer);

            for (int i = 0; i < numClients; i++) {
                if (strcmp(clients[i].username, recipient) == 0) {
                    printf("\nRecepient:%s!!!\n",recipient);
                    send(clients[i].socket, username, strlen(username), 0);
                    send(clients[i].socket, ": ", 2, 0);
                    send(clients[i].socket, buffer, strlen(buffer), 0);
                    break;
                }
            }
            pthread_mutex_unlock(&mutex);
        }else {
            printf("Message from %s: %s\n", username, buffer);

            // Send the message to all other connected clients
            pthread_mutex_lock(&mutex);
            for (int i = 0; i < numClients; i++) {
                if (clients[i].socket != clientSocket) {
                    send(clients[i].socket, username, strlen(username), 0);
                    send(clients[i].socket, ": ", 2, 0);
                    send(clients[i].socket, buffer, strlen(buffer), 0);
                }
            }
            pthread_mutex_unlock(&mutex);
        }

        memset(buffer, 0, sizeof(buffer));
    }

    pthread_exit(NULL);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength;
    pthread_t thread;

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Configure server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(SERVER_PORT);

    // Bind the server socket to a specific address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error binding socket");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(serverSocket, MAX_CLIENTS) < 0) {
        perror("Error listening for connections");
        exit(1);
    }

    printf("Server started. Listening on port %d\n", SERVER_PORT);

    while (1) {
        // Accept a client connection
        clientAddressLength = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if (clientSocket < 0) {
            perror("Error accepting connection");
            continue;
        }

        // Limit the number of clients
        if (numClients >= MAX_CLIENTS) {
            printf("Max clients reached. Connection rejected.\n");
            close(clientSocket);
            continue;
        }

        // Prompt the client for a username
        char username[50];
        strcpy(username, " ");

        // Send prompt to enter username
        send(clientSocket, "Enter your username: ", 21, 0);

        // Receive username from the client
        int bytesRead = recv(clientSocket, username, sizeof(username), 0);
        if (bytesRead > 0) {
            // Remove newline character
            username[strcspn(username, "\n")] = '\0';
        }

        // Add the client to the list of connected clients
        pthread_mutex_lock(&mutex);
        clients[numClients].socket = clientSocket;
        strcpy(clients[numClients].username, username);
        numClients++;
        pthread_mutex_unlock(&mutex);

        // Create a new thread to handle the client
        pthread_create(&thread, NULL, handleClient, &clientSocket);
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}