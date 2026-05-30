#ifndef ROUTER_H
#define ROUTER_H

typedef struct
{
    int status_code;
    const char *html;
} RouteResult;

RouteResult route_request(const char *request);

#endif