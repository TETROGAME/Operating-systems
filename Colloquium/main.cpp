#include <iostream>
#include "include/factorial.h"
using std::cout;
using std::cin;
int main() {
    int factorials_number;
    cin >> factorials_number;
    auto facts = generate_factorials(factorials_number);
    cout << "factorials (" << facts.size() << "):\n";
    for (size_t i = 0; i < facts.size(); ++i) {
        cout << (i + 1) << "! = " << facts[i] << '\n';
    }

    return 0;
}