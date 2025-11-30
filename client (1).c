#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main() {
    int sfd;
    fd_set rset;
    char buff[1024];
    struct sockaddr_in server;

    // Create socket
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) {
        printf("Socket creation failed.\n");
        return -1;
    }

    // Configure server
    bzero(&server, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    inet_aton("192.168.0.140", &server.sin_addr);  // Use your machine's IP address

    // Connect to server
    if (connect(sfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Connection failed.\n");
        return -1;
    }

    for (;;) {
        FD_ZERO(&rset);
        FD_SET(0, &rset); // Monitor stdin
        FD_SET(sfd, &rset); // Monitor socket

        select(sfd + 1, &rset, NULL, NULL, NULL);

        // If user types a message
        if (FD_ISSET(0, &rset)) {
            memset(buff, 0, sizeof(buff));  // Clear the buffer
            printf("Enter the message:\n");

            // Use fgets to read a full line (including spaces)
            if (fgets(buff, sizeof(buff), stdin) == NULL) {
                break;
            }

            // Remove newline character if present
            buff[strcspn(buff, "\n")] = '\0';

            // If user types "quit", exit the loop
            if (strcmp(buff, "quit") == 0) {
                printf("Exiting...\n");
                break;
            }
            
            write(sfd, buff, strlen(buff));
        }

        // If message is received from server
        if (FD_ISSET(sfd, &rset)) {
            memset(buff, 0, sizeof(buff));  // Clear the buffer
            ssize_t bytesRead = read(sfd, buff, sizeof(buff) - 1);
            if (bytesRead > 0) {
                buff[bytesRead] = '\0';  // Ensure null-termination
                printf("Message received: %s\n", buff);
            }
        }
    }

    close(sfd);
    return 0;
}

