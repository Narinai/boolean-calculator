//
// Created by narinai on 28/09/22.
//

#ifndef MYFANTASTICPROJECT_STRATEGY_H
#define MYFANTASTICPROJECT_STRATEGY_H

namespace boolcalc {

    // Pattern strategy, class OperatorNode uses them to calculate value and fetch symbol
    class Strategy {
    public:
        [[nodiscard]] virtual char DisplaySign() const = 0;
        [[nodiscard]] virtual bool Calculate(bool a, bool b) const = 0;
        virtual ~Strategy() = default;
    };

    class And : Strategy {
        [[nodiscard]] char DisplaySign() const override { return '&'; }
        [[nodiscard]] bool Calculate(bool a, bool b) const override {
            return b && a;
        }
        ~And() override = default;
    };

    class Or : Strategy {
        [[nodiscard]] char DisplaySign() const override { return 'V'; }
        [[nodiscard]] bool Calculate(bool a, bool b) const override {
            return b || a;
        }
        ~Or() override = default;
    };


    class Impl : Strategy {
        [[nodiscard]] char DisplaySign() const override { return '>'; }
        [[nodiscard]] bool Calculate(bool a, bool b) const override {
            return !b && a;
        }
        ~Impl() override = default;
    };


    class RevImpl : Strategy {
        [[nodiscard]] char DisplaySign() const override { return '<'; }
        [[nodiscard]] bool Calculate(bool a, bool b) const override {
            return b && !a;
        }
        ~RevImpl() override = default;
    };


    class Xor : Strategy {
        [[nodiscard]] char DisplaySign() const override { return '+'; }
        [[nodiscard]] bool Calculate(bool a, bool b) const override {
            return a != b;
        }
        ~Xor() override = default;
    };

    class Eq : Strategy {
        [[nodiscard]] char DisplaySign() const override { return '='; }
        [[nodiscard]] bool Calculate(bool a, bool b) const override {
            return a == b;
        }
        ~Eq() override = default;
    };


    class Sheffer : Strategy {
        [[nodiscard]] char DisplaySign() const override { return '|'; }
        [[nodiscard]] bool Calculate(bool a, bool b) const override {
            return !a || !b;
        }
        ~Sheffer() override = default;
    };


    class Pierce : Strategy {
        [[nodiscard]] char DisplaySign() const override { return '^'; }
        [[nodiscard]] bool Calculate(bool a, bool b) const override {
            return !a && !b;
        }
        ~Pierce() override = default;
    };

} // boolcalc

#endif //MYFANTASTICPROJECT_STRATEGY_H
