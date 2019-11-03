#ifndef HASH_H
#define HASH_H

#include <deque>
#include <algorithm>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <iomanip>
// #include <openssl/md5.h>

#include "Tree.h"
// #include "exclude.h"
#include "xxhash64.h"
#include "progressbar.hpp"

namespace fs = std::filesystem;
using std::cout;
using std::deque;
using std::endl;
using std::hash;
using std::ifstream;
using std::string;

#define BUFFSIZE 16384

// deque<string> exclude = load_exclude("exclude.txt");

u_int64_t hashFile(const string &filename)
{
    ifstream input(filename, std::ios::binary);
    std::deque<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
    input.close();

    u_int64_t myseed = 0;
    XXHash64 myHash(myseed);
    for (unsigned char c : buffer)
    {
        myHash.add(&c, 1);
    }
    u_int64_t result = myHash.hash();
    return result;
}

void count_files(const string path, int &num_files)
{
    for (const auto &entry : fs::directory_iterator(path))
    {
        if (fs::is_directory(entry.path()))
        {
            count_files(entry.path(), num_files);
        }
        else if (fs::is_regular_file(entry.path()))
        {
            string path = entry.path();
            if (path.find(".git") == string::npos)
            {
                num_files++;
            }
        }
    }

    return;
}

void load_files(string path, Tree<u_int64_t> &T, deque<string> &d, ProgressBar &pb)
{
    for (const auto &entry : fs::directory_iterator(path))
    {
        if (fs::is_directory(entry.path()))
        {
            load_files(entry.path(), T, d, pb);
        }
        else if (fs::is_regular_file(entry.path()))
        {
            string path = entry.path();
            if (path.find(".git") == string::npos)
            {
                T.insert(hashFile(entry.path()));
                d.push_back(entry.path());
                ++pb;
            }
        }
        pb.display();
    }
}
#endif
