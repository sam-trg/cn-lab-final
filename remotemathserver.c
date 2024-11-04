//SERVER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#define PORT 8080
void performOperation(int num1, int num2, char operator, char *result) {
    int res;
    switch (operator) {
        case '+': res = num1 + num2; break;
        case '-': res = num1 - num2; break;
        case '*': res = num1 * num2; break;
        case '/': if (num2 != 0) { res = num1 / num2; }
 else { strcpy(result, "Error: Division by zero");
 return; } break;
default: strcpy(result, "Error: Invalid operator"); return; }
    sprintf(result, "Result: %d", res);}
int main() {
    int server_fd, new_socket, num1, num2;
    char operator, result[1024] = {0};
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pid_t childpid;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
 perror("Socket failed"); exit(EXIT_FAILURE); }
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
            read(new_socket, &num1, sizeof(int)); 
read(new_socket, &num2, sizeof(int));
 read(new_socket, &operator, sizeof(char));
performOperation(num1, num2, operator, result);
send(new_socket, result, strlen(result), 0);
close(new_socket);
 exit(0);
 } else {
 close(new_socket);
 wait(NULL);}}return 0;}





//CLIENT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
int main() {
    int sock = 0, valread, num1, num2;
    char operator, buffer[1024] = {0};
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
printf("Socket creation error\n"); return -1; }
    serv_addr.sin_family = AF_INET; 
serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
 printf("Invalid address or address not supported\n"); 
return -1; }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
 printf("Connection failed\n"); return -1; }
    printf("Enter two integers: ");
 scanf("%d %d", &num1, &num2);
    printf("Enter an operator (+, -, *, /): "); 
scanf(" %c", &operator);
    send(sock, &num1, sizeof(int), 0);
 send(sock, &num2, sizeof(int), 0); 
send(sock, &operator, sizeof(char), 0);
    read(sock, buffer, 1024);
 printf("Result from server: %s\n", buffer);
    close(sock); return 0;}

