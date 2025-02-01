#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 1024

void handle_download(int socket, char *file_name)
{
    printf("handle download\n");
    char buffer[BUFFER_SIZE];

    FILE *file = fopen(file_name, "wb");
    if (file == NULL)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    long fsize;
    read(socket, &fsize, sizeof(fsize));

    // Receive data in chunks of buffer_size
    ssize_t numBytes;
    long total_bytes_received = 0;
    while (total_bytes_received < fsize && (numBytes = recv(socket, buffer, sizeof(buffer), 0)) > 0)
    {
        // Write the buffer to the file
        fwrite(buffer, 1, numBytes, file);
        total_bytes_received += numBytes;
    }

    if (numBytes < 0)
    {
        perror("recv() error");
        exit(EXIT_FAILURE);
    }

    printf("File downloaded successfully.\n");
    fclose(file);
}

int main()
{
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    int clientSocket;

    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0)
    {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        write(clientSocket, "ready", strlen("ready"));

        memset(buffer, 0, sizeof(buffer));
        ssize_t len = read(clientSocket, buffer, sizeof(buffer));
        buffer[len] = '\0';

        printf("%s", buffer);

        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);

        if (strcmp(buffer, "1\n") == 0) {
            write(clientSocket, "\\service 1", strlen("\\service 1"));

            memset(buffer, 0, sizeof(buffer));
            read(clientSocket, buffer, sizeof(buffer));
            printf("%s", buffer);

            write(clientSocket, "done", strlen("done"));
        } 
        else if (strcmp(buffer, "2\n") == 0) {
            write(clientSocket, "\\service 2", strlen("\\service 2"));

            memset(buffer, 0, sizeof(buffer));
            read(clientSocket, buffer, sizeof(buffer));
            printf("%s", buffer);

    
            memset(buffer, 0, sizeof(buffer));
            fgets(buffer, sizeof(buffer), stdin);
            if (strcmp(buffer, "1\n") == 0){
                write(clientSocket, "1\n", strlen("1\n"));  

                handle_download(clientSocket, "./Book.txt");

                write(clientSocket, "done", strlen("done"));
            } else if (strcmp(buffer, "2\n") == 0){
                write(clientSocket, "2\n", strlen("2\n"));  

                handle_download(clientSocket, "./Linux.png");

                write(clientSocket, "done", strlen("done"));
            } else {
                write(clientSocket, "done", strlen("done"));
            }
        } 
        else if (strcmp(buffer, "3\n") == 0) {
            write(clientSocket, "\\service 3", strlen("\\service 3"));
            
            memset(buffer, 0, sizeof(buffer));
            read(clientSocket, buffer, sizeof(buffer));
            
            while (1) {
                // Receive input from stdin
                memset(buffer, 0, sizeof(buffer));
                fgets(buffer, sizeof(buffer), stdin);

                // Send buffer to server
                send(clientSocket, buffer, sizeof(buffer), 0);

                memset(buffer, 0, sizeof(buffer));
                recv(clientSocket, buffer, sizeof(buffer), 0);

                printf("[You] %s", buffer);

                if (strcmp(buffer, "\\quit\n") == 0)
                    break;
            }
            // write(clientSocket, "done", strlen("done"));
        }
    }

    close(clientSocket);
    return 0;
}
