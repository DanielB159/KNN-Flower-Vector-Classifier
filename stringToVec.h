#ifndef STRING_TO_VEC_H
#define STRING_TO_VEC_H
#include <iostream>
#include <vector>
#include <cstring>
#include <sstream>


using namespace std;

namespace vectFunc{
    bool assertInput(string s1, string s2);
    bool assertVector(string s);
    bool assertNumber(string s);
    const vector<double> stringToVec(string s);
}

#endif
