#include "metrics.h"
#include "messaging.h"
#include <iostream>

int main() {
    std::cout << "Initializing Prometheus metrics..." << std::endl;
    initMetrics();

    std::cout << "Starting Prometheus metrics server on port 8080..." << std::endl;
    startMetricsServer(8080);

    std::cout << "Creating ZeroMQ context and socket..." << std::endl;
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PUSH);
    socket.bind("tcp://*:5555");

    std::cout << "Sending messages..." << std::endl;
    for (int i = 0; i < 10; ++i) {
        trackAndSendMessage(socket, "Hello, World!");
        std::cout << "Sent message " << (i + 1) << std::endl;
    }

    std::cout << "All messages sent. Press Enter to exit." << std::endl;
    std::cin.get(); // Wait for user input

    std::cout << "Exiting program." << std::endl;
    return 0;
}