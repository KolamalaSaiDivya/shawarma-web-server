#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#include "server.h"
#include "router.h"

int start_server(int port)
{
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("WSAStartup failed\n");
        return 1;
    }

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == INVALID_SOCKET)
    {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(
            server_fd,
            (struct sockaddr *)&server_addr,
            sizeof(server_addr)
        ) == SOCKET_ERROR)
    {
        printf("Bind failed\n");

        closesocket(server_fd);
        WSACleanup();

        return 1;
    }

    if (listen(server_fd, 5) == SOCKET_ERROR)
    {
        printf("Listen failed\n");

        closesocket(server_fd);
        WSACleanup();

        return 1;
    }

    printf("Shawarma Web Server listening on port %d\n", port);

    while (1)
    {
        SOCKET client_socket = accept(
            server_fd,
            NULL,
            NULL
        );

        if (client_socket == INVALID_SOCKET)
        {
            printf("Accept failed\n");
            continue;
        }

        char buffer[4096];

        int bytes_received = recv(
            client_socket,
            buffer,
            sizeof(buffer) - 1,
            0
        );

        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0';

            printf("\n===== HTTP REQUEST =====\n");
            printf("%s\n", buffer);
            printf("========================\n");

            const char *html = route_request(buffer);

            char response[8192];

            snprintf(
                response,
                sizeof(response),
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Connection: close\r\n"
                "\r\n"
                "%s",
                html
            );

            send(
                client_socket,
                response,
                (int)strlen(response),
                0
            );
        }

        closesocket(client_socket);
    }

    closesocket(server_fd);
    WSACleanup();

    return 0;
}