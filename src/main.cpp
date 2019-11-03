#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <filesystem>
#include <map>
#include <fstream>

#include <boost/program_options.hpp>
#include "hash.h"
// #include "progressbar.hpp"

namespace fs = std::filesystem;
namespace po = boost::program_options;

using std::cout;
using std::deque;
using std::endl;
using std::map;
using std::ofstream;
using std::pair;
using std::string;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: ./main.out PATH_TO_DIR" << endl;
        return 0;
    }

    Tree<u_int64_t> T;
    deque<string> files;
    // Associate hash with number of occurences
    map<int, deque<string>> found;

    // Load all the files into a binary tree.
    load_files(argv[1], T, files);
    // Load all the files into a deque
    ofstream outFile("Duplicates.txt");

    // const int limit = files.size();

    for (string file : files)
    {
        int hash = hashFile(file);
        found[hash].push_back(file);
    }

    for (pair<int, deque<string>> p : found)
    {
        if (p.second.size() > 1)
        {
            outFile << p.first << " : ";
            for (string file : p.second)
                outFile << file << ", ";
            outFile << "[ " << p.second.size() << " ]" << endl;
        }
    }
    outFile.close();

    return 0;
}
