#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT_NUM 9002
#define N_CLIENTS 3
#define TRUE 1

int maxElement(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

int main(void) {
    char buffer[256];
    struct sockaddr_in server_addr, client_addr;
    fd_set readfds;
    int server_socket, client_sockets[N_CLIENTS];
    int client_len = sizeof(client_addr);
    int ret, i;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    memset((char *)&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NUM);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(server_socket, N_CLIENTS) == -1) {
        perror("listen");
        exit(1);
    }

    for (i = 0; i < N_CLIENTS; i++) {
        if ((client_sockets[i] = accept(server_socket, (struct sockaddr *)&client_addr, &client_len)) == -1) {
            perror("accept");
            break;
        } else
            printf("Client #%d connected\n", i + 1);
    }

    while (TRUE) {
        FD_ZERO(&readfds);
        for (i = 0; i < N_CLIENTS; i++) {
            FD_SET(client_sockets[i], &readfds);
        }
        printf("Waiting at select...\n");
        ret = select(maxElement(client_sockets, N_CLIENTS) + 1, &readfds, NULL, NULL, NULL);
        printf("select returned: %d\n", ret);

        switch (ret) {
        case -1:
            perror("select");
            break;

        case 0:
            printf("select returns 0\n");
            break;

        default:
            i = 0;
            while (ret > 0) {
                if (FD_ISSET(client_sockets[i], &readfds)) {
                    memset(buffer, 0, sizeof(buffer));
                    if (recv(client_sockets[i], buffer, sizeof(buffer), 0) < 0) {
                        perror("recv");
                        break;
                    }
                    ret--;
                    printf("MSG from Client %d: %s\n", i, buffer);
                } else {
                    i++;
                }
            }
            break;
        }
    }

    return 0;
}

