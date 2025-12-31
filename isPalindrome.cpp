#include <iostream>
#include <string>
using namespace std;

bool isPalindrome(const string& s, int left, int right) {
    if (left >= right) return true;
    if (s[left] != s[right]) return false;
    return isPalindrome(s, left + 1, right - 1);
}

int main() {
    cout << "Input string: ";
    string input;
    cin >> input;

    if (isPalindrome(input, 0, input.size() - 1))
        cout << "Palindrome\n";
    else
        cout << "Not a palindrome\n";
}