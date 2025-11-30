// udp_server.c : Simple UDP Server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUF_SIZE];
    socklen_t client_len = sizeof(client_addr);

    // 1. Create UDP socket
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Bind socket to port and IP
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind error");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Server listening on port %d...\n", PORT);

    // 3. Receive message from client
    ssize_t bytes = recvfrom(server_fd, buffer, BUF_SIZE, 0,
                             (struct sockaddr *)&client_addr, &client_len);
    if (bytes < 0) {
        perror("recvfrom");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    buffer[bytes] = '\0';
    printf("Received from client: %s\n", buffer);

    // 4. Send reply
    const char *reply = "Hello from UDP server!";
    sendto(server_fd, reply, strlen(reply), 0,
           (struct sockaddr *)&client_addr, client_len);

    printf("Reply sent to client.\n");

    // 5. Close socket
    close(server_fd);
    return 0;
}
