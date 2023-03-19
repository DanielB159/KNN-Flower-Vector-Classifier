#include "UploadFile.h"


UploadFile::UploadFile(int clientSocket) {
    setDescription("upload an unclassified csv data file");
    setDio(clientSocket);
}

/// @brief Get vectors and parse them.
/// @param areClassified classified vectors or not
/// @param cliDetails details for classifiaction
void UploadFile::readVectors(bool areClassified, CLIDetails* cliDetails) {
    //if there were vectors before, delete them.
    if (cliDetails->unclassifiedVectors.size() != 0) {
        cliDetails->classifiedVectors.clear();
        cliDetails->unclassifiedVectors.clear();
    }
    //vector file will be the whole file, currentRead is the current packet sent from the client.
    std::string vectorFile = "", currentRead;
    while (currentRead = getFromClient(), currentRead != "endOfFile") {
        vectorFile.append(currentRead);
        sendToClient("accept_package");
    }

    std::string line;
    //stream will be a stream that reads all of the vectors in the file.
    std::stringstream vstream(vectorFile);
    while(getline(vstream, line, '\n')) {
        //checking for problematic character with ascii encoding 13 that is the last digit at each line
        if (line.find(13) != std::string::npos) {
            int toRemove = line.find(13);
            line.erase(toRemove, 1);
        }
        //will be a string representing the vector of this row
        std::string vectorString = "";
        //will be a stream to decode this vector
        std::stringstream stream(line);
        //will represent each number in the vector.
        std::string numberInString;
        //will vbe the type of flower that the vector is describing.
        std::string flowerType = "";
        bool firstNum = true;
        //making sure that all of the numbers that are stored in the vector are valid.
        while(getline(stream, numberInString, ',')) {
            if (numberInString[0] == '.') { //sometimes 0.12 is read as .12.
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
                    double finalNum = number*pow(10, exp);
                    //converting finalNum back to string to continue checks.
                    std::ostringstream s;
                    s << finalNum;
                    numberInString = s.str();
                } else {
                    //if the vector has invalid characters before or after exponent, dont read this line.
                    continue;                    
                }
            }
            //assert that the number in the vector is correct
            if (vectFunc::assertNumber(numberInString)){
                if (firstNum) {
                    vectorString += numberInString;
                    firstNum = false;
                } else {
                    vectorString += " " + numberInString;
                }                
            } else {
                //got here only if the vectors are classified, the assertion failed because
                //numberInString is the type of the vector.
                flowerType = numberInString;                   
            }
        }
        //add the vector and its Type to cevtor of vectors
        std::vector<double> v1;
        if (vectFunc::assertVector(vectorString)){
            v1 = vectFunc::stringToVec(vectorString);
            vectorFeatures::vectorType newVType = {v1, flowerType};
            //pushing the newly made vector to the mainVector in CLI.
            if (areClassified) {
                cliDetails->classifiedVectors.push_back(newVType);
            } else {
                cliDetails->unclassifiedVectors.push_back(newVType);
            }
            
        }
    }
}

/// @brief execute command
/// @param cliDetails details for execution
void UploadFile::execute(CLIDetails* cliDetails) {
    //send the client the message to import the train vectors
    sendToClient(ENTER_ADRESS_OF_TRAIN_VECTORS_MESSAGE);

    //read the classified vectors
    readVectors(true, cliDetails);

    //send the client the message to import the train vectors
    sendToClient(ENTER_ADRESS_OF_TEST_VECTORS_MESSAGE);

    //read the unclassified vectors
    readVectors(false, cliDetails);
}
