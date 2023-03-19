#include <string>
#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include "VectorRelated/stringToVec.h"
#include "VectorRelated/typeDistances.h"
#include "networkFunctions.h"
using namespace std;

#include <thread>

/// @brief send message to server
/// @param sock socket identifier
/// @param inputString message to send
void sendMessage(int sock, string inputString) {
    //sending the data to the server
    int sent_bytes = send(sock, inputString.c_str(), inputString.length() + 1, 0);
    if (sent_bytes < 0) {
        cout << "error while sending to the server!" << endl;
    }
}

/// @brief Receive message from server.
/// @param sock socket identifier to receive from
/// @return string of the message receieved
std::string receieveMessage(int sock) {
    char buffer[4096];
    int expected_data_size = sizeof(buffer);

    //recieving the message from the server
    // "please upload train csv file"
    int read_bytes = recv(sock, buffer, expected_data_size, 0);
    //validating that data was recieved
    if (read_bytes == 0) {
        cout << "the server is closed!" << endl;
        //close the client
        close(sock);
        exit(0);
    }
    if (read_bytes < 0) {
        cout << "error while recieving from the server!" << endl;
    }

    std::string output(buffer);
    return output;
}


//receieve the entire long classified string from server
std::string getClassifiedData(int sock) {
    //read the long string from the server
    std::string output = "", currentRead;
    currentRead = receieveMessage(sock);

    if (currentRead == "please upload data.") {
        return "please upload the data.";
    } else if (currentRead == "please classify the data.") {
        return "please classify the data.";
    }

    //get the file while sending confirmation each package.
    while (currentRead != "endOfFile") {
        output.append(currentRead);
        sendMessage(sock, "accept_read");
        currentRead = receieveMessage(sock);
    }

    return output;
}

/// @brief save results in a file
/// @param filepath path for saving the file (overwrites!!!)
/// @param data data to save
void save_result(std::string filepath, std::string data) {
    //TODO:: SAVE OUTPUT IN FILE!!!
    //cout << file_path << endl;
    //cout << data << endl;

    std::ofstream outfile;
    outfile.open(filepath);
    outfile << data;
    outfile.close();
}

//this function checks weather a filepath is valid or not.
bool isPathValid(std::string filepath) {
    std::ifstream file(filepath);
    return file.good();
}

//gets request string from server, asks user for path, and then reads and sends file
void sendFile(int sock) {
    char buffer[4096];
    int expected_data_size = sizeof(buffer);

    //recieving the message from the server
    int read_bytes = recv(sock, buffer, expected_data_size, 0);
    //validating that data was recieved
    if (read_bytes == 0) {
        cout << "the server is closed!" << endl;
        //close the client
        close(sock);
        exit(0);
    }
    if (read_bytes < 0) {
        cout << "error while recieving from the server!" << endl;
        return;
    }
    //and print it
    string buffer_string(buffer);
    buffer_string.substr(0, read_bytes - 1);
    cout << buffer_string << endl;
    std::string filePath;
    //get input filepath
    std::getline(std::cin, filePath);
    //make sure that a path is valid
    while(!isPathValid(filePath)) {
        std::cout << "bad file path. please try again." << std::endl;
        std::getline(std::cin, filePath);
    }
    std::fstream file(filePath, std::ios::in);   //start reading from file
    std::string line;
    //if did not fild the file
    //reading from the file
    if(file.is_open()){
        std::string msg_block = ""; 
        //while still getting rows of vectors
        while(getline(file, line)){
            if (msg_block.length() + line.length() >= 4080) { //based on buffer size
                //send current block and reset block
                sendMessage(sock, msg_block);  //for '\0'
                msg_block = line + "\n";
                if (receieveMessage(sock) != "accept_package") {
                    cout << "Error sending file." << endl;
                    return;
                }   
            } else {
                //append to block, differentiating classified and unclassified files
                msg_block.append(line + "\n");
            }
        }
        //send last line
        sendMessage(sock, msg_block);  //for '\0'
        if (receieveMessage(sock) != "accept_package") {
            cout << "Error sending file." << endl;
            return;
        }
        sendMessage(sock, "endOfFile");
    }
    file.close();
}

/// @brief main function for the client
/// @param argc num of arguments
/// @param argv arguments
/// @return exit code
int main(int argc, char* argv[]) {
    //first, check if there are three arguments,
    if (argc != 3) {
        cout << "invalid input" << endl;
        return 0;
    }
    
    //now, configuring the server ip_adress and port
    const char* ip_adress = argv[1];
    string port = argv[2];
    int portCheck;

    //now, checking if the port is correct
    try {
        portCheck = stoi(port);
    }
    catch (...) {
        //conversion to integet was unsuccesfull. cancelling.
        cout << "invalid input" << endl;
        return 0;
    }
    const int port_no = portCheck;
    //checking for valid port number. The valud port number ranges from 0 to 65535
    if (port_no < 0 || port_no > 65535) {
        cout << "invalid input" << endl;
        return 0;
    }
    int sock = networkFunctions::createTCPclientSocket(ip_adress, port_no);
    if (sock < 0) {
        return 0;
    }
    //connection established
    string inputString = "";
    while (inputString != "8") {        
        char buffer[4096];
        int expected_data_size = sizeof(buffer);

        //GET MENU
        //recieving the answer from the server
        int read_bytes = recv(sock, buffer, expected_data_size, 0);
        //validating that data was recieved
        if (read_bytes == 0) {
            cout << "the server is closed!" << endl;
            //close the client
            close(sock);
            return 0;
        }
        if (read_bytes < 0) {
            cout << "error while recieving from the server!" << endl;
        }

        string buffer_string(buffer);
        buffer_string.substr(0, read_bytes - 1);
        cout << buffer_string << endl;

        //SEND OPTION
        //get input from user
        std::getline(std::cin, inputString);

        
        int option = 0;
        try {
            option = stoi(inputString.substr(0, 2));
        } catch (...) {
            cout << "invalid input" << endl;
            continue;
        }
        std::string serverAnswer = "";
        switch (option) {
            case 8:
                //stop client
                inputString = "8";
                break;
            case 1:
            {
                sendMessage(sock, "1");
                
                //classified data
                sendFile(sock);

                //unclassified data
                sendFile(sock);
                break;
            }
            case 2:
            {
                //send the server that user has chosen option number 2
                sendMessage(sock, "2");
                //sent the user the current KNN parameters, returned from server
                cout << receieveMessage(sock) << endl;
                //get the new parameters from the user
                std::getline(std::cin, inputString);
                //if the user has pressed enter, meaning inputString is empty,
                //notify the server to keep current parameters
                if (inputString == "") {
                    sendMessage(sock, "keepCurrent");
                    break;
                } else {
                    //otherwise, send the server the input of the new parameters
                    sendMessage(sock, inputString);
                }
                serverAnswer = receieveMessage(sock);
                if (serverAnswer == "valid") {
                    break;
                } else {
                    std::cout << serverAnswer << std::endl;
                    std::cout << "the format is <K> <Distance metric>" << std::endl;
                    break;
                }
            }
            case 3:
            {
                sendMessage(sock, "3");

                std::string currentRead = receieveMessage(sock);
                if (currentRead == "please upload data.") {
                    cout << "please upload the data." << endl;
                    break;
                }
                
                cout << currentRead << endl;
                break;
            }
            case 4:
            {
                sendMessage(sock, "4");

                std::string output = getClassifiedData(sock);

                //print it
                cout << output << endl;

                break;
            }
            case 5:
            {
                sendMessage(sock, "5");

                //get resposne asserting if the request is valid
                std::string assertion = receieveMessage(sock);
                if (assertion == "please upload data.") {
                    cout << "please upload data." << endl;
                    break;
                } else if (assertion == "please classify the data.") {
                    cout << "please classify the data." << endl;
                    break;
                } else if (assertion != "go ahead.") {
                    cout << "Warning: odd assertion." << endl;
                }

                //get classified data
                std::string classifiedData = getClassifiedData(sock);
                
                //remove the done.
                classifiedData = classifiedData.substr(0, classifiedData.length() - 6);

                cout << "please enter filepath for download." << endl;
                //get input filepath
                std::getline(std::cin, inputString);

                //otherwise, get the info in a new thread
                std::thread command5Thread(save_result, inputString, classifiedData);
                command5Thread.detach();
                break;
            }
            default:
                //sent the server that the menu choice was invalid
                cout << "invalid menu choice" << endl;
                sendMessage(sock, "invalid");
        }
        //send to server
    }
    //user has exited. closing the socket and exiting.
    close(sock);
    return 0;
}