//server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/wait.h>
#define PORT 8080
#define BUFFER_SIZE 1024

void get_daytime(char *buffer, int size) {
 time_t rawtime;
 struct tm *timeinfo;
 time(&rawtime);
 timeinfo = localtime(&rawtime);

 strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeinfo);
}

void handle_client(int client_socket) {
 char buffer[BUFFER_SIZE];
 char result[BUFFER_SIZE];
 pid_t pid = getpid(); 
 
 get_daytime(buffer, sizeof(buffer));
 
 snprintf(result, sizeof(result), "Time: %s | Process ID: %d\n", buffer,
pid);
 
 send(client_socket, result, strlen(result), 0);
 
 close(client_socket);
 printf("Server: Closed connection with client (PID: %d).\n", pid);
 exit(0); 
}
int main(){
 int server_socket, client_socket;
 struct sockaddr_in server_addr, client_addr;
 socklen_t addr_len = sizeof(client_addr);
 pid_t child_pid;

 if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
 perror("Server: Socket creation failed");
 exit(EXIT_FAILURE);
 }
 
 server_addr.sin_family = AF_INET;
 server_addr.sin_addr.s_addr = INADDR_ANY;
 server_addr.sin_port = htons(PORT);
  if (bind(server_socket, (struct sockaddr *)&server_addr,
sizeof(server_addr)) < 0) {
 perror("Server: Bind failed");
 close(server_socket);
 exit(EXIT_FAILURE);
 }
 
 if (listen(server_socket, 5) < 0) {
 perror("Server: Listen failed");
 close(server_socket);
 exit(EXIT_FAILURE);
 }
 printf("Server: Listening on port %d...\n", PORT);
 while (1) {
  if ((client_socket = accept(server_socket, (struct sockaddr
*)&client_addr, &addr_len)) < 0) {
 perror("Server: Accept failed");
 continue;
 }
 printf("Server: New client connected.\n");
  if ((child_pid = fork()) == 0) {
  close(server_socket);
 handle_client(client_socket);
 } else if (child_pid > 0) {
  close(client_socket); 
 } else {
 perror("Server: Fork failed");
 }
 }
 close(server_socket);
 return 0;
}






//client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define BUFFER_SIZE 1024
int main() {
 int client_socket;
 struct sockaddr_in server_addr;
 char buffer[BUFFER_SIZE];
 int read_size;
 
 if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
 perror("Client: Socket creation failed");
 exit(EXIT_FAILURE);
 }
  server_addr.sin_family = AF_INET;
 server_addr.sin_port = htons(PORT);
 server_addr.sin_addr.s_addr = INADDR_ANY;
  if (connect(client_socket, (struct sockaddr *)&server_addr,
sizeof(server_addr)) < 0) {
 perror("Client: Connection failed");
 exit(EXIT_FAILURE);
 }
 printf("Client: Connected to server.\n");
 read_size = recv(client_socket, buffer, BUFFER_SIZE, 0);
 buffer[read_size] = '\0';
  printf("Received from server: %s\n", buffer);
  close(client_socket);
 return 0;
}
