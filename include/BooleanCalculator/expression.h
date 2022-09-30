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
        Node *expression_;

        // Lazy-loaded values
        std::string string_;
        bool *truth_table_ = nullptr;
        bool *zhegalkin_ = nullptr;
        char *variables_ = nullptr;
        uint32_t size_;

        // "Lazy-loaders"
        void GenerateString();
        void GenerateTruthTable();
        void GenerateZhegalkin();
        void GenerateVariables();
    public:
        Expression(const std::string& string);
        ~Expression();

        void TruthTable(std::ostream &output = std::cout);

    private:
        static int Priority(Symbol a, Symbol b);
        void SimplifyTree();
        static void IncrementVariables(std::map<char, bool> &vars);
        static Node *ParseNode(std::stack<Node *> &nodes, std::stack<Symbol> &symbols);

    };

} // boolcalc

#endif //BOOLEAN_CALCULATOR_EXSPRESSION_H
