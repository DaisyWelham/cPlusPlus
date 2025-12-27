#include <iostream>
#include <string>
using namespace std;

//Creating the "Animal" class to see class implementation in C++
class Animal {
public:
    string name;
    int legNum;
    double averageHeight;
    string latinGenus;

    Animal(string n, int legs, double height, string genus)
        : name(n), legNum(legs), averageHeight(height), latinGenus(genus) {}

    void introduceSelf() const {
        cout << "Hello, my name is " << name 
             << " and I have " << legNum << " legs." << endl;
    }
};

int main() {
    Animal spider("Spider", 8, 0.02, "Araneus");
    Animal duck("Duck", 2, 0.4, "Anas");
    Animal human("Human", 2, 1.7, "Homo");
    Animal horse("Horse", 4, 1.6, "Equus");

    spider.introduceSelf();
    duck.introduceSelf();
    human.introduceSelf();
    horse.introduceSelf();

    return 0;
}
