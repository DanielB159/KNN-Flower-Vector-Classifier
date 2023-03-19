#include "stringToVec.h"

/// @brief This functions asserts if two string inputs are vectors of doubles of the same size
/// @param s1 First input string
/// @param s2 Second input string
/// @return if the two vectors depict actual vector<double> and are the same size
bool vectFunc::assertInput(string s1, string s2) {
    //first, check if one or more is an empty string.
    if(s1.length() == 0 || s2.length() == 0) {
        return 0;
    }
    
    //check if the first or last character is ' ' (incorrect format)
    if (s1[0] == ' ' || s2[0] == ' ' || s1[s1.length() - 1] == ' ' || s2[s2.length() - 1] == ' ') {
        return 0;
    }
    
    //check if there is a double space or more in the input strings.
    if (s1.find("  ") != string::npos || s2.find("  ") != string::npos) {
        return 0;
    }

    //count vector sizes by counting their spaces
    int c1 = 0;
    for (int i = 0; i < s1.length(); i++) { 
        if (s1[i] == ' ') {
            c1++;
        }
    }

    int c2 = 0;
    for (int i = 0; i < s2.length(); i++) { 
        if (s2[i] == ' ') {
            c2++;
        }
    }

    //check if the elements are eligable doubles and their sizes are the same.
    return (c1 == c2) && assertVector(s1) && assertVector(s2);
}

/// @brief This function asserts if a given input is an eligable vector<double>
/// @param s string input
/// @return if the input is a valid vector<double>
bool vectFunc::assertVector(string s) {
    
    //first, check if s is an empty string.
    if(s.length() == 0) {
        return 0;
    }
    
    //check if the first or last character is ' ' (incorrect format)
    if (s[0] == ' ' || s[s.length() - 1] == ' ') {
        return 0;
    }
    
    //check if there is a double space or more in the input strings.
    if (s.find("  ") != string::npos) {
        return 0;
    }
    
    //passed first assertions
    int dim = 0;
    string num;
    //splitting the string s
    stringstream stream(s);
    while(getline(stream, num, ' ')){
        if (!vectFunc::assertNumber(num)){
            return false;
        }
    }
    
    //passed all assertions
    return true;
}

/// @brief This function checks weather the string is a valid number
/// @param s Input string
/// @return true if number, false otherwise,
bool vectFunc::assertNumber(string num) {
        //allow for negative numbers
        if (!('0' <= num[0] && num[0] <= '9') && !(num[0] == '-')) {
            return false;
        }

        //asserting whether or not it is an eligable number
        bool fraction = false;
        for (int i = 1; i < num.size(); i++) {
            //if not a number, or not a dot before there was a fraction point, is not eligable
            if (!('0' <= num[i] && num[i] <= '9') && !(num[i] == '.' && !fraction)) {
                return false;
            } else if (num[i] == '.') {
                fraction = true;
            }
        }
        //passed all aseertions
        return true;
}
 
/// @brief This function converts a formatted input string to a vector<double>
/// @param s Input string
/// @return const vector<double> - the vector that was in the string.
const vector<double> vectFunc::stringToVec(string s) {
    vector<double> v;
    string num;
    //splitting the string s and adding each element to v1.
    stringstream stream(s);
    while(getline(stream, num, ' ')){
        v.push_back(stod(num));
    }
    return v;
}