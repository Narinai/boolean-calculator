//
// Created by narinai on 28/09/22.
//

#include "../include/BooleanCalculator/expression.h"

namespace boolcalc {

    //
    // "Lazy-loaders"
    //

    void Expression::GenerateString() {
        if (!string_.empty()) return;
        string_ = expression_->String();
    }

    void Expression::GenerateTruthTable() {
        if (truth_table_ != nullptr) return;
        GenerateVariables();
        std::map<char, bool> vars;
        for (int i = 0; i < size_; ++i)
            vars.insert({variables_[i], false});
        truth_table_ = new bool[1ULL << size_];
        for (int i = 0; i < 1ULL << size_; ++i) {
            truth_table_[i] = expression_->Calculate(vars, std::cin, std::cout);
            IncrementVariables(vars);
        }
    }

    void Expression::GenerateZhegalkin() {
        if (zhegalkin_ != nullptr) return;
        GenerateTruthTable();
        const uint64_t array_size = 1ULL << size_;
        zhegalkin_ = new bool[array_size];
        bool *tmp =  new bool[array_size];
        std::copy(truth_table_, truth_table_ + array_size, zhegalkin_);
        bool flag = true;
        for (int i = 0; i < size_; ++i) {
            for (int j = 0; j < array_size; ++j) {
                if (!(j % (1ULL << i)))
                    flag = !flag;
                if (flag) {
                    tmp[j] = (zhegalkin_[j] != zhegalkin_[j - (1ULL << (i))]);
                } else {
                    tmp[j] = zhegalkin_[j];
                }
            }
            std::copy(tmp, tmp + array_size, zhegalkin_);
        }
        delete[] tmp;
    }

    void Expression::GenerateVariables() {
        if (variables_ != nullptr) return;
        std::set<char> vars;
        expression_->FindVariables(vars);
        variables_ = new char[vars.size()];
        auto j = vars.begin();
        for (int i = 0; i < vars.size(); ++i, ++j)
            variables_[i] = *j;
        size_ = vars.size();
    }

    Expression::Expression(boolcalc::Node *node) : expression_(node) {

    }

    //
    // Public
    //

    Expression::Expression(const std::string& string) {
        std::stack<Node *> nodes;
        std::stack<Symbol> symbols;

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
                                symbols.pop();
                                skip = true;
                            }
                            if (Priority(symbols.top(), Symbol(character)) == -1)
                                break;
                            ParseNode(nodes, symbols);
                        }
                        if (!skip)
                            symbols.push(Symbol(character));
                    }
                } // default

            } // switch
        }
        while (!symbols.empty())
            ParseNode(nodes, symbols);
        expression_ = std::shared_ptr<Node>(nodes.top());
    }

    Expression::Expression(const boolcalc::Expression &expression) {
        if (expression.size_ != -1) {
            if (expression.truth_table_ != nullptr){
                truth_table_ = new bool[1 << expression.size_];
                for (int i = 0; i < 1 << expression.size_; ++i) {
                    truth_table_[i] = expression.truth_table_[i];
                }
            }
            if (expression.zhegalkin_ != nullptr) {
                zhegalkin_ = new bool[1 << expression.size_];
                for (int i = 0; i < 1 << expression.size_; ++i) {
                    zhegalkin_[i] = expression.zhegalkin_[i];
                }
            }
            if (expression.variables_ != nullptr) {
                variables_ = new char[expression.size_];
                for (int i = 0; i < expression.size_; ++i) {
                    variables_[i] = expression.variables_[i];
                }
            }
        }
    }

    Expression::~Expression() {
        delete[] truth_table_;
        delete[] zhegalkin_;
        delete[] variables_;
    }

    std::string Expression::String() {
        GenerateString();
        return string_;
    }

    Expression Expression::CNF() {
        GenerateTruthTable();
        GenerateVariables();

        auto *result = new OperationNode(new And);

        for (int i = 0; i < 1ULL << size_; ++i) {
            if (!truth_table_[i]) {
                auto element = new OperationNode(new Or);
                for (int k = 0; k < size_; ++k) {
                    Node *var = new VariableNode(variables_[k]);
                    if (i >> k & 0b1) {
                        var = new NegNode(var);
                    }
                    element->AddChild(var, false);
                }
                result->AddChild(element, false);
            }
        }
        return Expression(result);
    }

    Expression Expression::DNF() {
        GenerateTruthTable();
        GenerateVariables();

        auto *result = new OperationNode(new Or);

        for (int i = 0; i < 1ULL << size_; ++i) {
            if (truth_table_[i]) {
                auto element = new OperationNode(new And);
                for (int k = 0; k < size_; ++k) {
                    Node *var = new VariableNode(variables_[k]);
                    if (!(i >> k & 0b1)) {
                        var = new NegNode(var);
                    }
                    element->AddChild(var, false);
                }
                result->AddChild(element, false);
            }
        }
        return Expression(result);
    }

    void Expression::TruthTable(std::ostream &output) {
        GenerateVariables();
        GenerateString();
        GenerateTruthTable();

        const uint32_t extend_by = 2;
        const uint32_t expression_length = string_.size() + extend_by;
        const uint32_t variables_length = 1 + extend_by;

        // Print title
        for (int i = 0; i < size_; i++)
            output << std::setfill(' ') << std::right << std::setw(variables_length) << (variables_[i]);
        output << std::setfill(' ') << std::right << std::setw(expression_length) << string_ << std::endl;

        // Print values
        std::map<char, bool> vars;
        for (int i = 0; i < size_; ++i)
            vars.insert({variables_[i], false});

        for (int i = 0; i < (1ULL << size_); ++i) {
            for (int j = 0; j < size_; j++)
                output << std::setfill(' ') << std::right << std::setw(variables_length) << vars[variables_[j]];
            output << std::setfill(' ') << std::right << std::setw(expression_length) << truth_table_[i] << std::endl;
            IncrementVariables(vars);
        }
    }

    int Expression::Priority(Symbol a, Symbol b) {
        // ")\0~\0&\0+\0v\0><=|^\0(\0\0";
        const static char priority[] = {
                kNeg, '\0',
                kAnd, '\0',
                kXor, '\0',
                kOr, '\0',
                kImpl, kRevImpl, kEq, kNand, kNor,
                kRightBracket, kLeftBracket, '\0', '\1'
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
                        return (a_priority > b_priority ? -1 : a_priority < b_priority ? 1 : 0);
                    }
            }
            if (for_break)
                break;
        }
        return true;
    }

    Node *Expression::ParseNode(std::stack<Node *> &nodes, std::stack<Symbol> &symbols) {
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
                    case kAnd: {
                        strategy = new And;
                        break;
                    }
                    case kOr: {
                        strategy = new Or;
                        break;
                    }
                    case kImpl: {
                        strategy = new Impl;
                        break;
                    }
                    case kRevImpl: {
                        strategy = new RevImpl;
                        break;
                    }
                    case kXor: {
                        strategy = new Xor;
                        break;
                    }
                    case kEq: {
                        strategy = new Eq;
                        break;
                    }
                    case kNand: {
                        strategy = new Nand;
                        break;
                    }
                    case kNor: {
                        strategy = new Nor;
                        break;
                    }
                } // operators switch
                auto tmp = new OperationNode(strategy);
                if (b->symbol() == tmp->symbol()) {
                    auto cast = dynamic_cast<OperationNode *>(b);
                    cast->AddChild(a, false);
                    delete tmp;
                    new_node = b;
                } else {
                    tmp->AddChild(a);
                    tmp->AddChild(b);
                    new_node = tmp;
                }
                break;
            } // Binary nodes case
            case kNeg: { // Unary nodes
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

    void Expression::IncrementVariables(std::map<char, bool> &vars) {
        for (auto &a : vars) {
            if (!a.second) {
                a.second = true;
                break;
            } else {
                a.second = false;
            }
        }
    }
} // boolcalc