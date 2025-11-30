// operation_server.c : Perform operations received from client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUF_SIZE];
    char response[BUF_SIZE];
    int a, b;
    char op;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Address initialization
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind server
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for connection
    listen(server_fd, 1);
    printf("Server waiting for client...\n");

    // Accept client
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Client connected.\n");

    // Receive expression
    recv(new_socket, buffer, BUF_SIZE, 0);
    printf("Received expression: %s\n", buffer);

    // Parse operation (format: a op b)
    sscanf(buffer, "%d %c %d", &a, &op, &b);

    // Perform operation
    float result;
    switch(op) {
        case '+': result = a + b; break;
        case '-': result = a - b; break;
        case '*': result = a * b; break;
        case '/': 
            if (b == 0) {
                strcpy(response, "Error: Division by zero");
                send(new_socket, response, strlen(response), 0);
                close(new_socket);
                close(server_fd);
                return 0;
            }
            result = (float)a / b; 
            break;
        default:
            strcpy(response, "Invalid operator");
            send(new_socket, response, strlen(response), 0);
            close(new_socket);
            close(server_fd);
            return 0;
    }

    // Send response back to client
    sprintf(response, "Result = %.2f", result);
    send(new_socket, response, strlen(response), 0);

    printf("Result sent to client.\n");

    close(new_socket);
    close(server_fd);

    return 0;
}
