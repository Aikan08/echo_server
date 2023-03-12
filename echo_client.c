#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>


#define PORT 8000
#define SIZE 1024
#define MAX 80


int create_echo_client(int8_t *server_socket, const char* address, struct sockaddr_in* address_socket) {

    *server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (*server_socket < 0) {
        printf("[-]Create socket failed.");
        return -1;
    } else {
        printf("\033[0;32m[+]Created socket successfully.\033[0m\n");
    }

    bzero(address_socket, sizeof(*address_socket));
 
    address_socket -> sin_family = AF_INET;
    address_socket -> sin_port = htons(PORT);

    switch (inet_pton(AF_INET, address, &(address_socket -> sin_addr.s_addr))) {
        case 0: 
            printf("[-] Invalid IP-address"); 
            return -1;
        case -1: 
            printf("[-] Invalid the address family"); 
            return -1;
        default: 
            break;
    }


    if (connect(*server_socket, (struct sockaddr *) address_socket, sizeof(*address_socket)) != 0) {
        printf("[-] Connection with the server failed...\n");
        return -1;
    } else {
        printf("\033[0;32m[+] Connected with the server...\033[0m\n");
    }

    return 0;
}


void run_echo_client(int8_t* server_socket, char* address, struct sockaddr_in* address_socket) {
    char buff[MAX];
    int n;


    if (create_echo_client(server_socket, address, address_socket) != 0) {
        exit(0);
    }

    while (1) {
        bzero(buff, sizeof(buff));

        printf("Enter the string: ");
        n = 0;

        while((buff[n++] = getchar()) != '\n');

        write(*server_socket, buff, sizeof(buff));
       
        bzero(buff, sizeof(buff));

        read(*server_socket, buff, sizeof(buff));

        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client exit...");
            break;
        } else {
            printf("From server %s : %s", address, buff);
        }
    }

    close(*server_socket);
}


int main(int argc, char **argv) {
    int8_t sockfd;
    struct sockaddr_in address_socket;
   
    if (argv[1] != NULL) {
       run_echo_client(&sockfd, argv[1], &address_socket);
    } else {
       printf("Please input IP-address server");
    }
}
