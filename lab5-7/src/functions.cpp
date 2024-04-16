#include "functions.hpp"

#include <iostream>
#include <sstream>

std::string getAddres(size_t port) {
    return std::string("tcp://127.0.0.1:") + std::to_string(port);
}

// MESSAGE VALIDATOR --------------------------------------------------------------
void validateOperationType(Request& request, std::string& operationType) {
    if (operationType == "quit") {
        request.operationType = OperationType::QUIT;
    } else if (operationType == "create") {
        request.operationType = OperationType::CREATE;
    } else if (operationType == "exec") {
        request.operationType = OperationType::EXEC;
    } else if (operationType == "ping") {
        request.operationType = OperationType::PING;
    } else {
        // throw std::invalid_argument("Invalid operation");
        std::cout << "Error: Invalid operation" << std::endl;
    }
}
void validateId(Request& request, ssize_t id) {
    if (id < 0) {
        // throw std::invalid_argument("Invalid worker node id");
        std::cout << "Error: " << id << " : Invalid worker node id" << std::endl;
    }
    request.id = id;
}
void validateSubrequest(Request& request, std::string& timerSubrequest) {
    using std::invalid_argument;
    if (request.operationType == OperationType::PING && !timerSubrequest.empty()) {
        // throw invalid_argument("Ping can't accept third argument");
        std::cout << "Error: Ping can't accept third argument" << std::endl;
    } else if (request.operationType == OperationType::CREATE && timerSubrequest != "-1") {
        // throw invalid_argument("Create can push only in end");
        std::cout << "Error: Create can push only in end" << std::endl;
    } else if (request.operationType == OperationType::PING || request.operationType == OperationType::CREATE) {
        request.subrequest = TimerSubrequest::NOTHING;
    }

    if (request.operationType == OperationType::EXEC && timerSubrequest == "start") {
        request.subrequest = TimerSubrequest::START;
    } else if (request.operationType == OperationType::EXEC && timerSubrequest == "stop") {
        request.subrequest = TimerSubrequest::STOP;
    } else if (request.operationType == OperationType::EXEC && timerSubrequest == "time") {
        request.subrequest = TimerSubrequest::TIME;
    } else if (request.operationType == OperationType::EXEC) {
        // throw invalid_argument("Invalid subrequest");
        std::cout << "Error: Invalid subrequest" << std::endl;
    }
}

Request readRequest() {
    using std::cout, std::cin, std::endl, std::string, std::stringstream, std::getline, std::invalid_argument;

    string operation;
    Request request;

    string operationType;
    size_t id = -1;
    string timerSubrequest;

    getline(cin, operation);

    stringstream ss(operation);

    ss >> operationType;
    validateOperationType(request, operationType);

    if (request.operationType == OperationType::QUIT) {
        if (ss >> operationType) {
            // throw invalid_argument("Invalid input");
            std::cout << "Error: Invalid input" << std::endl;
        }
        request.id = -1;
        request.subrequest = TimerSubrequest::NOTHING;
        return request;
    }

    ss >> id;
    validateId(request, id);
    ss >> timerSubrequest;
    validateSubrequest(request, timerSubrequest);

    // Пришла еще одно строка
    if ((ss >> timerSubrequest)) {
        // throw invalid_argument("Invalid input");
        std::cout << "Error: Invalid input" << std::endl;
    }

    return request;
}

// MESSAGE VALIDATOR --------------------------------------------------------------

// MESSAGE SENDER/PULLER --------------------------------------------------------------

void pushRequest(zmq::socket_t& socket, Request& request) {
    zmq::message_t message(&request, sizeof(Request));

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-result"
    socket.send(message, zmq::send_flags::none);
    #pragma GCC diagnostic pop
}

void pushReply(zmq::socket_t& socket, Reply& reply) {
    zmq::message_t message(&reply, sizeof(Reply));

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-result"
    socket.send(message, zmq::send_flags::none);
    #pragma GCC diagnostic pop
}

Reply pullReply(zmq::socket_t& socket) {
    zmq::message_t message(sizeof(Reply));

    auto reply = socket.recv(message, zmq::recv_flags::dontwait);
    if (reply.has_value()) {
        return *message.data<Reply>();
    }
    return Reply {OperationType::NOTHING, -1, ErrorTypes::NO_ERRORS, -1, TimerSubrequest::NOTHING};
}

Request pullRequest(zmq::socket_t& socket) {
    zmq::message_t message(sizeof(Request));

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-result"
    socket.recv(message, zmq::recv_flags::none);
    #pragma GCC diagnostic pop

    return *message.data<Request>();
}
// MESSAGE SENDER/PULLER --------------------------------------------------------------

// Валидация порта и nodeId
std::pair<pid_t, std::pair<bool, size_t>> createNewNode(std::unordered_map<ssize_t, std::pair<pid_t, size_t>>& nodes, ssize_t nodeId, size_t& currentPort) {
    
    bool replace = false;

    if (nodes.count(nodeId) > 0) {
        std::cout << "Error: Already exists" << std::endl;
        return std::pair<pid_t, std::pair<bool, size_t>> {PID_FOR_ALREADY_EXIST_NODE, {replace, 0}};
    }
    

    size_t currentPortCopy = currentPort;
    for (auto& node : nodes) {
        if (!isProcessExists(node.second.first) && node.second.second != ALREADY_REPLACED) {
            currentPortCopy = node.second.second;
            node.second.second = ALREADY_REPLACED;
            replace = true;
            break;
        }
    }

    pid_t processId = fork();
    if (processId == -1) {
        perror("Fork error (server)");
        exit(1);
    }

    // Нормальный путь сделать
    if (processId == 0) {
        char srtNodeId[MAX_NODE_ID_LENGTH + 1];
        char strCurrentPort[MAX_PORT_LENGTH + 1];
        sprintf(srtNodeId, "%zd", nodeId);
        sprintf(strCurrentPort, "%zu", currentPortCopy);
        if (execl("./build/worker_exe", "./build/worker_exe", srtNodeId, strCurrentPort, NULL) == -1) {
            perror("Exec error (server)");
            exit(1);
        }
    }

    std::cout << "Ok: " << processId << std::endl;
    return std::pair<pid_t, std::pair<bool, size_t>> {processId, std::pair<bool, size_t>{replace, currentPortCopy}};
}

bool isNodeAvaliable(std::map<size_t, std::vector<pid_t>>& nodeByPort, pid_t processId) {
    for (auto& portInfo : nodeByPort) {
        bool wasGoodWorker = false;
        for (pid_t id : nodeByPort[portInfo.first]) {
            if (id == processId) {
                return isProcessExists(processId);
            } else if (!wasGoodWorker) {
                wasGoodWorker = isProcessExists(id);
            }
        }
        if (!wasGoodWorker) {
            return false;
        }
    }
    return true;
}

void updateNodeMap(std::unordered_map<ssize_t, std::pair<pid_t, size_t>>& nodes, std::map<size_t, std::vector<pid_t>>& nodeByPort, size_t& currentPort, Request& request) {
    std::pair<pid_t, std::pair<bool, size_t>> newWorkerInfo = createNewNode(nodes, request.id, currentPort);
    if (newWorkerInfo.first != PID_FOR_ALREADY_EXIST_NODE) {
        nodes[request.id] = std::pair<pid_t, size_t>{newWorkerInfo.first, currentPort};

        if (nodeByPort.count(newWorkerInfo.second.second) == 0) {
            std::vector<pid_t> vct;
            vct.push_back(newWorkerInfo.first);
            nodeByPort[newWorkerInfo.second.second] = vct;
        } else {
            nodeByPort[newWorkerInfo.second.second].push_back(newWorkerInfo.first);
        }
        if (!newWorkerInfo.second.first) {
            currentPort += 2;
        }
    }
}

void killWorkers(std::unordered_map<ssize_t, std::pair<pid_t, size_t>>& nodes) {
    for (auto worker : nodes) {
        kill(worker.second.first, SIGTERM);
    }
}

bool isProcessExists(pid_t pid) {
    int errnoBefore = errno;
    kill(pid, 0);
    int newErrno = errno;
    errno = errnoBefore;
    return newErrno != ESRCH;
}