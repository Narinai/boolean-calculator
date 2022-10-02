//
// Created by narinai on 28/09/22.
//

#include "../include/BooleanCalculator/expression.h"
#include "../include/BooleanCalculator/exception.h"

int main() {

  bool loop = true;
  while (loop) {
    try {
      std::cout << "Expression: \t";
      std::string input;
      std::getline(std::cin, input);
      std::cout << std::endl;
      boolcalc::Expression expression(input);
      std::cout << "Parsed: \t\t" << expression.String() << std::endl;
      std::cout << "Truth table:" << std::endl;
      expression.TruthTable(std::cout);
      std::cout << "Conjunctive normal form: \t" << expression.CNF().String() << std::endl;
      std::cout << "Disjunctive normal form: \t" << expression.DNF().String() << std::endl;
      std::cout << "Zhegalkin polynomial: \t\t" << expression.Zhegalkin().String() << std::endl;
      loop = false;
    } catch (boolcalc::UnexpectedSign &unexpected_sign) {
      std::cout << "Error: " << unexpected_sign.what() << std::endl;
    } catch (std::exception &exception) {
      std::cout << "Error: " << exception.what() << "\n Exit" << std::endl;
      return 1;
    }
  }
  return 0;
}
