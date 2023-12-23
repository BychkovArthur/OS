#include <iostream>

#include "definitions.hpp"
#include "functions.hpp"


// Добавить отчистку ресурсов после ошибки
zmq::context_t context1;
zmq::context_t context2;

zmq::socket_t pullReplySocket(context1, zmq::socket_type::pull);
zmq::socket_t pushRequestSocket(context2, zmq::socket_type::push);

int main() {
    /*
        Key: nodeId
        Value: <ProcessId, currentPort>
    */ 
    std::unordered_map<ssize_t, std::pair<pid_t, size_t>> nodes;

    size_t currentPort = MIN_DYNAMIC_PORT + 20;
    Command command;
    bool waitForNewCommand = true;

    pullReplySocket.bind(getAddres(currentPort + 0));
    pushRequestSocket.bind(getAddres(currentPort + 1));

    std::cout << "Сервер отправляет в " << currentPort + 1 << std::endl;
    std::cout << "Сервер принимает из " << currentPort << std::endl;

    currentPort += 2;

    while (waitForNewCommand) {
        command = readCommand();

        switch (command.operationType) {
        case OperationType::QUIT:
            waitForNewCommand = false;
            break;
        case OperationType::EXEC:
            pushMessage(pushRequestSocket, command);
            break;
        case OperationType::CREATE:
            // мьютекс навесить
            updateNodeMap(nodes, currentPort, command);
            break;
        case OperationType::PING:
            pushMessage(pushRequestSocket, command);
            break;
        }
    }

    pullReplySocket.close();
    pushRequestSocket.close();
    killWorkers(nodes);
    
    return 0;
}