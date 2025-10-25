#include <iostream>
#include <vector>
#include "include/factorial.h"
#include "include/delete_duplicates.h"
#include "include/linked_list.h"
namespace solution {
    using std::cout;
    using std::cin;
    using std::vector;
}
using namespace solution;
int main() {
    // Task 1: first n factorials (stops before overflow)
    cout << "Input number of factorials to calculate: ";
    int factorial_number;
    cin >> factorial_number;
    auto facts = generate_factorials(factorial_number);
    cout << "factorials (" << facts.size() << "):\n";
    for (size_t i = 0; i < facts.size(); ++i) {
        cout << (i + 1) << "! = " << facts[i] << '\n';
    }

    // Task 2: remove duplicates while preserving order
    cout << "Input array size: ";
    int size;
    cin >> size;
    cout << "Input " << size << "array elements: ";
    vector<int> v(size);
    for (int i = 0; i < size; i++) {
        cin >> v[i];
    }
    auto u = delete_duplicates(v);
    std::cout << "Unique numbers:";
    for (auto x : u) std::cout << ' ' << x;
    std::cout << '\n';


    // Task 3: reverse singly linked list using recursion
    cout << "Input LinkedList size: ";
    cin >> size;
    cout << "Input " << size << " elements for LinkedList";
    LinkedList<int> list;
    int temp;
    for (int i = 0; i < size; i++) {
        cin >> temp;
        list.append(temp);
    }
    std::cout << "\nlist before:";
    for (auto x : list.to_vector()) std::cout << ' ' << x;
    std::cout << '\n';
    list.reverse_recursive();
    std::cout << "list after:";
    for (auto x : list.to_vector()) std::cout << ' ' << x;
    std::cout << '\n';

    return 0;
}