#include <iostream>
#include <string>
#include <vector>
using namespace std;

//Evaluate the polynomial a + bx  + cx^2 ... at x = input
double polynomial(double x, const vector<double>& parameters){
    double output = 0;
    for(int i = 0; i < parameters.size(); i++){
        double exp = 1.0;
        for(int j = 0; j < i; j++){
            exp *= x;
        }
        output += parameters[i] * exp;
    }
    return(output);
}

int main() {
    double inputX = 2.0;
    vector<double> params = {1.25, 1.00, 1.00, 1.00, 1.00};
    double result = polynomial(inputX, params);
    string output = to_string(result);
    cout << "Result: " << output << "\n";
}