#include <iostream>
#include <thread>
#include <chrono>

#include "definitions.hpp"
#include "functions.hpp"


/*
    TODO
    - Сейчас у меня при выходе не дожидается ответа
    - Добавить отчистку ресурсов после ошибки
    - Валидация порта и nodeId (createNewNode)
    - Нормальный путь сделать (createNewNode)
*/

zmq::context_t context1;
zmq::context_t context2;

zmq::socket_t pullReplySocket(context1, zmq::socket_type::pull);
zmq::socket_t pushRequestSocket(context2, zmq::socket_type::push);


void getReply() {
    while (true) {
        Reply data = pullReply(pullReplySocket);
        std::cout << "Server: Answer: " << data.result << std::endl;
    }
}

int main() {
    /*
        Key: nodeId
        Value: <ProcessId, currentPort>
    */ 
    std::unordered_map<ssize_t, std::pair<pid_t, size_t>> nodes;

    size_t currentPort = MIN_DYNAMIC_PORT + 20;
    Request request;
    bool waitForNewRequest = true;

    pullReplySocket.bind(getAddres(currentPort + 0));
    pushRequestSocket.bind(getAddres(currentPort + 1));

    std::cout << "Сервер отправляет в " << currentPort + 1 << std::endl;
    std::cout << "Сервер принимает из " << currentPort << std::endl;

    currentPort += 2;

    std::thread replyThread(getReply);

    while (waitForNewRequest) {
        request = readRequest();

        switch (request.operationType) {
        case OperationType::QUIT:
            waitForNewRequest = false;
            break;
        case OperationType::EXEC:
            pushRequest(pushRequestSocket, request);
            break;
        case OperationType::CREATE:
            // мьютекс навесить
            updateNodeMap(nodes, currentPort, request);
            break;
        case OperationType::PING:
            // pushMessage(pushRequestSocket, request);
            break;
        }
    }

    replyThread.detach();
    pullReplySocket.close();
    pushRequestSocket.close();
    killWorkers(nodes);
    
    return 0;
}