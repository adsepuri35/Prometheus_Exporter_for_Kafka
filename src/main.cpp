#include "metrics.h"
#include "messaging.h"
#include <iostream>
#include <zmq.hpp>

int main() {
    std::cout << "Initializing Prometheus metrics..." << '\n';
    initMetrics();

    std::cout << "Starting Prometheus metrics server on port 8080..." << '\n';
    startMetricsServer(8080);

    std::cout << "Creating ZeroMQ context and socket..." << '\n';
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PUSH);
    socket.bind("tcp://*:5555");

    trackConnectionOpened();

    std::cout << "Sending messages..." << '\n';
    for (int i = 1; i <= 3; i++) {
        try {
            trackAndSendMessage(socket, "Hello, World!");
            std::cout << "Sent message " << i << '\n';
        } catch (const zmq::error_t& e) {
            trackError();
            std::cerr << "Error sending message: " << e.what() << '\n';
        }
    }

    std::cout << "All messages sent. Press Enter to exit." << '\n';
    std::cin.get(); // Wait for user input

    trackConnectionClosed();

    std::cout << "Exiting program." << '\n';
    return 0;
}