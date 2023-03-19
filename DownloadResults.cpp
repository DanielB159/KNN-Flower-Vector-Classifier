#include "DownloadResults.h"

DownloadResults::DownloadResults(int clientSocket) {
    setDescription("download results");
    setDio(clientSocket);
};

/// @brief execute command
/// @param cliDetails details for execution
void DownloadResults::execute(CLIDetails* cli) {
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