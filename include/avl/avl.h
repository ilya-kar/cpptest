#include <cstdint>
#include <functional>
#include <iostream>
#include <iterator>

// можно создать контейнер с любым типом данных, который поддерживает логическую операцию
// в Comparator
template <typename Key, typename Comparator = std::less<Key>>
class AVLTree {
public:
    AVLTree() = default;

    // можно явно указать компаратор
    AVLTree(const Comparator &cmp) : m_cmp(cmp) {}

    // размер контейнера
    int size() const {
        return m_size;
    }

    // метод вставки с флагом успеха
    bool insert(const Key &key) {

        bool inserted = false;
        m_root = insert(m_root, key, inserted);

        if (inserted) {
            m_size++;
        }

        return inserted;
    }

    // метод удаления с флагом успеха
    bool remove(const Key &key) {
        bool removed = false;
        m_root = remove(m_root, key, removed);

        if (removed) {
            m_size--;
        }

        return removed;
    }

private:
    // структура данных в контейнере
    struct Node {
        Node(const Key &key) : key(key) {}

        Key key;
        Node *left = NULL;
        Node *right = nullptr;
        uint8_t height = 1;
    };

    // вспомогательный метод для получения высоты поддерева
    uint8_t height(Node *node) {
        return node ? node->height : 0;
    }

    // метод для расчета фактора баланса
    uint8_t b_factor(Node *node) {
        return height(node->left) - height(node->right);
    }

    // метод для обновления высоты поддерева
    void fix_height(Node *node) {
        uint8_t lh = height(node->left);
        uint8_t rh = height(node->right);
        node->height = std::max(lh, rh) + 1;
    }

    // левый поворот
    Node *rotate_left(Node *a) {
        Node *b = a->right;
        a->right = b->left;
        b->left = a;
        fix_height(a);
        fix_height(b);
        return b;
    }

    // правый поворот
    Node *rotate_right(Node *a) {
        Node *b = a->left;
        a->left = b->right;
        b->right = a;
        fix_height(a);
        fix_height(b);
        return b;
    }

    Node *rebalance(Node *node) {
        fix_height(node);
        uint8_t diff = b_factor(node);

        if (diff < -1) {
            if (b_factor(node->right) > 0) {
                node->right = rotate_right(node->right);
            }
            return rotate_left(node);
        }

        if (diff > 1) {
            if (b_factor(node->left) < 0) {
                node->left = rotate_left(node->left);
            }
            return rotate_right(node);
        }

        return node;
    }

    Node *insert(Node *node, const Key &key, bool &inserted) {
        if (!node) {
            inserted = true;
            return new Node(key);
        }

        if (m_cmp(key, node->key)) {
            node->left = insert(node->left, key, inserted);
        } else if (m_cmp(node->key, key)) {
            node->right = insert(node->right, key, inserted);
        } else {
            return node;
        }

        return rebalance(node);
    }

    void foo(int x) {
        std::cout << x << '\n';
    }

    void foo(double x) {
        std::cout << x << '\n';
    }

    std::pair<Node *, Node *> extract_min(Node *node) {
        if (!node->left) {
            return {node, node->right};
        }

        auto [min_node, new_left] = extract_min(node->left);
        node->left = new_left;

        return {min_node, rebalance(node)};
    }

    Node *remove(Node *node, const Key &key, bool &removed) {
        if (!node) {
            return node;
        }

        if (m_cmp(key, node->key)) {
            node->left = remove(node->left, key, removed);
        } else if (m_cmp(node->key, key)) {
            node->right = remove(node->right, key, removed);
        } else {
            Node *left_child = node->left;
            Node *right_child = node->right;

            delete node;

            removed = true;

            if (!right_child) {
                return left_child;
            }

            auto [min_node, new_right] = extract_min(right_child);

            node = min_node;

            node->right = new_right;
            node->left = left_child;
        }

        return rebalance(node);
    }

    Node *m_root = nullptr;
    int m_size = 0;
    Comparator m_cmp;
};
