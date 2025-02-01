#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT_NUM 9002
#define TRUE 1

int main(void) {
    char buffer[256];
    int client_socket;
    struct sockaddr_in server_addr;

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    memset((char *)&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NUM);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(1);
    }

    while(TRUE) {
        memset(buffer, 0, sizeof(buffer));
        printf("Enter: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (send(client_socket, buffer, strlen(buffer), 0) < 0) {
            perror("send");
            exit(1);
        }
        if (strcmp(buffer, "quit\n") == 0) {
            printf("Client Exit...\n");
            break;
        }
    }

    close(client_socket);

    return 0;
}

