#include <iostream>
#include <vector>
#include <cmath>
#include "include/factorial.h"
#include "include/delete_duplicates.h"
#include "include/linked_list.h"

int main() {
    // Task 1: first n factorials (stops before overflow)
    auto facts = generate_factorials(25);
    std::cout << "factorials (" << facts.size() << "):\n";
    for (size_t i = 0; i < facts.size(); ++i) {
        std::cout << (i + 1) << "! = " << facts[i] << '\n';
    }

    // Task 2: remove duplicates while preserving order
    std::vector<int> v = {1,2,3,2,4,1};
    auto u = delete_duplicates(v);
    std::cout << "\nunique ints:";
    for (auto x : u) std::cout << ' ' << x;
    std::cout << '\n';


    // Task 3: reverse singly linked list using recursion
    LinkedList<int> list = {1,2,3,4,5};
    std::cout << "\nlist before:";
    for (auto x : list.to_vector()) std::cout << ' ' << x;
    std::cout << '\n';
    list.reverse_recursive();
    std::cout << "list after:";
    for (auto x : list.to_vector()) std::cout << ' ' << x;
    std::cout << '\n';

    return 0;
}