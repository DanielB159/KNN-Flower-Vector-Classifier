#include "CLI.h"

/// @brief Constructor for CLI.
/// @param clientSocket identifier for client socket.
CLI::CLI(int clientSocket) {
        //setting the default cli details for this cli
        
        this->details.distanceMetricName = "AUC";
        this->details.distanceMetricPtr = new EuclidianDist();
        this->details.k = 5;
        this->details.source = clientSocket;

        //make 5 commands, pointers to commands!!!
        this->commands[0] = new UploadFile(clientSocket);
        this->commands[1] = new AlgorithmSettings(clientSocket);
        this->commands[2] = new ClassifyData(clientSocket);
        this->commands[3] = new DisplayResults(clientSocket);
        this->commands[4] = new DownloadResults(clientSocket);
        for (int i = 0; i < 5; i++) {
            this->menu += std::to_string(i+1) + ". " + commands[i]->getDescription() + "\n";
        }
        this->menu.append("8. exit");
}

/// @brief Destructor for CLI.
CLI::~CLI() {
    delete(details.distanceMetricPtr);
    //free commands
    for (int i = 0; i < 5; i++) {
        delete(this->commands[i]);
    }
}

//this is the set function for the CLI source
void CLI::setSource(int source) {
    this->details.source = source;
}


//this is the get function for the source of the CLI
int CLI::getSource() {
    return this->details.source;
}

//this function adds the given vectorType to the CLI details
void CLI::addVectorType(vectorFeatures::vectorType vec) {
    this->details.classifiedVectors.push_back(vec);
}

/// @brief Send error message to client.
/// @param client_sock identifier for client's socket.
void CLI::send_error(int client_sock) { 
            // error reading message, send "invalid input"
            int sent_bytes = send(client_sock, INVALID_INPUT_STRING, INVALID_INPUT_MSG_LEN, 0);
            if (sent_bytes < 0) {
                perror("error sending to client");
            }
        }

/// @brief Start back and forth communication with client.
void CLI::start() {
    int clientSocket = getSource();
    //while the client is connected, send the client the menu and recieve choices
    while(true) {
        //first, send the client the menu
        int sent_bytes = send(clientSocket, this->menu.c_str(), this->menu.length() + 1, 0);
        if (sent_bytes < 0) {
            perror("error sending to client");
        }

        //allocate buffer for incoming messages, true if there is an error in the message
        char buffer[4096];
        int expected_data_len = sizeof(buffer);
        //receive message
        int read_bytes = recv(getSource(), buffer, expected_data_len, 0);
        if (read_bytes == 0) {
            // connection is closed 
            //cout << "connection is closed" << endl;
            return;
        } else if (read_bytes < 0) {
            // error reading message, send "invalid input"
            send_error(clientSocket);
            return;
        } else {
            //expecting to receieve a message for which option was chosen
            int option = 0;
            try {
                std::string buffer_string(buffer);
                option = stoi(buffer_string.substr(0, 1));
            } catch (...) {
                //continue with a new request
                continue;
            }
            if (option <= 0 || (option >= 7 && option != 8)) {
                //invalid option
                send_error(clientSocket);
                continue;   //should contine to a new request
                //break;
            } else {
                commands[option - 1]->execute(&this->details);   
                //sleeping for half a second to make sure that the client is ready to recieve the menu.
                sleep(1);          
            }
        }
    }
}