//
// Created by narinai on 28/09/22.
//

#ifndef MYFANTASTICPROJECT_STRATEGY_H
#define MYFANTASTICPROJECT_STRATEGY_H

namespace boolcalc {

    class Strategy {
    public:
        virtual char DisplaySign() const = 0;
        virtual bool Calculate(bool a, bool b) const = 0;
    };

    class And : Strategy {
        char DisplaySign() const override { return '&'; }
        bool Calculate(bool a, bool b) const override {
            return b && a;
        }
    };

    class Or : Strategy {
        char DisplaySign() const override { return 'V'; }
        bool Calculate(bool a, bool b) const override {
            return b || a;
        }
    };


    class Impl : Strategy {
        char DisplaySign() const override { return '>'; }
        bool Calculate(bool a, bool b) const override {
            return !b && a;
        }
    };


    class RevImpl : Strategy {
        char DisplaySign() const override { return '<'; }
        bool Calculate(bool a, bool b) const override {
            return b && !a;
        }
    };


    class Xor : Strategy {
        char DisplaySign() const override { return '+'; }
        bool Calculate(bool a, bool b) const override {
            return a != b;
        }
    };

    class Eq : Strategy {
        char DisplaySign() const override { return '='; }
        bool Calculate(bool a, bool b) const override {
            return a == b;
        }
    };


    class Sheffer : Strategy {
        char DisplaySign() const override { return '|'; }
        bool Calculate(bool a, bool b) const override {
            return !a || !b;
        }
    };


    class Pierce : Strategy {
        char DisplaySign() const override { return '^'; }
        bool Calculate(bool a, bool b) const override {
            return !a && !b;
        }
    };

} // boolcalc

#endif //MYFANTASTICPROJECT_STRATEGY_H
