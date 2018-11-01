//
// Created by boy on 23.10.18.
//

#ifndef ESERVER_SERVER_H
#define ESERVER_SERVER_H


#include <sys/param.h>
#include <unordered_map>
#include "epoll.h"
#include "Socket.h"

class Server {
public:
    Server(epoll &,uint16_t);
    void handler(int,uint32_t);
    void handleRecv(int);
    void handleErrors(int);
    void handleSend(int);
    void run();

private:
    std::string handleRequest(std::string & str);
    std::string serverTime();

    epoll & ep_;
    ServerSocket sock;
    int sockId;

    std::unordered_map<int,Socket> socks;

    std::unordered_map<int,std::string> received;
    std::unordered_map<int,std::string> send_to;
};


#endif //SERVER_SERVER_H
