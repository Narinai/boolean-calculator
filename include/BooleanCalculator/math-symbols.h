//
// Created by narinai on 28/09/22.
//

#ifndef BOOLEAN_CALCULATOR_MATH_SYMBOLS_H
#define BOOLEAN_CALCULATOR_MATH_SYMBOLS_H

namespace boolcalc {

// All symbols for calculator
// a-z are reserved for variables
enum Symbol {
  kAnd            = '&',
  kOr             = 'V',
  kImpl           = '>',
  kRevImpl        = '<',
  kXor            = '+',
  kEq             = '=',
  kNand           = '|',
  kNor            = '^',
  kLeftBracket    = '(',
  kRightBracket   = ')',
  kNeg            = '~',
  kVariable,
  kConst
};
}

#endif //BOOLEAN_CALCULATOR_MATH_SYMBOLS_H
