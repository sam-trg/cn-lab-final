// SERVER
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORTNO 10200
int main()
{
int sockfd,newsockfd,portno,clilen,n=1;
struct sockaddr_in seraddr,cliaddr;
int i,value;
sockfd = socket(AF_INET,SOCK_STREAM,0);
seraddr.sin_family = AF_INET;
seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
seraddr.sin_port = htons(PORTNO);
bind(sockfd,(struct sockaddr *)&seraddr,sizeof(seraddr));

listen(sockfd,5);
while (1) {
char buf[256];
printf("server waiting");
clilen = sizeof(clilen);
newsockfd=accept(sockfd,(struct sockaddr *)&cliaddr,&clilen);
n = read(newsockfd,buf,sizeof(buf));
for(int i=0;i<strlen(buf);i++)
{
buf[i]=buf[i]-1;
}
printf(" \nMessage from Client %s \n",buf);
n = write(newsockfd,buf,sizeof(buf));
}
}



// client
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>  
#include <unistd.h>
int main()
{
int len,result,sockfd,n=1;
struct sockaddr_in address;
char ch[256],buf[256];

sockfd = socket(AF_INET, SOCK_STREAM, 0);
address.sin_family=AF_INET;
address.sin_addr.s_addr=inet_addr("127.0.0.1");
address.sin_port=htons(10200);
len = sizeof(address);
result=connect(sockfd,(struct sockaddr *)&address,len);
if(result==-1)
{
perror("\nCLIENT ERROR");
exit(1);
}
printf("\nENTER STRING\t");
fgets(ch,256,stdin);

for(int i=0;i<strlen(ch);i++)
{
ch[i]=ch[i]+1;
}
ch[strlen(ch)]='\0';
write(sockfd,ch,strlen(ch));
printf("STRING SENT BACK FROM SERVER IS ..... ");
while(n){
n=read(sockfd,buf,sizeof(buf));
puts(buf);
}
}

