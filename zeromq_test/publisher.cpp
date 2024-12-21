#include <zmq.hpp>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PUB);
    socket.bind("tcp://*:5555");

    while (true) {
        zmq::message_t message("Hello, World!", 13);
        socket.send(message, zmq::send_flags::none);
        std::cout << "Sent: Hello, World!" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}