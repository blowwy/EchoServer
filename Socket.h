//
// Created by boy on 23.10.18.
//

#ifndef ESERVER_SOCKET_H
#define ESERVER_SOCKET_H

#include <cstdint>
#include <string>
#include "epoll.h"

class Socket {
public:

    virtual ~Socket();
    Socket();
    Socket(Socket &&)                   noexcept;
    Socket &operator=(Socket &&)        noexcept;

    Socket(const Socket &)    =         delete;
    Socket &operator=(const Socket &) = delete;

    void swap(Socket &)                 noexcept;
    int getFd() const                   noexcept;

    std::string recvMessage();
    std::string sendMessage(std::string &);

    explicit Socket(int);
private:
    void close();

    int fd_;
};

class ClientSocket : public Socket{
public:
    explicit ClientSocket(int);
};

class ServerSocket : public Socket {
public:
    explicit ServerSocket(epoll &, uint16_t);
    int accept();
private:
    epoll & ep_;
};


#endif //ESERVER_SOCKET_H
