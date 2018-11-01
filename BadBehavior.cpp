//
// Created by boy on 23.10.18.
//

#include "BadBehavior.h"

BadBehavior::BadBehavior(int errn, const char * s) : errnum(errn), message(s){}

const char *BadBehavior::what() const noexcept {
    return message.c_str();
}
