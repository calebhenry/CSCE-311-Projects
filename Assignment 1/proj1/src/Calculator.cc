/**
 * @file Calculator.cc
 * @author Caleb Henry
 * @brief Uses the arguments supplied to main to calculate the result
 * @date 2023-01-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "./Calculator.h"

using namespace std;

/**
 * @brief Populates the supplied arguments into stacks and executes the functions
 * 
 * @param argc The number of arguments
 * @param argv The argument array
 */
void calculate(int argc, char *argv[]){
    stack<char> charStack;
    stack<double> doubleStack;
    for(int i = 1; i< argc; i++){
        if(i%2!=0){
            doubleStack.push(atof(argv[i])); 
        }
        else{
            if(argv[i][0] == 'x'){
                double product = doubleStack.top() * atof(argv[i+1]);
                doubleStack.pop();
                doubleStack.push(product);
                i++;
            }
            else if(argv[i][0] == '/'){
                double product = doubleStack.top()/atof(argv[i+1]);
                doubleStack.pop();
                doubleStack.push(product);
                i++; 
            }
            else{
                charStack.push((argv[i][0]));
            }
        }
    }
    int numOps = charStack.size();
    for(int i = 0; i<numOps; i++){
        if(charStack.top() == '+'){
            add(doubleStack);
        }
        else if(charStack.top() == '-'){
            subtract(doubleStack);
        }
        charStack.pop();
    }
    cout << doubleStack.top() << endl;
}

/**
 * @brief Adds the top two of the double stack and places that back on top
 * 
 * @param doubleStack The stack that contains the values
 */
void add(stack<double> &doubleStack){
    double secondAddand = doubleStack.top();
    doubleStack.pop();
    double firstAddand = doubleStack.top();
    doubleStack.pop();
    doubleStack.push(firstAddand+secondAddand);
}

/**
 * @brief Subtacts the first value on the stack from the second one
 * 
 * @param doubleStack The stack that contains the values
 */
void subtract(stack<double> &doubleStack){
    double secondAddand = doubleStack.top();
    doubleStack.pop();
    double firstAddand = doubleStack.top();
    doubleStack.pop();
    doubleStack.push(firstAddand-secondAddand);
}