
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
    //Inserting
    auto insert = [&reader](){reader.readFile("/home/taha/CLionProjects/redblacktree/words.txt");};
    cout << "Filling Tree Execution time: " << measureExecutionTime(insert) << " ms" << endl;

    //Searching
    string str = "example";
    reader.text.contains(str)? (cout << "The file contains "<< str << endl) : (cout << "file does not contains "<< str << endl);
    auto search = [&reader](){reader.text.contains("example");};
    cout << "Searching in Tree Execution time: " << measureExecutionTime(search) << " ms" << endl;


    //
    //
    // auto start = std::chrono::high_resolution_clock::now();     // start timer
    // reader.text.contains("example");                                    // execute the function
    // auto end =  std::chrono::high_resolution_clock::now();       // end timer
    //
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>((end - start));
    // cout << "Duration for searching: " << duration.count() << endl;

    return 0;
}