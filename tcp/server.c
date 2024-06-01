#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

#define PORT 0x1234
#define USERS_NUMBER 5
#define BUFF_SIZE 256


int main() {
    int server_socket;
    int client_socket;
    struct sockaddr_in server_sockaddr;
    struct sockaddr_in client_sockaddr;

    int error_check;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("error!!!\n");
        exit(1);
    }

    memset(&server_sockaddr, 0, sizeof(struct sockaddr));
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.8.1", &server_sockaddr.sin_addr);
    bzero(&(server_sockaddr.sin_zero),8);

    error_check = bind(server_socket, (struct sockaddr *) &server_sockaddr, sizeof (server_sockaddr));
    if (error_check == -1) {
        printf("bind error: %s\n", strerror(errno));
        close(server_socket);
        exit(1);
    }

    error_check = listen(server_socket, USERS_NUMBER);
    if (error_check == -1) {
        printf("listen() error: %s\n", strerror(errno));
        close(server_socket);
        exit(1);
    }

    while (1) {
        int len;
        client_socket = accept(server_socket, (struct sockaddr *) &client_sockaddr, &len);
        if (client_socket == -1) {
            printf("accept() error: %s\n", strerror(errno));
            close(server_socket);
            exit(1);
        }

        int pid = fork();

        if (pid < 0) {
            printf("fork error: %s\n", strerror(errno));
            close(server_socket);
            close(client_socket);
            exit(1);
        } else if (pid == 0) {
            break;
        } else {
            close(client_socket);
            continue;
        }
    }

    char buffer[BUFF_SIZE];
    while (1) {
        error_check = read(client_socket, buffer, sizeof(buffer));
        if (error_check == -1) {
            printf("read error: %s\n", strerror(errno));
            close(client_socket);
            close(server_socket);
            exit(1);
        }
        if (error_check == 0) {
            close(client_socket);
            close(server_socket);
            exit(1);
        }

        printf("message from client: %s\n", buffer);

        error_check = write(client_socket, buffer, error_check);
        if (error_check == -1) {
            printf("write error: %s\n", strerror(errno));
            close(client_socket);
            close(server_socket);
            exit(1);
        }
        printf("message was sent to client\n");
    }

    close(client_socket);
    close(server_socket);
    return 0;

}
