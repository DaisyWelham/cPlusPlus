#include <iostream>
#include <string>
#include <functional>
using namespace std;

// Function we find the derivative of
double objective(double inputX) {
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

// Gradient ascent
double gradientAscent(std::function<double(double)> objective, int numIterations) {
    double learningRate = 0.01;
    double bestX = 1;
    double bestY = objective(bestX);
    for(int i = 0; i < numIterations; i++){
        bestX = bestX + learningRate * getGradient(bestX, objective);
        bestY = objective(bestX);
        cout << "After " << i + 1 << " iterations " << "y = " << bestY << " at x = " << bestX << "\n";
    }
    return(bestX);
}

int main() {
    double bestX = gradientAscent(objective, 1000);
}
