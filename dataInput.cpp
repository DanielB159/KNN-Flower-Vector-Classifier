#include "dataInput.h"
#include "stringToVec.h"
#include <stdio.h>
#include <string>
#include <stdexcept>
#include <cmath>

/// @brief this function is a constructor function for the class dataInput.
/// @param filepath - the path to a .csv file containing vector values..
/// @return dataInput.
dataInput::dataInput(std::string filepath) {
    this->m_filepath = filepath;
}

/// @brief this function reads all of the vectors in the given file path.
/// @return pointer to vectorType sved in the heap.
vectorFeatures::vectorType* dataInput::readVectorsFromFile(){
    std::string filepath = this->m_filepath;
    std::fstream file(filepath, std::ios::in);
    std::string line;
    //if did not fild the file
    if (!file.is_open()) {
        throw std::invalid_argument("bad file path");
    }
    //reading from the file
    if(file.is_open()){
        //while still getting rows of vectors
        while(getline(file, line)){
            //will be a string representing the vector of this row
            std::string vectorString = "";
            //will be a stream to decode this vector
            std::stringstream stream(line);
            //will represent each number in the vector.
            std::string numberInString;
            //will vbe the type of flower that the vector is describing.
            std::string flowerType;
            bool firstNum = true;
            //making sure that all of the numbers that are stored in the vector are valid.
            while(getline(stream, numberInString, ',')) {
                if (numberInString[0] == '.') { //when reading from a csv file, sometimes 0.12 is read as .12.
                    numberInString = "0" + numberInString;   
                }
                //check if the number is in exponent form. if so, convert it to regular decimal form.
                if (numberInString.find("E-") != std::string::npos || numberInString.find("E+") != std::string::npos) {
                    int indexOfE = numberInString.find("E");
                    std::string exponent = numberInString.substr(indexOfE + 1, numberInString.length() - (indexOfE + 1));
                    numberInString = numberInString.substr(0, indexOfE); //setting the number to be without the exponent
                    if (vectFunc::assertNumber(numberInString) && vectFunc::assertNumber(exponent)) {
                        double number = stod(numberInString);
                        double exp = stod(exponent);
                        //finalizing the number to be correct
                        double finalNum = number*std::pow(10, exp);
                        //converting finalNum back to string to continue checks.
                        std::ostringstream s;
                        s << finalNum;
                        numberInString = s.str();
                    } else {
                        //if the vector has invalid characters before or after exponent, dont read this line.
                        continue;                    
                    }
                }
                if (vectFunc::assertNumber(numberInString)){
                    if (firstNum) {
                        vectorString += numberInString;
                        firstNum = false;
                    } else {
                        vectorString += " " + numberInString;
                    }                
                } else {
                    flowerType = numberInString;
                }
            }
            std::vector<double> v1;
            if (vectFunc::assertVector(vectorString)){
                v1 = vectFunc::stringToVec(vectorString);
                vectorFeatures::vectorType newVType = {v1, flowerType};
                //pushing the newly made vector to the mainVector of dataInput.
                this->m_mainVector.push_back(newVType);
            }
            
        }
    }
    //allocating a vectorType array in the heap.
    vectorFeatures::vectorType* vectArr = new vectorFeatures::vectorType[this->getVecSize()];
    for (int i = 0; i < this->m_mainVector.size(); i++) {
        vectArr[i] = this->m_mainVector[i];
    }
    return vectArr;
}
/// @brief this function is the getter function for the mainVector size.
/// @return int - the size of mainVector
int dataInput::getVecSize() {
    return this->m_mainVector.size();
}

/// @brief this function is the getter function for the main vector.
/// @param vecArr - an array of distanceType.
/// @param k - the index to check until, from index 0 to index k (assumed to be non-negative).
/// @return vector<vectorType> - the main vector
std::vector<vectorFeatures::vectorType> dataInput::getMainVector() {
    return this->m_mainVector;
}

/// @brief this function is the getter function for the filename.
/// @return string - the filename
std::string dataInput::getFilename() {
    return this->m_filepath;
}
