#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define LISTEN_BACKLOG 5

void *handleConnection(int client_socket) {
    char buffer[BUFFER_SIZE];
    while (1) {
        int bytes_read = read(client_socket, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            break;
        }

        buffer[bytes_read] = '\0';
        printf("Received: %s\n", buffer);
        write(client_socket, buffer, bytes_read);
    }
    close(client_socket);
}

int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "-p") != 0) {
        fprintf(stderr, "Usage: %s -p <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[2]);
    // Create new socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in socket_address;
    memset(&socket_address, '\0', sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_address.sin_port = htons(port);

    printf("Port %d\n", port);

    int returnval;

    returnval = bind(socket_fd, (struct sockaddr *)&socket_address,
                     sizeof(socket_address));

    returnval = listen(socket_fd, LISTEN_BACKLOG);

    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    while (1) {
        int client_fd = accept(socket_fd, (struct sockaddr *)&client_address,
                               &client_address_len);
        handleConnection(client_fd);
        printf("Client disconnected\n");
    }

    close(socket_fd);
    return 0;
}