
#include <stdio.h>

#include <string.h>

#ifdef _WIN32
    #define strtok_r strtok_s
#endif

#include <stdint.h>
#include <stdlib.h>

#include "trace.h"
#include "cusHttpRequestParser.h"


static e_http_method_t get_http_method(const char *http_method);
static char *get_http_method_str(e_http_method_t method);
static int cus_parse_and_populate_headers(char *copied_header, size_t data_length, s_http_request_t *header_content, size_t offset);
static int cus_parse_and_populate_method_path_protocol(char *copied_header, size_t data_length, s_http_request_t *header_content);
static s_http_header_additional_fields_t *populate_name_and_value(char *header, size_t name_size, size_t value_size);
static int cus_header_fields_populate_to_ll(s_http_request_t *header_content, char *header, size_t name_size, size_t value_size);
static void print_fields(s_http_request_t *header_content);

s_http_request_t parse_http_header(const char *http_data, size_t data_length)
{
    s_http_request_t header_content;
    header_content.fields = NULL;
    size_t copied_buffer_length = data_length + 1;
    char *copied_header = (char *)malloc(copied_buffer_length);
    if (copied_header)
    {
        memset(copied_header, 0, copied_buffer_length);
        snprintf(copied_header, copied_buffer_length, "%s", http_data);
        int len = cus_parse_and_populate_method_path_protocol(copied_header, copied_buffer_length, &header_content);
        cus_parse_and_populate_headers(copied_header, copied_buffer_length, &header_content, len);
        free(copied_header);
    }
    return header_content;
}

void print_http_header(s_http_request_t *header_content)
{
    TRACE_B("Method: %s", get_http_method_str(header_content->method));
    TRACE_B("Path: %s", header_content->path);
    TRACE_B("Protocol: %s", header_content->protocol);
    print_fields(header_content);
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

static char *get_http_method_str(e_http_method_t method)
{
    char *method_name = NULL;
    switch (method)
    {
    case HTTP_METHOD_GET:
    {
        method_name = "GET";
        break;
    }
    case HTTP_METHOD_POST:
    {
        method_name = "POST";
        break;
    }
    default:
    {
        break;
    }
    }
    return method_name;
}

static int cus_parse_and_populate_headers(char *copied_header, size_t data_length, s_http_request_t *header_content, size_t offset)
{
    char *saved_pointer;
    char *saved_pointer2;
    char *header = NULL;
    char *headers_begin = strtok_r(copied_header, "\r\n", &saved_pointer);
    while (header = strtok_r(NULL, "\r\n", &saved_pointer))
    {
        size_t field_len = strcspn(header, "\r\n");
        size_t field_name_len = strcspn(header, " ");
        size_t field_value_len = field_len - field_name_len + 1;
        cus_header_fields_populate_to_ll(header_content, header, field_name_len, field_value_len);
    }
}

static int cus_parse_and_populate_method_path_protocol(char *copied_header, size_t data_length, s_http_request_t *header_content)
{
    int ret = -1;
    ret = strcspn(copied_header, "\r\n");
    if (0 < ret)
    {
        char *data_holder = (char *)malloc(ret);
        if (data_holder)
        {
            memset(data_holder, 0, ret);
            strncpy(data_holder, copied_header, ret);
            char *save_pointer2;
            char *method = strtok_r(data_holder, " ", &save_pointer2);
            char *path = strtok_r(NULL, " ", &save_pointer2);
            char *protocol = strtok_r(NULL, " ", &save_pointer2);
            if (method, path, protocol)
            {
                header_content->method = get_http_method(method);

                header_content->path = (char *)malloc(strlen(path) + 1);
                header_content->protocol = (char *)malloc(strlen(protocol) + 1);
                if (header_content->path && header_content->protocol)
                {
                    memset(header_content->path, 0, strlen(path) + 1);
                    memset(header_content->protocol, 0, strlen(protocol) + 1);

                    snprintf(header_content->path, strlen(path) + 1, "%s", path);
                    snprintf(header_content->protocol, strlen(protocol) + 1, "%s", protocol);
                }
            }
        }
    }
    return ret;
}

static s_http_header_additional_fields_t *populate_name_and_value(char *header, size_t name_size, size_t value_size)
{
    s_http_header_additional_fields_t *header_field = (s_http_header_additional_fields_t *)malloc(sizeof(s_http_header_additional_fields_t));
    header_field = (s_http_header_additional_fields_t *)malloc(sizeof(s_http_header_additional_fields_t));
    if (NULL == header_field)
    {
        return NULL;
    }
    header_field->next_field = NULL;
    header_field->name = (char *)malloc(name_size + 1);
    if (NULL == header_field->name)
    {
        free(header_field);
        header_field = NULL;
        return NULL;
    }
    memset(header_field->name, 0, name_size + 1);
    snprintf(header_field->name, name_size, "%s", header);

    header_field->value = (char *)malloc(value_size + 1);
    if (NULL == header_field->value)
    {
        free(header_field->name);
        free(header_field);
        header_field = NULL;
        return NULL;
    }
    memset(header_field->value, 0, value_size);
    snprintf(header_field->value, value_size, "%s", header + name_size + 1);
    return header_field;
}

static int cus_header_fields_populate_to_ll(s_http_request_t *header_content, char *header, size_t name_size, size_t value_size)
{
    if ((NULL == header) || (name_size < 1) || (value_size < 1))
    {
        return -1;
    }
    if (NULL == header_content->fields)
    {
        header_content->fields = populate_name_and_value(header, name_size, value_size);
        if (NULL == header_content)
        {
            return -1;
        }
    }
    else
    {
        s_http_header_additional_fields_t *field_head = header_content->fields;
        while (field_head->next_field)
        {
            field_head = field_head->next_field;
        }

        field_head->next_field = populate_name_and_value(header, name_size, value_size);
        if (NULL == field_head->next_field)
        {
            return -1;
        }
    }
    return 0;
}

static void print_fields(s_http_request_t *header_content)
{
    s_http_header_additional_fields_t *fields_head = header_content->fields;
    while (fields_head)
    {
        print_b("Name: %s and Value: %s", fields_head->name, fields_head->value);
        fields_head = fields_head->next_field;
    }
}
