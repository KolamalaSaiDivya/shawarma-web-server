#include <string.h>
#include "mime.h"

const char* get_mime_type(const char* path)
{
    if (strstr(path, ".css"))
        return "text/css";

    if (strstr(path, ".js"))
        return "application/javascript";

    if (strstr(path, ".png"))
        return "image/png";

    return "text/html";
}