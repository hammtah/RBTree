
#include <map>
//
// Created by taha on 10/15/25.
//
using namespace std;
#include <vector>
#include <iostream>
#include <fstream>

#include <iostream>
#include <functional>


enum Color { RED, BLACK };

//We can define our comparator to customize keys comparison
template <typename Key, typename Compare = std::less<Key>>
class RBTree {
private:
    struct Node {
        Key   key;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        explicit Node(const Key& k)
            : key(k), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
    };

public:
    RBTree() : comp(Compare()), root(nullptr), node_count(0) {}

    explicit RBTree(Compare cmp) : comp(std::move(cmp)), root(nullptr), node_count(0) {}

    ~RBTree() {
        clear();
    }

    void insert(const Key& key) {
        Node* z = new Node(key);

        // Standard BST insert
        Node* y = nullptr;
        Node* x = root;
        while (x != nullptr) {
            y = x;
            if (comp(z->key, x->key)) {
                x = x->left;
            } else {
                // On equal keys, go right to allow duplicates
                x = x->right;
            }
        }
        z->parent = y;
        if (y == nullptr) {
            root = z; // tree was empty
        } else if (comp(z->key, y->key)) {
            y->left = z;
        } else {
            y->right = z;
        }

        // Fix Red-Black properties
        node_count++;
        fixInsert(z);
    }

    bool contains(const Key& key) const {
        Node* x = root;
        while (x != nullptr) {
            if (!comp(key, x->key) && !comp(x->key, key)) return true;
            if (comp(key, x->key)) x = x->left;
            else x = x->right;
        }
        return false;
    }

    void inorder() const {
        inorderRec(root);
        std::cout << std::endl;
    }

    // Print the tree sideways (right subtree up), including node colors.
    void printTree(std::ostream& os = std::cout) const {
        std::function<void(Node*, int)> print = [&](Node* n, int depth) {
            if (!n) return;
            print(n->right, depth + 1);
            for (int i = 0; i < depth; ++i) os << "    ";
            os << n->key << (n->color == RED ? " (R)" : " (B)") << '\n';
            print(n->left, depth + 1);
        };
        print(root, 0);
    }

    size_t size() const { return node_count; }

    void clear() {
        clearRec(root);
        root = nullptr;
        node_count = 0;
    }

private:
    Compare comp;
    Node* root;
    size_t node_count;

    static Color colorOf(Node* n) {
        return n == nullptr ? BLACK : n->color;
    }

    static void setColor(Node* n, Color c) {
        if (n != nullptr) n->color = c;
    }

    void leftRotate(Node* x) {
        Node* y = x->right;
        if (y == nullptr) return; // nothing to rotate

        // Turn y's left subtree into x's right subtree
        x->right = y->left;
        if (y->left != nullptr) {
            y->left->parent = x;
        }

        // Link x's parent to y
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        // Put x on y's left
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node* y) {
        Node* x = y->left;
        if (x == nullptr) return; // nothing to rotate

        // Turn x's right subtree into y's left subtree
        y->left = x->right;
        if (x->right != nullptr) {
            x->right->parent = y;
        }

        // Link y's parent to x
        x->parent = y->parent;
        if (y->parent == nullptr) {
            root = x;
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }

        // Put y on x's right
        x->right = y;
        y->parent = x;
    }

    void fixInsert(Node* z) {
        while (z != root && colorOf(z->parent) == RED) {
            Node* gp = z->parent->parent; // grandparent exists because parent is red and not root
            if (z->parent == gp->left) {
                Node* y = gp->right; // uncle
                if (colorOf(y) == RED) {
                    // Case 1: Uncle is red -> recolor
                    setColor(z->parent, BLACK);
                    setColor(y, BLACK);
                    setColor(gp, RED);
                    z = gp;
                } else {
                    // Uncle is black
                    if (z == z->parent->right) {
                        // Case 2: turn into case 3
                        z = z->parent;
                        leftRotate(z);
                    }
                    // Case 3
                    setColor(z->parent, BLACK);
                    setColor(gp, RED);
                    rightRotate(gp);
                }
            } else {
                // Mirror cases: parent is right child of grandparent
                Node* y = gp->left; // uncle
                if (colorOf(y) == RED) {
                    // Case 1 (mirror)
                    setColor(z->parent, BLACK);
                    setColor(y, BLACK);
                    setColor(gp, RED);
                    z = gp;
                } else {
                    if (z == z->parent->left) {
                        // Case 2 (mirror)
                        z = z->parent;
                        rightRotate(z);
                    }
                    // Case 3 (mirror)
                    setColor(z->parent, BLACK);
                    setColor(gp, RED);
                    leftRotate(gp);
                }
            }
        }
        setColor(root, BLACK);
    }

    static void inorderRec(Node* n) {
        if (n == nullptr) return;
        inorderRec(n->left);
        std::cout << n->key << (n->color == RED ? "R " : "B ") << endl;
        inorderRec(n->right);
    }

    static void clearRec(Node* n) {
        if (n == nullptr) return;
        clearRec(n->left);
        clearRec(n->right);
        delete n;
    }

public:
    // Delete a key from the RBTree. Returns true if a node was removed.
    bool erase(const Key& key) {
        Node* z = findNode(key);
        if (z == nullptr) return false;

        // If z has two children, copy successor's key into z and delete the successor node instead.
        Node* y = z;
        if (z->left != nullptr && z->right != nullptr) {
            y = minimum(z->right);
            z->key = y->key;
        }

        // Now y has at most one child
        Node* child = (y->left != nullptr) ? y->left : y->right;
        Node* yParent = y->parent;
        Color yOriginalColor = colorOf(y);

        // Replace y with its child
        if (child != nullptr) {
            child->parent = yParent;
        }
        if (yParent == nullptr) {
            root = child;
        } else if (y == yParent->left) {
            yParent->left = child;
        } else {
            yParent->right = child;
        }

        if (yOriginalColor == BLACK) {
            // child can be nullptr; pass yParent to allow proper sibling lookup
            fixDelete(child, yParent);
        }

        delete y;
        --node_count;
        return true;
    }

    // Convenience alias
    bool remove(const Key& key) { return erase(key); }

private:
    Node* findNode(const Key& key) const {
        Node* x = root;
        while (x != nullptr) {
            if (!comp(key, x->key) && !comp(x->key, key)) return x;
            if (comp(key, x->key)) x = x->left;
            else x = x->right;
        }
        return nullptr;
    }

    Node* minimum(Node* n) const {
        if (n == nullptr) return nullptr;
        while (n->left != nullptr) n = n->left;
        return n;
    }

    void fixDelete(Node* x, Node* xParent) {
        // x may be nullptr; treat nullptr as black using colorOf
        while (x != root && colorOf(x) == BLACK) {
            if (x == (xParent ? xParent->left : nullptr)) {
                Node* w = xParent ? xParent->right : nullptr; // sibling
                if (colorOf(w) == RED) {
                    setColor(w, BLACK);
                    setColor(xParent, RED);
                    leftRotate(xParent);
                    w = xParent ? xParent->right : nullptr;
                }
                if (colorOf(w ? w->left : nullptr) == BLACK &&
                    colorOf(w ? w->right : nullptr) == BLACK) {
                    setColor(w, RED);
                    x = xParent;
                    xParent = x ? x->parent : nullptr;
                } else {
                    if (colorOf(w ? w->right : nullptr) == BLACK) {
                        setColor(w ? w->left : nullptr, BLACK);
                        setColor(w, RED);
                        rightRotate(w);
                        w = xParent ? xParent->right : nullptr;
                    }
                    setColor(w, colorOf(xParent));
                    setColor(xParent, BLACK);
                    setColor(w ? w->right : nullptr, BLACK);
                    leftRotate(xParent);
                    x = root;
                    xParent = nullptr;
                }
            } else {
                Node* w = xParent ? xParent->left : nullptr; // sibling (mirror)
                if (colorOf(w) == RED) {
                    setColor(w, BLACK);
                    setColor(xParent, RED);
                    rightRotate(xParent);
                    w = xParent ? xParent->left : nullptr;
                }
                if (colorOf(w ? w->left : nullptr) == BLACK &&
                    colorOf(w ? w->right : nullptr) == BLACK) {
                    setColor(w, RED);
                    x = xParent;
                    xParent = x ? x->parent : nullptr;
                } else {
                    if (colorOf(w ? w->left : nullptr) == BLACK) {
                        setColor(w ? w->right : nullptr, BLACK);
                        setColor(w, RED);
                        leftRotate(w);
                        w = xParent ? xParent->left : nullptr;
                    }
                    setColor(w, colorOf(xParent));
                    setColor(xParent, BLACK);
                    setColor(w ? w->left : nullptr, BLACK);
                    rightRotate(xParent);
                    x = root;
                    xParent = nullptr;
                }
            }
        }
        setColor(x, BLACK);
    }
};


class Reader {
public:
    RBTree<string> text;
    void readFile(string file) {
        int i = 0;
        string name;
        ifstream inputFile(file);
        if (!inputFile.is_open()) {
        throw std::runtime_error("Error opening File");
        }
        // while (inputFile >> name) {
        int a = 0;
        while (getline(inputFile, name)) {
            // cout << "inserting " << name << " " << i++ << endl;
            // text.insert({name, a++});
            text.insert(name);
        }
        inputFile.close();
    }

    void print() {
        text.inorder();
    }
};