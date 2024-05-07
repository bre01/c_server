#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "a.h"

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    char buffer[BUFFER_SIZE];
    char resp[] = "HTTP/1.0 200 OK\r\n"
                  "Server: webserver-c\r\n"
                  "Content-type: application/json\r\n\r\n"
                  "{this:'hello'}\r\n";
    // the text inside the tcp payload, including
    // both header
    // and response
    // seperated with a empty line(twice \r\n)

    // Create a socket
    // SOCK_STREAM makes it tcp and AF_INET makes it ipv4
    // type code 0 specify the protocol type when necessary
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("webserver (socket)");
        return 1;
    }
    printf("socket created successfully\n");

    // Create the address to bind the socket to
    struct sockaddr_in host_addr;
    int host_addrlen = sizeof(host_addr);

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind the socket to the address
    if (bind(sockfd, (struct sockaddr *)&host_addr, host_addrlen) != 0)
    {
        perror("webserver (bind)");
        return 1;
    }
    printf("socket successfully bound to address\n");

    // Listen for incoming connections
    if (listen(sockfd, SOMAXCONN) != 0)
    {
        perror("webserver (listen)");
        return 1;
    }
    printf("server listening for connections\n");

    for (;;)
    {
        // Accept incoming connections
        int newsockfd = accept(sockfd, (struct sockaddr *)&host_addr,
                               (socklen_t *)&host_addrlen);
        if (newsockfd < 0)
        {
            perror("webserver (accept)");
            continue;
        }

        // printf("%s", inet_ntoa(host_addr.sin_addr));
        // printf("connection accepted\n");
        //  printf("%s\n",host_addr.sin_addr.s_addr);
        // printf("%d\n", host_addr.sin_port);
        // printf("another way to retreive the client and local");
        struct sockaddr_in test_addr;
        int test_len = sizeof(test_addr);
        getpeername(newsockfd, (struct sockaddr *)&test_addr, (socklen_t *)&test_len);
        // printf("client\n");
        // printf("%s,%d\n", inet_ntoa(test_addr.sin_addr), test_addr.sin_port);
        getsockname(newsockfd, (struct sockaddr *)&test_addr, (socklen_t *)&test_len);
        // printf("local\n");
        // printf("%s,%d\n", inet_ntoa(test_addr.sin_addr), test_addr.sin_port);

        // Read from the socket
        printf("now");
        int valread = read(newsockfd, buffer, BUFFER_SIZE);
        int requets_type = route(buffer, NULL);
        if (valread < 0)
        {
            perror("webserver (read)");
            continue;
        }

        printf("%d", requets_type);
        int valwrite;
        if (requets_type == 0)
        {

            char *file_name = "test.jpeg";
            // printf("%s", buffer);
            char return_buffer[10000] = "HTTP/1.0 200 OK\r\n"
                                        "Server: webserver-c\r\n"
                                        "Content-type:image/jpeg\r\n\r\n";
            int origin_len = strlen(return_buffer);
            int file_len = read_binary(file_name, &return_buffer[origin_len]);
            int final_len = origin_len + file_len;
            valwrite = write(newsockfd, return_buffer, final_len);
        }
        else if (requets_type == 3)
        {
            char *file_name = "test.jpeg";
            // printf("%s", buffer);
            char return_buffer[10000] = "HTTP/1.0 200 OK\r\n"
                                        "Server: webserver-c\r\n"
                                        "Content-type:image/x-icon\r\n\r\n";
            int origin_len = strlen(return_buffer);
            int file_len = read_binary(file_name, &return_buffer[origin_len]);
            int final_len = origin_len + file_len;
            valwrite = write(newsockfd, return_buffer, final_len);
        }
        else if (requets_type == 1)
        {
            char *payload = malloc(10000);
            int payload_len = extract_payload(buffer, payload);
            int new_fd = open("saved.jpeg", O_CREAT|O_WRONLY);
            write(new_fd, payload, payload_len);
            close(new_fd);
            char return_buffer[10000] = "HTTP/1.0 200 OK\r\n"
                                        "Server: webserver-c\r\n"
                                        "Content-type:text/plain\r\n\r\n"
                                        "ok!";
            int origin_len = strlen(return_buffer);
            valwrite = write(newsockfd, return_buffer, origin_len);
        }
        else
        {

            // Write to the socket

            // int file_len = read_binary(file_name, return_buffer[origin_len]);
            // valwrite = write(newsockfd, resp[origin_len], strlen(resp));
        }

        if (valwrite < 0)
        {
            perror("webserver (write)");
            continue;
        }

        close(newsockfd);
    }

    return 0;
}
