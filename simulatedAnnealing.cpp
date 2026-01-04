#include <iostream>
#include <vector>
#include <utility>
#include <random>
#include <functional>
#include <cassert>
#include <cmath>
#include <algorithm>

using namespace std;

// Function we optimize
double objective(const vector<double>& inputX) {
    return(100
    - (inputX[0] * inputX[0])
    - ((inputX[1] - 1) * (inputX[1] - 1))
    - ((inputX[2] - 4) * (inputX[2] - 4))
    - ((inputX[3] - 8) * (inputX[3] - 8))
    - ((inputX[4] + 1) * (inputX[4] + 1))
);
}

static mt19937& rng() {
    static random_device rd;
    static mt19937 gen(rd());
    return(gen);
}

vector<double> randomVector(const vector<pair<double, double>>& bounds) {
    vector<double> result(bounds.size());
    for (size_t i = 0; i < bounds.size(); i++) {
        uniform_real_distribution<double> dist(bounds[i].first, bounds[i].second);
        result[i] = dist(rng());
    }
    return(result);
}

// Returns true with prob p and false with prob 1 - p
bool probBool(double p){
    p = clamp(p, 0.0, 1.0);
    uniform_real_distribution<double> dist(0.0, 1.0);
    return(dist(rng()) < p);
}

void printVector(const vector<double>& v) {
    cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i + 1 < v.size()) cout << ", ";
    }
    cout << "]\n";
}

// For vector addition: add v1 to v2 (v2 + v1)
vector<double> vectorAdd(const vector<double>& v1, const vector<double>& v2) {
    assert(v1.size() == v2.size());
    vector<double> output = v2;
    for (size_t i = 0; i < v1.size(); i++) {
        output[i] += v1[i];
    }
    return(output);
}

static void clampToRanges(vector<double>& x, const vector<pair<double,double>>& ranges) {
    for (size_t i = 0; i < x.size(); i++) {
        x[i] = clamp(x[i], ranges[i].first, ranges[i].second);
    }
}

vector<double> simulatedAnnealing(
    function<double(const vector<double>&)> obj,
    int numIterations,
    double heatFactor
){
    vector<pair<double, double>> ranges     = {{-10,10}, {-10,10}, {-10,10}, {-10,10}, {-10,10}};
    vector<pair<double, double>> stepRanges = {{-0.1,0.1}, {-0.1,0.1}, {-0.1,0.1}, {-0.1,0.1}, {-0.1,0.1}};

    double temperature = 1.0;

    vector<double> currentX = randomVector(ranges);
    double currentY = obj(currentX);

    vector<double> bestX = currentX;
    double bestY = currentY;

    for (int i = 0; i < numIterations; i++){
        temperature *= heatFactor;
        // Avoid division by ~0 late in the run
        temperature = max(temperature, 1e-12);

        vector<double> stepX = randomVector(stepRanges);
        vector<double> newX = vectorAdd(stepX, currentX);
        clampToRanges(newX, ranges);

        double newY = obj(newX);

        // Update global best (always)
        if (newY > bestY) {
            bestY = newY;
            bestX = newX;
        }

        // Accept if better, otherwise accept with exp((new-current)/T)
        if (newY >= currentY) {
            currentX = newX;
            currentY = newY;
        } else {
            double p = exp((newY - currentY) / temperature); // in (0,1)
            if (probBool(p)) {
                currentX = newX;
                currentY = newY;
            }
        }
    }

    return(bestX);
}

int main(){
    int numIterations = 10000;
    vector<double> bestX = simulatedAnnealing(objective, numIterations, 0.99);
    printVector(bestX);
    return(0);
}