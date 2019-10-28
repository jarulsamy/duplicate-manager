#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <filesystem>
#include <map>

#include "hash.h"

namespace fs = std::filesystem;
using std::cout;
using std::deque;
using std::endl;
using std::map;
using std::pair;
using std::string;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: ./main.out PATH_TO_DIR" << endl;
        return 0;
    }

    Tree<unsigned long long int> T;
    deque<string> files;
    // Associate hash with number of occurences
    map<int, deque<string>> found;

    // Load all the files into a binary tree.
    load_files(argv[1], T);
    // Load all the files into a deque
    load_files(argv[1], files);

    for (string file : files)
    {
        int hash = hashFile(file);
        found[hash].push_back(file);
    }

    for (pair<int, deque<string>> p : found)
    {
        cout << p.first << " : ";
        for (string file : p.second)
            cout << file << ", ";
        cout << endl;
    }

    return 0;
}