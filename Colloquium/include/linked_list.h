#ifndef COLLOQUIUM_LINKED_LIST_H
#define COLLOQUIUM_LINKED_LIST_H
#include <memory>
#include <vector>
#include <initializer_list>

template <typename T>
struct Node {
    T v;
    std::shared_ptr<Node<T>> next;
    explicit Node(T val): v(std::move(val)), next(nullptr) {}
};

template <typename T>
class LinkedList {
    std::shared_ptr<Node<T>> head;
public:
    LinkedList() = default;
    LinkedList(std::initializer_list<T> init);
    void append(const T& value);
    std::vector<T> to_vector() const;
    void reverse_recursive();
private:
    std::shared_ptr<Node<T>> reverse_impl(std::shared_ptr<Node<T>> node);
};

#endif //COLLOQUIUM_LINKED_LIST_H
