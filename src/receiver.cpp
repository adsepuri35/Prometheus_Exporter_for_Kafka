#include "metrics.h"
#include <zmq.hpp>
#include <string>
#include <iostream>

int main() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PULL);
    socket.connect("tcp://localhost:5555");

    std::cout << "Receiver connected to tcp://localhost:5555..." << std::endl;

    while (true) {
        zmq::message_t message;
        auto result = socket.recv(message, zmq::recv_flags::none);
        if (!result.has_value()) {
            // Handle error or break
            break;
        }

        std::string msg_str(static_cast<char*>(message.data()), message.size());
        std::cout << "Received message: " << msg_str << std::endl;

        trackMessageReceived();
    }

    return 0;
}