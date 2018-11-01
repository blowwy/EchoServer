//
// Created by boy on 24.10.18.
//


#include <gtest/gtest.h>

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 45000

int main(int argc, char const *argv[])
{
    struct sockaddr_in address{};
    int sock = 0, valread;
    struct sockaddr_in serv_addr{};
    char hello[] = ("Hello from client");
    if ((sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    while (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        if (errno == EAGAIN || errno == EALREADY) continue;
        printf("\nConnection Failed \n");
        return -1;
    }
    time_t start,end;
    time(&start);
    double sec = 0;
    for (int i = 0; i < 10000;i++){
        send(sock , hello, strlen(hello) , 0 );


    }
    for (int i = 0; i < 10000;i++){
        char buffer[1024];
        valread = read( sock , buffer, 1024);
        if (valread == 0) break;
        buffer[valread] = '\0';
        printf("%d : got : %s\n",i,buffer);
    }
    time(&end);
    sec = difftime(end,start);
    std::cout << sec << std::endl;
    return 0;
}