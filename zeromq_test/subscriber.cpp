#include <zmq.hpp>
#include <iostream>

int main() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_SUB);
    socket.connect("tcp://localhost:5555");
    socket.set(zmq::sockopt::subscribe, "");

    while (true) {
        zmq::message_t message;
        socket.recv(message, zmq::recv_flags::none);
        std::string received_message(static_cast<char*>(message.data()), message.size());
        std::cout << "Received: " << received_message << std::endl;
    }

    return 0;
}