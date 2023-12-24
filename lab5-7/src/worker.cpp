#include <iostream>
#include <zmq_addon.hpp>
#include <zmq.hpp>
#include <thread>
#include <chrono>
#include <sys/time.h>
#include <semaphore>

#include "definitions.hpp"
#include "functions.hpp"

zmq::context_t context1;
zmq::context_t context2;
zmq::context_t context3;
zmq::context_t context4;

zmq::socket_t pullRequestSocket(context1, zmq::socket_type::pull);
zmq::socket_t pushRequestSocket(context2, zmq::socket_type::push);
zmq::socket_t pullReplySocket(context3, zmq::socket_type::pull);
zmq::socket_t pushReplySocket(context4, zmq::socket_type::push);

bool processWorking = true;

std::binary_semaphore killSemaphore(0);

void signal_handler(int signal) {
    if (signal == SIGTERM) {
        processWorking = false;
        killSemaphore.acquire(); // Чтобы мы сначала вышли из цикла, и не было ошибок
        pushRequestSocket.close();
        pullReplySocket.close();
    }
    exit(0);
}

/*

pushRequest старшего
| |
| |, + нечетное число к MIN_DYNAMIC_PORT
| |
pullRequest       pushRequest
-----------> NODE ----------->
<----------- NODE <-----------
pushReply         pullReply
| |
| |, + четное число к MIN_DYNAMIC_PORT
| |
pullReply старшего

*/

void getReply() {
    while (processWorking) {
        zmq::message_t replyFromWorker(sizeof(size_t));
        Reply reply = pullReply(pullReplySocket);
        if (reply.operationType != OperationType::NOTHING) {
            pushReply(pushReplySocket, reply); 
            std::cout << "HERE time2 = " << reply.result << std::endl;
        }
    }
    killSemaphore.release();
}

ssize_t getMillseconds() {
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return ((size_t)currentTime.tv_sec * (size_t)1e6 + (size_t)currentTime.tv_usec) / 1000;
}

int main(int argc, char* argv[]) {

    ssize_t startTime;
    ssize_t stopTime;
    ssize_t currentTime = 0;

    if (argc != 3) {
        std::cerr << "Invalid worker exec (worker)" << std::endl;
        exit(1);
    }

    if (signal(SIGTERM, signal_handler) == SIG_ERR) {
        perror("Can't catch signal");
        exit(1);
    }

    ssize_t nodeId = (ssize_t)atoll(const_cast<char*>(argv[1]));
    size_t currentPort = (size_t)atoll(const_cast<char*>(argv[2]));

    pushReplySocket.connect(getAddres(currentPort - 2));
    pullRequestSocket.connect(getAddres(currentPort - 1));

    pushRequestSocket.bind(getAddres(currentPort + 1));
    pullReplySocket.bind(getAddres(currentPort + 0));

    std::cout << "Создался worker с pid = " << getpid() << " workerID = " << nodeId << std::endl;
    std::cout << "Рабочий считывает request из" <<  currentPort - 1 << std::endl;
    std::cout << "Рабочий отправляет request в" <<  currentPort + 1 << std::endl;
    std::cout << "Рабочий считывает reply из" <<  currentPort << std::endl;
    std::cout << "Рабочий отправляет reply в" <<  currentPort - 2 << std::endl;

    std::thread replyThread(getReply);

    while (true) {
        Request request = pullRequest(pullRequestSocket);
        if (request.id == nodeId) { // Ответ 

            Reply reply;
            reply.operationType = request.operationType;

            if (request.subrequest == TimerSubrequest::START) {
                startTime = getMillseconds();
                reply.result = startTime;
            } else if (request.subrequest == TimerSubrequest::STOP) {
                stopTime = getMillseconds();
                currentTime = stopTime - startTime;
                reply.result = stopTime;
            } else if (request.subrequest == TimerSubrequest::TIME) {
                std::cout << "HERE time1 = " << currentTime << std::endl;
                reply.result = currentTime;
            }

            pushReply(pushReplySocket, reply);

        } else {
            pushRequest(pushRequestSocket, request);
        }
    }
    
    
    return 0;
}