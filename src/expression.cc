//
// Created by narinai on 28/09/22.
//

#include "../include/BooleanCalculator/expression.h"

namespace boolcalc {

    bool Expression::Priority(const char a, const char b) {
        // ")\0~\0&\0+\0v\0><=|^\0(\0\0";
        const static char priority[] = {
                kRightBracket, '\0',
                kNeg, '\0',
                kAnd, '\0',
                kXor, '\0',
                kOr, '\0',
                kImpl, kRevImpl, kEq, kNand, kNor, '\1'
        };

        int j = 1;
        int a_priority = 0;
        int b_priority = 0;

        bool for_break = false;
        for (char i = 0; priority[i] != '\1'; i++) {
            switch (priority[i]) {
                case '\1':
                    for_break = true;
                    break;
                case '\0':
                    j++;
                    break;
                default:
                    if (a == priority[i]) {
                        a_priority = j;
                    }
                    if (b == priority[i]) {
                        b_priority = j;
                    }
                    if (a_priority && b_priority) {
                        return a_priority > b_priority;
                    }
            }
            if (for_break)
                break;
        }
        return true;
    }

    Node *Expression::ParseNode(std::stack<Node *> &nodes, std::stack<char> &symbols) {
        Node *new_node = nullptr;
        switch (symbols.top()) {
            case kAnd:
            case kOr:
            case kImpl:
            case kRevImpl:
            case kXor:
            case kEq:
            case kNand:
            case kNor: {   // Binary nodes
                Node *a = nodes.top();
                nodes.pop();
                Node *b = nodes.top();
                nodes.pop();
                Strategy *strategy;
                switch (symbols.top()) {
                    case '&': {
                        strategy = new And;
                        break;
                    }
                    case 'v': {
                        strategy = new Or;
                        break;
                    }
                    case '>': {
                        strategy = new Impl;
                        break;
                    }
                    case '<': {
                        strategy = new RevImpl;
                        break;
                    }
                    case '+': {
                        strategy = new Xor;
                        break;
                    }
                    case '=': {
                        strategy = new Eq;
                        break;
                    }
                    case '|': {
                        strategy = new Sheffer;
                        break;
                    }
                    case '^': {
                        strategy = new Pierce;
                        break;
                    }
                } // operators switch
                auto tmp = new OperationNode(strategy);
                tmp->AddChild(a);
                tmp->AddChild(b);
                new_node = tmp;
                break;
            } // Binary nodes case
            case '~': { // Unary nodes
                Node *a = nodes.top();
                nodes.pop();
                new_node = new NegNode(a);
                break;
            }
        } // Binary/Unary switch

        if (new_node != nullptr)
            nodes.push(new_node);
        symbols.pop();
        return new_node;
    }


    void Expression::SimplifyTree(boolcalc::Node *root) {
        OperationNode *operation_node = dynamic_cast<OperationNode *>(root);
        if (!operation_node) return;
        operation_node->Simplify();
    }

    Expression::Expression(std::string string) {
        std::stack<Node *> nodes;
        std::stack<char> symbols;

        for (auto character : string) {
            switch (character) {
                case ' ': {   // Empty line
                    continue;
                }
                case '1':     // Constants
                case '0': {
                    nodes.push(new ConstNode(character - '0'));
                    break;
                }
                default: {    // Operator or variable
                    if (character >= 'a' && character <= 'z') {
                        nodes.push(new VariableNode(character));
                    } else {
                        bool skip = false;
                        while (!symbols.empty()) {
                            if (character == kLeftBracket)
                                break;
                            if (character == kRightBracket && symbols.top() == kLeftBracket) {
                                nodes.pop();
                                skip = true;
                            }
                            if (Priority(symbols.top(), character))
                                break;
                            ParseNode(nodes, symbols);
                        } // while
                        if (!skip)
                            symbols.push(character);
                    }


                } // default

            } // switch
        }
        while (symbols.size() != 0)
            ParseNode(nodes, symbols);

        SimplifyTree(nodes.top());

        expression_ = nodes.top();
    }

} // boolcalc