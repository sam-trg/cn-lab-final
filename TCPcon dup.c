//SERVER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#define PORT 8080
void removeDuplicates(char *str) {
    char *words[100], result[1024] = {0};
    int found, n = 0;
    char *token = strtok(str, " ");
    while (token != NULL) {
        found = 0;
        for (int i = 0; i < n; i++) {
 if (strcmp(words[i], token) == 0) { found = 1; break; } }
        if (!found) {
words[n++] = token;
strcat(result, token);
strcat(result, " "); }
 token = strtok(NULL, " "); }
 strcpy(str, result);}
int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024];
    pid_t childpid;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
 perror("Socket failed");
 exit(EXIT_FAILURE); }
    address.sin_family = AF_INET; 
address.sin_addr.s_addr = INADDR_ANY;
 address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
 perror("Bind failed"); exit(EXIT_FAILURE); }
    if (listen(server_fd, 3) < 0) { 
perror("Listen failed"); exit(EXIT_FAILURE); }
    while (1) {
 if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) { 
perror("Accept failed"); exit(EXIT_FAILURE); }
 if ((childpid = fork()) == 0) {
close(server_fd);
while (1) {
 memset(buffer, 0, sizeof(buffer));
read(new_socket, buffer, 1024);
if (strcmp(buffer, "Stop") == 0) { break; }
removeDuplicates(buffer);
send(new_socket, buffer, strlen(buffer), 0);
}close(new_socket); exit(0);
} else { close(new_socket); wait(NULL); }}
return 0;}



//CLIENT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char message[1024], buffer[1024];
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
printf("Socket creation error\n"); return -1; }
    serv_addr.sin_family = AF_INET; serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) { 
printf("Invalid address\n"); return -1; }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
printf("Connection failed\n"); return -1; }
    while (1) {
        printf("Enter a sentence (or 'Stop' to exit): "); 
fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0;
        send(sock, message, strlen(message), 0);
        if (strcmp(message, "Stop") == 0) { break; }
        valread = read(sock, buffer, 1024); buffer[valread] = '\0';
        printf("Processed sentence: %s\n", buffer);
    }
    close(sock); return 0;
}

