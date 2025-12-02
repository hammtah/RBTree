
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
    RBTree() : comp(Compare()), root(nullptr) {}

    explicit RBTree(Compare cmp) : comp(std::move(cmp)), root(nullptr) {}

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
            if (!comp(z->key, x->key) && !comp(x->key, z->key)) {
                // No duplicates for simplicity: keys equivalent under comparator
                delete z;
                return;
            } else if (comp(z->key, x->key)) {
                x = x->left;
            } else {
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

    void clear() {
        clearRec(root);
        root = nullptr;
    }

private:
    Compare comp;
    Node* root;

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
        std::cout << n->key << (n->color == RED ? "R " : "B ");
        inorderRec(n->right);
    }

    static void clearRec(Node* n) {
        if (n == nullptr) return;
        clearRec(n->left);
        clearRec(n->right);
        delete n;
    }
};


class Reader {
public:
    RBTree<string> text;
    void readFile(string file) {
        int i = 0;
        string name;
        ifstream inputFile(file);
        // if (!inputFile.is_open()) {
        //     throw std::runtime_error("Error opening File");
        // }
        // while (inputFile >> name) {
        while (getline(inputFile, name)) {
            // cout << "inserting " << name << " " << i++ << endl;
            text.insert(name);
        }
        inputFile.close();
    }

    void print() {
        text.inorder();
    }
};