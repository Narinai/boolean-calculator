//
// Created by narinai on 28/09/22.
//

#ifndef MYFANTASTICPROJECT_NODE_H
#define MYFANTASTICPROJECT_NODE_H

#include <string>
#include <map>
#include <iostream>
#include <vector>

#include "strategy.h"
#include "exception.h"

namespace boolcalc {

    class Node {
    public:
        [[maybe_unused]] [[nodiscard]] virtual bool Calculate(
                std::map<unsigned int, bool> vars,
                std::istream &input,
                std::ostream &output
        ) const = 0;
        [[maybe_unused]] [[nodiscard]] virtual std::string String() const = 0;

        virtual ~Node() = default;
    };

    class ConstNode : public Node {
        const bool value_;
    public:
        explicit ConstNode(const bool value) : value_(value) { };

        [[nodiscard]] bool Calculate(
                std::map<unsigned int, bool> vars,
                std::istream &input,
                std::ostream &output
        ) const override { return value_; };
        [[nodiscard]] std::string String() const override { return std::to_string(value_); }
        ~ConstNode() override = default;
    };

    class VariableNode : public Node {
        const char id_;
    public:
        explicit VariableNode(const char id) : id_(id) { };

        [[nodiscard]] bool Calculate(
                std::map<unsigned int, bool> vars,
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
        [[nodiscard]] std::string String() const override { return std::to_string(id_); }
        ~VariableNode() override = default;
    };

    class NegNode : public Node {
    private:
        Node *child_;
    public:
        explicit NegNode(Node *child) : child_(child) { };
        std::string String() const override { return "~" + child_->String(); }
        bool Calculate(std::map<unsigned int, bool> vars, std::istream &input, std::ostream &output) const override { return !child_->Calculate(vars, input, output); }
    };

    class OperationNode : public Node {
    protected:
        Strategy *strategy_;
        std::vector<Node *> children_ = { };
    public:
        OperationNode(Strategy *strategy) : strategy_(strategy) { };

        void AddChild(Node *child) {
            children_.push_back(child);
        }

        [[maybe_unused]] [[nodiscard]] bool Calculate(
                std::map<unsigned int, bool> vars,
                std::istream &input,
                std::ostream &output
        ) const override {
            if (children_.size() < 2)
                throw InvalidArgumentCount();
            bool buffer = children_[0]->Calculate(vars, input, output);
            for (auto i = children_.begin()+1; i != children_.end(); ++i)
                buffer = strategy_->Calculate(buffer, (*i)->Calculate(vars, input, output));
            return buffer;

        };
        [[maybe_unused]] [[nodiscard]] std::string String() const override {
            std::string result = "(";
            const char sign = strategy_->DisplaySign();
            for (const Node *child : children_) {
                result += child->String() + " " + sign + " ";
            }
            result.erase(result.end()-2, result.end());
            result += ")";
            return result;
        };
        ~OperationNode() override {
            delete strategy_;
        };
    };



} // boolcalc

#endif //MYFANTASTICPROJECT_NODE_H
