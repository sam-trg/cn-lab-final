//server of UDP (lab3)
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define PORT 8080
#define MAXLINE 1000
int main()

{
struct sockaddr_in serveraddr,clientaddr;
int serverfd;
char *msg= "Hello from server";
char buffer[100];
socklen_t addrlen;
bzero(&serveraddr, sizeof(serveraddr));
serverfd = socket(AF_INET,SOCK_DGRAM,0);
printf("socket created\n");
serveraddr.sin_family= AF_INET;
serveraddr.sin_port= htons(PORT);
serveraddr.sin_addr.s_addr= htonl(INADDR_ANY);

bind(serverfd, (struct sockaddr*)&serveraddr,sizeof(serveraddr));


int len= sizeof(serverfd);

int n= recvfrom(serverfd, buffer, sizeof(buffer),0,(struct sockaddr*)&clientaddr,&len);
buffer[n]='\0';
puts(buffer);
sendto(serverfd, msg, MAXLINE, 0 , (struct sockaddr*)&clientaddr, sizeof(clientaddr));
close(serverfd);
}



//client of UDP (lab3)
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define PORT 8080
#define MAXLINE 1000
int main()

{
struct sockaddr_in serveraddr;
int clientfd;
char *msg= "Hello from client";
char buffer[100];
bzero(&serveraddr, sizeof(serveraddr));
clientfd = socket(AF_INET,SOCK_DGRAM,0);
printf("socket created\n");
serveraddr.sin_family= AF_INET;
serveraddr.sin_port= htons(PORT);
serveraddr.sin_addr.s_addr= inet_addr("127.0.0.1");

connect(clientfd, (struct sockaddr*)&serveraddr,sizeof(serveraddr));

sendto(clientfd, msg, MAXLINE, 0 , (struct sockaddr*)NULL, sizeof(serveraddr));

recvfrom(clientfd, buffer, sizeof(buffer),0,(struct sockaddr*)NULL,NULL);
puts(buffer);
close(clientfd);
}