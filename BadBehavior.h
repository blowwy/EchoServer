//
// Created by boy on 23.10.18.
//

#ifndef ESERVER_BADBEHAVIOR_H
#define ESERVER_BADBEHAVIOR_H


#include <exception>
#include <string>

class BadBehavior : public std::exception {
public:
    explicit BadBehavior(int,const char *);
    const char * what() const noexcept override;

private:
    int errnum;
    std::string message;
};


#endif //ESERVER_BADBEHAVIOR_H
