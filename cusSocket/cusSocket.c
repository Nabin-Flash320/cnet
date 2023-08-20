// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "cusSocket.h"
#include "cusHttpParser.h"
#include "cusHttpUrlRouter.h"
#include "trace.h"

#define BUFFER_SIZE 4096
#define PORT 8080

static void process_http_headers(const char *data, ssize_t data_length);

int cus_socket_create()
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        TRACE_E("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)))
    {
        TRACE_E("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        TRACE_E("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        TRACE_E("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
        TRACE_E("accept");
        exit(EXIT_FAILURE);
    }
    else
    {
        valread = read(new_socket, buffer, 1024);
        process_http_headers(buffer, valread);
        send(new_socket, hello, strlen(hello), 0);

        // closing the connected socket
        close(new_socket);
        // closing the listening socket
        shutdown(server_fd, SHUT_RDWR);
    }
    return 0;
}

// Function to extract and process HTTP headers
static void process_http_headers(const char *data, ssize_t data_length)
{
    s_http_header_content_t header_content = parse_http_data(data, data_length);
    TRACE_B("Method: %d", header_content.method);
    TRACE_B("Path: %s", header_content.path);
    TRACE_B("Protocol: %s", header_content.protocol);
    // http_register_handler
}
