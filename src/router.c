#include <string.h>
#include "router.h"

RouteResult route_request(const char *request)
{
    RouteResult result;

    if (strstr(request, "GET / ") != NULL)
    {
        result.status_code = 200;
        result.file_path = "public/index.html";
        return result;
    }

    if (strstr(request, "GET /about ") != NULL)
    {
        result.status_code = 200;
        result.file_path = "public/about.html";
        return result;
    }

    if (strstr(request, "GET /contact ") != NULL)
    {
        result.status_code = 200;
        result.file_path = "public/contact.html";
        return result;
    }

    result.status_code = 404;
    result.file_path = "public/404.html";

    return result;
}