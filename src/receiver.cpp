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
        socket.recv(message, zmq::recv_flags::none);
        std::string msg_str(static_cast<char*>(message.data()), message.size());
        std::cout << "Received message: " << msg_str << std::endl;
    }

    return 0;
}