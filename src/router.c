#include <string.h>
#include "router.h"

const char* route_request(const char* request)
{
    if (strstr(request, "GET /about ") != NULL)
    {
        return
            "<!DOCTYPE html>"
            "<html>"
            "<body>"
            "<h1>About Page</h1>"
            "<p>This is the about page.</p>"
            "</body>"
            "</html>";
    }

    if (strstr(request, "GET /contact ") != NULL)
    {
        return
            "<!DOCTYPE html>"
            "<html>"
            "<body>"
            "<h1>Contact Page</h1>"
            "<p>contact@shawarma.local</p>"
            "</body>"
            "</html>";
    }

    return
        "<!DOCTYPE html>"
        "<html>"
        "<body>"
        "<h1>Home Page</h1>"
        "<p>Welcome to Shawarma Web Server.</p>"
        "</body>"
        "</html>";
}