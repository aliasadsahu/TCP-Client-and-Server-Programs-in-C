#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close


int main(int argc, char * argv[]) {
   char message[1000];
   int clientSocket;
   struct sockaddr_in serverAddr;
   socklen_t addr_size;

   // Create the socket. 
   clientSocket = socket(PF_INET, SOCK_STREAM, 0);

   //Configure settings of the server address
   // Address family is Internet 
   serverAddr.sin_family = AF_INET;

   //Set IP address to 127.0.0.1
   //serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
   //printf("Server IP Address: %d\n",serverAddr.sin_addr.s_addr);
   // Getting Server IP address from Command Line
   serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
   //printf("Server IP Address: %d\n",serverAddr.sin_addr.s_addr);
   memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

   //Set port number, using htons function 
   //serverAddr.sin_port = htons(7799);
   //Set Port Number from Command Line 
   serverAddr.sin_port = htons(atoi(argv[2]));
   //printf("Server IP Port: %d\n", serverAddr.sin_port);

   //Connect the socket to the server using the address
   addr_size = sizeof serverAddr;
   if (!connect(clientSocket, (struct sockaddr * ) & serverAddr, addr_size)) {
      // Getting and Printing Server IP Address
      char * server_ip = inet_ntoa(serverAddr.sin_addr);
      printf("established connection with server %s\n", server_ip);
   } else {
      printf("Could not connect to sever...");
   }

   int count[2];
   while (1) {

      //printf("Please enter the message: ");
      bzero(message, 1000);
      fgets(message, 1000, stdin);
      //printf("\nSending to SERVER: %s ",message);

      if (strncmp(message, "exit", 4) == 0) {
         close(clientSocket);
         break;
      }

      /* Send message to the server */
      if (send(clientSocket, message, strlen(message), 0) < 0) {
         printf("Send failed\n");
      }

      /* Receive message to the server */
      if (recv(clientSocket, count, 8, 0) < 0) {
         printf("Receive failed\n");
      }
      printf("%d %d\n", count[0], count[1]);

   }

   return 0;
}
