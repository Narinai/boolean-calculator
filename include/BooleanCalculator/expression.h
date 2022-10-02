//
// Created by narinai on 28/09/22.
//

#ifndef BOOLEAN_CALCULATOR_EXSPRESSION_H
#define BOOLEAN_CALCULATOR_EXSPRESSION_H

#include <stack>
#include <iostream>
#include <iomanip>

#include "node.h"

namespace boolcalc {

class Expression {
  std::shared_ptr<Node> expression_;

  // Lazy-loaded values
  std::string string_;
  bool *truth_table_ = nullptr;
  bool *zhegalkin_ = nullptr;
  char *variables_ = nullptr;
  int32_t size_ = -1;

  // "Lazy-loaders"
  void GenerateString();
  void GenerateTruthTable();
  void GenerateZhegalkin();
  void GenerateVariables();

  Expression(Node *node);
 public:
  Expression(const std::string& string);
  Expression(const Expression &expression);
  ~Expression();

  std::string String();
  Expression CNF();
  Expression DNF();
  Expression Zhegalkin();
  void TruthTable(std::ostream &output = std::cout);

 private:
  static int Priority(Symbol a, Symbol b);
  static void IncrementVariables(std::map<char, bool> &vars);
  static Node *ParseNode(std::stack<Node *> &nodes, std::stack<Symbol> &symbols);
  static Node *BuildOperationNode(const std::vector<Node *> &nodes, Strategy *strategy);
};

} // boolcalc

#endif //BOOLEAN_CALCULATOR_EXSPRESSION_H
