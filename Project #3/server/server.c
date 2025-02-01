#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void send_time(int new_socket)
{
    char buffer[BUFFER_SIZE];
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strcpy(buffer, asctime(timeinfo));
    send(new_socket, buffer, strlen(buffer), 0);
}

void send_file(int new_socket, char *filename)
{
    char buffer[BUFFER_SIZE];
    FILE *file = fopen(filename, "rb");

    if (file == NULL)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);

    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    send(new_socket, &fsize, sizeof(fsize), 0);

    ssize_t numBytes;
    while ((numBytes = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0)
    {
        send(new_socket, buffer, numBytes, 0);
    }

    fclose(file);
}

void echo(int new_socket)
{
    char buffer[BUFFER_SIZE];

    write(new_socket, "echo start", strlen("echo start"));

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t len = read(new_socket, buffer, sizeof(buffer) - 1);

        send(new_socket, buffer, len, 0);
        if (strcmp(buffer, "\\quit\n") == 0)
            break;
    }
}

int main()
{
    struct sockaddr_in address;
    int serverSocket, newSocket;
    socklen_t addrLen = sizeof(address);
    char* menu_list = "[Service List]\n1. Get Current Time\n2. Download File\n3. Echo Server\nEnter: ";
    char* file_list = "[Available File List]\n1. Book.txt\n2. Linux.png\n3. Go back\nEnter: ";

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Socket bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 3) < 0)
    {
        perror("Socket listen failed");
        exit(EXIT_FAILURE);
    }

    if ((newSocket = accept(serverSocket, (struct sockaddr *)&address, &addrLen)) < 0)
    {
        perror("Socket accept failed");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        read(newSocket, buffer, sizeof(buffer));

        write(newSocket, menu_list, strlen(menu_list));

        memset(buffer, 0, BUFFER_SIZE);
        read(newSocket, buffer, sizeof(buffer));

        if (strcmp(buffer, "\\service 1") == 0) {
            send_time(newSocket);

            memset(buffer, 0, BUFFER_SIZE); 
            read(newSocket, buffer, sizeof(buffer));
        } else if (strcmp(buffer, "\\service 2") == 0) {
            write(newSocket, file_list, strlen(file_list));

            memset(buffer, 0, BUFFER_SIZE); 
            read(newSocket, buffer, sizeof(buffer));

            if (strcmp(buffer, "1\n") == 0) {
                send_file(newSocket, "./Book.txt");

                memset(buffer, 0, BUFFER_SIZE); 
                read(newSocket, buffer, sizeof(buffer));
            } else if (strcmp(buffer, "2\n") == 0) {
                send_file(newSocket, "./Linux.png");

                memset(buffer, 0, BUFFER_SIZE); 
                read(newSocket, buffer, sizeof(buffer));
            }
        } else if (strcmp(buffer, "\\service 3") == 0) {
            echo(newSocket);

            // memset(buffer, 0, BUFFER_SIZE); 
            // ssize_t len = read(newSocket, buffer, sizeof(buffer));
            // buffer[len] = '\0';
        }
    }

    close(newSocket);
    close(serverSocket);
    return 0;
}
