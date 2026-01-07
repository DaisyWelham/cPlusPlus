#include <iostream>
#include <string>
#include <random>
using namespace std;

//Random class
class Random {
public:
    // choice({a, b, c})
    template<typename T>
    T choice(std::initializer_list<T> xs) {

        std::uniform_int_distribution<size_t> dist(0, xs.size() - 1);
        auto it = xs.begin();
        std::advance(it, dist(gen));
        return(*it);
    }

    // uniform(a, b) for doubles (Python-style)
    double uniform(double a, double b) {
        std::uniform_real_distribution<double> dist(a, b);
        return(dist(gen));
    }

    // uniform(a, b) for ints (inclusive, like Python)
    int uniform(int a, int b) {
        std::uniform_int_distribution<int> dist(a, b);
        return(dist(gen));
    }

private:
    std::mt19937 gen{std::random_device{}()};
};

int main() {
    Random r;
    int result = r.choice({0, 1, 2, 3});
    cout << "Result: " << result;
    return(0);
}