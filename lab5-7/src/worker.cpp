#include <iostream>
#include <zmq_addon.hpp>
#include <zmq.hpp>

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

void signal_handler(int signal) {
    std::cerr << "IM KILLED!!!" << std::endl;
    if (signal == SIGTERM) {
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

int main(int argc, char* argv[]) {

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

    std::cout << "Создался worker с pid = " << getpid() << " workerID = " << nodeId << " port = " << currentPort << std::endl;

    pushReplySocket.connect(getAddres(currentPort - 2));
    pullRequestSocket.connect(getAddres(currentPort - 1));

    pushRequestSocket.bind(getAddres(currentPort + 1));
    pullReplySocket.bind(getAddres(currentPort + 0));

    while (true) {
        Command cmd = pullMessage(pullRequestSocket);
        std::cout << cmd.id << " " << (size_t)cmd.operationType << " " << (size_t)cmd.subcommand << std::endl;
    }
    
    
    


    return 0;
}