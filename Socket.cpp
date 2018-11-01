//
// Created by boy on 23.10.18.
//

#include <sys/socket.h>

#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

#include "Socket.h"
#include <arpa/inet.h>
#include "BadBehavior.h"


void bindSocket(int fd,uint16_t port){
    struct sockaddr_in info{};
    info.sin_family = AF_INET;
    info.sin_addr.s_addr = htonl(INADDR_ANY);
    info.sin_port = htons(port);
    int res = ::bind(fd, reinterpret_cast<sockaddr const *>(&info), sizeof info);
    if (res == -1){
        throw BadBehavior(errno,"bind()");
    }

}

void listenSocket(int fd){
    int res = ::listen(fd,SOMAXCONN);
    if (res == -1) {
        throw BadBehavior(errno,"listen()");
    }
}

Socket::Socket(int sfd) : fd_(sfd) {
    if (sfd == -1) {
        throw BadBehavior(0,"socket()");
    }
    fd_ = sfd;
}

Socket::Socket(Socket && other) noexcept : fd_(-1) {
    this->swap(other);
}

Socket &Socket::operator=(Socket && other) noexcept {
    other.swap(*this);
    return *this;
}

void Socket::swap(Socket & other) noexcept {
    using std::swap;
    swap(fd_,other.fd_);
}

Socket::~Socket() {
    if (fd_ == -1){
        return;
    }
    try{
        close();
    } catch(BadBehavior & ex){
        std::cout << ex.what();
        std::exit(EXIT_FAILURE);
    }

}

void Socket::close() {
    if (fd_ == -1){
        throw BadBehavior(0,"trying close() not existing or closed file");
    } else if (::close(fd_) == -1){
        throw BadBehavior(errno,"close()");
    }
}

int Socket::getFd() const noexcept{
    return fd_;
}

ServerSocket::ServerSocket(epoll & ep,uint16_t port) try :
    Socket(::socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK,0) ), ep_(ep) {
   bindSocket(getFd(),port);
   listenSocket(getFd());
} catch(BadBehavior & ex){
    throw;
}

int ServerSocket::accept() {
    int tmp = ::accept(getFd(),nullptr,nullptr);
    if (tmp == -1){
        perror("accept() failed");
    }
    return tmp;
}

ClientSocket::ClientSocket(int fd) try : Socket(fd){

}catch (BadBehavior & ex){
    throw;
}

std::string Socket::recvMessage() {
    char buf[500];
    ssize_t nread = ::recv(getFd(),buf,sizeof buf,0);
    if (nread == -1){
        throw BadBehavior(errno,"recv()");
    }
    buf[nread] = '\0';
    return std::string(buf);

}

std::string Socket::sendMessage(std::string & message) {
    ssize_t nsend = ::send(getFd(),message.data(),message.size(),0);
    if (nsend == -1){
        throw BadBehavior(errno,"send()");
    }
    return message.substr(static_cast<unsigned long>(nsend));
}

Socket::Socket() : fd_(-1){

}
