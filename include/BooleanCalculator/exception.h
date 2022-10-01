//
// Created by narinai on 28/09/22.
//

#ifndef BOOLEAN_CALCULATOR_EXCEPTION_H
#define BOOLEAN_CALCULATOR_EXCEPTION_H

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

#endif //BOOLEAN_CALCULATOR_EXCEPTION_H
