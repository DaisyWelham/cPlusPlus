#include <iostream>
#include <vector>
#include <functional>
using namespace std;

// Function we optimize
double objective(const vector<double>& inputX) {
    return 100 - ((inputX[0] + 1) * (inputX[1] + 1));
}

// Finding the gradient vector
vector<double> getGradient(
    const vector<double>& inputX,
    std::function<double(const vector<double>&)> objective
) {
    vector<double> gradientVector(inputX.size(), 0.0);
    vector<double> xPlusDx = inputX;
    double dx = 1e-7;

    for (size_t i = 0; i < inputX.size(); i++) {
        xPlusDx = inputX;
        xPlusDx[i] += dx;

        double y1 = objective(inputX);
        double y2 = objective(xPlusDx);

        gradientVector[i] = (y2 - y1) / dx;
    }

    return gradientVector;
}

void printVector(const vector<double>& v) {
    cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i + 1 < v.size()) cout << ", ";
    }
    cout << "]";
}

// Gradient ascent
vector<double> gradientAscent(
    std::function<double(const vector<double>&)> objective,
    int numIterations,
    int vectorSize
) {
    double learningRate = 0.01;

    vector<double> bestX(vectorSize, 1.0);
    double bestY = objective(bestX);

    for (int i = 0; i < numIterations; i++) {
        vector<double> gradientVector = getGradient(bestX, objective);

        for (size_t j = 0; j < bestX.size(); j++) {
            bestX[j] = bestX[j] + learningRate * gradientVector[j];
        }

        bestY = objective(bestX);

        cout << "After " << i + 1 << " iterations "
             << "y = " << bestY << " at x = ";
        printVector(bestX);
        cout << "\n";
    }

    return bestX;
}

int main() {
    vector<double> bestX = gradientAscent(objective, 1000, 2);

    cout << "Final best x = ";
    printVector(bestX);
    cout << "\n";
}