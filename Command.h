#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cmath>

#include "DefaultIO.h"
#include "stringToVec.h"
#include "typeDistances.h"
#include "slct.h"
#include "vectorType.h"


/// @brief information shared between commands.
struct CLIDetails {
    int k;
    std::string distanceMetricName;
    Distance* distanceMetricPtr;
    std::vector<vectorFeatures::vectorType> classifiedVectors;
    std::vector<vectorFeatures::vectorType> unclassifiedVectors;    //NULL pre classification
    int source;
};

/// @brief Abstract command class.
class Command {
    private:
        std::string description;
        SocketIO dio;
    
    public:
        Command() : dio(0){}
        ~Command() {}

        void setDio(int clientSocket) {
            SocketIO sio(clientSocket);
            this->dio = sio;
        }
        void setDescription(std::string s) {
            this->description = s;
        }
        std::string getDescription() {
            return this->description;
        }
        //send a message to the client
        void sendToClient(std::string message) {
            dio.write(message);
        }
        //get a message from the client
        std::string getFromClient() {
            return dio.read();
        }
        virtual void execute(CLIDetails* cli) = 0;
};

/// @brief Command 1, upload files for classification.
class UploadFile : public Command {
    private:
        const std::string ENTER_ADRESS_OF_TRAIN_VECTORS_MESSAGE = "Please upload your local train csv file.";
        const std::string ENTER_ADRESS_OF_TEST_VECTORS_MESSAGE = "Please upload your local test csv file.";
    public:
        UploadFile(int clientSocket) {
            setDescription("upload an unclassified csv data file");
            setDio(clientSocket);
        }

        /// @brief Get vectors and parse them.
        /// @param areClassified classified vectors or not
        /// @param cliDetails details for classifiaction
        void readVectors(bool areClassified, CLIDetails* cliDetails) {
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
        void execute(CLIDetails* cliDetails) {
            //send the client the message to import the train vectors
            sendToClient(ENTER_ADRESS_OF_TRAIN_VECTORS_MESSAGE);

            //read the classified vectors
            readVectors(true, cliDetails);

            //send the client the message to import the train vectors
            sendToClient(ENTER_ADRESS_OF_TEST_VECTORS_MESSAGE);

            //read the unclassified vectors
            readVectors(false, cliDetails);
        }
    };

/// @brief Command 2, set settings for classifiaction.
class AlgorithmSettings : public Command {
    private:
        const std::string INVALID_INPUT_METRIC = "invalid value for metric";
        const std::string INVALID_INPUT_K = "invalid value for K";
    public:
        AlgorithmSettings(int clientSocket) {
            setDescription("algorithm settings");
            setDio(clientSocket);
        };

        /// @brief execute command
        /// @param cliDetails details for execution
        void execute(CLIDetails* cli) {
            //send to client the current KNN parameters as they are set in the CLIDetails
            std::string currentParams = "The current KNN parameters are: K = " + std::to_string(cli->k) + 
                ", distance metric = " + cli->distanceMetricName;
            sendToClient(currentParams);

            bool validK = true, validMetric = true;

            //get answer from client on how to change them and if to change them
            std::string clientAnswer = getFromClient();
            //if the user does not want to change tham - the sentBack message will be "keepCurrent"
            if (clientAnswer == "keepCurrent") {
                return;
            } else {
                std::string answer = "";
                //otherwise, parse the string and set new K and DistanceMetric
                int spacePos = clientAnswer.find(" ");
                int newKValue;
                //check if the distance metric and k are valid
                if (spacePos == string::npos) {
                    //if the clientAnswer does not contain any " ", it is not valid.
                    answer += INVALID_INPUT_METRIC + "\n" + INVALID_INPUT_K;
                    validK = false;
                    validMetric = false;
                } else {
                    //if does contain " ", parse it and check for new parameters
                    std::string newK = clientAnswer.substr(0, spacePos);
                    std::string newDist = clientAnswer.substr(spacePos + 1, clientAnswer.length() - spacePos + 1);
                    try {
                        newKValue = stoi(newK);
                        if (newKValue <= 0) {
                            validK = false;
                        }
                    } catch (...) {
                        validK = false;
                    }
                    //filter by all of the options of newDist
                    if (newDist == "AUC") {
                        if (validK) {
                            delete(cli->distanceMetricPtr);
                            cli->distanceMetricName = "AUC";
                            cli->distanceMetricPtr = new EuclidianDist();
                        }
                    } else if (newDist == "MAN") {
                        if (validK) {
                            delete(cli->distanceMetricPtr);
                            cli->distanceMetricName = "MAN";
                            cli ->distanceMetricPtr = new ManhattanDist();
                        }
                    } else if (newDist == "MIN") {
                        if (validK) {
                            delete(cli->distanceMetricPtr);
                            cli->distanceMetricName = "MIN";
                            cli ->distanceMetricPtr = new MinkowskiDist();
                        }
                    } else if (newDist == "CHB") {
                        if (validK) {
                            delete(cli->distanceMetricPtr);
                            cli->distanceMetricName = "CHB";
                            cli ->distanceMetricPtr = new ChebyshevDist();
                        }
                    } else if (newDist == "CAN") {
                        if (validK) {
                            delete(cli->distanceMetricPtr);
                            cli->distanceMetricName = "CAN";
                            cli ->distanceMetricPtr = new CanberraDist();
                        }
                    } else {
                        validMetric = false;
                    }


                    if (!validMetric && validK) {
                        answer += INVALID_INPUT_METRIC;
                    }
                    else if (!validK && validMetric) {
                        answer += INVALID_INPUT_K;
                    } else if (!validMetric && !validK){
                        answer += INVALID_INPUT_METRIC + "\n" + INVALID_INPUT_K;
                    } else {
                        answer += "valid";
                        //set the new k to be newKValue
                        cli->k = newKValue;
                    }
                }
                sendToClient(answer);
                sleep(0.5);
            }
        }
};

namespace slct {
    //creating a Comparator for dataInput::distanceType
    template <>
    class Comparator<vectorFeatures::distanceType> {
        public:
            bool operator()(const vectorFeatures::distanceType& obj1, const vectorFeatures::distanceType& obj2) {
                return obj1.distance < obj2.distance;
            }
    };
}

/// @brief classify data in CLIDetails
class ClassifyData : public Command {
    public:
        ClassifyData(int clientSocket) {
            setDescription("classify data");
            setDio(clientSocket);
        };

        /// @brief execute command
        /// @param cliDetails details for execution
        void execute(CLIDetails* cli) {
            if (cli->classifiedVectors.size() == 0) {
                sendToClient("please upload data.");
                return;
            }
            //create comparator
            slct::Comparator<vectorFeatures::distanceType> comparator;
            //classify unclassified vectors
            for (int i = 0; i < cli->unclassifiedVectors.size(); i++) {
                vector<vectorFeatures::distanceType> classificationDistances;
                for (int j = 0; j < cli->classifiedVectors.size(); j++) {
                    //get distances for all classified vectors and this current one.
                    vectorFeatures::distanceType tempDist = {(*(cli->distanceMetricPtr))(cli->unclassifiedVectors[i].vect, cli->classifiedVectors[j].vect), cli->classifiedVectors[j].type};
                    classificationDistances.push_back(tempDist);
                }

                //select and partition k closest elements
                vectorFeatures::distanceType* distanceArray = classificationDistances.data();
                slct::select(distanceArray, classificationDistances.size(), comparator, cli->k);
                //count occurances of a classification
                cli->unclassifiedVectors[i].type = classificationOfVec::getVecType(distanceArray, cli->k);
            }
            sendToClient("classifying data complete");
        }
};

/// @brief Send results of classification
class DisplayResults : public Command {
    public:
        DisplayResults(int clientSocket) {
            setDescription("display results");
            setDio(clientSocket);
        };

        /// @brief execute command
        /// @param cliDetails details for execution
        void execute(CLIDetails* cli) {
            if (cli->classifiedVectors.size() == 0) {
                sendToClient("please upload data.");
                return;
            }
            //send classified information
            string output = "";
            int line = 0;  //for line number
            for (int i = 0; i < cli->unclassifiedVectors.size(); i++) {
                line++;
                if (cli->unclassifiedVectors[i].type == "") {
                    sendToClient("please classify the data.");
                    return;
                }
                output.append(std::to_string(line) + " " + cli->unclassifiedVectors[i].type + "\n");
            }
            output.append("Done.");

            //if message is too long, split it up to packets
            while (output.length() >= 4080) {
                sendToClient(output.substr(0, 4080));
                output = output.substr(4080);
                if (getFromClient() != "accept_read") {
                    cout << "Error sending message." << endl;
                    return;
                }
            }
            sendToClient(output);
            if (getFromClient() != "accept_read") {
                return;
            }
            //send end of message
            sendToClient("endOfFile");
        }
};

/// @brief command 5, send results to be saved into a file.
class DownloadResults : public Command {
    public:
        DownloadResults(int clientSocket) {
            setDescription("download results");
            setDio(clientSocket);
        };

        /// @brief execute command
        /// @param cliDetails details for execution
        void execute(CLIDetails* cli) {
            //assert that there is data and it is classified
            if (cli->classifiedVectors.size() == 0) {
                sendToClient("please upload data.");
                return;
            }
            else if (cli->unclassifiedVectors.size() != 0 && cli->unclassifiedVectors[0].type == "") {
                sendToClient("please classify the data.");
                return;
            }
            else {
                sendToClient("go ahead.");
            }

            //send classified information
            string output = "";
            int line = 0;  //for line number
            for (int i = 0; i < cli->unclassifiedVectors.size(); i++) {
                line++;
                output.append(std::to_string(line) + " " + cli->unclassifiedVectors[i].type + "\n");
            }
            output.append("Done.");

            sleep(1);

            //if message is too long, split it up to packets
            while (output.length() >= 4080) {
                sendToClient(output.substr(0, 4080));
                output = output.substr(4080);
                if (getFromClient() != "accept_read") {
                    cout << "Error sending message." << endl;
                    return;
                }
            }
            sendToClient(output);
            if (getFromClient() != "accept_read") {
                return;
            }
            //send end of message
            sendToClient("endOfFile");
        }
};

#endif