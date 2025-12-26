#include <iostream>
#include <string>
using namespace std;

// Factorial to test recursion and function definition in C++
int factorial(int n) {
    if (n == 0) {
        return 1;
    }
    else {
        return n * factorial(n - 1);
    }
}

int main() {
    int inputX;

    cout << "Enter a non-negative integer: ";
    cin >> inputX;

    if (inputX < 0) {
        cout << "Factorial is not defined for negative numbers.\n";
        return 0;
    }

    int result = factorial(inputX);
    cout << "Result: " << result << "\n";
}
