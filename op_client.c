// operation_client.c : Send operation to server and receive answer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE];
    char expression[BUF_SIZE];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    // Take input for operation
    printf("Enter expression (format: a + b): ");
    fgets(expression, BUF_SIZE, stdin);

    // Send expression to server
    send(sock, expression, strlen(expression), 0);

    // Receive and print result
    recv(sock, buffer, BUF_SIZE, 0);
    printf("Server Response: %s\n", buffer);

    close(sock);
    return 0;
}
