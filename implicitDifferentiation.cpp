#include <iostream>
#include <string>
#include <functional>
using namespace std;

// Function we find the derivative of
double objective(double inputX) {
    return(-(inputX * inputX) + (5 * inputX) - 3);
}

// Finding the gradient
double getGradient(double inputX, std::function<double(double)> objective) {
    double dx = 0.0000001;
    double xPlusDx = inputX + dx;
    double y1 = objective(inputX);
    double y2 = objective(xPlusDx);
    double output = (y2 - y1)/ dx;
    return(output);
}

int main() {
    float inputX;
    cout << "Enter the value of x we need the derivative at: ";
    cin >> inputX;
    float result = getGradient(inputX, objective);
    cout << "Result: " << result << "\n";
}
