//
// Created by narinai on 28/09/22.
//

#ifndef MYFANTASTICPROJECT_EXCEPTION_H
#define MYFANTASTICPROJECT_EXCEPTION_H

#include <exception>

namespace boolcalc {
class Exception : public std::exception { };

class InvalidArgumentCount : Exception {
public:
    InvalidArgumentCount() : Exception() {}
    const char * what() const noexcept override {
        return "Invalid Argument Count";
    }
};

}

#endif //MYFANTASTICPROJECT_EXCEPTION_H
