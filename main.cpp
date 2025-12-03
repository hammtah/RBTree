
#include <chrono>
using namespace std;
#include"Tree.h"

#include <chrono>

//Function to calculate execution time of a function
template <typename Func>
long measureExecutionTime(Func callback) {
    using namespace std::chrono;

    auto start = high_resolution_clock::now();     // start timer
    callback();                                    // execute the function
    auto end = high_resolution_clock::now();       // end timer

    auto duration = duration_cast<milliseconds>(end - start);
    return duration.count();                       // return duration in ms
}

int main() {
    Reader reader;
    // auto start = chrono::high_resolution_clock::now();  // start timer
    // reader.readFile("/home/taha/CLionProjects/redblacktree/words.txt");
    // auto end = chrono::high_resolution_clock::now();    // end timer
    // auto duration = std::chrono::duration_cast<chrono::milliseconds>((end - start));
    auto callback = [&reader](){reader.readFile("/home/taha/CLionProjects/redblacktree/words.txt");};
    cout << "Filling Tree Execution time: " << measureExecutionTime(callback) << " ms" << endl;

    // start = chrono::high_resolution_clock::now();  // start timer
    reader.text.contains("exaple")? cout << "yes contains": cout << "no not contains";
    // cout << "Do you want to read file ?" << endl;
    // reader.print();
    return 0;
}