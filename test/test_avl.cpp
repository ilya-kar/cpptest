#include <iostream>

#include "avl/avl.h"

int main() {
    AVLTree<int> tree;

    for (int i = 0; i < 1048576; i++) {
        tree.insert(i);
    }

    std::cout << "tree height: " << (int)tree.height() << "\n";

    return 0;
}