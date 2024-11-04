//server 4 encryptdecrypt
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
void decryptMessage(char *message) {
    for (int i = 0; message[i] != '\0'; i++) {
        if ((message[i] >= 'a' && message[i] <= 'z') || (message[i] >= 'A' && message[i] <= 'Z')) {
            message[i] -= 4;}}}
int main(int argc, char const* argv[])
{
  char encryptedMessage[1024] = {0};
 int server_fd, new_socket;
 ssize_t valread;
 struct sockaddr_in address;
 int opt = 1;
 socklen_t addrlen = sizeof(address);
 char buffer[1024] = { 0 };
 char* hello = "Hello from server";
 
 server_fd = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(server_fd, SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(opt));
 address.sin_family = AF_INET;
 address.sin_addr.s_addr = INADDR_ANY;
 address.sin_port = htons(PORT);
  bind(server_fd, (struct sockaddr*)&address,sizeof(address));
 listen(server_fd, 3);
new_socket= accept(server_fd, (struct sockaddr*)&address,&addrlen);
 valread = read(new_socket, buffer, 1024 - 1);
  printf("Encrypted msg rcvd from client side : %s\n", buffer);
 strcpy(encryptedMessage, buffer);
    decryptMessage(buffer);
    printf("after performing deryption =  message: %s\n", buffer);
 send(new_socket, hello, strlen(hello), 0);
 printf("Hello message sent\n");
  close(new_socket);
  close(server_fd);
 return 0;
}

//CLIENT LAB1 TCP EX1

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

void encryptMessage(char *message) {
    for (int i = 0; message[i] != '\0'; i++) {
        if ((message[i] >= 'a' && message[i] <= 'z') || (message[i] >= 'A' && message[i] <= 'Z')) {
            message[i] += 4;
        }
    }
}

int main(int argc, char const* argv[]) {
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    
    
    char hello[1024] = "Hello from client";
    char buffer[1024] = { 0 };
    
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    
     inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    
    status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  
    printf("Enrter message before enryption :");
     fgets(hello, sizeof(hello), stdin);
hello[strcspn(hello, "\n")] = 0; 
        encryptMessage(hello);
    
    
    send(client_fd, hello, strlen(hello), 0);
    printf("Encrypted message sent: %s\n", hello);
    
        valread = read(client_fd, buffer, 1024 - 1); 
    printf(" msg from server: %s\n", buffer);
    
        close(client_fd);
    return 0;
}

