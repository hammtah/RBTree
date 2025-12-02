
#include"Tree.h"
int main() {
    RBTree<int> t;

    int values[] = {10, 20, 30, 15, 25, 5, 1, 50, 60, 55, 8, 12};
    for (int v : values) {
        t.insert(v);
    }

    std::cout << "Inorder traversal (key with color R/B): ";
    t.inorder();

    std::cout << "Contains 15? " << (t.contains(15) ? "yes" : "no") << std::endl;
    std::cout << "Contains 100? " << (t.contains(100) ? "yes" : "no") << std::endl;

    return 0;
}