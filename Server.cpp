//
// Created by boy on 23.10.18.
//

#include <utility>
#include <array>
#include <assert.h>
#include <sys/socket.h>
#include <iostream>
#include <ctime>
#include <iomanip>

#include "Server.h"
#include "BadBehavior.h"


Server::Server(epoll & ep,uint16_t port) try : ep_(ep), sock(ServerSocket(ep,port) ), sockId(sock.getFd()) {
    ep_.add(sockId,EPOLLIN);
}catch(BadBehavior & ex){
    throw;
}

void Server::run() {
    std::array<epoll_event,100> ev{};
    for (;;){
        bool repeat = false;
        int tmp = ::epoll_wait(ep_.getFd(),ev.data(),ev.size(),5000);
        if (tmp <= 0){
            if (errno == EINTR || tmp == 0) {
                repeat = true;
            }
            else {
                throw BadBehavior(errno, "epoll_wait()");
            }
        }
        if (repeat) continue;
        assert(tmp > 0);
        auto eventsN = static_cast<size_t>(tmp);
        assert(eventsN <= ev.size());
        for (auto i = ev.begin();i != ev.begin() + eventsN;++i){
                handler((*i).data.fd,(*i).events);
        }
    }
}

void Server::handleRecv(int fd) {
    if (fd == sockId){
        Socket connSock(sock.accept());
        ep_.add(connSock.getFd(),EPOLLIN | EPOLLOUT);
        send_to [connSock.getFd()] = std::string("WELCOME TO SERVER\n--help to get usage\n");
        socks[connSock.getFd()] = std::move(connSock);
    }  else{
            received[fd] = socks[fd].recvMessage();
            if (received[fd].empty()){
                ep_.remove(fd);
            } else{
                ep_.mod(fd,EPOLLIN | EPOLLOUT);
                send_to[fd] += handleRequest(received[fd]);
            }
    }
}

void Server::handleErrors(int fd) {
    socks.erase(fd);
    send_to.erase(fd);
    received.erase(fd);
}

void Server::handleSend(int fd) {
    send_to[fd] = socks[fd].sendMessage(send_to[fd]);
    if (send_to[fd].empty()){
        send_to.erase(fd);
        ep_.mod(fd,EPOLLIN);
    }
}

void Server::handler(int fd, uint32_t ev){
    if (ev & EPOLLIN){
        handleRecv(fd);
    }
    if (ev & EPOLLOUT){
        handleSend(fd);
    }
    if (ev & EPOLLERR){
        handleErrors(fd);
    }
}



std::string Server::handleRequest(std::string &str) {
    if (str == std::string("time\r\n") ){
        return serverTime();
    }
    if (str == std::string("--help\r\n") ){
        return std::string("Server can handle requests such as :\n"
                           "time                # You will receive server time\n"
                           "echo (your message) # You will receive your message back\n\n");
    }
    if (str.size() >= 5 && str.substr(0,5) == std::string("echo ") ) {
        return std::string(str.substr(5) + "\n");
    }
    return std::string("Invalid request\n");
}

std::string Server::serverTime() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm,"%H-%M-%S %d-%m-%Y\n");
    auto str = oss.str();
    return str;
}



