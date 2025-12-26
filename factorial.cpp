#include <iostream>
#include <string>
using namespace std;

//Factorial to test recursion and function definition in C++
int factorial(int n){
    if(n == 0){
        return(1);
    }
    else{
        return(n * factorial(n - 1));
    }
}

int main() {
    int inputX = 5;
    int result = factorial(inputX);
    string output = to_string(result);
    cout << "Result: " << output << "\n";
}