
#include <chrono>
using namespace std;
#include"Tree.h"
int main() {
    Reader reader;
    auto start = chrono::high_resolution_clock::now();  // start timer
    reader.readFile("/home/taha/CLionProjects/redblacktree/words.txt");
    auto end = chrono::high_resolution_clock::now();    // end timer
    auto duration = std::chrono::duration_cast<chrono::milliseconds>((end - start));
    cout << "Filling Tree Execution time: " << duration.count() << " ms" << endl;
    // cout << "Do you want to read file ?" << endl;
    // reader.print();
    return 0;
}