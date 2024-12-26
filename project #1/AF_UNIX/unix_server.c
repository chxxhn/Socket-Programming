#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER_PATH     "./sock_addr"
#define BUFFER_LENGTH    250
#define TRUE              1
#define FALSE             0

void main() {
   int sd = -1, sd2 = -1;
   int rc, length;
   char buffer[BUFFER_LENGTH];
   struct sockaddr_un serveraddr;
   char* return_message = "Received";

   do {
      sd = socket(AF_UNIX, SOCK_STREAM, 0);
      if (sd < 0) {
         perror("socket() failed");
         break;
      }

      memset(&serveraddr, 0, sizeof(serveraddr));
      serveraddr.sun_family = AF_UNIX;
      strcpy(serveraddr.sun_path, SERVER_PATH);

      rc = bind(sd, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
      if (rc < 0) {
         perror("bind() failed");
         break;
      }

      rc = listen(sd, 10);
      if (rc < 0) {
         perror("listen() failed");
         break;
      }

      printf("Ready for client connect().\n");

      sd2 = accept(sd, NULL, NULL);
      if (sd2 < 0) {
         perror("accept() failed");
         break;
      }

      length = BUFFER_LENGTH;
      rc = setsockopt(sd2, SOL_SOCKET, SO_RCVLOWAT, (char *)&length, sizeof(length));
      if (rc < 0) {
         perror("setsockopt(SO_RCVLOWAT) failed");
         break;
      }
      
      while (TRUE) {
        memset(buffer, 0, BUFFER_LENGTH);
        rc = recv(sd2, buffer, sizeof(buffer), 0);

        if (rc < 0) {
            perror("recv() failed");
            break;
        }

        printf("%s\n", buffer);
        
        if (strcmp(buffer, "\\quit\n") == 0) {
            break;
        }

        rc = send(sd2, return_message, sizeof(return_message), 0);

        if (rc < 0) {
            perror("send() failed");
            break;
        }
      }

   } while (FALSE);

   if (sd != -1)
      close(sd);

   if (sd2 != -1)
      close(sd2);

   unlink(SERVER_PATH);
}
