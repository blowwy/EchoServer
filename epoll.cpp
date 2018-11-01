//
// Created by boy on 23.10.18.
//


#include <stdexcept>
#include <assert.h>
#include <unistd.h>
#include <iostream>

#include "epoll.h"
#include "BadBehavior.h"

epoll::epoll(){
    int tmp = ::epoll_create1(0);
    if (tmp == -1){
        throw BadBehavior(errno,"epoll_creat1()");
    }

    assert(tmp >= 0);

    fd_ = tmp;
}

epoll::epoll(epoll &&ep) noexcept : fd_(ep.fd_) {}

epoll & epoll::operator=(epoll ep) {
    std::swap(fd_,ep.fd_);
    return *this;
}

void epoll::add(int fd, uint32_t event) {
    epoll_event ev = {0, nullptr};
    ev.events = event;
    ev.data.fd = fd;

    int tmp = ::epoll_ctl(fd_,EPOLL_CTL_ADD,fd,&ev);
    if (tmp == -1){
        throw BadBehavior(errno,"EPOLL_CTL_ADD");
    }
}

void epoll::mod(int fd, uint32_t event) {
    epoll_event ev = {0, nullptr};
    ev.events = event;
    ev.data.fd = fd;

    int tmp = ::epoll_ctl(fd_,EPOLL_CTL_MOD,fd,&ev);
    if (tmp == -1){
        throw BadBehavior(errno,"EPOLL_CTL_MOD");
    }
}

void epoll::remove(int fd) {
    int tmp = ::epoll_ctl(fd_,EPOLL_CTL_DEL,fd, nullptr);
    if (tmp == -1){
        throw BadBehavior(errno,"EPOLL_CTL_DEL");
    }
}

epoll::~epoll() {
    assert(fd_ >= 0);

    ::close(fd_);
}

int epoll::getFd() const {
    return fd_;
}


