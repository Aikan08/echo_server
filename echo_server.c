#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT 8000
#define SIZE 1024
#define MAX 80



int get_host_socket(struct sockaddr_in* address_socket) {

    char host_buffer[INET_ADDRSTRLEN];
    char service_buffer[NI_MAXSERV];

    socklen_t addr_len = sizeof(*address_socket);

    int err = getnameinfo(
        (struct sockaddr *) address_socket,
        addr_len,
        host_buffer,
        sizeof(host_buffer),
        service_buffer,
        sizeof(service_buffer),
        NI_NUMERICHOST
    );


    if (err != 0) {
        printf("[-]It's not working %d!\n", err);
        return -1;
    }

    printf("Client %s:%s\n", host_buffer, service_buffer);

    return 0;
}


int create_echo_server(int8_t* server_socket, struct sockaddr_in* address_socket) {

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

    switch (inet_pton(AF_INET, "127.0.0.1", &(address_socket -> sin_addr.s_addr))) {
        case 0: 
            printf("[-] Invalid IP-address");
            return -1; 
        case -1: 
            printf("[-] Invalid the address family"); 
            return -1;
        default: 
            break;
    }

    if (bind(*server_socket, (struct sockaddr *) address_socket, sizeof(*address_socket)) < 0) {
        printf("[-] Bind socket failed.");
        return -1;
    } else {
        printf("\033[0;32m[+]Bound socket successfully.\033[0m\n");
    }
  
    if (listen(*server_socket, 3) < 0) {
        printf("[-]Listen socket failed.");
        return -1;
    } else {
        printf("\033[0;32m[+]Listened socket successfully.\033[0m\n");
    }

    return 0;
}


void run_echo_server(int8_t* server_socket, struct sockaddr_in* address_socket) {
    int8_t client_socket;
    socklen_t len_address;
    struct sockaddr_in address_client_socket;

    char buff[MAX];
    char host_buffer[100];

    if (create_echo_server(server_socket, address_socket) != 0) {
        exit(0);
    }

    client_socket = accept(*server_socket, (struct sockaddr *) &address_client_socket, &len_address);

    if (client_socket < 0) {
        printf("[-]Socket server accept failed.");
        exit(0);
    } else {
        printf("\033[0;32m[+]Server socket accept the client...\033[0m\n");
    }

    if (get_host_socket(&address_client_socket) != 0) {
        exit(0);
    }

    while (1) {

        bzero(buff, MAX);

        read(client_socket, buff, sizeof(buff));
        
        bzero(host_buffer, 100);

        write(client_socket, buff, sizeof(buff));
    }

    close(*server_socket);
}


int main(void) {

    int8_t sockfd;
    struct sockaddr_in address_socket;

    run_echo_server(&sockfd, &address_socket);
}