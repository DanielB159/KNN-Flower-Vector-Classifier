#include "DisplayResults.h"

DisplayResults::DisplayResults(int clientSocket) {
    setDescription("display results");
    setDio(clientSocket);
};

/// @brief execute command
/// @param cliDetails details for execution
void DisplayResults::execute(CLIDetails* cli) {
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