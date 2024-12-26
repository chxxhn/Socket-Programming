#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER_PATH     "./sock_addr"
#define BUFFER_LENGTH    250
#define TRUE              1
#define FALSE              0

void main(int argc, char *argv[]) {
   int sd = -1, rc, bytesReceived;
   char buffer[BUFFER_LENGTH];
   struct sockaddr_un serveraddr;

   do {
      sd = socket(AF_UNIX, SOCK_STREAM, 0);
      if (sd < 0) {
         perror("socket() failed");
         break;
      }

      memset(&serveraddr, 0, sizeof(serveraddr));
      serveraddr.sun_family = AF_UNIX;
      if (argc > 1)
         strcpy(serveraddr.sun_path, argv[1]);
      else
         strcpy(serveraddr.sun_path, SERVER_PATH);

      rc = connect(sd, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
      if (rc < 0) {
         perror("connect() failed");
         break;
      }

      while (TRUE) {
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);

        rc = send(sd, buffer, sizeof(buffer), 0);
        if (rc < 0) {
            perror("send() failed");
            break;
        }

        if (strcmp(buffer, "\\quit\n") == 0)
            break;

        memset(buffer, 0, sizeof(buffer));
        rc = recv(sd, buffer, sizeof(buffer), 0);

        if (rc < 0) {
            perror("recv() failed");
            break;
        }
      }
      
   } while (FALSE);

   if (sd != -1)
      close(sd);
}
