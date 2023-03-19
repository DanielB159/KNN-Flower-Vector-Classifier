#ifndef DATA_INPUT_H
#define DATA_INPUT_H
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cmath>
#include "stringToVec.h"


//this nemaspace defines two structs of ways to save a vector. {vector, type} or {distanceFromInput, type}
namespace vectorFeatures{
    struct vectorType{
        std::vector<double> vect;
        std::string type;
    };

    struct distanceType{
        double distance;
        std::string type;
    };
}

//this class is in charge of parsing the input file to a local vector array.
class dataInput{
    private:
    //filePath is the path to a csv file containing the vectors.
    std::string m_filepath;
    //mainVector is a vector that contains all of the vectorTypes. 
    std::vector<vectorFeatures::vectorType> m_mainVector;

    public:
    dataInput(std::string filepath);
    ~dataInput() {}
    vectorFeatures::vectorType* readVectorsFromFile();
    int getVecSize();
    std::vector<vectorFeatures::vectorType> getMainVector();
    std::string getFilename();
};

#endif