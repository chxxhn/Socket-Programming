#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define MAX_NUM_CLIENTS 10

int main(void) {
    char msg_buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int serv_sock, cli_sock, cli_addr_len = sizeof(cli_addr);
    int count_clients = 0;

    if ((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(1);
    }

    memset((char *)&serv_addr, '\0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind error");
        exit(1);
    }

    if (listen(serv_sock, MAX_NUM_CLIENTS) == -1) {
        perror("listen error");
        exit(1);
    }

    while (1) {
        if ((cli_sock = accept(serv_sock, (struct sockaddr *)&cli_addr, &cli_addr_len)) == -1) {
            perror("accept error");
            exit(1);
        }

        printf("New client!\n");
        printf("Number of service client : %d\n", ++count_clients);

        switch (fork()) {
        case 0:
            close(serv_sock);

            while (1) {
                if (recv(cli_sock, msg_buffer, sizeof(msg_buffer), 0) == -1) {
                    perror("recv error");
                    exit(1);
                }

                if (strcmp(msg_buffer, "quit") == 0) {
                    printf("Client has ended the connection.\n");
                    close(cli_sock);
                    exit(0);
                }

                printf("Recv from client: %s\n\n", msg_buffer);

                if (send(cli_sock, msg_buffer, strlen(msg_buffer) + 1, 0) == -1) {
                    perror("send error");
                    exit(1);
                }
            }
        default:
            close(cli_sock);
        }
    }

    return 0;
}
