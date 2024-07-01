// Server side C/C++ program to demonstrate Socket
// programming

/*
_WIN32 for Windows 32 bit OR 64 bit
_WIN64 for Windows 64 bit only
__unix__ for Unix
*/

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "cusSocket.h"
#include "cusHttpRequestParser.h"
#include "cusHttpResponseParser.h"
#include "cusHttpUrlRouter.h"
#include "trace.h"

#if defined(_WIN32)
#define STARTUP                                               \
    WSADATA wsaData;                                          \
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)            \
    {                                                         \
        printf("WSAStartup failed: %d\n", WSAGetLastError()); \
        exit(EXIT_FAILURE);                                   \
    }
#define CLEANUP                                                 \
    WSACleanup();                                               \
    exit(EXIT_FAILURE)
#else
#define STARTUP
#define CLEANUP                  \
    TRACE_E("Operation failed"); \
    exit(EXIT_FAILURE);
#endif

#define BUFFER_SIZE 4096
#define PORT 8080
#define IP "127.0.0.1"

static void process_http_headers(const char *data, ssize_t data_length);

int cus_socket_create()
{
    STARTUP

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    char *ip = IP;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        CLEANUP;
    }

    printf("Soccket successfully created with fd %d\n", server_fd);

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = inet_addr(ip);

    // Forcefully attaching socket to port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        CLEANUP;
    }
    if (listen(server_fd, 3) < 0)
    {
        CLEANUP;
    }
    TRACE_I("Socket started and listening for incoming request...");
    // Get and print the bound address and port
    struct sockaddr_in bound_addr;
    socklen_t len = sizeof(bound_addr);
    if (getsockname(server_fd, (struct sockaddr *)&bound_addr, &len) == -1)
    {
        close(server_fd);
        CLEANUP;
    }

    print_i("Server started at: \nhttp://%s:%d", IP, PORT);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
        CLEANUP;
    }
    else
    {
        valread = recv(new_socket, buffer, 1024, 0);
        process_http_headers(buffer, valread);
        send(new_socket, hello, strlen(hello), 0);

        // closing the connected socket
        close(new_socket);
        // closing the listening socket
        // shutdown(server_fd, SHUT_RDWR);
        shutdown(server_fd, SD_BOTH);

        CLEANUP;
    }
    return 0;
}

// Function to extract and process HTTP headers
static void process_http_headers(const char *data, ssize_t data_length)
{
    // printf("Buffer incoming: \n%s", data);
    s_http_request_t header_content = parse_http_header(data, data_length);
    print_http_header(&header_content);
    csu_http_response_prepare(&header_content, HTTP_RESPONSE_STATUS_200);
}
