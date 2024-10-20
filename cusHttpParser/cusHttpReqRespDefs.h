

#ifndef __CUS_HTTP_REQ_RESP_DEFS_H__
#define __CUS_HTTP_REQ_RESP_DEFS_H__

typedef enum e_http_method
{
    HTTP_METHOD_GET = 0,
    HTTP_METHOD_POST,
    HTTP_METHOD_MAX,
} e_http_method_t;

typedef enum e_http_response_status_code
{
    HTTP_RESPONSE_STATUS_200 = 200,
    HTTP_RESPONSE_STATUS_404 = 404,
    HTTP_RESPONSE_STATUS_MAX,
}e_http_response_status_code_t;

typedef struct s_http_header_additional_fields
{
    char *name;
    char *value;
    struct s_http_header_additional_fields *next_field;
} s_http_header_additional_fields_t;

typedef struct s_http_request
{
    e_http_method_t method;
    char *path;
    char *protocol;
    s_http_header_additional_fields_t *fields;
} s_http_request_t;

typedef struct s_http_response
{
    char *protocol;
    e_http_response_status_code_t status_code;
    s_http_header_additional_fields_t *fields;
    uint8_t *data;
}s_http_response_t;

#endif // __CUS_HTTP_REQ_RESP_DEFS_H__
