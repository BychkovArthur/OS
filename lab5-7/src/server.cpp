#include <iostream>
#include <map>

#include "definitions.hpp"
#include "functions.hpp"

// Валидация порта и nodeId

pid_t createNewNode(ssize_t nodeId, size_t currentPort) {
    pid_t processId = fork();
    if (processId == -1) {
        perror("fork");
        exit(1);
    }

    // Нормальный путь сделать
    if (processId == 0) {
        char srtNodeId[MAX_NODE_ID_LENGTH + 1];
        char strCurrentPort[MAX_PORT_LENGTH + 1];
        sprintf(srtNodeId, "%zd", nodeId);
        sprintf(strCurrentPort, "%zu", currentPort);
        if (execl("./build/worker_exe", "./build/worker_exe", srtNodeId, strCurrentPort, NULL) == -1) {
            perror("Exec error (server)");
            exit(1);
        }
    }
    return processId;
}

void updateNodeMap(std::unordered_map<ssize_t, std::pair<pid_t, size_t>>& nodes, size_t& currentPort, Command& command) {
    pid_t newWorkerPid = createNewNode(command.id, currentPort);
    nodes[command.id] = std::pair<pid_t, size_t>{newWorkerPid, currentPort};
    std::cout << "MAP UPDATIND KEY = " << command.id << " pid = " << newWorkerPid << " port = " << currentPort << std::endl;
    currentPort += 2;
}

void killWorkers(std::unordered_map<ssize_t, std::pair<pid_t, size_t>>& nodes) {
    for (auto worker : nodes) {
        std::cout << "Killing node " << worker.first << std::endl;
        kill(worker.second.first, SIGTERM);
    }
}

int main() {
    /*
        Key: nodeId
        Value: <ProcessId, currentPort>
    */ 
    std::unordered_map<ssize_t, std::pair<pid_t, size_t>> nodes;

    size_t currentPort = MIN_DYNAMIC_PORT + 160;
    Command command;
    bool waitForNewCommand = true;

    zmq::context_t context1;
    zmq::context_t context2;

    zmq::socket_t pullReplySocket(context1, zmq::socket_type::pull);
    zmq::socket_t pushRequestSocket(context2, zmq::socket_type::push);
    pullReplySocket.bind(getAddres(currentPort + 0));
    pushRequestSocket.bind(getAddres(currentPort + 1));

    currentPort += 2;

    while (waitForNewCommand) {
        command = readCommand();

        switch (command.operationType) {
        case OperationType::QUIT:
            waitForNewCommand = false;
            break;
        case OperationType::EXEC:
            std::cout << "Exec..." << std::endl;
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