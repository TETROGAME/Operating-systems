#include "include/linked_list.h"
#include <memory>
#include <vector>
#include <utility>
#include <stdexcept>

using std::vector;

template <typename T>
Node<T>::Node(T val)
    : v(std::move(val)), next(nullptr) {}

template <typename T>
LinkedList<T>::LinkedList()
    : head(nullptr) {}

template <typename T>
LinkedList<T>::LinkedList(std::initializer_list<T> init)
    : head(nullptr) {
    for (const auto& x : init) append(x);
}

template <typename T>
void LinkedList<T>::append(const T& value) {
    auto n = std::make_shared<Node<T>>(value);
    if (!head) { head = n; return; }
    auto cur = head;
    while (cur->next) cur = cur->next;
    cur->next = n;
}

template <typename T>
vector<T> LinkedList<T>::to_vector() const {
    vector<T> out;
    for (auto cur = head; cur; cur = cur->next) out.push_back(cur->v);
    return out;
}

template <typename T>
void LinkedList<T>::reverse_recursive() {
    if (!head) {
        throw std::runtime_error("reverse_recursive: cannot reverse an empty list");
    }
    head = reverse_impl(head);
}

template <typename T>
std::shared_ptr<Node<T>> LinkedList<T>::reverse_impl(std::shared_ptr<Node<T>> node) {
    if (!node || !node->next) return node;
    auto new_head = reverse_impl(node->next);
    node->next->next = node;
    node->next = nullptr;
    return new_head;
}

template class LinkedList<int>;
template struct Node<int>;