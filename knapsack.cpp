#include <iostream>
#include <string>
#include <vector>
#include <initializer_list>
#include <random>
#include <cassert>
using namespace std;

void printVector(const vector<int>& v) {
    cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i + 1 < v.size()) cout << ", ";
    }
    cout << "]";
}

//Random class
class Random {
public:
    // choice({a, b, c})
    template<typename T>
    T choice(std::initializer_list<T> xs) {
        assert(xs.size() > 0);

        std::uniform_int_distribution<size_t> dist(0, xs.size() - 1);
        auto it = xs.begin();
        std::advance(it, dist(gen));
        return(*it);
    }

    // uniform(a, b) for doubles (Python-style)
    double uniform(double a, double b) {
        assert(a <= b);
        std::uniform_real_distribution<double> dist(a, b);
        return(dist(gen));
    }

    // uniform(a, b) for ints (inclusive, like Python)
    int uniform(int a, int b) {
        assert(a <= b);
        std::uniform_int_distribution<int> dist(a, b);
        return(dist(gen));
    }

private:
    std::mt19937 gen{std::random_device{}()};
};

//Creating the "KnapsackItem" class for knapsacks
class KnapsackItem {
public:
    string name;
    double mass;
    double value;

    KnapsackItem(string n, double m, double v)
        : name(n), mass(m), value(v) {}

    string getName() const{
        return(name);
    }

    double getMass() const {
        return(mass);
    }

    double getValue() const {
        return(value);
    }
};

//Evaluate a knapsack: value if valid, negative mass overflow if invalid.
double evaluateKnapsack(vector<KnapsackItem> knapsack, double massLimit){
    double mass = 0;
    double value = 0;
    for(int i = 0; i < knapsack.size(); i++){
        mass += knapsack[i].getMass();
        value += knapsack[i].getValue();
    }
    //Is our knapsack valid?
    if (mass > massLimit){
        return(massLimit - mass);
    }
    else{
        return(value);
    }
}

//Evaluate a solution
double objective(vector<int> selections, vector<KnapsackItem> knapsack, double massLimit){
    double mass = 0;
    double value = 0;
    for(int i = 0; i < knapsack.size(); i++){
        if (selections[i] == 1){
            mass += knapsack[i].getMass();
            value += knapsack[i].getValue();
        }
    }
    //Is our knapsack valid?
    if (mass > massLimit){
        return(massLimit - mass);
    }
    else{
        return(value);
    }
}

//Generate any random solution
vector<int> generateIndividual(int size){
    vector<int> individual(size);
    Random r;
    for (int i = 0; i < size; i++){
        individual[i] = r.choice({0, 1});
    }
    return(individual);
}

vector<int> mutateIndividual(vector<int> individual, double mutationRate){
    Random r;
    vector<int> output(individual.size());
    for(int i = 0; i < individual.size(); i++){
        if(r.uniform(0, 1) < mutationRate){
            output[i] = r.choice({0, 1});
        }
        else{
            output[i] = individual[i];
        }
    }
    return(output);
}

//Perform asexual genetic algorithm
vector<int> geneticAlgorithm(
    vector<KnapsackItem> knapsack, 
    double massLimit, 
    int solutionSize, 
    int populationSize, 
    int numIterations
){
    Random r;
    vector<vector<int>> population;
    population.reserve(populationSize);
    vector<int> ind;
    vector<int> bestX;
    vector<int> newIndividual;
    double mutationRate = 0.1;
    float newY;
    float bestY = -1e300;

    //Make populationSize unique individuals
    for(int i = 0; i < populationSize; i++){
        ind = generateIndividual(solutionSize);
        population.emplace_back(ind);
        newY = objective(ind, knapsack, massLimit);
        if(newY > bestY){
            bestY = newY;
            bestX = ind;
        }
    }

    int index;
    for(int i = 0; i < numIterations; i++){
        index = int(r.uniform(0, populationSize - 1));

        //New individual. Add them?
        newIndividual = mutateIndividual(population[index], mutationRate);
        newY = objective(newIndividual, knapsack, massLimit);
        if(newY > bestY){
            bestY = newY;
            bestX = newIndividual;
            population[0] = newIndividual;
        }
        else{
            for(int j = 0; j < populationSize; j++){
                if(
                    newY > objective(population[j], knapsack, massLimit)
                ){
                    population[j] = newIndividual;
                    break;
                }
            }
        }  
    }
    return(bestX);
}

int main() {
    vector<KnapsackItem> knapsack = {
        KnapsackItem("laptop", 2, 8),
        KnapsackItem("mini fridge", 10, 1),
        KnapsackItem("book1", 1.5, 5),
        KnapsackItem("book2", 1.5, 5),
        KnapsackItem("book3", 1.5, 5),
        KnapsackItem("book4", 1.5, 5),
        KnapsackItem("book5", 1.5, 5),
        KnapsackItem("Dress", 1.5, 5),
        KnapsackItem("Coat", 2.5, 7)
    };
    double limit = 12;
    int populationSize = 10;
    int numIterations = 500;

    vector<int> solution = geneticAlgorithm(knapsack, limit, knapsack.size(), populationSize, numIterations);
    printVector(solution);
    return(0);
}