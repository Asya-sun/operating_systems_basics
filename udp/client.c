#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

#define PORT 0x1235
#define USERS_NUMBER 5
#define BUFF_SIZE 256

int main() {
    int client_socket;
    struct sockaddr_in server_sockaddr;

    int error_check;
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        printf("error!!! %s\n", strerror(errno));
        exit(1);
    }

    memset(&server_sockaddr, 0, sizeof(struct sockaddr));
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.8.2", &server_sockaddr.sin_addr);
    bzero(&(server_sockaddr.sin_zero),8);


    char buffer[BUFF_SIZE];
    while (1) {
        memset(buffer, 0, BUFF_SIZE);
        printf("enter message: ");
        char *user_message = fgets(buffer, BUFF_SIZE, stdin);
        if (user_message == NULL) {
            perror("fgets() failed");
            close(client_socket);
            exit(1);
        }
        int i = 0;
        while(user_message[i] != '\n') {
            i++;
        }
        i++;
        buffer[i] = '\0';

        size_t len = sizeof(struct sockaddr);
        error_check = sendto(client_socket, buffer, BUFF_SIZE, 0,(struct sockaddr*) &server_sockaddr, len);
        if (error_check == -1) {
            printf("read error: %s\n", strerror(errno));
            close(client_socket);
            exit(1);
        }

        error_check = recvfrom(client_socket, buffer, BUFF_SIZE, 0, (struct sockaddr*) &server_sockaddr,
                               (socklen_t *) &len);
        if (error_check == -1) {
            printf("write error: %s\n", strerror(errno));
            close(client_socket);
            exit(1);
        }
        printf("get from server: %s\n", buffer);
    }

    close(client_socket);
    return 0;
}
