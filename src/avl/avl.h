#pragma once

#include <functional>
#include <utility>

template <typename T, typename Comparator = std::less<T>>
class AVL {
public:
    bool insert(T value);

private:
    std::pair<bool, Node *> insert(T value, Node *node);

    struct Node {
        T value;
        Node *right;
        Node *left;
    };

    Node *m_root = nullptr;
};
