#include <functional>
#include <stdint.h>

template <typename Key, typename Comparator = std::less<Key>>
class AVLTree {
public:
    AVLTree() = default;
    AVLTree(const Comparator &cmp) : m_cmp(cmp) {}

    int size() const {
        return m_size;
    }

    bool insert(const Key &key) {
        bool inserted = false;
        m_root = insert(m_root, key, inserted);
        return inserted;
    }

private:
    struct Node {
        Node(const Key &key) : key(key) {}

        Key key;
        Node *left = nullptr;
        Node *right = nullptr;
        uint8_t height = 1;
    };

    uint8_t height(Node *node) {
        return node ? node->height : 0;
    }

    uint8_t b_factor(Node *node) {
        return height(node->left) - height(node->right);
    }

    void fix_height(Node *node) {
        uint8_t lh = height(node->left);
        uint8_t rh = height(node->right);
        node->height = std::max(lh, rh) + 1;
    }

    Node *rotate_left(Node *a) {
        b = a->right;
        a->right = b->left;
        b->left = a;
        fix_height(a);
        fix_height(b);
        return b;
    }

    Node *rotate_right(Node *a) {
        b = a->left;
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
            m_size++;
            return new Node(key);
        }

        if (m_cmp(key, node->key)) {
            node->left = insert(node->left, key);
        } else if (m_cmp(node->key, key)) {
            node->right = insert(node->right, key);
        } else {
            return node;
        }

        return rebalance(node);
    }

    Node *m_root = nullptr;
    int m_size = 0;
    Comparator m_cmp;
};