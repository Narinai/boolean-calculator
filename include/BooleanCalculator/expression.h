//
// Created by narinai on 28/09/22.
//

#ifndef BOOLEAN_CALCULATOR_EXSPRESSION_H
#define BOOLEAN_CALCULATOR_EXSPRESSION_H

#include <stack>

#include "node.h"

namespace boolcalc {

    class Expression {
    public:
        Node *expression_;
        static bool Priority(const char a, const char b);
        static Node *ParseNode(std::stack<Node *> &nodes, std::stack<char> &symbols);
    public:
        Expression(std::string string);
        ~Expression() {};

    };

} // boolcalc

#endif //BOOLEAN_CALCULATOR_EXSPRESSION_H
