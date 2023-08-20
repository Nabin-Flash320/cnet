
#include "cusHttpUrlRouter.h"
#include "stddef.h"
#include "stdint.h"
#include "trace.h"

struct s_http_url_handler
{
    e_http_method_t method;
    char *url;
    http_callback cb;
    void *user_args;
};

s_http_url_handler_t http_register_handler(e_http_method_t method, const char *url, http_callback cb, void *user_args)
{
    TRACE_E("%s", url);
    return NULL;
}
