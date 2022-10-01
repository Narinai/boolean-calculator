//
// Created by narinai on 28/09/22.
//

#ifndef BOOLEAN_CALCULATOR_EXCEPTION_H
#define BOOLEAN_CALCULATOR_EXCEPTION_H

#include <exception>
#include <cstring>

namespace boolcalc {
class Exception : public std::exception { };

class InvalidArgumentCount : Exception {
 public:
  InvalidArgumentCount() : Exception() {}
  const char * what() const noexcept override {
    return "Invalid Argument Count";
  }
};

class UnexpectedSign : Exception {
  char *string_;
 public:
  UnexpectedSign(char *string) : Exception() {
    string_ = new char[12+strlen(string)];
    int j = 0;
    for (; j < 11; ++j) {
      string_[j] = "Unexpected "[j];
    }
    for (; j < 11+strlen(string); ++j) {
      string_[j] = string[j-11];
    }
  }
  const char * what() const noexcept override {
    return string_;
  }
};

}

#endif //BOOLEAN_CALCULATOR_EXCEPTION_H
