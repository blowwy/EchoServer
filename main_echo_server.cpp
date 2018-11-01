//
// Created by boy on 23.10.18.
//


#include <iostream>
#include "BadBehavior.h"
#include "epoll.h"
#include "Server.h"

int main(int argc, char * argv[]){
    try{
        epoll ep;
        uint16_t port = 45000;
        Server server(ep,port);
        server.run();
    }catch (BadBehavior & ex ){
        std::cout << ex.what() << std::endl;
    } catch(...){
        std::cout << "undefined error" << std::endl;
    }
}
