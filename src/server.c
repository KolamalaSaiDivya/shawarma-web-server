#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#include "server.h"
#include "router.h"
#include "mime.h"

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

    if (bind(
            server_fd,
            (struct sockaddr*)&server_addr,
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

        char request_buffer[4096];

        int bytes_received = recv(
            client_socket,
            request_buffer,
            sizeof(request_buffer) - 1,
            0
        );

        if (bytes_received > 0)
        {
            request_buffer[bytes_received] = '\0';

            printf("\n===== HTTP REQUEST =====\n");
            printf("%s\n", request_buffer);
            printf("========================\n");

            RouteResult route =
                route_request(request_buffer);

            char* file_content =
                read_file(route.file_path);

            if (file_content)
            {
                const char* mime_type =
                    get_mime_type(route.file_path);

                char response[32768];

                if (route.status_code == 200)
                {
                    snprintf(
                        response,
                        sizeof(response),
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: %s\r\n"
                        "Connection: close\r\n"
                        "\r\n"
                        "%s",
                        mime_type,
                        file_content
                    );
                }
                else
                {
                    snprintf(
                        response,
                        sizeof(response),
                        "HTTP/1.1 404 Not Found\r\n"
                        "Content-Type: %s\r\n"
                        "Connection: close\r\n"
                        "\r\n"
                        "%s",
                        mime_type,
                        file_content
                    );
                }

                send(
                    client_socket,
                    response,
                    (int)strlen(response),
                    0
                );

                free(file_content);
            }
            else
            {
                const char* response =
                    "HTTP/1.1 500 Internal Server Error\r\n"
                    "Content-Type: text/plain\r\n"
                    "Connection: close\r\n"
                    "\r\n"
                    "Unable to read requested file.";

                send(
                    client_socket,
                    response,
                    (int)strlen(response),
                    0
                );
            }
        }

        closesocket(client_socket);
    }

    closesocket(server_fd);
    WSACleanup();

    return 0;
}