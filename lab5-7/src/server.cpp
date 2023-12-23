#include <iostream>

#include "definitions.hpp"
#include "functions.hpp"

int main() {
    Command command;
    // bool waitForNewCommand = true;

    zmq::context_t context1;
    zmq::context_t context2;

    zmq::socket_t pullReplySocket(context1, zmq::socket_type::pull);
    zmq::socket_t pushRequestSocket(context2, zmq::socket_type::push);
    pullReplySocket.bind(getAddres(MIN_DYNAMIC_PORT + 0));
    pushRequestSocket.bind(getAddres(MIN_DYNAMIC_PORT + 1));

    // while (waitForNewCommand) {
        command = readCommand();

        switch (command.operationType) {
        case OperationType::QUIT:
            // waitForNewCommand = false;
            break;
        case OperationType::EXEC:
            std::cout << "Exec..." << std::endl;
            pushMessage(pushRequestSocket, command);
            break;
        case OperationType::CREATE:
            pushMessage(pushRequestSocket, command);
            break;
        case OperationType::PING:
            pushMessage(pushRequestSocket, command);
            break;
        }
    // }
    
    return 0;
}