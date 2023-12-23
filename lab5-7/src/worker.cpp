#include <iostream>
#include <zmq_addon.hpp>
#include <zmq.hpp>

#include "definitions.hpp"
#include "functions.hpp"


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

int main() {

    zmq::context_t context1;
    zmq::context_t context2;
    zmq::context_t context3;
    zmq::context_t context4;

    zmq::socket_t pullRequestSocket(context1, zmq::socket_type::pull);
    zmq::socket_t pushRequestSocket(context2, zmq::socket_type::push);
    zmq::socket_t pullReplySocket(context3, zmq::socket_type::pull);
    zmq::socket_t pushReplySocket(context4, zmq::socket_type::push);

    pushReplySocket.connect(getAddres(MIN_DYNAMIC_PORT + 0));
    pullRequestSocket.connect(getAddres(MIN_DYNAMIC_PORT + 1));

    pushRequestSocket.bind(getAddres(MIN_DYNAMIC_PORT + 3));
    pullReplySocket.bind(getAddres(MIN_DYNAMIC_PORT + 2));

    Command cmd = pullMessage(pullRequestSocket);
    std::cout << cmd.id << " " << (size_t)cmd.operationType << " " << (size_t)cmd.subcommand << std::endl;
    
    return 0;
}