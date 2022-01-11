# TCP-Client-and-Server-Programs-in-C
Implement a concurrent server using threads.

In this project, you will implement TCP client and server programs in C. The specifications for
server and clients are given below.

<h1>Server:</h1>
In this project you will implement a concurrent server using threads. In a concurrent server, the parent thread initializes the socket to listening socket, and waits for connection request from a client. Once the server receives a connection
request, it creates a worker thread to handle the client. The server then waits for the next
request.

The worker thread outputs the following and then waits for a message from the client.

worker id: established connection with client ip_addr#port

In the above output, 'id' is the thread id of the worker (check gettid()), 'ip_address' is the ip address of the client, and port is the port number of the client.
Once the worker thread receives a message from the client, it counts the number of words and number of characters. It outputs the following.

worker id: received message from client. # words = wnum and # characters = cnum

In the above output, wnum is the number of words and cnum is the number of characters in the message sent by the client. Skip the white spaces when couting the number of characters.
The worker then sends the two values, wnum and cnum, to the client and waits for the next message from the client. The worker thread will terminate only after the client terminates. Before terminating it ouputs the following.

worker id: client terminated

The concurrent server never terminates. It always waits for a new connection request from a client.
Name your server program server.c

Compile it as follows:
gcc server.c -pthread -o server

Sample usage:
./server 3648
3648 is the port number on which the server will be listening. Pick a number randomly. If the number is already used, your server will fail. If so, pick another port number.


<h1>Client:</h1>
The client program expects two command line arguments: server DNS name and port number. The client establishes a connection with the server and outputs the following.

client: established connection with server ip_addr

The client then waits for the user to input a text. It then send the input text to the server and waits for a response message from the server. Once the response from the server arrives, the client client displays the message and waits for the next input from the user. The client program terminates, when the user inputs 'exit'.
Name your client program client.c

Compile it as follows:
gcc client.c -o client

Sample usage:
./client cs2.utdallas.edu 3648

Sample output of a client:

established connection with server 10.176.92.16

hi there // this is the input from the user

2 7

Good to chat with you

5 17

exit

Sample output of a server:

worker 28456: established connection with client 10.176.92.88#47974

worker 28456: received message from client. # words = 2 and # characters = 7

worker 28456: received message from client. # words = 5 and # characters = 17

worker 28456: client terminated


If there are more than one worker thread running at the same time, the output from threads may be interleaved. Your server should be able to handle many clients at the same time. The upper limit on the number of clients is system imposed. You program should not put a limit on the number of clients that can connect with server at any given time. Test your program with multiple clients connected to the server concurrently. Feel free to reuse the code server.c, sendandreceive.c. Please look at the sample programs we wrote for thread creation. It will be very useful. Most of the code required are already there in these source files. You have to understand well to use them. Submit server.c and client.c
