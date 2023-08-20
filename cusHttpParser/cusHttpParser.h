

#ifndef __CUS_HTTP_PARSER_H__
#define __CUS_HTTP_PARSER_H__

#include "cusHttpUrlRouter.h"
#include "stdint.h"

typedef struct s_http_header_content
{
    e_http_method_t method;
    char *path;
    char *protocol;
} s_http_header_content_t;

s_http_header_content_t parse_http_data(const char *http_data, ssize_t data_length);

#endif // __CUS_HTTP_PARSER_H__
