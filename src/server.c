#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#include "server.h"
#include "router.h"

static char* read_file(const char* path)
{
    FILE* file = fopen(path, "rb");

    if (!file)
    {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* content = malloc(size + 1);

    if (!content)
    {
        fclose(file);
        return NULL;
    }

    fread(content, 1, size, file);
    content[size] = '\0';

    fclose(file);

    return content;
}

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

    if (bind(server_fd,
             (struct sockaddr*)&server_addr,
             sizeof(server_addr)) == SOCKET_ERROR)
    {
        printf("Bind failed\n");
        return 1;
    }

    if (listen(server_fd, 5) == SOCKET_ERROR)
    {
        printf("Listen failed\n");
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

            RouteResult route = route_request(buffer);

            char* html = read_file(route.file_path);

            if (html)
            {
                char response[16384];

                if (route.status_code == 200)
                {
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
                }
                else
                {
                    snprintf(
                        response,
                        sizeof(response),
                        "HTTP/1.1 404 Not Found\r\n"
                        "Content-Type: text/html\r\n"
                        "Connection: close\r\n"
                        "\r\n"
                        "%s",
                        html
                    );
                }

                send(
                    client_socket,
                    response,
                    (int)strlen(response),
                    0
                );

                free(html);
            }
        }

        closesocket(client_socket);
    }

    closesocket(server_fd);
    WSACleanup();

    return 0;
}