#include <iostream>
#include <vector>
#include <utility>
#include <random>
#include <functional>
#include <cassert>

using namespace std;

void printVector(const vector<double>& v) {
    cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i + 1 < v.size()) cout << ", ";
    }
    cout << "]";
}

class Particle {
public:
    vector<double> currentX;
    double currentY;
    vector<double> bestX;
    double bestY;
    vector<double> currentVelocity;
    double solo;
    double social;
    double wander;

    Particle(
        const vector<double>& x,
        double y,
        double s,
        double t,
        double w
    )
        : currentX(x),
          currentY(y),
          bestX(x),
          bestY(y),
          currentVelocity(x.size(), 0.0),
          solo(s),
          social(t),
          wander(w)
    {}

    void reportState() const {
        cout << "Position ";
        printVector(currentX);
        cout << "\n";
        cout << "Objective Value: " << currentY << "\n";

        cout << "Best position ";
        printVector(bestX);
        cout << "\n";
        cout << "Best Objective Value: " << bestY << "\n";

        cout << "Solo constant: " << solo << "\n";
        cout << "Social constant: " << social << "\n";
        cout << "Wander constant: " << wander << "\n";
    }
};

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

// Used to make a vector of random numbers within the domain
vector<double> randomVector(const vector<pair<double, double>>& bounds) {
    vector<double> result(bounds.size());

    static random_device rd;
    static mt19937 gen(rd());

    for (size_t i = 0; i < bounds.size(); i++) {
        double low = bounds[i].first;
        double high = bounds[i].second;

        uniform_real_distribution<double> dist(low, high);
        result[i] = dist(gen);
    }

    return result;
}

// For vector subtraction: subtract v1 from v2 (v2 - v1)
vector<double> vectorSub(
    const vector<double>& v1,
    const vector<double>& v2
) {
    assert(v1.size() == v2.size());
    vector<double> output = v2;
    for (size_t i = 0; i < v1.size(); i++) {
        output[i] -= v1[i];
    }
    return output;
}

// For vector addition: add v1 to v2 (v2 + v1)
vector<double> vectorAdd(
    const vector<double>& v1,
    const vector<double>& v2
) {
    assert(v1.size() == v2.size());
    vector<double> output = v2;
    for (size_t i = 0; i < v1.size(); i++) {
        output[i] += v1[i];
    }
    return output;
}

// For vector multiplication: multiply every element of v1 by c
vector<double> vectorMul(
    const vector<double>& v1,
    double c
) {
    vector<double> output = v1;
    for (size_t i = 0; i < v1.size(); i++) {
        output[i] = c * v1[i];
    }
    return output;
}

// Random ±1 vector for wandering direction
vector<double> randomSignVector(size_t dim) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 1);

    vector<double> v(dim);
    for (size_t i = 0; i < dim; ++i) {
        v[i] = (dist(gen) == 0) ? -1.0 : 1.0;
    }
    return v;
}

// Used to make a random particle
Particle randomParticle(const vector<pair<double,double>>& ranges) {
    vector<double> currentX = randomVector(ranges);

    // solo, social, wander in [0, 0.5]
    vector<pair<double, double>> probs = {
        {0.0, 0.5},
        {0.0, 0.5},
        {0.0, 0.5}
    };
    vector<double> constants = randomVector(probs);

    double currentY = objective(currentX);

    return Particle(currentX, currentY, constants[0], constants[1], constants[2]);
}

// Used to initialize a random population of particles
vector<Particle> initializePopulation(
    const vector<pair<double, double>>& ranges,
    int populationSize
){
    vector<Particle> population;
    population.reserve(populationSize);

    for (int i = 0; i < populationSize; i++) {
        population.push_back(randomParticle(ranges));
    }

    return population;
}

// PSO algorithm
vector<double> PSO(
    std::function<double(const vector<double>&)> objectiveFn,
    int numIterations,
    int populationSize,
    const vector<pair<double, double>>& ranges
){
    // Make the population
    vector<Particle> population = initializePopulation(ranges, populationSize);

    // Find initial best
    vector<double> globalBestX = population[0].currentX;
    double globalBestY = population[0].currentY;
    for (int i = 1; i < populationSize; i++) {
        if (population[i].currentY > globalBestY) {
            globalBestY = population[i].currentY;
            globalBestX = population[i].currentX;
        }
    }

    // Iterate over iterations
    for (int h = 0; h < numIterations; h++) {
        // Iterate over particles
        for (int i = 0; i < populationSize; i++) {

            // Accelerate towards global best
            vector<double> vecToGlobal = vectorSub(population[i].currentX, globalBestX);

            // Accelerate towards own best
            vector<double> vecToSolo = vectorSub(population[i].currentX, population[i].bestX);

            // Accelerate randomly (±1 per dimension)
            vector<double> vecRandom = randomSignVector(population[i].currentX.size());

            // Update velocity
            vector<double> globalUpdate = vectorMul(vecToGlobal, population[i].social);
            vector<double> soloUpdate   = vectorMul(vecToSolo,   population[i].solo);
            vector<double> randomUpdate = vectorMul(vecRandom,   population[i].wander);

            population[i].currentVelocity = vectorAdd(globalUpdate, population[i].currentVelocity);
            population[i].currentVelocity = vectorAdd(soloUpdate,   population[i].currentVelocity);
            population[i].currentVelocity = vectorAdd(randomUpdate, population[i].currentVelocity);

            // Update position
            population[i].currentX = vectorAdd(population[i].currentVelocity, population[i].currentX);

            // New value
            double newY = objectiveFn(population[i].currentX);
            population[i].currentY = newY;

            // New global best?
            if (newY > globalBestY) {
                globalBestY = newY;
                globalBestX = population[i].currentX;
            }

            // New solo best?
            if (newY > population[i].bestY) {
                population[i].bestY = newY;
                population[i].bestX = population[i].currentX;
            }
        }
    }
    return globalBestX;
}

int main() {
    // Algorithm constants
    int populationSize = 50;
    int numIterations = 1000;

    // Vector of low/high values for each x in X.
    vector<pair<double, double>> ranges = {
        {-10, 10}, {-10, 10}, {-10, 10}, {-10, 10}, {-10, 10}
    };

    vector<double> result = PSO(objective, numIterations, populationSize, ranges);

    cout << "Result: ";
    printVector(result);
    cout << "\n";
}