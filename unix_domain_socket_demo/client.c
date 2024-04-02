/*
   Send message to server
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#define SERVER_SOCKET_NAME "server.socket"
#define CLIENT_SOCKET_NAME "client.socket"
#define BUFFER_SIZE (256)

int main(int argc, char *argv[])
{
    int data_socket;
    int ret;
    struct sockaddr_un server_name, client_name;
    char buffer[BUFFER_SIZE] = {0};

    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (data_socket == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&client_name, 0, sizeof(client_name));
    client_name.sun_family = AF_UNIX;
    strncpy(client_name.sun_path, CLIENT_SOCKET_NAME,
            sizeof(client_name.sun_path));
    ret = bind(data_socket, (const struct sockaddr *)&client_name,
            sizeof(client_name));
    if (ret == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    memset(&server_name, 0, sizeof(server_name));
    server_name.sun_family = AF_UNIX;
    strncpy(server_name.sun_path, SERVER_SOCKET_NAME,
            sizeof(server_name.sun_path) - 1);

    ret = connect(data_socket, (const struct sockaddr *)&server_name,
            sizeof(server_name));
    if (ret == -1)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        memset(buffer, 0, sizeof(buffer));
        printf("Client sends: ");
        ret = scanf("%s", buffer);
        if (ret == EOF && errno != 0)
        {
            perror("scanf");
            exit(EXIT_FAILURE);
        }

        if (ret > 0)
        {
            printf("len %lu\n", strlen(buffer));
            ret = write(data_socket, buffer, strlen(buffer));
            if (ret == -1)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }

            if (!strncmp("CLOSE", buffer, 5))
            {
                puts("Bye");
                break;
            }

            ret = read(data_socket, buffer, sizeof(buffer) - 1);
            if (ret == -1)
            {
                perror("read");
                exit(EXIT_FAILURE);
            }

            printf("client received: %s\n", buffer);
        }

        while (getchar() != '\n');
    }

    close(data_socket);
    unlink(CLIENT_SOCKET_NAME);

    exit(EXIT_SUCCESS);
}
