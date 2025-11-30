// server.c : Simple TCP server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>         // close()
#include <arpa/inet.h>      // sockaddr_in, inet_ntoa()
#include <sys/socket.h>     // socket functions

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUF_SIZE] = {0};
    const char *reply = "Hello from server (got your message)\n";

    // 1. Create socket (IPv4, TCP)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Bind to address and port
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // listen on all interfaces
    address.sin_port = htons(PORT);

    // Reuse the address if in TIME_WAIT
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // 4. Accept a client connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected with client: %s:%d\n",
           inet_ntoa(address.sin_addr),
           ntohs(address.sin_port));

    // 5. Receive data from client
    ssize_t bytes_read = recv(new_socket, buffer, BUF_SIZE - 1, 0);
    if (bytes_read < 0) {
        perror("recv");
    } else {
        buffer[bytes_read] = '\0';  // null-terminate received data
        printf("Received from client: %s\n", buffer);

        // 6. Send reply back to client
        if (send(new_socket, reply, strlen(reply), 0) < 0) {
            perror("send");
        } else {
            printf("Reply sent to client.\n");
        }
    }

    // 7. Close sockets
    close(new_socket);
    close(server_fd);
    return 0;
}
