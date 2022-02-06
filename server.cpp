/* PLEASE include these headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include <string>
#include <iostream>
#define MYPORT 8080
#define BACKLOG 10 /* pending connections queue size */

int main()
{
    int sockfd, new_fd; /* listen on sockfd, new connection on new_fd */
    struct sockaddr_in my_addr; /* my address */
    struct sockaddr_in their_addr; /* connector addr */
    unsigned int sin_size;

    /* create a socket */
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* set the address info */
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT); /* short, network byte order */
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    /* INADDR_ANY allows clients to connect to any one of the host’s IP
        address. Optionally, use this line if you know the IP to use:
        my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    */
    memset(my_addr.sin_zero, '\0', sizeof(my_addr.sin_zero));
    /* bind the socket */
    if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    while (1) { /* main accept() loop */
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr*) &their_addr, &sin_size)) == -1) {
            perror("accept");
            continue;
        }

        // Read stream of bytes from HTTP request
        uint8_t buf[1024];
        int bytes_read = read(new_fd, buf, sizeof(buf));
        // printf("%s", (char*)buf);
        // Parse HTTP request
        std::string buf_str((char*)buf);
        int start = buf_str.find(' ') + 2;
        int end = buf_str.find(' ', start);
        
        std::string object = buf_str.substr(start, end - start);
        const char* object_c = object.c_str();

        int file_fd;
        // If there's no extension
        if (object.find('.') == -1)
        {
            DIR *folder;
            struct dirent *entry;

            folder = opendir(".");
            if(folder == NULL)
            {
                perror("Unable to read directory");
                return(1);
            }

            bool found = false;
            // printf("Object: %s\n", object_c);
            while( (entry=readdir(folder)) )
            {
                std::string dirname(entry->d_name);
                std::string dirraw = dirname.substr(0, dirname.find("."));
                // printf("File: %s\t Name: %s\n", dirname.c_str(), dirraw.c_str());
                if (strcasecmp(dirraw.c_str(), object_c) == 0) 
                {
                    file_fd = open(dirname.c_str(), O_RDONLY);
                    found = true;
                    break;
                }
            }
            closedir(folder);
            if (!found)
            {
                write(new_fd, "HTTP/1.0 404 Not Found\r\n", 22);
                close(new_fd);
                continue;
            }
        }
        else 
        {
            if((file_fd = open(object_c, O_RDONLY)) == -1)
            {
                write(new_fd, "HTTP/1.0 404 Not Found\r\n", 22);
                close(new_fd);
                continue;
            }
        }

        struct stat fd_stat;
        fstat(file_fd, &fd_stat);
        uint8_t file_buf[fd_stat.st_size];
        bytes_read = read(file_fd, file_buf, sizeof(file_buf));
        close(file_fd);

        // Get last access time
        char file_ctime[100];
        struct tm* file_tm;
        file_tm = gmtime(&(fd_stat.st_ctime));
        // Format timestamp
        strftime(file_ctime, sizeof(file_ctime), "%a,%e %b %G %T GMT", file_tm);

        // Fill in headers
        char last_modified_header[128];
        memset(last_modified_header, 0, 128);
        sprintf(last_modified_header, "Last Modified: %s\n\n", file_ctime);
        char content_length_header[128];
        memset(content_length_header, 0, 128);
        sprintf(content_length_header, "Content-Length:%ld\n", fd_stat.st_size);

        char header[1024];
        memset(header, 0, 1024);
        // Status code may vary
        strcat(header, "HTTP/1.0 200 OK\r\nServer: CS118 Project 1\r\n");
        // READ RFC1945
        strcat(header, "Content-Type: text/plain\r\n");
        strcat(header, content_length_header);
        strcat(header, last_modified_header);
        // strcat(header, "\r\n");

        // Write HTTP response header
        write(new_fd, header, strlen(header));
        // Write HTTP response
        write(new_fd, file_buf, bytes_read);
        write(new_fd, "\r\n", 2);
        
        close(new_fd);
    }
}
