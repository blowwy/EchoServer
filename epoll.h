//
// Created by boy on 23.10.18.
//

#ifndef ESERVER_EPOLL_H
#define ESERVER_EPOLL_H

#include <sys/epoll.h>

class epoll {
public:
    explicit epoll();
    ~epoll();

    epoll(const epoll &) = delete;
    epoll(epoll && ) noexcept;

    epoll & operator=(epoll);

    void add(int,uint32_t);
    void mod(int,uint32_t);
    void remove(int);

    int getFd() const;
private:
    int fd_;
};


#endif //ESERVER_EPOLL_H
