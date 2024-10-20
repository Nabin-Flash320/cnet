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
#define STARTUP                                                \
    WSADATA wsaData;                                           \
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)             \
    {                                                          \
        TRACE_I("WSAStartup failed: %d\n", WSAGetLastError()); \
        exit(EXIT_FAILURE);                                    \
    }
#define CLEANUP   \
    WSACleanup(); \
    exit(EXIT_FAILURE)
#else
#define STARTUP
#define CLEANUP \
    exit(EXIT_FAILURE);
#endif

#define BUFFER_SIZE 4096
#define PORT 8080
#define IP "127.0.0.2"

static int server_fd;

static s_http_request_t process_http_headers(const char *data, ssize_t data_length);

int cus_socket_create()
{
    STARTUP

    int new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *ip = IP;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        CLEANUP;
    }

    TRACE_I("Socket successfully created with fd %d", server_fd);

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = inet_addr(ip);

    int opt = 1;
    // Set this opetion to reuse the port immediately after it is freed.
    // Due to TIME_WAIT feature of TCP stack, this might prevent from binding to the 
    // port after cleaned and restarted the server.
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        TRACE_E("Failed to set SO_REUSEADDR");
        CLEANUP;
    }

    // Forcefully attaching socket to port 8080
    int bind_err = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    if (bind_err < 0)
    {
        TRACE_E("Binding error(code: %d)", bind_err);
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

    TRACE_I("Server started at: http://%s:%d", IP, PORT);

    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0)
        {
            continue;
        }
        else
        {
            valread = recv(new_socket, buffer, 1024, 0);
            s_http_request_t request = process_http_headers(buffer, valread);
            s_http_response_t *response = cus_http_response_prepare(&request, HTTP_RESPONSE_STATUS_200);
            const char *response_str = cus_http_response_prepare_str(response);
            TRACE_B("\n%s", response_str);
            send(new_socket, response_str, strlen(response_str), 0);

            // closing the connected socket
            close(new_socket);
        }
    }

    return 0;
}

int cus_socket_close()
{
#if defined(_WIN32)
    shutdown(server_fd, SD_BOTH);
#else
    shutdown(server_fd, SHUT_RDWR);
#endif
    CLEANUP;
}

// Function to extract and process HTTP headers
static s_http_request_t process_http_headers(const char *data, ssize_t data_length)
{
    TRACE_I("Buffer incoming: \n%s", data);
    s_http_request_t header_content = parse_http_header(data, data_length);
    // print_http_header(&header_content);

    return header_content;
}
