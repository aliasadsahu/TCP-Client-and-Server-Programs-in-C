#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <sys/syscall.h>
#include <pthread.h>



char client_message[2000];
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void * socketThread(void * arg) {
   int tid;
   tid = syscall(SYS_gettid);
   int newSocket = * ((int * ) arg);

   // Logic to get Client IP Address and Port from Socket File Descriptor
   struct sockaddr_in addr;
   socklen_t addr_size = sizeof(struct sockaddr_in);
   getpeername(newSocket, (struct sockaddr * ) & addr, & addr_size);
   char * client_ip = inet_ntoa(addr.sin_addr);
   int client_port = ntohs(addr.sin_port);
   printf("worker %d: established connection with client %s#%d \n", tid , client_ip, client_port);

   int i = 0;
   int count[2];

   while (recv(newSocket, client_message, 2000, 0) > 0) {

      int words = 1, characters = -1;

      for (i = 0; client_message[i] != '\0'; i++) {
         if (client_message[i] != ' ') { // check characters
            characters++;
         } else if (client_message[i] == ' ' || client_message[i] != '\n' || client_message[i] != '\t') { //check words
            words++;
         }
      }

      printf("worker %d: received message from client. # words = %d and # characters = %d \n", tid, words, characters);
      //printf("worker %d: received message from client. Message: %s \n",tid,client_message);

      count[0] = words;
      count[1] = characters;

      // Sending the buffer
      if (send(newSocket, count, 8, 0) < 0) {
         printf("Send failed\n");
      }

      //char *nbuff = strrev(buff);
      //printf("Server Sending: %s",strrev(buff));
      //Send the reversed input
      //send(in, nbuff, strlen(nbuff), 0);

      bzero(client_message, 2000);

   }
   printf("worker %d: client terminated \n", tid);
   close(newSocket);
   pthread_exit(NULL);
}

int main(int argc, char * argv[]) {
   int serverSocket, newSocket;
   // The sockaddr_in structure specifies a transport address and port for the AF_INET address family.
   // The AF_INET address family is the address family for IPv4.
   // SOCK_STREAM Supports reliable connection-oriented byte stream communication.
   // The SOCKADDR_STORAGE structure is a generic structure that specifies a transport address.

   struct sockaddr_in serverAddr;
   struct sockaddr_storage serverStorage;
   socklen_t addr_size;

   // Create the socket. 
   serverSocket = socket(PF_INET, SOCK_STREAM, 0);

   // Configure settings of the server address struct
   // Address family = Internet 
   serverAddr.sin_family = AF_INET;

   //Set port number to 7799, using htons function to use proper byte order 
   //serverAddr.sin_port = htons(7799);
   serverAddr.sin_port = htons(atoi(argv[1]));

   //Set IP address to localhost 
   serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

   //Set all bits of the padding field to 0 
   memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

   //Bind the address struct to the socket 
   bind(serverSocket, (struct sockaddr * ) & serverAddr, sizeof(serverAddr));

   // Listen on the socket, with 50 max connection requests queued 
   // listen() marks the socket referred to by sockfd as a passive socket.
   // A socket that will be used to accept incoming connection requests using accept().
   // Acting as Server
   if (listen(serverSocket, 50) == 0)
      printf("Listening\n");

   else
      printf("Error\n");

   // pthread_t is the data type used to uniquely identify a thread. 
   // It is returned by pthread_create() and used by the application in function calls that require a thread identifier.
   pthread_t tid[60];

   int i = 0;
   while (1) {
      // Accept call creates a new socket for the incoming connection
      addr_size = sizeof serverStorage;

      // newSocket = Client Server connection established
      newSocket = accept(serverSocket, (struct sockaddr * ) & serverStorage, & addr_size);

      // for each client request creates a thread and assign the client request to it to process
      // so the main thread can entertain next request
      // int pthread_create(pthread_t *restrict thread,
      //                   const pthread_attr_t *restrict attr,
      //                   void *(*start_routine)(void *),
      //                   void *restrict arg);
      // The pthread_create() function starts a new thread in the calling process.  
      // The new thread starts execution by invoking start_routine(); arg is passed as the sole argument of start_routine().

      if (pthread_create( & tid[i++], NULL, socketThread, & newSocket) != 0)
         printf("Failed to create thread\n");

      if (i >= 50) {
         i = 0;
         while (i < 50) {
            pthread_join(tid[i++], NULL);
         }
         i = 0;
      }
   }
   return 0;
}
