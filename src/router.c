#include <string.h>
#include "router.h"

RouteResult route_request(const char *request)
{
    RouteResult result;

    if (strstr(request, "GET / ") != NULL)
    {
        result.status_code = 200;
        result.html =
            "<!DOCTYPE html>"
            "<html>"
            "<body>"
            "<h1>Home Page</h1>"
            "<p>Welcome to Shawarma Web Server.</p>"
            "</body>"
            "</html>";

        return result;
    }

    if (strstr(request, "GET /about ") != NULL)
    {
        result.status_code = 200;
        result.html =
            "<!DOCTYPE html>"
            "<html>"
            "<body>"
            "<h1>About Page</h1>"
            "<p>This is the about page.</p>"
            "</body>"
            "</html>";

        return result;
    }

    if (strstr(request, "GET /contact ") != NULL)
    {
        result.status_code = 200;
        result.html =
            "<!DOCTYPE html>"
            "<html>"
            "<body>"
            "<h1>Contact Page</h1>"
            "<p>contact@shawarma.local</p>"
            "</body>"
            "</html>";

        return result;
    }

    result.status_code = 404;
    result.html =
        "<!DOCTYPE html>"
        "<html>"
        "<body>"
        "<h1>404 Not Found</h1>"
        "<p>The requested page does not exist.</p>"
        "</body>"
        "</html>";

    return result;
}