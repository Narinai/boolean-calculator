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
        bool *truth_table_ = nullptr;
        bool *zhegalkin_ = nullptr;
        uint32_t size_;

        Node *expression_;
        static int Priority(Symbol a, Symbol b);
        static Node *ParseNode(std::stack<Node *> &nodes, std::stack<Symbol> &symbols);
        static void SimplifyTree(Node *root);
        static void IncrementVariables(std::map<char, bool> &vars);
        //std::string BuildNormalForm(bool And);
        void GenerateTruthTable();
        void GenerateZhegalkin();
    public:
        Expression(const std::string& string);
        ~Expression() {};

    };

} // boolcalc

#endif //BOOLEAN_CALCULATOR_EXSPRESSION_H
