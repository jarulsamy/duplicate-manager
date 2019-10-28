#ifndef HASH_H
#define HASH_H

#include <deque>
#include <algorithm>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <openssl/md5.h>

#include "Tree.h"
#include "exclude.h"

namespace fs = std::filesystem;
using std::cout;
using std::deque;
using std::endl;
using std::hash;
using std::ifstream;
using std::string;

#define BUFFSIZE 16384

deque<string> exclude = load_exclude("exclude.txt");

unsigned long long int hashFile(const string &fname)
{

    hash<string> hasher;
    char buffer[BUFFSIZE];
    unsigned char digest[MD5_DIGEST_LENGTH];

    std::stringstream ss;
    std::string md5string;

    std::ifstream ifs(fname, std::ifstream::binary);

    MD5_CTX md5Context;

    MD5_Init(&md5Context);

    while (ifs.good())
    {

        ifs.read(buffer, BUFFSIZE);

        MD5_Update(&md5Context, buffer, ifs.gcount());
    }

    ifs.close();

    int res = MD5_Final(digest, &md5Context);

    if (res == 0)  // hash failed
        return {}; // or raise an exception

    // set up stringstream format
    ss << std::hex << std::uppercase << std::setfill('0');

    for (unsigned char uc : digest)
        ss << std::setw(2) << (int)uc;

    md5string = ss.str();

    return hasher(md5string);
}

/*
Potentially instead of overloading
this should be consolidated into one function
to avoid transversing all dirs twice.
*/

void load_files(string path, Tree<unsigned long long int> &T)
{
    for (const auto &entry : fs::directory_iterator(path))
    {
        if (fs::is_directory(entry.path()))
        {
            load_files(entry.path(), T);
        }
        else if (fs::is_regular_file(entry.path()))
        {
            string path = entry.path();
            if (exclude.size() > 0)
            {
                for (string s : exclude)
                {
                    if (path.find(s) == string::npos)
                    {
                        T.insert(hashFile(entry.path()));
                        break;
                    }
                }
            }
            else
            {
                T.insert(hashFile(entry.path()));
            }
        }
    }

    return;
}

void load_files(string path, deque<string> &d)
{
    for (const auto &entry : fs::directory_iterator(path))
    {
        if (fs::is_directory(entry.path()))
        {
            load_files(entry.path(), d);
        }
        else if (fs::is_regular_file(entry.path()))
        {
            string path = entry.path();
            if (exclude.size() > 0)
            {
                for (string s : exclude)
                {
                    if (path.find(s) == string::npos)
                    {
                        d.push_back(entry.path());
                        break;
                    }
                }
            }
            else
            {
                d.push_back(entry.path());
            }
        }
    }
}

#endif
