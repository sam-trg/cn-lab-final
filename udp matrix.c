//  server
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>


#define PORT 8080
#define MAXLINE 1024


int main() {
    int sockfd, matrix[3][3], i = 0;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("Server listening...\n");
                                                   
socklen_t len = sizeof(cliaddr);
    while (i < 3) {
        recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        sscanf(buffer, "%d %d %d", &matrix[i][0], &matrix[i][1], &matrix[i][2]);
        i++;
    }

    printf("Matrix received:\n");
    for (i = 0; i < 3; i++) printf("%d %d %d\n", matrix[i][0], matrix[i][1], matrix[i][2]);

    close(sockfd);
    return 0;
}



// CLIENT
#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 8080

int main() {
    int sockfd, matrix[3][3];
    struct sockaddr_in servaddr;
    char buffer[50];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // Take matrix input from user
    printf("Enter a 3x3 matrix (row-wise):\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            scanf("%d", &matrix[i][j]);


   }
    }

    // Send matrix rows to server
    for (int i = 0; i < 3; i++) {
        sprintf(buffer, "%d %d %d", matrix[i][0], matrix[i][1], matrix[i][2]);
        sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    }

    close(sockfd);
    return 0;
}
