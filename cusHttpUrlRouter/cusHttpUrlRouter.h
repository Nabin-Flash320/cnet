

#ifndef __CUS_HTTP_URL_ROUTER_H__
#define __CUS_HTTP_URL_ROUTER_H__


typedef enum e_http_method
{
    HTTP_METHOD_GET = 0,
    HTTP_METHOD_POST,
    HTTP_METHOD_MAX,
} e_http_method_t;

typedef void (*http_callback)(e_http_method_t method, void *user_args);

typedef struct s_http_url_handler *s_http_url_handler_t;

s_http_url_handler_t http_register_handler(e_http_method_t method, const char *url, http_callback cb, void *user_args);


#endif // __CUS_HTTP_URL_ROUTER_H__

