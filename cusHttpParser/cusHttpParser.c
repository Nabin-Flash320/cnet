
#include "stdio.h"
#include "cusHttpParser.h"
#include "string.h"
#include "stdint.h"
#include "stdlib.h"
#include "trace.h"

static e_http_method_t get_http_method(const char *http_method);

s_http_header_content_t parse_http_data(const char *http_data, ssize_t data_length)
{
    s_http_header_content_t header_content;
    ssize_t copied_buffer_length = data_length + 1;
    char *copied_header = (char *)malloc(copied_buffer_length);
    if (copied_header)
    {
        memset(copied_header, 0, copied_buffer_length);
        snprintf(copied_header, copied_buffer_length, "%s", http_data);
        // TRACE_I("\n%s", copied_header);
        char *stack_pointer1;
        char *line = strtok_r(copied_header, "\r\n", &stack_pointer1);
        if (line)
        {
            // TRACE_I("Line is %s\n", line);
            char *stack_pointer2;
            char *method = strtok_r(line, " ", &stack_pointer2);
            char *path = strtok_r(NULL, " ", &stack_pointer2);
            char *protocol = strtok_r(NULL, " ", &stack_pointer2);

            if (method && path && protocol)
            {
                // TRACE_B("Method: %d", get_http_method(method));
                // TRACE_B("Path: %s", path);
                // TRACE_B("Protocol: %s", protocol);
                header_content.method = get_http_method(method);

                header_content.path = (char *)malloc(strlen(path) + 1);
                header_content.protocol = (char *)malloc(strlen(protocol) + 1);
                if (header_content.path && header_content.protocol)
                {
                    memset(header_content.path, 0, strlen(path) + 1);
                    memset(header_content.protocol, 0, strlen(protocol) + 1);

                    snprintf(header_content.path, strlen(path) + 1, "%s", path);
                    snprintf(header_content.protocol, strlen(protocol) + 1, "%s", protocol);
                }

                // Parse rest of the headers
                // while ((line = strtok_r(NULL, "\r\n", &stack_pointer1)) != NULL)
                // {
                //     char *stack_pointer3; // Pointer for strtok_r within the line
                //     char *header_key = strtok_r(line, ":", &stack_pointer3);
                //     char *header_value = strtok_r(NULL, ":", &stack_pointer3);
                //     if (header_key && header_value)
                //     {
                //         // Trim leading spaces from header value
                //         while (*header_value == ' ')
                //         {
                //             header_value++;
                //         }
                //         // TRACE_B("Header: %s: %s\n", header_key, header_value);
                //     }
                // }
            }
            else
            {
                TRACE_E("Method, path or protocol not found.");
            }
        }
    }
    free(copied_header);
    return header_content;
}

static e_http_method_t get_http_method(const char *http_method)
{
    e_http_method_t ret = HTTP_METHOD_MAX;
    if (0 == strcmp(http_method, "GET"))
    {
        ret = HTTP_METHOD_GET;
    }
    else if (0 == strcmp(http_method, "POST"))
    {
        ret = HTTP_METHOD_POST;
    }
    return ret;
}
