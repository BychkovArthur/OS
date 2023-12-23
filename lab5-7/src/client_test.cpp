#include <iostream>
#include <zmq_addon.hpp>
#include <zmq.hpp>

#include "definitions.hpp"
#include "functions.hpp"

int main() {
    zmq::context_t context;

    zmq::socket_t socket(context, zmq::socket_type::req);
    socket.connect(getAddres(MIN_DYNAMIC_PORT + 1));

    for (size_t i = 0; i < 5; ++i) {
        std::cout << "I'am client. Input message for server:\n>>> ";

        size_t dataForServer;
        std::cin >> dataForServer;
        zmq::message_t messageForServer(&dataForServer, sizeof(size_t));

        std::cout << "Complex calculations...." << std::endl;
        sleep(3);

        std::cout << "Client sending to server: " << dataForServer << std::endl;
        zmq::send_result_t res =  socket.send(messageForServer, zmq::send_flags::none);


        size_t serverResponseData;
        zmq::message_t serverResponse(sizeof(size_t));
        socket.recv(serverResponse, zmq::recv_flags::none);

        serverResponseData = *serverResponse.data<size_t>();
        std::cout << "Client get response from server = " << serverResponseData << std::endl;
    }
    
    return 0;
}