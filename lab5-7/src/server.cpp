#include <iostream>
#include <zmq_addon.hpp>
#include <zmq.hpp>

#include "definitions.hpp"
#include "functions.hpp"

using namespace std;

int main() {
    zmq::context_t context;

    zmq::socket_t socket(context, zmq::socket_type::rep);
    socket.bind(getAddres(MIN_DYNAMIC_PORT + 1));

    for (size_t i = 0; i < 5; ++i) {
        zmq::message_t messageFromClient;

        socket.recv(messageFromClient, zmq::recv_flags::none);

        size_t dataFromClient = *messageFromClient.data<size_t>();
        size_t dataForClient = 2 * dataFromClient;
        std::cout << "Server get from client: " << dataFromClient << std::endl;
        std::cout << "Server sending " << dataForClient << " to client" << std::endl;

        std::cout << "Complex calculations...." << std::endl;
        sleep(3);

        zmq::message_t messageForClient(&dataForClient, sizeof(size_t));
        socket.send(messageForClient, zmq::send_flags::none);
    }
    socket.close();
    return 0;
}