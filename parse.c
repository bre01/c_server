#include <stdio.h>
#include <string.h>


// Function to extract HTTP payload from an HTTP POST request
int extract_payload(const char *http_request, char *payload) {
    const char *payload_start = strstr(http_request, "\r\n\r\n"); // Finding the end of headers
    if (payload_start != NULL) {
        payload_start += 4; // Move past the "\r\n\r\n"
        strcpy(payload, payload_start);
        return strlen(payload);
    }
    return -1;
}