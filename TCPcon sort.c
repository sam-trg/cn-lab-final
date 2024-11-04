//SERVER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#define PORT 8080
void sortArray(int *arr, int n) {
    int temp;
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;}}}}
int main() {
    int server_fd, new_socket, valread, arr[100], n;
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
            read(new_socket, &n, sizeof(int));
            read(new_socket, arr, sizeof(int) * n);
            sortArray(arr, n);
            send(new_socket, arr, sizeof(int) * n, 0);
            int pid = getpid();
            send(new_socket, &pid, sizeof(int), 0);
            close(new_socket);
            exit(0);  } else {
            close(new_socket);
            wait(NULL);}}
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
    int arr[100], n, sortedArr[100], pid;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
 printf("Socket creation error\n"); return -1; }
    serv_addr.sin_family = AF_INET; serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) { 
printf("Invalid address or address not supported\n"); return -1; }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
printf("Connection failed\n"); return -1; }
    printf("Enter number of elements: "); 
scanf("%d", &n);
    printf("Enter elements of array: ");
    for (int i = 0; i < n; i++) {
 scanf("%d", &arr[i]); }
    send(sock, &n, sizeof(int), 0);
    send(sock, arr, sizeof(int) * n, 0);
    valread = read(sock, sortedArr, sizeof(int) * n);
    read(sock, &pid, sizeof(int));
    printf("Sorted array: ");
    for (int i = 0; i < n; i++) {
 printf("%d ", sortedArr[i]); }
    printf("\nProcess ID: %d\n", pid);
    close(sock); return 0;}

