/*
   Echo what server received
*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE (256)
#define SERVER_SOCKET_NAME "server.socket"

int main(int argc, char *argv[])
{
    int connection_socket = -1;
    int data_socket = -1;
    int ret = -1;
    struct sockaddr_un server_name, client_name;
    socklen_t client_socklen = 0;
    char buffer[BUFFER_SIZE] = {0};
    int close_flag = 0;

    connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (connection_socket == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_name, 0, sizeof(server_name));
    server_name.sun_family = AF_UNIX;
    strncpy(server_name.sun_path, SERVER_SOCKET_NAME,
            sizeof(server_name.sun_path) - 1);

    ret = bind(connection_socket, (const struct sockaddr *)&server_name,
            sizeof(server_name));
    if (ret == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    ret = listen(connection_socket, 20);
    if (ret == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        client_socklen = sizeof(client_name);
        data_socket = accept(connection_socket, (struct sockaddr *)&client_name,
                &client_socklen);
        if (data_socket == -1)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("client name: %s|\n", client_name.sun_path);
        printf("client name len: %u\n", client_socklen);
        printf("struct sockaddr len %lu\n", sizeof(struct sockaddr));
        printf("sockaddr_un len %lu\n", sizeof(struct sockaddr_un));
        printf("offset of sun_path %lu\n", offsetof(struct sockaddr_un, sun_path));
        printf("sa_family_t len %lu\n", sizeof(sa_family_t));

        for (;;)
        {
            memset(buffer, 0, sizeof(buffer));
            ret = read(data_socket, buffer, sizeof(buffer) - 1);
            printf("read returns: %d\n", ret);

            if (ret == -1)
            {
                perror("read");
                exit(EXIT_FAILURE);
            }

            if (!strncmp("CLOSE", buffer, 5))
            {
                close_flag = 1;
                break;
            }

            printf("Server received: %s\n", buffer);
            for (int i = 0; i < strlen(buffer); ++i)
            {
                buffer[i] = toupper(buffer[i]);
            }

            printf("Server sent back: %s\n", buffer);
            ret = write(data_socket, buffer, strlen(buffer));
            if (ret == -1)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        if (close_flag)
        {
            puts("Closing communication");
            break;
        }

        close(data_socket);
    }

    close(connection_socket);
    unlink(SERVER_SOCKET_NAME);

    exit(EXIT_SUCCESS);
}
