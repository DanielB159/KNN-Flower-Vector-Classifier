#include "Command.h"
Command::Command() : dio(0){}
Command::~Command() {}
void Command::setDio(int clientSocket) {
    SocketIO sio(clientSocket);
    this->dio = sio;
}
void Command::setDescription(std::string s) {
    this->description = s;
}
std::string Command::getDescription() {
    return this->description;
}
//send a message to the client
void Command::sendToClient(std::string message) {
    dio.write(message);
}
//get a message from the client
std::string Command::getFromClient() {
    return dio.read();
}