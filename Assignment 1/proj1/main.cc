#include <iostream>
#include <stack>
#include "calculate.h"
#include "calculate.cc"

/**
 * @author Caleb Henry
 */

using namespace std;

int main(int argc, char *argv[]) {
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
            double secondAddand = doubleStack.top();
            doubleStack.pop();
            double firstAddand = doubleStack.top();
            doubleStack.pop();
            doubleStack.push(firstAddand+secondAddand);
        }
        else if(charStack.top() == '-'){
            double secondAddand = doubleStack.top();
            doubleStack.pop();
            double firstAddand = doubleStack.top();
            doubleStack.pop();
            doubleStack.push(firstAddand-secondAddand);
        }
        charStack.pop();
    }
    cout << doubleStack.top() << endl;
}
