//
// Created by narinai on 28/09/22.
//

#ifndef BOOLEAN_CALCULATOR_NODE_H
#define BOOLEAN_CALCULATOR_NODE_H

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <set>
#include <memory>

#include "strategy.h"
#include "exception.h"

namespace boolcalc {
class Node {
 public:
  virtual bool Calculate(
      std::map<char, bool> &vars,
      std::istream &input,
      std::ostream &output
  ) const = 0;
  virtual std::string String() const = 0;
  virtual Symbol symbol() const = 0;
  virtual void FindVariables(std::set<char> &vars) const = 0;


  virtual ~Node() = default;

};

class ConstNode : public Node {
  const bool value_;
 public:
  explicit ConstNode(const bool value) : value_(value) { };

  [[nodiscard]] bool Calculate(
      std::map<char, bool> &vars,
      std::istream &input,
      std::ostream &output
  ) const override { return value_; };
  [[nodiscard]] std::string String() const override { return std::to_string(value_); }
  [[nodiscard]] enum Symbol symbol() const override { return  kConst; }
  void FindVariables(std::set<char> &vars) const override { }
  ~ConstNode() override = default;
};

class VariableNode : public Node {
  const char id_;
 public:
  explicit VariableNode(const char id) : id_(id) { };

  [[nodiscard]] bool Calculate(
      std::map<char, bool> &vars,
      std::istream &input,
      std::ostream &output
  ) const override {
    auto cursor = vars.find(id_);
    if (cursor != vars.end())
      return cursor->second;
    bool tmp;
    output << "Variable " << id_ << ": \t";
    input >> tmp;
    input.ignore();
    vars.insert({id_, tmp});
    return tmp;
  };

  void FindVariables(std::set<char> &vars) const override { vars.insert(id_); }
  [[nodiscard]] std::string String() const override { return std::string(1, id_); }

  [[nodiscard]] enum Symbol symbol() const override { return  kVariable; }
  ~VariableNode() override = default;
};

class NegNode : public Node {
 private:
  Node *child_;
 public:
  explicit NegNode(Node *child) : child_(child) { };
  std::string String() const override { return "~" + child_->String(); }
  [[nodiscard]] enum Symbol symbol() const override { return  kNeg; }
  void FindVariables(std::set<char> &vars) const override {
    child_->FindVariables(vars);
  }

  bool Calculate(std::map<char, bool> &vars, std::istream &input, std::ostream &output) const override { return !child_->Calculate(vars, input, output); }

  ~NegNode() { delete child_; }
};

class OperationNode : public Node {
 protected:
  std::shared_ptr<Strategy> strategy_;
  std::vector<Node *> children_ = { };
 public:
  OperationNode(Strategy *strategy) : strategy_(strategy) { };

  void AddChild(Node *child, bool end = true) {
    if (end)
      children_.push_back(child);
    else
      children_.insert(children_.begin(), child);
  }

  void RemoveChildren(bool delete_children = true) {
    if (delete_children)
      for (auto child : children_) {
        delete child;
      }
    children_.clear();
  };

  void FindVariables(std::set<char> &vars) const override {
    for (auto child : children_) {
      child->FindVariables(vars);
    }
  }

  bool Calculate(
      std::map<char, bool> &vars,
      std::istream &input,
      std::ostream &output
  ) const override {
    if (children_.size() < 2)
      throw InvalidArgumentCount();
    bool buffer = children_.back()->Calculate(vars, input, output);
    for (auto i = children_.end()-2; i >= children_.begin(); --i)
      buffer = strategy_->Calculate((*i)->Calculate(vars, input, output), buffer);
    return buffer;

  };
  std::string String() const override {
    std::string result = "(";
    const char sign = strategy_->DisplaySign();
    for (auto i = children_.end()-1; i >= children_.begin() ; --i) {
      result += (*i)->String() + " " + sign + " ";
    }

    result.erase(result.end()-3, result.end());
    result += ")";
    return result;
  };

  [[nodiscard]] enum Symbol symbol() const override { return  strategy_->DisplaySign(); }
  ~OperationNode() override {
    for (auto i : children_) {
      delete i;
    }
  };
};
} // boolcalc

#endif //BOOLEAN_CALCULATOR_NODE_H
