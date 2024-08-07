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
    int server_socket;
    int client_socket;
    struct sockaddr_in server_sockaddr;
    struct sockaddr_in client_sockaddr;

    int error_check;
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        printf("error!!!\n");
        exit(1);
    }

    memset(&server_sockaddr, 0, sizeof(struct sockaddr));
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.8.2", &server_sockaddr.sin_addr);
    bzero(&(server_sockaddr.sin_zero),8);

    error_check = bind(server_socket, (struct sockaddr *) &server_sockaddr, sizeof (server_sockaddr));
    if (error_check == -1) {
        printf("bind error!!! %s\n", strerror(errno));
        close(server_socket);
        exit(1);
    }

    char buffer[BUFF_SIZE];
    while (1) {
        printf("ready to hear messages\n");
        size_t len = sizeof(struct sockaddr);
        if (recvfrom(server_socket, buffer, BUFF_SIZE, 0, (struct sockaddr *) &client_sockaddr, (socklen_t *) &len) == -1) {
            perror("recvfrom() failed");
            close(server_socket);
            exit(1);
        }

        printf("message from client: %s\n", buffer);


        if (sendto(server_socket, buffer, BUFF_SIZE, 0, (struct sockaddr *) &client_sockaddr, len) == -1) {
            printf("sendto error!!!\n");
            close(server_socket);
            exit(1);
        }

        printf("message was sent to client\n");
    }


    close(client_socket);
    close(server_socket);
    return 0;

}
