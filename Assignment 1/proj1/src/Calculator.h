/**
 * @file Calculator.h
 * @author Caleb Henry
 * @brief Contains the function prototypes that use the arguments supplied to main to calculate the result
 * @date 2023-01-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stack>
#include <iostream>

using namespace std;

void calculate(int argc, char *argv[]);
void add(stack<double> &doubleStack);

#endif // CALCULATOR_H