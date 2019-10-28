#ifndef EXCLUDE_H
#define EXCLUDE_H

#include <iostream>
#include <fstream>
#include <deque>
#include <string>

using std::cout;
using std::deque;
using std::endl;
using std::ifstream;
using std::string;

deque<string> load_exclude(string filename)
{
    deque<string> exclude;
    ifstream file(filename);
    string str;
    if (!file)
    {
        std::cerr << "Couldn't open file: " << filename << endl;
        exit(-1);
    }

    while (std::getline(file, str))
    {
        exclude.push_back(str);
    }
    file.close();

    return exclude;
}

#endif
