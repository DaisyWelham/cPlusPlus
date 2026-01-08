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

    // Like choice, but without replacement.
    template<typename T>
    vector<T> sample(const vector<T>& xs, size_t k) {
        assert(k <= xs.size());
        vector<T> copy = xs;
        shuffle(copy);
        return(vector<T>(copy.begin(), copy.begin() + k));
    }

    // uniform(a, b) for doubles (Python-style)
    double uniform(double a, double b) {
        std::uniform_real_distribution<double> dist(a, b);
        return(dist(gen));
    }

    // uniform(a, b) for ints (inclusive, like Python)
    int randint(int a, int b) {
        std::uniform_int_distribution<int> dist(a, b);
        return(dist(gen));
    }

    // Gives a gaussian/normal about mean with standard deviation STD
    double gauss(double mean = 0.0, double STD = 1.0) {
        std::normal_distribution<double> dist(mean, STD);
        return(dist(gen));
    }

    // Shuffle
    template<typename T>
    void shuffle(vector<T>& xs) {
        std::shuffle(xs.begin(), xs.end(), gen);
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