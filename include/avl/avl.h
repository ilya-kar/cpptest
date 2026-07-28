#include <cstdint>
#include <functional>
#include <ostream>

// можно создать контейнер с любым типом данных, который поддерживает логическую операцию
// в Comparator, который в свою очередь должен поддерживать bool operator(left, right)
template <typename Key, typename Comparator = std::less<Key>>
class AVLTree {
public:
    AVLTree() = default;

    // можно явно указать компаратор
    AVLTree(const Comparator &cmp) : cmp_(cmp) {}

    // деструктор через postorder traversal
    ~AVLTree() {
        destroy(root_);
    }

    // размер контейнера
    int size() const {
        return size_;
    }

    // высота дерева, можно проверить корректность балансировки в тестах
    uint8_t height() const {
        return height(root_);
    }

    // метод вставки с флагом успеха
    bool insert(const Key &key) {
        bool inserted = false;
        root_ = insert(root_, key, inserted);

        if (inserted) {
            size_++;
        }

        return inserted;
    }

    // метод удаления с флагом успеха
    bool remove(const Key &key) {
        bool removed = false;
        root_ = remove(root_, key, removed);

        if (removed) {
            size_--;
        }

        return removed;
    }

    // перегрузка оператора << для поддержки вывода в поток ostream
    friend std::ostream &operator<<(std::ostream &out, const AVLTree &tree) {
        tree.print(out, tree.root_);
        return out;
    }

private:
    // структура данных в контейнере
    struct Node {
        Node(const Key &key) : key_(key) {}

        Key key_;
        Node *left_ = nullptr;
        Node *right_ = nullptr;
        uint8_t height_ = 1;
    };

    // вспомогательный метод для получения высоты поддерева
    uint8_t height(Node *node) const {
        return node ? node->height_ : 0;
    }

    // метод для расчета фактора баланса
    int8_t b_factor(Node *node) const {
        return height(node->left_) - height(node->right_);
    }

    // метод для обновления высоты поддерева
    void fix_height(Node *node) const {
        uint8_t lh = height(node->left_);
        uint8_t rh = height(node->right_);
        node->height_ = std::max(lh, rh) + 1;
    }

    // левый поворот
    Node *rotate_left(Node *a) const {
        Node *b = a->right_;
        a->right_ = b->left_;
        b->left_ = a;
        fix_height(a);
        fix_height(b);
        return b;
    }

    // правый поворот
    Node *rotate_right(Node *a) const {
        Node *b = a->left_;
        a->left_ = b->right_;
        b->right_ = a;
        fix_height(a);
        fix_height(b);
        return b;
    }

    // метод балансировки дерева
    Node *rebalance(Node *node) const {
        fix_height(node);
        int8_t diff = b_factor(node);

        // если высота правого поддерево больше
        if (diff < -1) {
            // случай, когда у правого узла высота левого поддерева больше
            if (b_factor(node->right_) > 0) {
                node->right_ = rotate_right(node->right_);
            }
            return rotate_left(node);
        }

        // если высота левого поддерева больше
        if (diff > 1) {
            // случай, когда у левого узла высота правого поддерева больше
            if (b_factor(node->left_) < 0) {
                node->left_ = rotate_left(node->left_);
            }
            return rotate_right(node);
        }

        return node;
    }

    // метод вставки
    Node *insert(Node *node, const Key &key, bool &inserted) const {
        // если node == nullpt, то найдено место для вставки
        if (!node) {
            inserted = true;
            return new Node(key);
        }

        if (cmp_(key, node->key_)) { // если true, идем влево
            node->left_ = insert(node->left_, key, inserted);
        } else if (cmp_(node->key_, key)) { // если true, идем вправо
            node->right_ = insert(node->right_, key, inserted);
        } else { // иначе в контейнере уже существует ключ с таким значением
            return node;
        }

        // балансировка
        return rebalance(node);
    }

    // метод извлечения самого левого узла
    std::pair<Node *, Node *> extract_leftmost(Node *node) const {
        // если дошли до упора, возврат текущего минимального узла и его правого ребенка
        if (!node->left_) {
            return {node, node->right_};
        }

        // иначе идем дальше влево
        auto [leftmost, new_left] = extract_leftmost(node->left_);

        // обновление левого поддерева
        node->left_ = new_left;

        // возвращаемся из рекурсии, сохраняя крайний левый узел и попутно делая балансировку
        return {leftmost, rebalance(node)};
    }

    // метод удаления
    Node *remove(Node *node, const Key &key, bool &removed) const {
        // если node == nullptr, то узел с таким ключом не найден
        if (!node) {
            return node;
        }

        if (cmp_(key, node->key)) { // если true, идем влево
            node->left_ = remove(node->left_, key, removed);
        } else if (cmp_(node->key, key)) { // если true, идем вправо
            node->right_ = remove(node->right_, key, removed);
        } else {                              // нашли узел, который надо удалить
            Node *left_child = node->left_;   // левый ребенек удаляемого узла
            Node *right_child = node->right_; // правый ребенок удаляемого узла

            delete node;

            removed = true;

            // если правого ребенка нет, то просто возвращаем левого
            if (!right_child) {
                return left_child;
            }

            // иначе получаем самый левый узел в правом поддереве
            auto [leftmost, new_right] = extract_leftmost(right_child);

            // обновляем связи с потомками в узле
            leftmost->left_ = left_child;
            leftmost->right_ = new_right;

            return rebalance(leftmost);
        }

        return rebalance(node);
    }

    // вспомогательный метод для освобождения памяти при уничтожении контейнера
    void destroy(Node *node) const {
        if (!node) {
            return;
        }
        destroy(node->left_);
        destroy(node->right_);
        delete node;
    }

    // вспомогательный метод для вывода содержимого контейнера в порядке возрастания
    void print(std::ostream &out, Node *node) const {
        if (!node) {
            return;
        }
        print(out, node->left_);
        out << node->key_ << " ";
        print(out, node->right_);
    }

    Node *root_ = nullptr; // корень дерева
    int size_ = 0;         // размер контейнера
    Comparator cmp_;       // класс, который должен поддерживать bool operator(left, right)
};
