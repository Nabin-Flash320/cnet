

#ifndef __CUS_HTTP_PARSER_H__
#define __CUS_HTTP_PARSER_H__

#include <stdint.h>
#include <stddef.h>

#include "cusHttpReqRespDefs.h"

s_http_request_t parse_http_header(const char *http_data, size_t data_length);
void print_http_header(s_http_request_t *header_content);

#endif // __CUS_HTTP_PARSER_H__
