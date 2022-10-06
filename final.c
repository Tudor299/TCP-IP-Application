#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define SERVER_PORT 80 //define the port of the IPv6 server, ipv6 standard
#define BUFFER_LENGTH  250 //define the length of the buffer 

int main(int argc, char *argv[])
{
	//receive HTML from IPv6 server
	struct sockaddr_in6 server_addr; //server address structure for the representation of the IPv6 address	
	int sock_fd = 1; //initialize the value returned by the socket function	
	int ret = 1; //initialize the return variable for the connect function
	int snd = 1; //initialize the return variable for send function
        int rc = 1; //variable for the length of the retured data from the IPv6 server
	int length = BUFFER_LENGTH; //variable for buffer length
	char buffer[BUFFER_LENGTH] = "GET / HTTP/1.0\r\n\r\n"; //initialize the buffer
	char reply[6000]; //initialize the variable for the HTML content
	char html[6000]; //initialize the variable for the concatenated HTML content

	/* Create socket for communication with server in order to obtain the HTML content */
	sock_fd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP); //AF_INET for IPv6; SOCK_STREAM for TCP; IPPROTO_TCP for TCP (allows acknowledgement)
	if (sock_fd < 0)
		{	
		printf("socket function failed\n");
		printf("error: %d", errno);
		exit(1);
		}
	else
		printf("socket function successful\n");
 
	/* Connect to server */
	server_addr.sin6_family = AF_INET6; //connection to IPv6 server
	inet_pton(AF_INET6, "2600:3c01::f03c:91ff:fe93:48f8", &server_addr.sin6_addr); //binary transformation of the IPv6 server address 
	server_addr.sin6_port = htons(SERVER_PORT); //makes sure that numbers are stored in memory in network byte order and msb is the first
	ret = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)); //client connection to the IPv6 server
	if (ret < 0) 
		{
		close(sock_fd); //socket closing function
		printf("conncet function  failed\n"); 
		printf("error: %d", errno);
		exit(1);
		}
	else
		printf("connect function  successful\n");

	/* Send data to the IPv6 server */
	snd = send(sock_fd, buffer, length, 0); //buffer contains the command for obtaining the HTML content
	if (snd < 0)
		{
		printf("send function failed \n"); //verifies if the request is sent or not
		printf("error: %d", errno);
		exit(1);
		}
	else
		printf("send function successful \n");
	
       	/* Receive data from the IPv6 server */
        FILE *fp; //file for storing the html content
	fp = fopen("/home/g5s1e6/final/index.html", "w+"); //opens or creates the file, it can read and write by w+
	if(fp == NULL)
		printf("Error! File could not be opened"); 
	while((rc=recv(sock_fd,reply,length-1,0)) >0) //in reply we read what we have in opened socket (the HTML content)
		{	
  		reply[rc]='\0';
		printf("%s", reply); //we display in the console the content of the HTML 
		fprintf(fp, "%s",  reply); //we write in the file the content of the HTML
		strcat(html, reply); //we created a new variable to concatenate the bytes arrived (reply from the websit)e
		}		
	fclose(fp); //close the file

	close(sock_fd); //close the socket after connection because we have obtained the HTML content
	printf("socket closed\n\n");


	//IPv4 client commands
	struct sockaddr_in serverSa; //declare structure for bind() function
  	int serverPort = 22516; //set up a port 
  	int sock_s = socket(AF_INET,SOCK_STREAM,0); //initialize server socket, opening the socket for the server
  	int sock_c = 1; //declare client socket
  	int r = 1; //declare the return value for the bind() and listen() functions
  	int rec = 1; //declare the return value for the recv() function

  	memset(&serverSa, 0, sizeof(serverSa)); //set the memory size of the server structure
  	serverSa.sin_family = AF_INET; //sets the socket family for IPv4
  	serverSa.sin_addr.s_addr = htonl(INADDR_ANY); //set the server address to any
  	serverSa.sin_port = htons(serverPort); //set the port number 

  	r = bind(sock_s, (struct sockaddr *)&serverSa,sizeof(serverSa)); //call the bind() function with the above selected parameters 
  	if (rc < 0)
  		{
    		printf("bind function failed\n"); 
		printf("error: %d", errno);
    		exit(1);
  		}
  	else
 		printf("bind function successful\n");
  
  	r = listen(sock_s, 10); //listen to the server
  	if (r < 0)
  		{
    		printf("listen function failed\n");
    		printf("error: %d", errno);
		exit(1);
  		}
  	else
		printf("listen function successful\n");
        
	/* commands interpretation */
 	char nu[] = "Command NOT implemented\n"; 
  
  	do{
		sock_c = accept(sock_s, NULL, NULL); //initialize the client socket with the accept() function
  		if (sock_c < 0)
  			{
    			perror("accept function failed\n");
   			printf("error: %d", errno);
			exit(1);
  			}
  		else
  			printf("accept function successfull\n");
  	  	
		char command[] = "16#"; //command specified at the laboratory 
  		char buffer[6000]; //declare a buffer for the recv() function, verifying the command received from the client 
  		rec = 1;	
		
		while(rec !=0)
			{
			rec = recv(sock_c, buffer, sizeof(buffer), 0); //call the recv() function where we get the commands from the client
			if(rec < 0)
				{
				printf("recv function failed\n");
				printf("error: %d", errno);
				exit(1);
				}
			else
				printf(buffer); //buffer = command number
			if(strcmp(buffer, command)==0) //verify if the buffer contains the desired command
				send(sock_c , html, sizeof(html), 0);
			else
				send(sock_c, nu, sizeof(nu), 0);	
				
			}
		close(sock_c);
		printf("client socket closed\n");
		 	}while(1);
	close(sock_s);
	printf("server socket closed\n\n");
       	return 0;
} 
