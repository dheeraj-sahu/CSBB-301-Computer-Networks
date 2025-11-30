// client.c : Simple TCP client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>         // close()
#include <arpa/inet.h>      // sockaddr_in, inet_pton()
#include <sys/socket.h>     // socket functions

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE] = {0};
    const char *message = "Hello from client\n";

    // 1. Create socket (IPv4, TCP)
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    // 2. Specify server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary (server is localhost)
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // 3. Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect failed");
        close(sock);
        exit(EXIT_FAILURE);
    }
    printf("Connected to server.\n");

    // 4. Send data to server
    if (send(sock, message, strlen(message), 0) < 0) {
        perror("send");
        close(sock);
        exit(EXIT_FAILURE);
    }
    printf("Message sent: %s", message);

    // 5. Receive reply from server
    ssize_t bytes_read = recv(sock, buffer, BUF_SIZE - 1, 0);
    if (bytes_read < 0) {
        perror("recv");
    } else {
        buffer[bytes_read] = '\0';
        printf("Reply from server: %s\n", buffer);
    }

    // 6. Close socket
    close(sock);
    return 0;
}
