#include <iostream>
#include <vector>
#include <utility>
#include <random>

using namespace std;

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

    return(result);
}

void printVector(const vector<double>& v) {
    cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i + 1 < v.size()) cout << ", ";
    }
    cout << "]";
}

int main(){
    vector<pair<double, double>> ranges = {{0,1}, {0,1}, {0,1}, {0,1}, {0,1}};
    vector<double> randomV = randomVector(ranges);
    printVector(randomV);
}