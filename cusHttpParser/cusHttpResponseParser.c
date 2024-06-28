
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "trace.h"
#include "cusHttpReqRespDefs.h"
#include "cusHttpResponseParser.h"

static s_http_header_additional_fields_t *cus_header_fields_populate_name_and_value(char *name, char *value)
{
    s_http_header_additional_fields_t *field = (s_http_header_additional_fields_t *)malloc(sizeof(s_http_header_additional_fields_t));
    if (NULL == field)
    {
        return NULL;
    }

    field->next_field = NULL;

    size_t name_size = strlen(name) + 1;
    field->name = (char *)malloc(name_size);
    if (NULL == field->name)
    {
        free(field);
        return NULL;
    }
    memset(field->name, 0, name_size);
    snprintf(field->name, name_size, "%s", name);

    size_t value_size = strlen(value) + 1;
    field->value = (char *)malloc(value_size);
    if (NULL == field->value)
    {
        free(field->name);
        free(field);
        return NULL;
    }
    memset(field->value, 0, value_size);
    snprintf(field->value, value_size, "%s", value);
    return field;
}

static int cus_header_fields_populate_to_ll(s_http_response_t *response, char *name, char *value)
{
    if ((NULL == response) || (NULL == name) || (NULL == value))
    {
        return -1;
    }
    if (NULL == response->fields)
    {
        response->fields = cus_header_fields_populate_name_and_value(name, value);
        if (NULL == response->fields)
        {
            return -1;
        }
    }
    else
    {
        print_i("Field exist");
        s_http_header_additional_fields_t *field = response->fields;
        while (field->next_field)
        {
            field = field->next_field;
        }
        field->next_field = cus_header_fields_populate_name_and_value(name, value);
        if (NULL == field->next_field)
        {
            return -1;
        }
    }
    return 0;
}

static int cus_http_add_date(s_http_response_t *response)
{
    if (NULL == response)
    {
        return -1;
    }

    time_t now;
    struct tm *tm_info;

    time(&now);
    tm_info = gmtime(&now);

    char date_str[64];
    char *name = "Date";
    strftime(date_str, sizeof(date_str), "%a, %d %b %Y %H:%M:%S GMT", tm_info);
    return cus_header_fields_populate_to_ll(response, name, date_str);
}

static int cus_http_add_server(s_http_response_t *response)
{
    if (NULL == response)
    {
        return -1;
    }
    char *name = "Server";
    char *value = "Apache/2.4.41 (Unix)";
    return cus_header_fields_populate_to_ll(response, name, value);
}

static int cus_http_add_content_type(s_http_response_t *response)
{
    if (NULL == response)
    {
        return -1;
    }
    char *name = "Content-Type";
    char *value = "text/html; charset=UTF-8";
    return cus_header_fields_populate_to_ll(response, name, value);
}

static int cus_http_add_connection(s_http_response_t *response)
{
    if (NULL == response)
    {
        return -1;
    }
    char *name = "Connection";
    char *value = "close";
    return cus_header_fields_populate_to_ll(response, name, value);
}

static void cus_http_print_response(s_http_response_t *response)
{
    if (NULL != response)
    {
        print_e("Protocol: %s", response->protocol);
        print_e("Status: %d", response->status_code);
        s_http_header_additional_fields_t *field = response->fields;
        while (NULL != field)
        {
            print_e("Name: %s", field->name);
            print_e("Value: %s", field->value);
            field = field->next_field;
        }
    }
}

s_http_response_t *csu_http_response_prepare(s_http_request_t *request, e_http_response_status_code_t status_code)
{

    if (NULL == request)
    {
        print_e("Empty request.");
        return NULL;
    }
    if (NULL == request->protocol)
    {
        print_e("No protocol provided.");
        return NULL;
    }
    if ((status_code < HTTP_RESPONSE_STATUS_200) || (status_code > HTTP_RESPONSE_STATUS_MAX))
    {
        print_e("Illegal HTTP response status code provided.");
        return NULL;
    }

    s_http_response_t *response = (s_http_response_t *)malloc(sizeof(s_http_response_t));
    if (NULL == response)
    {
        return NULL;
    }

    size_t protocol_size = strlen(request->protocol) + 1;
    response->protocol = (char *)malloc(protocol_size);
    if (NULL == response->protocol)
    {
        free(response);
        return NULL;
    }
    memset(response->protocol, 0, protocol_size);
    snprintf(response->protocol, protocol_size, "%s", request->protocol);
    response->status_code = status_code;
    cus_http_add_date(response);
    cus_http_add_server(response);
    cus_http_add_content_type(response);
    cus_http_add_connection(response);

    return response;
}

char *cus_http_response_prepare_str(s_http_response_t *response)
{
    if(NULL == response)
    {
        return NULL;
    }
}
