

#ifndef __CUS_HTTP_RESPONSE_PARSER_H__
#define __CUS_HTTP_RESPONSE_PARSER_H__

#include "cusHttpReqRespDefs.h"

s_http_response_t *cus_http_response_prepare(s_http_request_t *request, e_http_response_status_code_t status_code);
const char *cus_http_response_prepare_str(s_http_response_t *response);

#endif // __CUS_HTTP_RESPONSE_PARSER_H__
