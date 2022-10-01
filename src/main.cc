//
// Created by narinai on 28/09/22.
//

#include "../include/BooleanCalculator/expression.h"

int main() {
    boolcalc::Expression test("a > b > c"); // ISSUE

    //boolcalc::Expression test("a ^ b");

    test.TruthTable();


    // TODO
    // Get rid of simpify
    // Make Zhegalkin prettier

    //test.GenerateZhegalkin();

    return 0;
}
