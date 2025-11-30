// udp_client.c : Simple UDP Client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    const char *message = "Hello from UDP client!";
    socklen_t addr_len = sizeof(server_addr);

    // 1. Create UDP socket
    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Define server information
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; // use localhost

    // 3. Send message to server
    sendto(client_fd, message, strlen(message), 0,
           (struct sockaddr *)&server_addr, addr_len);

    printf("Message sent: %s\n", message);

    // 4. Receive response from server
    ssize_t bytes = recvfrom(client_fd, buffer, BUF_SIZE, 0,
                             (struct sockaddr *)&server_addr, &addr_len);
    if (bytes < 0) {
        perror("recvfrom");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    buffer[bytes] = '\0';
    printf("Reply from server: %s\n", buffer);

    // 5. Close socket
    close(client_fd);
    return 0;
}
