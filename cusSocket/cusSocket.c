// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "cusSocket.h"
#include "cusHttpRequestParser.h"
#include "cusHttpResponseParser.h"
#include "cusHttpUrlRouter.h"
#include "trace.h"

#define BUFFER_SIZE 4096
#define PORT 8080
#define IP "127.0.0.0"

static void process_http_headers(const char *data, ssize_t data_length);

int cus_socket_create()
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    char *ip = IP;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        TRACE_E("socket failed");
        exit(EXIT_FAILURE);
    }

    printf("Soccket successfully created with fd %d\n", server_fd);
    // Forcefully attaching socket to port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)))
    {
        TRACE_E("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
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
    TRACE_I("Socket started and listening for incoming request...");
    // Get and print the bound address and port
    struct sockaddr_in bound_addr;
    socklen_t len = sizeof(bound_addr);
    if (getsockname(server_fd, (struct sockaddr *)&bound_addr, &len) == -1)
    {
        perror("getsockname failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    char bound_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &bound_addr.sin_addr, bound_ip, sizeof(bound_ip));
    print_i("Server started at: \nhttp://%s:%d", bound_ip, ntohs(bound_addr.sin_port));

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
    printf("Buffer incoming: \n%s", data);
    // s_http_request_t header_content = parse_http_header(data, data_length);
    // print_http_header(&header_content);
    // csu_http_response_prepare(&header_content, HTTP_RESPONSE_STATUS_200);
}
