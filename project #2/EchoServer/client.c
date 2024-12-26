#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define IP "127.0.0.1"
#define PORT 8080

int main(void) {
    char input_msg[256];
    int client_sock;
    struct sockaddr_in serv_addr;

    if ((client_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(1);
    }

    memset((char *)&serv_addr, '\0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(IP);

    if (connect(client_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("connect error");
        exit(1);
    }

    while (1) {
        printf("Enter your message: ");
        fgets(input_msg, sizeof(input_msg), stdin);
        input_msg[strlen(input_msg) - 1] = '\0'; 

        if (send(client_sock, input_msg, strlen(input_msg) + 1, 0) == -1) {
            perror("send error");
            exit(1);
        }

        if (recv(client_sock, input_msg, sizeof(input_msg), 0) == -1) {
            perror("recv error");
            exit(1);
        }

        printf("Recv from server:\n%s\n\n", input_msg);

        if (strcmp(input_msg, "quit") == 0) {
            printf("Server connection ended.\n");
            break;
        }
    }

    close(client_sock);

    return 0;
}
