#include <iostream>
#include <string>
#include <functional>
using namespace std;

// Function we solve for f(x) = 0
double functionSolve(double inputX) {
    return(100 - ((inputX + 1) * (inputX + 1)));
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

// Newton Raphson
double newtonRaphson(std::function<double(double)> functionSolve, int numIterations) {
    double currentX = 1;
    double currentY = functionSolve(currentX);
    for(int i = 0; i < numIterations; i++){
        currentX = currentX - ((functionSolve(currentX)) / (getGradient(currentX, functionSolve)));
        currentY = functionSolve(currentX);
        cout << "After " << i + 1 << " iterations " << "y = " << currentY << " at x = " << currentX << "\n";
    }
    return(currentX);
}

int main() {
    double solutionX = newtonRaphson(functionSolve, 1000);
}