//
// Created by narinai on 28/09/22.
//

#ifndef BOOLEAN_CALCULATOR_STRATEGY_H
#define BOOLEAN_CALCULATOR_STRATEGY_H

#include "math-symbols.h"

namespace boolcalc {
// Pattern strategy, class OperatorNode uses them to calculate value and fetch symbol
class Strategy {
 public:
  [[nodiscard]] virtual Symbol DisplaySign() const = 0;
  [[nodiscard]] virtual bool Calculate(bool a, bool b) const = 0;
  virtual ~Strategy() = default;
};

class And : public Strategy {
  [[nodiscard]] Symbol DisplaySign() const override { return Symbol::kAnd; }
  [[nodiscard]] bool Calculate(bool a, bool b) const override {
    return b && a;
  }
  ~And() override = default;
};

class Or : public Strategy {
  [[nodiscard]] Symbol DisplaySign() const override { return Symbol::kOr; }
  [[nodiscard]] bool Calculate(bool a, bool b) const override {
    return b || a;
  }
  ~Or() override = default;
};


class Impl : public Strategy {
  [[nodiscard]] Symbol DisplaySign() const override { return Symbol::kImpl; }
  [[nodiscard]] bool Calculate(bool a, bool b) const override {
    return !b || a;
  }
  ~Impl() override = default;
};


class RevImpl : public Strategy {
  [[nodiscard]] Symbol DisplaySign() const override { return Symbol::kRevImpl; }
  [[nodiscard]] bool Calculate(bool a, bool b) const override {
    return b || !a;
  }
  ~RevImpl() override = default;
};


class Xor : public Strategy {
  [[nodiscard]] Symbol DisplaySign() const override { return Symbol::kXor; }
  [[nodiscard]] bool Calculate(bool a, bool b) const override {
    return a != b;
  }
  ~Xor() override = default;
};

class Eq : public Strategy {
  [[nodiscard]] Symbol DisplaySign() const override { return Symbol::kEq; }
  [[nodiscard]] bool Calculate(bool a, bool b) const override {
    return a == b;
  }
  ~Eq() override = default;
};


class Nand : public Strategy {
  [[nodiscard]] Symbol DisplaySign() const override { return Symbol::kNand; }
  [[nodiscard]] bool Calculate(bool a, bool b) const override {
    return !a || !b;
  }
  ~Nand() override = default;
};


class Nor : public Strategy {
  [[nodiscard]] Symbol DisplaySign() const override { return Symbol::kNor; }
  [[nodiscard]] bool Calculate(bool a, bool b) const override {
    return !a && !b;
  }
  ~Nor() override = default;
};

} // boolcalc

#endif //BOOLEAN_CALCULATOR_STRATEGY_H
