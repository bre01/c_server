#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*format: GET /favicon.ico HTTP/1.1
     0 if get
     1 if post
     2 if others
     3 if for icon
    */
int route(char *line, char *paramter)
{
    printf("route");
    char url[100]; // Assuming URL won't exceed 100 characters
    char method[10];
    sscanf(line, "%s %s", method, url);

    paramter = url;
    if (strcmp(url, "/favicon.ico")==0)
    {
        printf("/favicon.ico,3 ");
        return 3;
    }
    else if (*line == 'G')
    {
        printf("G");
        return 0;
    }
    else if (*line == 'P')
    {
        printf("p");
        return 1;
    }
    else
    {
        printf("other");
        return 2;
    }

    /*
     0 if get
     1 if post
     2 if others
     3 if for icon
    */
}